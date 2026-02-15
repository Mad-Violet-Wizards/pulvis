import sys
from pathlib import Path

from collections import defaultdict
from detector import file_contains_rtti_marker
from scanner import find_cpp_headers, find_autogen_files
from parser import parse_file
from models import Model
from generate import generate_rtti_code, generate_project_register_script

valid_actions = ["--help", "--generate", "--clear"]

###############################################################################
# Validation
def validate_system_requirements() -> bool:
    if sys.version_info < (3, 7):
        print("Python 3.7 or higher is required.")
        return False
    return True

def validate_cli_arguments(args: list) -> bool:
    if len(args) < 2:
        print("Script expects root path to begin with.")
        return False
    if len(args) < 3:
        print("Script expects an action as the second argument. Run --help to display action flags")
        return False
    if len(args) > 3:
        print("Too many arguments provided. (Path, Action) required")
        return False
    return True

def validate_filesystem_path(path: Path) -> bool:
    if not path.exists():
        print(f"Path '{path}' does not exist.")
        return False
    if not path.is_dir():
        print(f"Path '{path}' is not a directory.")
        return False
    if not path.is_absolute():
        print(f"Path '{path}' is not an absolute path.")
        return False
    return True

def validate_action(action: str) -> bool:
    valid_actions = ["--help", "--generate", "--clear"]
    if action not in valid_actions:
        print(f"Invalid action '{action}'. Valid actions are: {', '.join(valid_actions)}")
        return False
    return True
###############################################################################
if __name__ == "__main__":
    if not validate_system_requirements():
        sys.exit(1)

    if not validate_cli_arguments(sys.argv):
        sys.exit(1)

    root_path: Path = Path(sys.argv[1])
    if not validate_filesystem_path(root_path):
        sys.exit(1)

    action: str = sys.argv[2]
    if not validate_action(sys.argv[2]):
        sys.exit(1)

    match action:
        case "--help":
            print("--help - Command display list of supported actions")
            print("--generate - Generate code based on supported RTTI C++ attributes")
            print("--clear - Clear all generated code files (_rtti_autogen.cpp/rtti_autogen.hpp)")
        case "--generate":
            data_to_autogen: defaultdict[str, list[Model]] = defaultdict(list)
            for path in find_cpp_headers(root_path):
                if file_contains_rtti_marker(path):
                    data_to_autogen[path].extend(parse_file(path))
            for key in data_to_autogen:
                generate_rtti_code(root_path, key, data_to_autogen[key])
            models_count = sum(len(models) for models in data_to_autogen.values())
            generate_project_register_script(root_path, data_to_autogen)
            print(f"RTTI code generation complete. Found {models_count} rtti attributes.")
        case "--clear":
            for path in find_autogen_files(root_path):
                path.unlink(missing_ok=True)
        case _:
            print(f"Unknown action '{action}'.")    
