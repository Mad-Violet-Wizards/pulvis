#pragma once

#include "engine/filesystem/FileDataModel.hpp"

namespace engine::fs::data_models
{
	class CPngFileDataModel : public IFileDataModel
	{
		public:
		
			CPngFileDataModel();
			~CPngFileDataModel();

			void Deserialize(std::fstream& _file_stream);
			EFileDataModelType GetFileDataModelType() const override { return EFileDataModelType::Png; }

			void* GetBuffer() const { return m_Buffer; }
			int GetWidth() const { return m_Width; }
			int GetHeight() const { return m_Height; }
			int GetChannels() const { return m_Channels; }

		private:

			void* m_Buffer;
			int m_Width;
			int m_Height;
			int m_Channels;
	};
}