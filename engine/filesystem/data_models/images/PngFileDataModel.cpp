#include "engine/engine_pch.hpp"
#include "PngFileDataModel.hpp"

#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
#include <vendor/common/include/stb/stb_image.h>

namespace engine::fs::data_models
{
	CPngFileDataModel::CPngFileDataModel()
		: m_Buffer(nullptr)
		, m_Width(0)
		, m_Height(0)
		, m_Channels(0)
	{
	}

	CPngFileDataModel::~CPngFileDataModel()
	{
		if (m_Buffer)
		{
			delete m_Buffer;
			m_Buffer = nullptr;
		}
	}

	void CPngFileDataModel::Deserialize(std::fstream& _file_stream)
	{
		_file_stream.seekg(0, std::ios::end);
		std::streampos l = _file_stream.tellg();
		_file_stream.seekg(0, std::ios::beg);

		const unsigned int size = static_cast<unsigned int>(l);
		std::vector<char> buffer(size);
		_file_stream.read(buffer.data(), l);
		stbi_set_flip_vertically_on_load(true);
		m_Buffer = stbi_load_from_memory(reinterpret_cast<unsigned char*>(buffer.data()), size, &m_Width, &m_Height, &m_Channels, 0);
	}
}
