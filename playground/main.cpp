#include <iostream>

#include "engine/engine_pch.hpp"
#include "engine/filesystem/FileHandle.hpp"
#include "engine/filesystem/serializable/Foo.hpp"

int main()
{
	std::cout << "[Playground] Hello world.\n";
	engine::fs::CFileHandle<Foo> foo_file_handle("test.dat", 1, 2, 3, 4);
	foo_file_handle.Open(engine::fs::EFileMode::Write | engine::fs::EFileMode::Binary);
	foo_file_handle.Serialize();
	foo_file_handle.Close();
	return 0;
}