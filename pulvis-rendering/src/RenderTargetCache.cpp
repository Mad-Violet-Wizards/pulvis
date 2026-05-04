#pragma once

#include "RenderTargetCache.hpp"
#include "Assert.hpp"

#include <algorithm>

namespace pulvis::rendering
{
	SRenderTargetHandle CRenderTargetCache::Register(const SRenderTargetDesc& _desc)
	{
		auto it = m_ByName.find(_desc.DebugName);

		if (it != m_ByName.end())
		{
			m_Targets[it->second - 1] = _desc;
			return { it->second };
		}

		m_Targets.push_back(_desc);
		const uint32_t id = static_cast<uint32_t>(m_Targets.size());
		m_ByName.emplace(_desc.DebugName, id);
		return { id };
	}

	SRenderTargetHandle CRenderTargetCache::Find(std::string_view _name) const
	{
		auto it = m_ByName.find(std::string(_name));
		return it != m_ByName.end() ? SRenderTargetHandle{ it->second } : SRenderTargetHandle{};
	}

	const SRenderTargetDesc& CRenderTargetCache::Get(SRenderTargetHandle _handle) const
	{
		ASSERT(_handle.Id > 0 && _handle.Id <= m_Targets.size(), "Invalid render target handle.");
		return m_Targets[_handle.Id - 1];
	}

	void CRenderTargetCache::Clear()
	{
		m_Targets.clear();
		m_ByName.clear();
	}
}