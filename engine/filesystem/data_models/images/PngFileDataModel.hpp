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

		private:

			void* m_Buffer;
			int m_Width;
			int m_Height;
			int m_Channels;
	};
}