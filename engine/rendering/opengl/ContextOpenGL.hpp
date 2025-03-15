#pragma once

namespace engine::rendering
{
	class CWindow;

	namespace opengl
	{
		class CShaderOpenGL;

		class CContext
		{
		public:

			CContext();
			CContext(engine::rendering::CWindow* _window);
			~CContext();

			CContext(const CContext&) = delete;
			CContext &operator=(const CContext&) = delete;

			CShaderOpenGL* FindShader(const std::string& _shader_name);
			void SetupShaders(const std::vector<CShaderOpenGL*>& _shaders_vec);

		private:

			std::vector<CShaderOpenGL*> m_Shaders;
		};
	}
}