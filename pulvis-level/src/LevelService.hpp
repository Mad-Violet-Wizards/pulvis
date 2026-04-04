#pragma once

namespace pulvis::fs
{
	class CMountSystem;

	namespace assets
	{
		class CAssetRegistry;
	}
}

namespace pulvis::level
{
	class CLevelService
	{
		public:

			CLevelService(pulvis::fs::assets::CAssetRegistry& _asset_registry, pulvis::fs::CMountSystem& _mount_system);
			~CLevelService() = default;

			CLevelService(const CLevelService&) = delete;
			CLevelService& operator=(const CLevelService&) = delete;

			void Initialize();
			void Shutdown();

		private:

			pulvis::fs::assets::CAssetRegistry& m_AssetRegistry;
			pulvis::fs::CMountSystem& m_MountSystem;
	};
}