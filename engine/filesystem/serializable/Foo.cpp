#include "engine/engine_pch.hpp"
#include <cereal/archives/json.hpp>

#include "Foo.hpp"

namespace engine
{
namespace fs
{
	Foo::Foo()
		: IFileDataModel("")
		, m_X1(0)
		, m_X2(0)
		, m_X3(0)
		, m_X4(0)
	{
	}

	Foo::Foo(const std::string& _absolute_path, int _x1, int _x2, int _x3, int _x4)
		: IFileDataModel(_absolute_path)
		, m_X1(_x1)
		, m_X2(_x2)
		, m_X3(_x3)
		, m_X4(_x4)
	{

	}

	void Foo::Serialize(std::fstream& _file_stream)
	{
		SerializeJSON(_file_stream, std::move(Wrapper(this)));
	}

	void Foo::Deserialize(std::fstream& _file_stream)
	{
		DeserializeJSON(_file_stream, std::move(Wrapper(this)));
	}
}
}

