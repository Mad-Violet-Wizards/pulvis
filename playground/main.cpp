#include <iostream>

#include "engine/engine_pch.hpp"
#include "engine/threads/ThreadPoolSettings.hpp"
#include "engine/threads/ThreadPool.hpp"
#include "engine/filesystem/FileHandle.hpp"
#include "engine/filesystem/serializable/Foo.hpp"
#include "engine/core/Setup.hpp"
#include "engine/core/Application.hpp"
#include "engine/filesystem/Utils.hpp"
#include "engine/filesystem/Filesystem.hpp"

int Test_Thread_Function_A()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(256));
	return 111;
}

void Test_Thread_Function_B(int& _val)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(128));
	_val = 111;
}

int Test_Thread_Function_C(int&& _a, int&& _b)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(64));
	return _a + _b;
}

int main()
{
	engine::core::ApplicationSetup app_setup;
	app_setup.m_WindowWidth = 800;
	app_setup.m_WindowHeight = 600;
	app_setup.m_WindowName = "Pulvis Playground (1.0.0)";
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

	engine::threads::SThreadPoolSettings settings; 
	settings.m_NumThreads = 2;
	engine::threads::CThreadPool tp(settings);

	engine::threads::CThreadTask task1(Test_Thread_Function_A);
	tp.EnqueueTask(&task1);
	int future1 = task1.GetFuture<int>().get();

	int t = 2;
	engine::threads::CThreadTask task2(Test_Thread_Function_B, t);
	tp.EnqueueTask(&task2);

	std::future<void> future2 = task2.GetFuture<void>();

	while (future2.wait_for(std::chrono::milliseconds(16)) != std::future_status::ready)
	{
		std::cout << "Waiting for T.\n";
	}
	std::cout << "T ready!\n";

	engine::threads::CThreadTask task3(Test_Thread_Function_C, future1, t);
	tp.EnqueueTask(&task3);
	std::future<int> future3 = task3.GetFuture<int>();

	while (future3.wait_for(std::chrono::milliseconds(16)) != std::future_status::ready)
	{
		std::cout << "Waiting for Future3.\n";
	}

	int future3_ret = future3.get();

	std::cout << "Future3: " << future3_ret << "\n";

	tp.Stop();

	return 0;
}