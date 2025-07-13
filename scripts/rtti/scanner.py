from pathlib import Path
from typing import Iterator

def find_cpp_headers(root: Path) -> Iterator[Path]:
    for extension in ["*.h", "*.[hH][pP][pP]"]:
        for path in root.rglob(extension):
            yield path
