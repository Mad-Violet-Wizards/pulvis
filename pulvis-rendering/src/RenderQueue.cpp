#include "RenderQueue.hpp"

#include <algorithm>

namespace pulvis::rendering
{
//////////////////////////////////////////////////////////////////////////
	namespace
	{
		const std::vector<SSpriteDrawCmd> g_EmptySprites;
		const std::vector<STileDrawCmd>   g_EmptyTiles;
	}

//////////////////////////////////////////////////////////////////////////
	void CRenderQueue::PushSprite(const SSpriteDrawCmd& _cmd)
	{
		m_SpriteBuckets[_cmd.Layer].push_back(_cmd);
	}

	void CRenderQueue::PushTileBatch(const STileDrawCmd& _cmd)
	{
		m_TileBuckets[_cmd.Layer].push_back(_cmd);
	}

	const std::vector<SSpriteDrawCmd>& CRenderQueue::GetSpritesForLayer(SRenderLayerHandle _layer) const
	{
		auto it = m_SpriteBuckets.find(_layer);
		return it != m_SpriteBuckets.end() ? it->second : g_EmptySprites;
	}

	const std::vector<STileDrawCmd>& CRenderQueue::GetTilesForLayer(SRenderLayerHandle _layer) const
	{
		auto it = m_TileBuckets.find(_layer);
		return it != m_TileBuckets.end() ? it->second : g_EmptyTiles;
	}

	void CRenderQueue::SortAll()
	{
		for (auto& [layer, sprites] : m_SpriteBuckets)
		{
			std::sort(sprites.begin(), sprites.end(),
				[](const SSpriteDrawCmd& _a, const SSpriteDrawCmd& _b)
				{
					return _a.SortOrder < _b.SortOrder;
				});
		}
	}

	void CRenderQueue::Clear()
	{
		for (auto& [layer, sprites] : m_SpriteBuckets) sprites.clear();
		for (auto& [layer, tiles] : m_TileBuckets)   tiles.clear();
	}
}