#pragma once

namespace engine::rendering::opengl
{
	class CTextureOpenGL
	{
		public:

			CTextureOpenGL();
			~CTextureOpenGL() = default;

			void LoadTexture(const std::string& _texture_path);
	
		private:

			unsigned int m_TextureID;
			
			void* m_Data;
			int m_Width;
			int m_Height;
			int m_Channels;
	};
}