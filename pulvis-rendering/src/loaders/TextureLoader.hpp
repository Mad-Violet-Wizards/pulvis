#pragma once

#include "assets/AssetLoader.hpp"
#include "opengl/GLRenderDevice.hpp"

namespace pulvis::rendering
{
	class CTextureLoader : public fs::assets::IAssetLoader
	{
		public:

			explicit CTextureLoader(gl::CGLRenderDevice& _renderDevice);

			bool Process(fs::assets::SAssetEntry& _entry) override;
			bool Unload(fs::assets::SAssetEntry& _entry) override;

		private:

			gl::CGLRenderDevice& m_RenderDevice;
	};
};
