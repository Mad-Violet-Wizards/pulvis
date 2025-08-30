from models import Model, ModelClass, ModelEnum, ModelField, ModelMethod
from collections import defaultdict
from pathlib import Path

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
            case ModelEnum() as enum_model:
                autogen_hpp_code += generate_rtti_code_enum_hpp(autogen_filename_hpp, enum_model)
                autogen_cpp_code += generate_rtti_code_enum_cpp(autogen_filename_cpp, enum_model)

    processed_header_path: str = f"{root_path.name}/{path.relative_to(root_path).as_posix()}"
    hpp_code = pack_hpp_code(autogen_hpp_code)
    cpp_code = pack_cpp_code(autogen_cpp_code, processed_header_path, autogen_filename_hpp)

    # Write the generated code to the respective files
    autogen_file_hpp_path = path.parent / autogen_filename_hpp
    print(autogen_file_hpp_path)
    with open(autogen_file_hpp_path, "w") as hpp_file:
        hpp_file.write(hpp_code)

    autogen_file_cpp_path = path.parent / autogen_filename_cpp
    with open(autogen_file_cpp_path, "w") as cpp_file:
        cpp_file.write(cpp_code)
###############################################################################
def generate_rtti_code_class_hpp(path: str, class_model: ModelClass) -> str:
    return f"void AutoGenRtti_RegisterClass_{class_model.name}();\n"

def generate_rtti_code_cpp_class_cpp(path: str, class_model: ModelClass) -> str:
    src_code: str = f"void AutoGenRtti_RegisterClass_{class_model.name}()\n{{\n";

    # Save rtti class name
    rtti_class_variable_name = f"{class_model.name}_rtti"

    # Create rtti class instance and register it
    src_code += f"\tstatic engine::rtti::CRTTIClass {rtti_class_variable_name}(\"{class_model.namespace}::{class_model.name}\");\n"
    src_code += f"\tengine::rtti::detail::CRTTIClassStorage::RegisterClass(&{rtti_class_variable_name});\n"

    # Attach parents
    parents: str = ", ".join([f"{parent}" for parent in class_model.parents])
    src_code += f"\tRegisterParents<{parents}>(\"{class_model.namespace}::{class_model.name}\");\n"

    # Register methods
    for method in class_model.methods:
        rtti_method_variable_name = f"{class_model.name}_{method.name}_rtti"
        src_code += f"\tstatic engine::rtti::CRTTIMethod {rtti_method_variable_name}(\"{class_model.namespace}::{class_model.name}\", \"{method.name}\", &{class_model.namespace}::{class_model.name}::{method.name});\n"
        src_code += f"\t{rtti_class_variable_name}.AddMethod(&{rtti_method_variable_name});\n"

    for field in class_model.fields:
        rtti_field_variable_name = f"{class_model.name}_{field.name}_rtti"
        src_code += f"\tstatic engine::rtti::CRTTIField {rtti_field_variable_name}(\"{field.name}\", &{class_model.namespace}::{class_model.name}::{field.name});\n"
        src_code += f"\t{rtti_class_variable_name}.AddField(&{rtti_field_variable_name});\n"
    src_code += f"}}\n"
    return src_code

###############################################################################
def generate_rtti_code_enum_hpp(path: str, enum_model: ModelEnum) -> str:
    return f"void AutoGenRtti_RegisterEnum_{enum_model.name}();\n"

def generate_rtti_code_enum_cpp(path: str, enum_model: ModelEnum) -> str:
    src_code: str = f"void AutoGenRtti_RegisterEnum_{enum_model.name}()\n{{\n"
    src_code += f"\tstatic engine::rtti::CRTTIEnum<{enum_model.namespace}::{enum_model.name}> {enum_model.name}_rtti;\n"
    src_code += f"}}\n"
    return src_code
###############################################################################
def generate_project_register_script(root_path: str, autogen_data: defaultdict[str, list[Model]]):
    autogen_filename_hpp: str = f"{root_path.name}_rtti_autogen.hpp"

    include_paths: set[str] = set()
    register_methods: str = ""
    for key, models in autogen_data.items():
        for model in models:
            generated_filename_hpp = key.stem + "_rtti_autogen.hpp"
            project_root = root_path.parent
            relative_path = key.relative_to(project_root).as_posix()
            include_path = Path(relative_path).parent / generated_filename_hpp
            include_paths.add(include_path.as_posix())
            match model:
                case ModelClass() as class_model:
                    register_methods += f"\tAutoGenRtti_RegisterClass_{class_model.name}();\n"
                case ModelEnum() as enum_model:
                    register_methods += f"\tAutoGenRtti_RegisterEnum_{enum_model.name}();\n"

    hpp_code = f"#pragma once\n"
    for include_path in include_paths:
        hpp_code += f"#include \"{include_path}\"\n"

    hpp_code += f"inline void RegisterRTTI_{root_path.name}()\n{{\n"
    hpp_code += f"{register_methods}"
    hpp_code += f"}}\n"

    autogen_file_hpp_path = root_path / autogen_filename_hpp
    with open(autogen_file_hpp_path, "w") as hpp_file:
        hpp_file.write(hpp_code)

###############################################################################
def pack_hpp_code(autogen_hpp_code: str) -> str:
    packed_hpp_code: str = f"#pragma once\n"
    packed_hpp_code += f"{autogen_hpp_code}"
    return packed_hpp_code

def pack_cpp_code(autogen_cpp_code: str, processed_header_path: str, autogen_filename_hpp: str) -> str:
    packed_cpp_code: str = f'#include "engine/engine_pch.hpp"\n'
    packed_cpp_code  += f'#include "{autogen_filename_hpp}"\n'
    packed_cpp_code += f'#include "{processed_header_path}"\n\n'

    packed_cpp_code += "#include \"engine/rtti/RTTITypeTraits.hpp\"\n"
    packed_cpp_code += "#include \"engine/rtti/RTTITypeName.hpp\"\n"
    packed_cpp_code += "#include \"engine/rtti/RTTITypeInfo.hpp\"\n"
    packed_cpp_code += "#include \"engine/rtti/RTTIMethodInfo.hpp\"\n"
    packed_cpp_code += "#include \"engine/rtti/RTTIMethod.hpp\"\n"
    packed_cpp_code += "#include \"engine/rtti/RTTIClass.hpp\"\n"
    packed_cpp_code += "#include \"engine/rtti/RTTIField.hpp\"\n"
    packed_cpp_code += "#include \"engine/rtti/RTTIEnum.hpp\"\n"
    packed_cpp_code += "#include \"engine/rtti/detail/RTTIClassDetail.hpp\"\n"
    packed_cpp_code += f"{autogen_cpp_code}"
    return packed_cpp_code