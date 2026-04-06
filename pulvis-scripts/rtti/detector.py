from pathlib import Path
from markers import *
from file_content_cache import file_content_cache

def file_contains_rtti_marker(file_path: Path) -> bool:
    content: str = file_content_cache.find_or_add(file_path)
    return any(marker in content for marker in RTTI_MARKERS)