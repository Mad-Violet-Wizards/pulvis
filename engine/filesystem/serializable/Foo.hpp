#pragma once

#include <fstream>
#include "engine/filesystem/FileDataModel.hpp"
#include <cereal/archives/portable_binary.hpp>
#include "engine/core/Export.hpp"

class PULVIS_API Foo : public IFileDataModel
{

public:

	Foo();
	Foo(const std::string& _absolute_path, int _x1, int _x2, int _x3, int _x4);

	void Serialize(std::fstream& _file_stream) override
	{
		cereal::PortableBinaryOutputArchive archive(_file_stream);
		archive(*this);
	}

	void Deserialize(std::fstream& _file_stream) override
	{
		cereal::PortableBinaryInputArchive archive(_file_stream);
		archive(*this);
	}

	template<class Archive>
	void serialize(Archive& ar)
	{
				ar(m_X1, m_X2, m_X3, m_X4);
	}

	private:

		int m_X1;
		int m_X2;
		int m_X3;
		int m_X4;

};