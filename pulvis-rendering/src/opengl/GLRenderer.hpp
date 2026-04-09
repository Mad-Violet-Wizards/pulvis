#pragma once

#include "Window.hpp"
#include "Camera2D.hpp"
#include "GLShader.hpp"
#include "GLRenderDevice.hpp"
#include "RenderQueue.hpp"
#include "opengl/GLSpriteRenderer.hpp"
#include "opengl/GLTileRenderer.hpp"

namespace pulvis::fs::assets { class CAssetRegistry; }

namespace pulvis::rendering::gl
{
	class CGLRenderer : public IRenderer
	{
		public:

			explicit CGLRenderer(pulvis::fs::assets::CAssetRegistry& _asset_registry);

			void Initialize() override;
			void BeginFrame() override;
			void Frame() override;
			void EndFrame() override;
			void Shutdown() override;
			bool GetShouldClose() override;

			CRenderQueue& GetRenderQueue() override { return m_RenderQueue; }
			const CRenderQueue& GetRenderQueue() const override { return m_RenderQueue; }

			CGLRenderDevice& GetRenderDevice()  { return m_Device; }
			const CGLRenderDevice& GetRenderDevice() const { return m_Device; }

			CCamera2D& GetCamera() { return m_Camera; }
			const CCamera2D& GetCamera() const { return m_Camera; }

			CWindow& GetWindow() { return m_Window; }
			const CWindow& GetWindow() const { return m_Window; }

		private:

			pulvis::fs::assets::CAssetRegistry& m_AssetRegistry;

			CWindow m_Window;
			CGLRenderDevice m_Device;
			CCamera2D m_Camera;
			CGLSpriteRenderer m_SpriteRenderer;
			CGLTileRenderer m_TileRenderer;
			CRenderQueue m_RenderQueue;
	};
}