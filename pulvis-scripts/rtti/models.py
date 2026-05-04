from dataclasses import dataclass
from pathlib import Path
from typing import List, Optional, Union

##################
@dataclass
class ModelField:
    name: str
    type: str
    tags: List[str]

@dataclass
class ModelMethod:
    name: str
    return_type: str
    parameters: List[ModelField]
    tags: List[str]

@dataclass
class ModelClass:
    namespace: str
    name: str
    parents: List[str]
    fields: List[ModelField]
    methods: List[ModelMethod]
    class_tags: List[str] = None  # Class attributes like "ScriptableHook"

    def __post_init__(self):
        if self.class_tags is None:
            self.class_tags = []

    def has_scriptable_hook(self) -> bool:
        return "ScriptableHook" in self.class_tags

    def inherits_from_ievent(self) -> bool:
        from markers import EVENT_BASE_QUALIFIED_NAMES
        return any(parent.strip() for parent in self.parents 
                  if any(event_base in parent for event_base in EVENT_BASE_QUALIFIED_NAMES))

    def needs_script_hook_generation(self) -> bool:
        return self.has_scriptable_hook() and self.inherits_from_ievent()

    def get_script_name(self) -> str:
        script_name = self.name
        if script_name.startswith('C') and len(script_name) > 1 and script_name[1].isupper():
            script_name = script_name[1:]
        if script_name.endswith('Event'):
            script_name = script_name[:-5]
        return script_name

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
    