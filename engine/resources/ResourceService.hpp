#pragma once

#include "engine/resources/TilesContext.hpp"

namespace engine::resources
{
	class CResourceService : public Singleton<CResourceService>
	{
		public:

			CResourceService();
			~CResourceService();
			
			void LoadTileDefinitions();
			void LoadTileDefinition(IScriptTile* _tile);

			const CTilesContext& GetTilesContext() const { return m_TilesContext; }

		private:

			CTilesContext m_TilesContext;
	};
}
