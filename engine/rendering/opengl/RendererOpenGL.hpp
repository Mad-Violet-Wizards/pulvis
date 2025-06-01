#pragma once

#include "engine/rendering/Renderer.hpp"
#include "engine/rendering/Window.hpp"

namespace engine::rendering::opengl
{
	class CContext;
	class CShaderOpenGL;
	class CTextureOpenGL;

	class RendererOpenGL : public IRenderer
	{
		public:

			RendererOpenGL();
			~RendererOpenGL();

			void Initialize() override;
			void BeginFrame() override;
			void Frame() override;
			void EndFrame() override;
			void Shutdown() override;

			bool ShouldClose() const override;

			const engine::rendering::opengl::CContext& GetContext() const { return *m_Context; }
			void SetupShaders(std::vector<CShaderOpenGL*>&& _shaders_vec);
			void SetupTextures(std::vector<CTextureOpenGL*>&& _textures_vec);

			engine::rendering::CWindow* GetWindow() const { return m_Window; }

	private:

		engine::rendering::CWindow* m_Window;
		engine::rendering::opengl::CContext* m_Context;


		bool m_Initialized;
	};
}