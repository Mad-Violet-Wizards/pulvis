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
            # Find '};' to determine the end of the class definition.
            class_scope = [line]
            idx_line_in_class_scope = idx_line
            while idx_line_in_class_scope < len(lines):
                idx_line_in_class_scope += 1
                next_line = lines[idx_line_in_class_scope].strip()
                class_scope.append(next_line)
                if next_line.endswith('};'):
                    break
            parse_class_scope(class_scope)

        if line.startswith(RTTI_ENUM_MARKER):
            print(f"Parsing enum in {path}: {line}")

        idx_line += 1

    return []

def parse_class_scope(scope: List[str]):
    class_model = ModelClass(name="", parents=[], fields=[], methods=[])
    idx_line = 0

    while idx_line < len(scope):
        line = scope[idx_line].strip()
        if line.startswith(RTTI_CLASS_MARKER):
            class_declaration = scope[idx_line + 1].strip()
            print(f"Parsing class declaration: {class_declaration}")
        elif line.startswith(RTTI_FIELD_MARKER):
            field_declaration = scope[idx_line + 1].strip()
            print(f"Parsing field  {field_declaration}")
        elif line.startswith(RTTI_METHOD_MARKER):
            method_declaration = scope[idx_line + 1].strip()
            print(f"Parsing method declaration {method_declaration}")
        
        idx_line += 1