#pragma once

#include "FileSource.hpp"
#include "FilePath.hpp"
#include "FileTypes.hpp"

#include <array>
#include <vector>
#include <memory>
#include <shared_mutex>

namespace pulvis::fs
{
	class CDomainRoots;

	class CMountSystem
	{
		public:

			struct SMountPoint
			{
				CFilePath VirtualPrefix;
				std::shared_ptr<IFileSource> Source;
				EMountTag Tag = EMountTag::Base;
			};

			struct SResolvedPath
			{
				IFileSource* Source = nullptr;
				CFilePath LocalPath;

				explicit operator bool() const { return Source != nullptr; }
			};

		public:

			CMountSystem() = default;

			CMountSystem(const CMountSystem&) = delete;
			CMountSystem& operator=(const CMountSystem&) = delete;
			CMountSystem(CMountSystem&) noexcept = default;
			CMountSystem& operator=(CMountSystem&) noexcept = default;

			void BootstrapDomains(const CDomainRoots& _domain_roots);

			void Mount(EDomain _doman, const CFilePath& _virtual_prefix, std::shared_ptr<IFileSource> _source, EMountTag _tag = EMountTag::Base);
			void Unmount(EDomain _domain, const CFilePath& _virtual_prefix);

			SResolvedPath Resolve(EDomain _domain, const CFilePath& _virtual_path, bool check_writable) const;
			std::vector<SResolvedPath> ResolveAll(EDomain _domain, const CFilePath& _virtual_path) const;

			/*
				ResolveAny - Searches across ALL domains in fixed order:
				Game -> Engine -> User -> Dev.
				Convenience method for reads where the caller does not care
				which domain owns the file.
			*/
			SResolvedPath ResolveAny(const CFilePath& _virtual_path) const;

			bool HasMount(EDomain _domain, const CFilePath& _virtual_prefix) const;
			size_t MountCount(EDomain _domain) const;

			private:

				struct SDomainData
				{
					std::vector<SMountPoint> MountPoints;
					mutable std::shared_mutex Mutex;
				};

				SDomainData& GetDomain(EDomain _domain);
				const SDomainData& GetDomain(EDomain _domain) const;

				static void SortMountPoints(std::vector<SMountPoint>& _mount_points);
				static CFilePath StripPrefix(const CFilePath& _path, const CFilePath& _prefix);

			private:

				std::array<SDomainData, DOMAIN_COUNT> m_Domains;
	};
}