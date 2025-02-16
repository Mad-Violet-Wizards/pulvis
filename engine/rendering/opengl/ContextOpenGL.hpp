#pragma once

namespace engine::rendering
{
	class CWindow;

	namespace opengl
	{
		class CShader;

		class CContext
		{
		public:

			CContext();
			CContext(engine::rendering::CWindow* _window);
			~CContext();

			CContext(const CContext&) = delete;
			CContext &operator=(const CContext&) = delete;

			CShader* FindShader(const std::string& _shader_name);
			void SetupShaders(const std::vector<CShader*>& _shaders_vec);

		private:

			std::vector<CShader*> m_Shaders;
		};
	}
}