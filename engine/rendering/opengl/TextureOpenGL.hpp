#pragma once

#include "engine/filesystem/data_models/images/PngFileDataModel.hpp"

namespace engine::rendering::opengl
{
	class CTextureOpenGL
	{
		public:

			CTextureOpenGL();
			CTextureOpenGL(const std::string& _texture_name);
			~CTextureOpenGL() = default;

			const std::string& GetTextureName() const { return m_TextureName; }
			void LoadTexture(engine::fs::data_models::CPngFileDataModel* _png_texture);

			void BindTexture() const;
			void UnbindTexture() const;

		private:

			std::string m_TextureName;
			unsigned int m_TextureID;
			
			void* m_Data;
			int m_Width;
			int m_Height;
			int m_Channels;
	};
}