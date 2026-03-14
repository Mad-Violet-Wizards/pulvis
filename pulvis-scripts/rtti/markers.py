RTTI_MARKERS = [
    "[[Reflection::Class]]",
    "[[Reflection::Field",
    "[[Reflection::Method",
    "[[Reflection::Enum]]"
]

RTTI_CLASS_MARKER = RTTI_MARKERS[0]
RTTI_FIELD_MARKER = RTTI_MARKERS[1]
RTTI_METHOD_MARKER = RTTI_MARKERS[2]
RTTI_ENUM_MARKER = RTTI_MARKERS[3]

RTTI_FIELD_NONE_ATTRIBUTE = "pulvis::rtti::ERTTIFieldAttribute::None"
RTTI_METHOD_NONE_ATTRIBUTE = "pulvis::rtti::ERTTIMethodAttribute::None"

RTTI_FIELD_ATTRIBUTE_MAP = {
    "Serializable": "pulvis::rtti::ERTTIFieldAttribute::Serializable",
    "Scriptable": "pulvis::rtti::ERTTIFieldAttribute::Scriptable",
    "Editable": "pulvis::rtti::ERTTIFieldAttribute::Editable",
    "EditorReadOnly": "pulvis::rtti::ERTTIFieldAttribute::EditorReadOnly",
    "Replicated": "pulvis::rtti::ERTTIFieldAttribute::Replicated",
}

RTTI_METHOD_ATTRIBUTE_MAP = {
    "Scriptable": "pulvis::rtti::ERTTIMethodAttribute::Scriptable",
    "EditorCallable": "pulvis::rtti::ERTTIMethodAttribute::EditorCallable",
    "Event": "pulvis::rtti::ERTTIMethodAttribute::Event",
    "Signal": "pulvis::rtti::ERTTIMethodAttribute::Signal",
    "Slot": "pulvis::rtti::ERTTIMethodAttribute::Slot",
    "Command": "pulvis::rtti::ERTTIMethodAttribute::Command",
    "Pure" : "pulvis::rtti::ERTTIMethodAttribute::Pure",
}
