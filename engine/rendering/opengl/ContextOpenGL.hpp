#pragma once

#include "engine/rendering/opengl/RenderLayerOpenGL.hpp"

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

			CContext(engine::rendering::CWindow* _window);
			~CContext();

			CContext(const CContext&) = delete;
			CContext &operator=(const CContext&) = delete;

			CShaderOpenGL* FindShader(const std::string& _shader_name) const;
			void SetupShaders(std::vector<CShaderOpenGL*>&& _shaders_vec);

			CTextureOpenGL* FindTexture(const std::string& _texture_name) const;
			void SetupTextures(std::vector<CTextureOpenGL*>&& _textures_vec);

			CRenderLayerOpenGL* GetRenderLayer(ERenderLayerOpenGL _layer) const
			{
				return m_RenderLayers[static_cast<size_t>(_layer)];
			}

		private:

			void InitializeRenderLayers();

		private:

			std::vector<CShaderOpenGL*> m_Shaders;
			std::vector<CTextureOpenGL*> m_Textures;

			static constexpr size_t s_RenderLayersCount = static_cast<size_t>(ERenderLayerOpenGL::Count);
			std::array<CRenderLayerOpenGL*, s_RenderLayersCount> m_RenderLayers;

		};
	}
}