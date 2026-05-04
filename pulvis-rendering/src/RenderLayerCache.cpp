#include "RenderLayerCache.hpp"
#include <algorithm>
#include "Assert.hpp"

namespace pulvis::rendering
{
	SRenderLayerHandle CRenderLayerCache::Register(const SRenderLayerDesc& _desc)
	{
		auto it = m_ByName.find(_desc.DebugName);
		if (it != m_ByName.end())
		{
			m_Layers[it->second - 1] = _desc;
			m_SortDirty = true;
			return { it->second };
		}

		m_Layers.push_back(_desc);
		const uint32_t id = static_cast<uint32_t>(m_Layers.size());
		m_ByName.emplace(_desc.DebugName, id);
		m_SortDirty = true;
		return { id };
	}

	SRenderLayerHandle CRenderLayerCache::Find(std::string_view _name) const
	{
		auto it = m_ByName.find(std::string(_name));
		return it != m_ByName.end() ? SRenderLayerHandle{ it->second } : SRenderLayerHandle{};
	}

	const SRenderLayerDesc& CRenderLayerCache::Get(SRenderLayerHandle _handle) const
	{
		ASSERT(_handle.Id > 0 && _handle.Id <= m_Layers.size(), "Invalid render layer handle.");
		return m_Layers[_handle.Id - 1];
	}

	const std::vector<SRenderLayerHandle>& CRenderLayerCache::GetSortedLayers() const
	{
		if (m_SortDirty)
		{
			RebuildSortCache();
			m_SortDirty = false;
		}

		return m_SortedCache;
	}

	void CRenderLayerCache::RebuildSortCache() const
	{
		m_SortedCache.clear();
		m_SortedCache.reserve(m_Layers.size());

		for (uint32_t i = 0; i < m_Layers.size(); ++i)
		{
			m_SortedCache.push_back({ i + 1 });
		}

		std::sort(m_SortedCache.begin(), m_SortedCache.end(),
			[this](SRenderLayerHandle _a, SRenderLayerHandle _b)
			{
				return m_Layers[_a.Id - 1].SortKey < m_Layers[_b.Id - 1].SortKey;
			});

		m_SortDirty = false;
	}

	void CRenderLayerCache::Clear()
	{
		m_Layers.clear();
		m_ByName.clear();
		m_SortedCache.clear();
		m_SortDirty = true;
	}
}