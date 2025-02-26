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

			void Clear();
			void LoadTile(ITile* _tile);

			const std::vector<CTile*>& GetTilesConstRef() const;
			const std::vector<CAtlasTile*>& GetAtlasTilesConstRef() const;

		private:

			std::vector<CTile*> m_Tiles;
			std::vector<CAtlasTile*> m_AtlasTiles;
	};
}