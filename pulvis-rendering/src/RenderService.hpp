#pragma once

#include "IRenderer.hpp"
#include "RendererType.hpp"
#include "Window.hpp"

#include <string>
#include <cstdint>
#include <memory>

namespace pulvis::fs::assets { class CAssetRegistry; }

namespace pulvis::rendering
{
	class CRenderService
	{
		public:

			explicit CRenderService(pulvis::fs::assets::CAssetRegistry& _asset_registry);
			~CRenderService();

			void Initialize(ERendererType _renderer_type,
				uint32_t _window_width, uint32_t _window_height,
				const std::string& _window_title);			
			void Shutdown();

			void BeginFrame();
			void Frame();
			void EndFrame();

			bool IsInitialized() const;
			bool GetShouldClose() const;

			IRenderer* GetRenderer();
			const IRenderer* GetRenderer() const;

		private:

			pulvis::fs::assets::CAssetRegistry& m_AssetRegistry;
			std::unique_ptr<IRenderer> m_Renderer = nullptr;
	};
}