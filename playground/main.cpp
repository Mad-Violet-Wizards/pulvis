#include <iostream>

#include "engine/engine_pch.hpp"
#include "engine/filesystem/FileHandle.hpp"
#include "engine/filesystem/serializable/Foo.hpp"
#include "engine/core/Assert.hpp"

int main()
{
	std::cout << "[Playground] Hello world.\n";

	std::shared_ptr<engine::fs::IFileDataModel> data_model_foo = std::make_shared<engine::fs::Foo>(1, 2, 3, 4);
	engine::fs::CFileHandle foo_file_handle("test1.json");
	foo_file_handle.SetFileDataModel(&data_model_foo);
	if (foo_file_handle.Open(engine::fs::EFileMode::ReadWrite | engine::fs::EFileMode::Truncate))
	{
		std::cout << "[0] File opened succesfully.\n";
		foo_file_handle.Serialize();
		std::cout << "[1] Test Foo X1 (after serialization): " << std::dynamic_pointer_cast<engine::fs::Foo>(data_model_foo)->GetX1() << "\n";
		std::dynamic_pointer_cast<engine::fs::Foo>(data_model_foo)->SetX1(123);
		std::cout << "[2] Test Foo X1 (set x1): " << std::dynamic_pointer_cast<engine::fs::Foo>(data_model_foo)->GetX1() << "\n";
		foo_file_handle.Deserialize();
		std::cout << "[3] Test Foo X1 (after deserialization): " << std::dynamic_pointer_cast<engine::fs::Foo>(data_model_foo)->GetX1() << "\n";
		foo_file_handle.Close();
	}
	else
	{
		std::cout << "[0] File failed to open.\n";
	}

	return 0;
}