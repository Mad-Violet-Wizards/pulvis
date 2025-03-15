#pragma once

namespace engine::rendering
{
	class CWindow;

	namespace opengl
	{
		class CShaderOpenGL;
		class CTextureOpenGL;

		class CContext
		{
		public:

			CContext();
			CContext(engine::rendering::CWindow* _window);
			~CContext();

			CContext(const CContext&) = delete;
			CContext &operator=(const CContext&) = delete;

			CShaderOpenGL* FindShader(const std::string& _shader_name) const;
			void SetupShaders(std::vector<CShaderOpenGL*>&& _shaders_vec);

			CTextureOpenGL* FindTexture(const std::string& _texture_name) const;
			void SetupTextures(std::vector<CTextureOpenGL*>&& _textures_vec);

		private:

			std::vector<CShaderOpenGL*> m_Shaders;
			std::vector<CTextureOpenGL*> m_Textures;
		};
	}
}