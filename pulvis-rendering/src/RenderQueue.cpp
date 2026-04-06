#include "RenderQueue.hpp"

#include <algorithm>

namespace pulvis::rendering
{
	void CRenderQueue::PushSprite(const SSpriteDrawCmd& _cmd)
	{
		m_SpriteDrawCommands.push_back(_cmd);
	}

	void CRenderQueue::PushTileBatch(const STileDrawCmd& _cmd)
	{
		m_TileDrawCommands.push_back(_cmd);
	}

	void CRenderQueue::SortAll()
	{
		std::stable_sort(m_TileDrawCommands.begin(), m_TileDrawCommands.end(),
			[](const STileDrawCmd& _a, const STileDrawCmd& _b)
			{
				return _a.Layer < _b.Layer;
			}
		);

		std::stable_sort(m_SpriteDrawCommands.begin(), m_SpriteDrawCommands.end(),
			[](const SSpriteDrawCmd& _a, const SSpriteDrawCmd& _b)
			{
				if (_a.Layer != _b.Layer) return _a.Layer < _b.Layer;
				return _a.SortOrder < _b.SortOrder;
			}
		);
	}

	void CRenderQueue::Clear()
	{
		m_SpriteDrawCommands.clear();
		m_TileDrawCommands.clear();
	}
}