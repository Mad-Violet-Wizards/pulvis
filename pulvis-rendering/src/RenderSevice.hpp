#pragma once

#include "IRenderer.hpp"
#include "RendererType.hpp"
#include "Window.hpp"

namespace pulvis::rendering
{
	class CRenderService
	{
		public:

			~CRenderService() = default;

			static CRenderService& GetInstance();

			void Initialize(ERendererType _renderer_type);
			void Shutdown();

			void Frame();
			bool IsInitialized() const;
			bool GetShouldClose() const;

///////////////////////////////////////////////////////////////////////////////////////////////////
			IRenderer* GetRenderer();
			const IRenderer* GetRenderer() const;
///////////////////////////////////////////////////////////////////////////////////////////////////

		private:

			CRenderService() = default;

		private:

			IRenderer* m_Renderer = nullptr;
	};
}