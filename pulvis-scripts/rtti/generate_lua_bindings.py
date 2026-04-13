from collections import defaultdict
from pathlib import Path
from typing import List

from models import Model, ModelClass, ModelField, ModelMethod


def _has_scriptable_fields(cls: ModelClass) -> bool:
    return any("Scriptable" in f.tags for f in cls.fields)


def _has_scriptable_methods(cls: ModelClass) -> bool:
    return any("Scriptable" in m.tags for m in cls.methods)


def _needs_lua_binding(cls: ModelClass) -> bool:
    return _has_scriptable_fields(cls) or _has_scriptable_methods(cls)


def _generate_field_binding(fqn: str, field: ModelField) -> str:
    if "Scriptable" not in field.tags:
        return ""
    return f'\t\t\t"{field.name}", &{fqn}::{field.name}'


def _generate_method_binding(fqn: str, method: ModelMethod) -> str:
    if "Scriptable" not in method.tags:
        return ""
    return f'\t\t\t"{method.name}", &{fqn}::{method.name}'


def _generate_class_binding(cls: ModelClass) -> str:
    if not _needs_lua_binding(cls):
        return ""

    fqn = f"{cls.namespace}::{cls.name}"
    lines = []
    lines.append(f'void AutoGenLua_Register_{cls.name}(sol::state& _lua)')
    lines.append('{')
    lines.append(f'\t_lua.new_usertype<{fqn}>("{cls.name}",')
    lines.append(f'\t\tsol::constructors<{fqn}()>(),')

    bindings = []
    for field in cls.fields:
        binding = _generate_field_binding(fqn, field)
        if binding:
            bindings.append(binding)

    for method in cls.methods:
        binding = _generate_method_binding(fqn, method)
        if binding:
            bindings.append(binding)

    lines.append(",\n".join(bindings))
    lines.append('\t);')
    lines.append('}')
    return "\n".join(lines) + "\n"


def generate_lua_bindings(root_path: Path, autogen_data: defaultdict[str, List[Model]]):
    project_name = root_path.name
    hpp_filename = f"{project_name}_lua_bindings_autogen.hpp"
    cpp_filename = f"{project_name}_lua_bindings_autogen.cpp"

    classes: List[ModelClass] = []

    for path, models in autogen_data.items():
        for model in models:
            if isinstance(model, ModelClass) and _needs_lua_binding(model):
                classes.append(model)

    if not classes:
        return

    # HPP
    hpp_lines = []
    hpp_lines.append("#pragma once")
    hpp_lines.append("")
    hpp_lines.append("namespace sol { class state; }")
    hpp_lines.append("")

    for cls in classes:
        hpp_lines.append(f"void AutoGenLua_Register_{cls.name}(sol::state& _lua);")

    hpp_lines.append("")
    hpp_lines.append(f"inline void RegisterLuaBindings_{project_name}(sol::state& _lua)")
    hpp_lines.append("{")
    for cls in classes:
        hpp_lines.append(f"\tAutoGenLua_Register_{cls.name}(_lua);")
    hpp_lines.append("}")
    hpp_lines.append("")

    hpp_path = root_path / hpp_filename
    with open(hpp_path, "w") as f:
        f.write("\n".join(hpp_lines))

    # CPP
    cpp_lines = []
    cpp_lines.append(f'#include "{hpp_filename}"')
    cpp_lines.append("")

    include_paths: set = set()
    for path, models in autogen_data.items():
        for model in models:
            if isinstance(model, ModelClass) and _needs_lua_binding(model):
                include_paths.add(Path(path).name)

    for inc in sorted(include_paths):
        cpp_lines.append(f'#include "{inc}"')

    cpp_lines.append("")
    cpp_lines.append("#include <sol3/sol.hpp>")
    cpp_lines.append("")

    for cls in classes:
        cpp_lines.append(_generate_class_binding(cls))

    cpp_path = root_path / cpp_filename
    with open(cpp_path, "w") as f:
        f.write("\n".join(cpp_lines))