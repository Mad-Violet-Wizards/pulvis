import sys
from pathlib import Path

from collections import defaultdict
from detector import file_contains_rtti_marker
from scanner import find_cpp_headers, find_autogen_files
from parser import parse_file
from models import Model
from generate import generate_rtti_code, generate_project_register_script
from generate_lua_bindings import generate_lua_bindings
from file_changes_cache import file_changes_cache
from file_content_cache import file_content_cache
from log import verbose_log
import config

valid_actions = ["--help", "--generate", "--clear", "--force", "--stats"]
valid_flags = ["--verbose"]

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
        print("Script expects at least one action/flag as argument. Run --help to display options")
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

def validate_arguments(arguments: list) -> bool:
    all_valid = valid_actions + valid_flags
    for arg in arguments:
        if arg not in all_valid:
            verbose_log(f"Invalid argument '{arg}'. Valid options are: {', '.join(all_valid)}")
            return False
    return True

def parse_arguments(args: list) -> tuple[list, list]:
    flags = [arg for arg in args if arg in valid_flags]
    actions = [arg for arg in args if arg in valid_actions]
    return flags, actions

def apply_flags(flags: list):
    if "--verbose" in flags:
        config.verbose_mode = True
###############################################################################
if __name__ == "__main__":
    if not validate_system_requirements():
        sys.exit(1)

    if not validate_cli_arguments(sys.argv):
        sys.exit(1)

    root_path: Path = Path(sys.argv[1])
    if not validate_filesystem_path(root_path):
        sys.exit(1)

    arguments = sys.argv[2:]
    if not validate_arguments(arguments):
        sys.exit(1)
    
    flags, actions = parse_arguments(arguments)
    apply_flags(flags)
    
    verbose_log(f"Processing with flags: {flags}, actions: {actions}")
    
    action_priority = ["--help", "--clear", "--force", "--generate", "--stats"]
    sorted_actions = sorted(actions, key=lambda x: action_priority.index(x) if x in action_priority else 999)
    
    if not sorted_actions:
        print("No valid actions specified. Use --help for available options.")
        sys.exit(1)

    for action in sorted_actions:
        match action:
            case "--help":
                print("--help - Command display list of supported actions")
                print("--generate - Generate code based on supported RTTI C++ attributes")
                print("--clear - Clear all generated code files (_rtti_autogen.cpp/rtti_autogen.hpp)")
                print("--force - Force regeneration of all RTTI files (ignoring cache)")
                print("--stats - Display cache statistics")
                print("--verbose - Display detailed information during processing")
                sys.exit(0)
            case "--generate":
                data_to_autogen: defaultdict[str, list[Model]] = defaultdict(list)
                skipped_files = 0
                processed_files = 0
                
                verbose_log("Scanning for RTTI files...")
                
                for path in find_cpp_headers(root_path):
                    if file_contains_rtti_marker(path):
                        content = file_content_cache.find_or_add(path)

                        if file_changes_cache.has_file_changed(path, content):
                            data_to_autogen[path].extend(parse_file(path))
                            file_changes_cache.update_file_info(path, content)
                            processed_files += 1
                        else:
                            skipped_files += 1
                
                for key in data_to_autogen:
                    generate_rtti_code(root_path, key, data_to_autogen[key])
                
                models_count = sum(len(models) for models in data_to_autogen.values())
                
                all_files_data: defaultdict[str, list[Model]] = defaultdict(list)
                for path in find_cpp_headers(root_path):
                    if file_contains_rtti_marker(path):
                        all_files_data[path].extend(parse_file(path))
                
                generate_project_register_script(root_path, all_files_data)
                generate_lua_bindings(root_path, all_files_data)

                file_changes_cache.save()
                
                verbose_log(f"RTTI code generation complete.")
                verbose_log(f"  Processed: {processed_files} files")
                verbose_log(f"  Skipped: {skipped_files} files")
                verbose_log(f"  Found: {models_count} rtti attributes")
            case "--clear":
                for path in find_autogen_files(root_path):
                    path.unlink(missing_ok=True)
                file_changes_cache.clear()
                verbose_log("Cleared all generated RTTI files and cache.")
            case "--force":
                verbose_log("Force regenerating all RTTI files...")
                file_changes_cache.clear()
                
                data_to_autogen: defaultdict[str, list[Model]] = defaultdict(list)
                for path in find_cpp_headers(root_path):
                    if file_contains_rtti_marker(path):
                        data_to_autogen[path].extend(parse_file(path))
                        # Aktualizuj cache dla przyszłych uruchomień
                        content = file_content_cache.find_or_add(path)
                        file_changes_cache.update_file_info(path, content)
                
                for key in data_to_autogen:
                    generate_rtti_code(root_path, key, data_to_autogen[key])
                
                models_count = sum(len(models) for models in data_to_autogen.values())
                generate_project_register_script(root_path, data_to_autogen)
                generate_lua_bindings(root_path, data_to_autogen)
                file_changes_cache.save()
                
                verbose_log(f"Force regeneration complete. Found {models_count} rtti attributes.")
            case "--stats":
                stats = file_changes_cache.get_stats()
                print("RTTI Cache Statistics:")
                print(f"  Cached files: {stats['total_files']}")
                print(f"  Last update: {stats['last_update']}")
                print(f"  Cache file: {stats['cache_file']}")
                
                if stats['total_files'] > 0:
                    print("\nCached files:")
                    for file_key, info in file_changes_cache.cache_data["files"].items():
                        file_path = Path(file_key)
                        print(f"  - {file_path.name}: {info['last_generated']}")
            case _:
                print(f"Unknown action '{action}'.")    
