import sys
from pathlib import Path

from detector import file_contains_rtti_marker
from scanner import find_cpp_headers
from parser import parse_file
from models import Model

###############################################################################
# Validation
def validate_system_requirements() -> bool:
    if sys.version_info < (3, 7):
        print("Python 3.7 or higher is required.")
        return False
    return True

def validate_cli_arguments(args: list) -> bool:
    if len(args) != 2:
        print("Script expects root path to begin with.")
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
###############################################################################
if __name__ == "__main__":
    if not validate_system_requirements():
        sys.exit(1)

    if not validate_cli_arguments(sys.argv):
        sys.exit(1)

    root: Path = Path(sys.argv[1])
    if not validate_filesystem_path(root):
        sys.exit(1)

    print(f"RTTI autogen started for {root}.")
    data_to_autogen: list[Model] = [] 
    for path in find_cpp_headers(root):
        if file_contains_rtti_marker(path):
            data_to_autogen.extend(parse_file(path))

    print(f"Parsing complete. Found {len(data_to_autogen)} RTTI markers.")
