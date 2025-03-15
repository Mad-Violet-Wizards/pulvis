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
			void LoadTileDefinition(ITile* _tile);

			const CTilesContext& GetTilesContext() const { return m_TilesContext; }

		private:

			CTilesContext m_TilesContext;
	};
}
