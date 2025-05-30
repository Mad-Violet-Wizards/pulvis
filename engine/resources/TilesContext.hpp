#pragma once
#include "ScriptTile.hpp"

namespace engine::resources
{
	class CTilesContext
	{
		public:

			CTilesContext();
			~CTilesContext();

			void Clear();
			void LoadTileDefinition(IScriptTile* _tile);

			const std::vector<CScriptTile*>& GetTilesConstRef() const;
			const std::vector<CScriptAtlasTile*>& GetAtlasTilesConstRef() const;

			void FillTilesToLoad(std::vector<IScriptTile*>& _tiles) const;

		private:

			std::vector<CScriptTile*> m_TilesDefinitions;
			std::vector<CScriptAtlasTile*> m_AtlasTilesDefinitions;
	};
}