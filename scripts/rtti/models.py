from dataclasses import dataclass
from pathlib import Path
from typing import List, Optional, Union

##################
@dataclass
class ModelField:
    name: str
    type: str

@dataclass
class ModelMethod:
    name: str
    return_type: str
    parameters: List[ModelField]

@dataclass
class ModelClass:
    name: str
    parents: List[str]
    fields: List[ModelField]
    methods: List[ModelMethod]

@dataclass
class ModelEnum:
    name: str
    values: List[str]

@dataclass
class Model:
    path: Path
    type: Union[ModelClass, ModelEnum]
    