#pragma once

#include "RenderLayer.hpp"
#include "RenderTarget.hpp"

#include <vector>
#include <unordered_map>
#include <string>
#include <string_view>

namespace pulvis::rendering
{
	class CRenderLayerCache
	{
		public:

			SRenderLayerHandle Register(const SRenderLayerDesc& _desc);
			SRenderLayerHandle Find(std::string_view _name) const;

			const SRenderLayerDesc& Get(SRenderLayerHandle _handle) const;
			const std::vector<SRenderLayerDesc>& All() const { return m_Layers; }

			const std::vector<SRenderLayerHandle>& GetSortedLayers() const;

			void Clear();

		private:

			void RebuildSortCache() const;

			std::vector<SRenderLayerDesc> m_Layers;
			std::unordered_map<std::string, uint32_t> m_ByName;
			mutable std::vector<SRenderLayerHandle> m_SortedCache;
			mutable bool m_SortDirty = true;
	};
}