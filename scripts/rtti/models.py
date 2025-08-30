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
    namespace: str
    name: str
    parents: List[str]
    fields: List[ModelField]
    methods: List[ModelMethod]

    def __str__(self):
        return f"ModelClass(name={self.name}, parents={self.parents}, fields={self.fields}, methods={self.methods})"

@dataclass
class ModelEnum:
    namespace: str
    name: str
    values: List[str]

    def __str__(self):
        return f"ModelEnum(name={self.name}, values={self.values})"

@dataclass
class Model:
    path: Path
    type: Union[ModelClass, ModelEnum]
    