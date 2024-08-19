#include <iostream>

#include "engine/engine_pch.hpp"
#include "engine/filesystem/FileHandle.hpp"
#include "engine/filesystem/serializable/Foo.hpp"
#include "engine/core/Setup.hpp"
#include "engine/core/Application.hpp"
#include "engine/filesystem/Utils.hpp"
#include "engine/filesystem/Filesystem.hpp"

int main()
{
	engine::core::ApplicationSetup app_setup;
    app_setup.m_ClientApp = engine::core::EClientApp::Playground;
	engine::core::Application app(app_setup);

	// std::shared_ptr<engine::fs::IFileDataModel> data_model_foo = std::make_shared<engine::fs::Foo>(1, 2, 3, 4);
	// engine::fs::CFileHandle foo_file_handle("test1.json", &data_model_foo);
	// if (foo_file_handle.Open(engine::fs::EFileMode::ReadWrite | engine::fs::EFileMode::Truncate))
	// {
	// 	std::cout << "[0] File opened succesfully.\n";
	// 	foo_file_handle.Serialize();
	// 	std::cout << "[1] Test Foo X1 (after serialization): " << std::dynamic_pointer_cast<engine::fs::Foo>(data_model_foo)->GetX1() << "\n";
	// 	std::dynamic_pointer_cast<engine::fs::Foo>(data_model_foo)->SetX1(123);
	// 	std::cout << "[2] Test Foo X1 (set x1): " << std::dynamic_pointer_cast<engine::fs::Foo>(data_model_foo)->GetX1() << "\n";
	// 	foo_file_handle.Deserialize();
	// 	std::cout << "[3] Test Foo X1 (after deserialization): " << std::dynamic_pointer_cast<engine::fs::Foo>(data_model_foo)->GetX1() << "\n";
	// 	foo_file_handle.Close();
	// }
	// else
	// {
	// 	std::cout << "[0] File failed to open.\n";
	// }

	std::cout << "Engine fs initialized: " << app.GetEngineFs().GetIsMounted() << "\n";

	PULVIS_FATAL_LOG("Hello FATAL COUNT ({}): {}", 0, "world");
	PULVIS_ERROR_LOG("Hello ERROR COUNT ({}): {}", 1, "world");
	PULVIS_WARNING_LOG("Hello WARNING COUNT ({}): {}", 2, "world");
	PULVIS_INFO_LOG("Hello INFO COUNT ({}): {}", 3, "world");
	PULVIS_DEBUG_LOG("Hello DEBUG COUNT ({}): {}", 4, "world");

	return 0;
}