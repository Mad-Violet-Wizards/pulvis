#pragma once

namespace engine::resources
{
	class ITile;
	class CTile;
	class CAtlasTile;

	class CTilesContext
	{
		public:

			CTilesContext();
			~CTilesContext();

			void Clear();
			void LoadTileDefinition(ITile* _tile);

			const std::vector<CTile*>& GetTilesConstRef() const;
			const std::vector<CAtlasTile*>& GetAtlasTilesConstRef() const;

		private:

			std::vector<CTile*> m_TilesDefinitions;
			std::vector<CAtlasTile*> m_AtlasTilesDefinitions;
	};
}