#pragma once

#include "RenderTarget.hpp"

#include <vector>
#include <unordered_map>
#include <string>
#include <string_view>

namespace pulvis::rendering
{
	class CRenderTargetCache
	{
		public:

			SRenderTargetHandle Register(const SRenderTargetDesc& _desc);
			SRenderTargetHandle Find(std::string_view _name) const;

			const SRenderTargetDesc& Get(SRenderTargetHandle _handle) const;
			const std::vector<SRenderTargetDesc>& All() const { return m_Targets; }

			void Clear();

		private:

			std::vector<SRenderTargetDesc> m_Targets;
			std::unordered_map<std::string, uint32_t> m_ByName;
	};
}