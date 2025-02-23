#pragma once

namespace engine::resources
{
	class CResourceService : public Singleton<CResourceService>
	{
		public:

			CResourceService();
			~CResourceService();
			
			void RegisterRTTI();

	};
}
