#pragma once

#include "engine/resources/TilesContext.hpp"

namespace engine::resources
{
	class CResourceService : public Singleton<CResourceService>
	{
		public:

			CResourceService();
			~CResourceService();
			
			void RegisterRTTI();

			void LoadTileDefinitions();
			void LoadTile(ITile* _tile);

		private:

			CTilesContext m_TilesContext;
	};
}
