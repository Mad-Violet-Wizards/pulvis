from pathlib import Path
from typing import Iterator

def find_cpp_headers(root: Path) -> Iterator[Path]:
    for extension in ["*.h", "*.[hH][pP][pP]"]:
        for path in root.rglob(extension):
            yield path

def find_autogen_files(root: Path) -> Iterator[Path]:
    for extension in ["*_rtti_autogen.hpp", "*_rtti_autogen.cpp"]:
        for path in root.rglob(extension):
            yield path