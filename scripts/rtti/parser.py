import re
from typing import List, Union
from models import Model, ModelClass, ModelEnum, ModelField, ModelMethod
from pathlib import Path

from markers import RTTI_CLASS_MARKER, RTTI_ENUM_MARKER, RTTI_FIELD_MARKER, RTTI_METHOD_MARKER
from file_content_cache import file_content_cache
from detector import file_contains_rtti_marker

def parse_file(path: Path) -> List[Model]:
    content: str = file_content_cache.find(path)

    if not content:
        if not file_contains_rtti_marker(path):
            return []

    content = file_content_cache.find(path)

    models: List[Model] = []
    lines = content.splitlines()
    idx_line = 0

    while idx_line < len(lines):
        line = lines[idx_line].strip()
        # We expect field & method markers to be parsed in class scope.
        if line.startswith(RTTI_CLASS_MARKER):
            class_scope = [line]
            idx_line_in_class_scope = idx_line
            while idx_line_in_class_scope < len(lines):
                idx_line_in_class_scope += 1
                next_line = lines[idx_line_in_class_scope].strip()
                class_scope.append(next_line)
                # Find '};' to determine the end of the class definition.
                if next_line.endswith('};'):
                    break
            class_model: ClassModel = parse_class_scope(class_scope)
            if class_model.name:
                models.append(class_model)

        if line.startswith(RTTI_ENUM_MARKER):
            # Find '};' to determine the end of the enum definition.
            enum_scope = [line]
            idx_line_in_enum_scope = idx_line
            while idx_line_in_enum_scope < len(lines):
                idx_line_in_enum_scope += 1
                next_line = lines[idx_line_in_enum_scope].strip()
                enum_scope.append(next_line)
                if next_line.endswith('};'):
                    break
            enum_model: ModelEnum = parse_enum_scope(enum_scope)
            if enum_model.name:
                models.append(enum_model)

        idx_line += 1

    print(f"Parsed {len(models)} models from {path}.")
    return models

def parse_enum_scope(scope: List[str]) -> ModelEnum:
    enum_model: ModelEnum = ModelEnum(name="", values=[])
    idx_line: int = 0

    while idx_line < len(scope):
        line: str = scope[idx_line].strip()
        if line.startswith(RTTI_ENUM_MARKER):
            enum_declaration = scope[idx_line + 1].strip()
            enum_model.name = re.match(r'enum(?:\s+class)?\s+(\w+)(?:\s*:\s*([\w:]+))?', enum_declaration).group(1)

        elif line and not line.startswith('//') and not line.startswith('/*') and not line.startswith('enum class') and line not in ('{', '}', '};'):
            # Assuming the rest of the lines are enum values
            value = re.split(r'\s*=\s*', line)[0].strip().rstrip(",")

            if value:
                enum_model.values.append(value)

        idx_line += 1
    return enum_model

def parse_class_scope(scope: List[str]) -> ModelClass:
    class_model: ClassModel = ModelClass(name="", parents=[], fields=[], methods=[])
    idx_line: int = 0

    while idx_line < len(scope):
        line: str = scope[idx_line].strip()
        if line.startswith(RTTI_CLASS_MARKER):
            class_declaration = scope[idx_line + 1].strip()
            parse_class_declaration(class_declaration, class_model)
            if not class_model.name:
                break

        elif line.startswith(RTTI_FIELD_MARKER):
            field_declaration = scope[idx_line + 1].strip()
            field_model = ModelField(name="", type="")
            parse_field_declaration(field_declaration, field_model)

            if not field_model.name or not field_model.type:
                idx_line += 1
                continue
            class_model.fields.append(field_model)

        elif line.startswith(RTTI_METHOD_MARKER):
            method_declaration = scope[idx_line + 1].strip()
            method_model = ModelMethod(name="", return_type="", parameters=[])
            parse_method_declaration(method_declaration, method_model)

            if not method_model.name or not method_model.return_type:
                idx_line += 1
                continue
            class_model.methods.append(method_model)
        
        idx_line += 1
    return class_model
            
def parse_class_declaration(declaration: str, out_model: ModelClass) -> None:
    class_match = re.match(r'class\s+(\w+)', declaration)
    if not class_match:
        return

    class_name = class_match.group(1)
    inheritance = []
    if ':' in declaration:
        parents_str = declaration.split(':', 1)[1]
        parents_raw = parents_str.split(',')
        for p in parents_raw:
            p = p.strip()
            cleaned = re.sub(r'^(public|protected|private)\s+', '', p).strip()
            inheritance.append(cleaned)

    out_model.name = class_name
    out_model.parents = inheritance

def parse_method_declaration(declaration: str, out_model: ModelMethod) -> None:
    method_match = re.match(r'(\w+)\s+(\w+)\((.*)\)', declaration)
    if not method_match:
        return

    return_type = method_match.group(1)
    method_name = method_match.group(2)
    params_str = method_match.group(3)

    parameters = []
    if params_str.strip():
        param_list = params_str.split(',')
        for param in param_list:
            param = param.strip()
            if param:
                field_match = re.match(r'(\w+)\s+(\w+)', param)
                if field_match:
                    field_type, field_name = field_match.groups()
                    parameters.append(ModelField(name=field_name, type=field_type))

    out_model.name = method_name
    out_model.return_type = return_type
    out_model.parameters = parameters

def parse_field_declaration(declaration: str, out_model: ModelField) -> None:
    field_match = re.match(r'([\w:\<\>\s&\*\[\]]+?)\s+(\w+)\s*(?:=[^;]*)?;', declaration.strip())
    if not field_match:
        return

    field_type, field_name = field_match.groups()
    out_model.name = field_name
    out_model.type = field_type