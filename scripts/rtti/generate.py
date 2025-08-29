from models import Model, ModelClass, ModelEnum, ModelField, ModelMethod

###############################################################################
def generate_rtti_code(path, models: list[Model]):

    autogen_filename_hpp: str = path.stem + "_rtti_autogen.hpp"
    autogen_filename_cpp: str = path.stem + "_rtti_autogen.cpp"

    autogen_hpp_code: str = ""
    autogen_cpp_code: str = ""

    for model in models:
        match model:
            case ModelClass() as class_model:
                autogen_hpp_code += generate_rtti_code_class_hpp(autogen_filename_hpp, class_model)
                autogen_cpp_code += generate_rtti_code_cpp_class_cpp(autogen_filename_cpp, class_model)
            case ModelEnum() as enum_model:
                autogen_hpp_code += generate_rtti_code_enum_hpp(autogen_filename_hpp, enum_model)
                autogen_cpp_code += generate_rtti_code_enum_cpp(autogen_filename_cpp, enum_model)

    hpp_code = pack_hpp_code(autogen_hpp_code)
    cpp_code = pack_cpp_code(autogen_cpp_code, autogen_filename_hpp)

    # Write the generated code to the respective files
    autogen_file_hpp_path = path.parent / autogen_filename_hpp
    with open(autogen_file_hpp_path, "w") as hpp_file:
        hpp_file.write(hpp_code)

    autogen_file_cpp_path = path.parent / autogen_filename_cpp
    with open(autogen_file_cpp_path, "w") as cpp_file:
        cpp_file.write(cpp_code)

###############################################################################
def generate_rtti_code_class_hpp(path: str, class_model: ModelClass) -> str:
    return f"void AutoGenRtti_RegisterClass_{class_model.name}();\n"

def generate_rtti_code_cpp_class_cpp(path: str, class_model: ModelClass) -> str:
    src_code: str = f"void AutoGenRtti_RegisterClass_{class_model.name}()\n{{\n";

    # Save rtti class name
    rtti_class_variable_name = f"{class_model.name}_rtti"

    # Create rtti class instance and register it
    src_code += f"\tstatic engine::rtti::CRTTIClass {rtti_class_variable_name}(\"{class_model.name}\"));\n"
    src_code += f"\tengine::rtti::detail::CRTTIClassStorage::RegisterClass(&{rtti_class_variable_name});\n"

    # Attach parents
    parents: str = ", ".join([f"{parent}" for parent in class_model.parents])
    src_code += f"\tRegisterParents<{parents}>(\"{class_model.name}\");\n"

    # Register methods
    for method in class_model.methods:
        rtti_method_variable_name = f"{class_model.name}_{method.name}_rtti"
        src_code += f"\tstatic engine::rtti::CRTTIMethod {rtti_method_variable_name}(\"{class_model.name}\", \"{method.name}\", &{class_model.name}::{method.name});\n"
        src_code += f"\t{rtti_class_variable_name}.AddMethod(&{rtti_method_variable_name});\n"

    for field in class_model.fields:
        rtti_field_variable_name = f"{class_model.name}_{field.name}_rtti"
        src_code += f"\tstatic engine::rtti::CRTTIField {rtti_field_variable_name}(\"{field.name}\", &{class_model.name}::{field.name});\n"
        src_code += f"\t{rtti_class_variable_name}.AddField(&{rtti_field_variable_name});\n"
    src_code += f"}}\n"
    return src_code

###############################################################################
def generate_rtti_code_enum_hpp(path: str, enum_model: ModelEnum) -> str:
    return f"void AutoGenRtti_RegisterEnum_{enum_model.name}();\n"

def generate_rtti_code_enum_cpp(path: str, enum_model: ModelEnum) -> str:
    src_code: str = f"void AutoGenRtti_RegisterEnum_{enum_model.name}()\n{{\n"
    src_code += f"\tstatic engine::rtti::CRTTIEnum<{enum_model.name}> {enum_model.name}_rtti();\n"
    src_code += f"}}\n"
    return src_code

###############################################################################
def pack_hpp_code(autogen_hpp_code: str) -> str:
    packed_hpp_code: str = f"#pragma once\n"
    packed_hpp_code += "#include engine/rtti/RTTIMacros.hpp"
    packed_hpp_code += f"{autogen_hpp_code}"
    return packed_hpp_code

def pack_cpp_code(autogen_cpp_code: str, autogen_filename_hpp: str) -> str:
    packed_cpp_code: str = f'#include "{autogen_filename_hpp}"\n\n'
    packed_cpp_code += f"{autogen_cpp_code}"
    return packed_cpp_code