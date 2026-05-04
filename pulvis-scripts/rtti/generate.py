from models import Model, ModelClass, ModelEnum, ModelField, ModelMethod
from collections import defaultdict
from pathlib import Path
from markers import RTTI_FIELD_ATTRIBUTE_MAP, RTTI_METHOD_ATTRIBUTE_MAP, RTTI_METHOD_NONE_ATTRIBUTE, RTTI_FIELD_NONE_ATTRIBUTE
from log import verbose_log

###############################################################################
# Lua-binding helpers
###############################################################################
def _has_scriptable_fields(cls: ModelClass) -> bool:
    return any("Scriptable" in f.tags for f in cls.fields)

def _has_scriptable_methods(cls: ModelClass) -> bool:
    return any("Scriptable" in m.tags for m in cls.methods)

def _needs_lua_binding(cls: ModelClass) -> bool:
    return _has_scriptable_fields(cls) or _has_scriptable_methods(cls)

def _is_abstract_class(cls: ModelClass) -> bool:
    # Heuristic: any method tagged 'Pure' -> class is abstract,
    # so it cannot be default-constructed by sol3.
    return any("Pure" in m.tags for m in cls.methods)

def _models_need_lua(models: list[Model]) -> bool:
    return any(isinstance(m, ModelClass) and _needs_lua_binding(m) for m in models)

###############################################################################
# Per-source generation
###############################################################################
def generate_rtti_code(root_path, path, models: list[Model]):

    autogen_filename_hpp: str = path.stem + "_rtti_autogen.hpp"
    autogen_filename_cpp: str = path.stem + "_rtti_autogen.cpp"

    autogen_hpp_code: str = ""
    autogen_cpp_code: str = ""

    for model in models:
        match model:
            case ModelClass() as class_model:
                autogen_hpp_code += generate_rtti_code_class_hpp(autogen_filename_hpp, class_model)
                autogen_cpp_code += generate_rtti_code_cpp_class_cpp(autogen_filename_cpp, class_model)
                autogen_hpp_code += generate_lua_class_func_decl(class_model)
                autogen_cpp_code += generate_lua_class_func_def(class_model)
                
                # Add ScriptableHook generation for eligible classes
                if class_model.needs_script_hook_generation():
                    autogen_hpp_code += generate_script_hook_func_decl(class_model) + "\n"
                    autogen_cpp_code += generate_script_hook_func_def(class_model) + "\n\n"
                    
            case ModelEnum() as enum_model:
                autogen_hpp_code += generate_rtti_code_enum_hpp(autogen_filename_hpp, enum_model)
                autogen_cpp_code += generate_rtti_code_enum_cpp(autogen_filename_hpp, enum_model)

    has_lua = _models_need_lua(models)
    has_script_hooks = _models_need_script_hooks(models)
    processed_header_path: str = path.name
    hpp_code = pack_hpp_code(autogen_hpp_code, has_lua, has_script_hooks)
    cpp_code = pack_cpp_code(autogen_cpp_code, processed_header_path, autogen_filename_hpp, has_lua, has_script_hooks)

    autogen_file_hpp_path = path.parent / autogen_filename_hpp
    with open(autogen_file_hpp_path, "w") as hpp_file:
        hpp_file.write(hpp_code)

    autogen_file_cpp_path = path.parent / autogen_filename_cpp
    with open(autogen_file_cpp_path, "w") as cpp_file:
        cpp_file.write(cpp_code)

###############################################################################
# RTTI emitters
###############################################################################
def generate_rtti_code_class_hpp(path: str, class_model: ModelClass) -> str:
    return f"void AutoGenRtti_RegisterClass_{class_model.name}();\n"

def generate_rtti_code_cpp_class_cpp(path: str, class_model: ModelClass) -> str:
    src_code: str = f"void AutoGenRtti_RegisterClass_{class_model.name}()\n{{\n"

    rtti_class_variable_name = f"{class_model.name}_rtti"

    src_code += f"\tstatic pulvis::rtti::CRTTIClass {rtti_class_variable_name}(\"{class_model.namespace}::{class_model.name}\");\n"
    src_code += f"\tpulvis::rtti::detail::CRTTIClassStorage::RegisterClass(&{rtti_class_variable_name});\n"

    parents: str = ", ".join([f"{parent}" for parent in class_model.parents])
    src_code += f"\tRegisterParents<{parents}>(\"{class_model.namespace}::{class_model.name}\");\n"

    for method in class_model.methods:
        rtti_method_variable_name = f"{class_model.name}_{method.name}_rtti"
        method_flags = generate_rtti_code_method_flags(method.tags)
        src_code += f"\tstatic pulvis::rtti::CRTTIMethod {rtti_method_variable_name}(\"{class_model.namespace}::{class_model.name}\", \"{method.name}\", &{class_model.namespace}::{class_model.name}::{method.name}, {method_flags});\n"
        src_code += f"\t{rtti_class_variable_name}.AddMethod(&{rtti_method_variable_name});\n"

    for field in class_model.fields:
        rtti_field_variable_name = f"{class_model.name}_{field.name}_rtti"
        field_flags = generate_rtti_code_field_flags(field.tags)
        src_code += f"\tstatic pulvis::rtti::CRTTIField {rtti_field_variable_name}(\"{field.name}\", &{class_model.namespace}::{class_model.name}::{field.name}, {field_flags});\n"
        src_code += f"\t{rtti_class_variable_name}.AddField(&{rtti_field_variable_name});\n"
    src_code += f"}}\n"
    return src_code

###############################################################################
def generate_rtti_code_enum_hpp(path: str, enum_model: ModelEnum) -> str:
    return f"void AutoGenRtti_RegisterEnum_{enum_model.name}();\n"

def generate_rtti_code_enum_cpp(path: str, enum_model: ModelEnum) -> str:
    src_code: str = f"void AutoGenRtti_RegisterEnum_{enum_model.name}()\n{{\n"
    src_code += f"\tstatic pulvis::rtti::CRTTIEnum<{enum_model.namespace}::{enum_model.name}> {enum_model.name}_rtti;\n"
    src_code += f"}}\n"
    return src_code

###############################################################################
# Lua emitters (use CScriptableService& - never sol::state& in headers)
###############################################################################
def generate_lua_class_func_decl(class_model: ModelClass) -> str:
    if not _needs_lua_binding(class_model):
        return ""
    return f"void AutoGenLua_Register_{class_model.name}(pulvis::scriptable::CScriptableService& _service);\n"

def generate_lua_class_func_def(class_model: ModelClass) -> str:
    if not _needs_lua_binding(class_model):
        return ""

    fqn = f"{class_model.namespace}::{class_model.name}"
    src = f"void AutoGenLua_Register_{class_model.name}(pulvis::scriptable::CScriptableService& _service)\n{{\n"
    src += "\tsol::state& _lua = _service.GetLuaState();\n"
    src += f"\t_lua.new_usertype<{fqn}>(\"{class_model.name}\""

    if _is_abstract_class(class_model):
        src += ",\n\t\tsol::no_constructor"
    else:
        src += f",\n\t\tsol::constructors<{fqn}()>()"

    for field in class_model.fields:
        if "Scriptable" in field.tags:
            src += f",\n\t\t\"{field.name}\", &{fqn}::{field.name}"

    for method in class_model.methods:
        if "Scriptable" in method.tags:
            src += f",\n\t\t\"{method.name}\", &{fqn}::{method.name}"

    src += "\n\t);\n}\n"
    return src

###############################################################################
def generate_rtti_code_field_flags(tags: list[str]) -> str:
    if not tags:
        return RTTI_FIELD_NONE_ATTRIBUTE

    flags: list[str] = []
    for tag in tags:
        if tag in RTTI_FIELD_ATTRIBUTE_MAP:
            flags.append(RTTI_FIELD_ATTRIBUTE_MAP[tag])
        else:
            verbose_log(f"Warning: Unrecognized field tag '{tag}'")

    if not flags:
        return RTTI_FIELD_NONE_ATTRIBUTE

    return " | ".join(flags)


def generate_rtti_code_method_flags(tags: list[str]) -> str:
    if not tags:
        return RTTI_METHOD_NONE_ATTRIBUTE

    flags: list[str] = []
    for tag in tags:
        if tag in RTTI_METHOD_ATTRIBUTE_MAP:
            flags.append(RTTI_METHOD_ATTRIBUTE_MAP[tag])
        else:
            verbose_log(f"Warning: Unrecognized method tag '{tag}'")

    if not flags:
        return RTTI_METHOD_NONE_ATTRIBUTE

    return " | ".join(flags)

###############################################################################
# ScriptableHook emitters
###############################################################################
def generate_script_hook_func_decl(class_model: ModelClass) -> str:
    """Generate declaration for ScriptableHook registration function."""
    return f"void AutoGenScriptHook_Register_{class_model.name}(pulvis::events::CEventScriptBridge& _bridge);"

def generate_script_hook_func_def(class_model: ModelClass) -> str:
    """Generate definition for ScriptableHook registration function."""
    qualified_name = f"{class_model.namespace}::{class_model.name}" if class_model.namespace else class_model.name
    script_name = class_model.get_script_name()
    
    return f"""void AutoGenScriptHook_Register_{class_model.name}(pulvis::events::CEventScriptBridge& _bridge)
{{
    static_assert(std::is_base_of_v<pulvis::events::IEvent, {qualified_name}>, 
        "{qualified_name} must inherit from pulvis::events::IEvent to use ScriptableHook");
    static_assert(std::is_default_constructible_v<{qualified_name}>, 
        "{qualified_name} must have default constructor for ScriptableHook factory");
    
    _bridge.RegisterEvent(
        {qualified_name}::GetStaticEventId(),
        "{script_name}",
        pulvis::rtti::CRTTIClass::FindConstInStorage(
            pulvis::rtti::CRTTITypeInfo<{qualified_name}>::GetTypeId()),
        []() -> std::unique_ptr<pulvis::events::IEvent>
        {{
            return std::make_unique<{qualified_name}>();
        }});
}}"""

def _models_need_script_hooks(models: list[Model]) -> bool:
    """Check if any models need ScriptableHook generation."""
    return any(isinstance(m, ModelClass) and m.needs_script_hook_generation() for m in models)

###############################################################################
# Project-level aggregator (one file per project)
###############################################################################
def generate_project_register_script(root_path: str, autogen_data: defaultdict[str, list[Model]]):
    autogen_filename_hpp: str = f"{root_path.name}_rtti_autogen.hpp"

    include_paths: set[str] = set()
    register_methods: str = ""
    lua_register_methods: str = ""
    script_hook_methods: str = ""
    has_any_lua = False
    has_any_script_hooks = False

    for key, models in autogen_data.items():
        for model in models:
            generated_filename_hpp = key.stem + "_rtti_autogen.hpp"
            include_paths.add(generated_filename_hpp)
            match model:
                case ModelClass() as class_model:
                    register_methods += f"\tAutoGenRtti_RegisterClass_{class_model.name}();\n"
                    if _needs_lua_binding(class_model):
                        lua_register_methods += f"\tAutoGenLua_Register_{class_model.name}(_service);\n"
                        has_any_lua = True
                    if class_model.needs_script_hook_generation():
                        script_hook_methods += f"\tAutoGenScriptHook_Register_{class_model.name}(_bridge);\n"
                        has_any_script_hooks = True
                case ModelEnum() as enum_model:
                    register_methods += f"\tAutoGenRtti_RegisterEnum_{enum_model.name}();\n"

    hpp_code = "#pragma once\n"
    for include_path in include_paths:
        hpp_code += f"#include \"{include_path}\"\n"

    hpp_code += f"\ninline void RegisterRTTI_{root_path.name}()\n{{\n"
    hpp_code += f"{register_methods}"
    hpp_code += "}\n"

    if has_any_lua:
        hpp_code += "\nnamespace pulvis::scriptable { class CScriptableService; }\n"
        hpp_code += f"inline void RegisterLuaBindings_{root_path.name}(pulvis::scriptable::CScriptableService& _service)\n{{\n"
        hpp_code += f"{lua_register_methods}"
        hpp_code += "}\n"

    # Add ScriptableHook aggregator function
    hpp_code += "\nnamespace pulvis::events { class CEventScriptBridge; }\n"
    hpp_code += f"inline void RegisterScriptHooks_{root_path.name}(pulvis::events::CEventScriptBridge& _bridge)\n{{\n"
    if has_any_script_hooks:
        hpp_code += f"{script_hook_methods}"
    else:
        hpp_code += "\t(void)_bridge;\n"
    hpp_code += "}\n"

    autogen_file_hpp_path = root_path / autogen_filename_hpp
    with open(autogen_file_hpp_path, "w") as hpp_file:
        hpp_file.write(hpp_code)

###############################################################################
# Packers
###############################################################################
def pack_hpp_code(autogen_hpp_code: str, has_lua: bool, has_script_hooks: bool = False) -> str:
    packed_hpp_code: str = "#pragma once\n"
    if has_lua:
        packed_hpp_code += "namespace pulvis::scriptable { class CScriptableService; }\n"
    if has_script_hooks:
        packed_hpp_code += "namespace pulvis::events { class CEventScriptBridge; }\n"
    packed_hpp_code += f"{autogen_hpp_code}"
    return packed_hpp_code

def pack_cpp_code(autogen_cpp_code: str, processed_header_path: str, autogen_filename_hpp: str, has_lua: bool, has_script_hooks: bool = False) -> str:
    packed_cpp_code: str = f'#include "{autogen_filename_hpp}"\n'
    packed_cpp_code += f'#include "{processed_header_path}"\n\n'

    packed_cpp_code += "#include \"RTTITypeTraits.hpp\"\n"
    packed_cpp_code += "#include \"RTTITypeName.hpp\"\n"
    packed_cpp_code += "#include \"RTTITypeInfo.hpp\"\n"
    packed_cpp_code += "#include \"RTTIMethodInfo.hpp\"\n"
    packed_cpp_code += "#include \"RTTIMethod.hpp\"\n"
    packed_cpp_code += "#include \"RTTIClass.hpp\"\n"
    packed_cpp_code += "#include \"RTTIField.hpp\"\n"
    packed_cpp_code += "#include \"RTTIEnum.hpp\"\n"
    packed_cpp_code += "#include \"RTTIAttributes.hpp\"\n"
    packed_cpp_code += "#include \"detail/RTTIClassDetail.hpp\"\n"
    if has_lua:
        packed_cpp_code += "#include \"ScriptableService.hpp\"\n"
        packed_cpp_code += "#include <sol3/sol.hpp>\n"
    if has_script_hooks:
        packed_cpp_code += "#include \"EventScriptBridge.hpp\"\n"
        packed_cpp_code += "#include \"RTTIClass.hpp\"\n"
        packed_cpp_code += "#include \"RTTITypeInfo.hpp\"\n"
        packed_cpp_code += "#include <memory>\n"
    packed_cpp_code += f"\n{autogen_cpp_code}"
    return packed_cpp_code