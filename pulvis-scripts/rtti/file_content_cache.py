from dataclasses import dataclass
from pathlib import Path

###############################################################################
@dataclass
class FileContentCacheNode:
    path: Path
    content: str

###############################################################################
class FileContentCache:
    def __init__(self):
        self.cache: List[FileContentCacheNode] = []

    def find(self, file_path: Path) -> str:
        for node in self.cache:
            if node.path == file_path:
                return node.content
        return ""

    def add(self, file_path: Path, content: str):
        if not any(node.path == file_path for node in self.cache):
            self.cache.append(FileContentCacheNode(path=file_path, content=content))

    def find_or_add(self, file_path: Path) -> str:
        content = self.find(file_path)
        if content:
            return content
        with file_path.open('r', encoding='utf-8') as file:
            content = file.read()
            self.add(file_path, content)
            return content

    def clear(self):
        self.cache.clear()

###############################################################################
file_content_cache: FileContentCache = FileContentCache()