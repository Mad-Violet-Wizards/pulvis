from pathlib import Path
from markers import *
from file_content_cache import file_content_cache

def file_contains_rtti_marker(file_path: Path) -> bool:
    content: str = file_content_cache.find(file_path)

    if not content:
        with file_path.open('r', encoding='utf-8') as file:
            content = file.read()
            file_content_cache.add(file_path, content)
    
    return any(marker in content for marker in RTTI_MARKERS)