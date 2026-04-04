#include "MountSystem.hpp"
#include "DomainRoots.hpp"
#include "Logger.hpp"
#include "FileSourceDisk.hpp"

#include <algorithm>

namespace pulvis::fs
{
	void CMountSystem::BootstrapDomains(const CDomainRoots& _domain_roots)
	{
		struct SDomainDesc
		{
			EDomain Domain;
			const char* VirtualPrefix;
			bool ReadOnly;
		};

		constexpr SDomainDesc domain_descs[] = {
			{ EDomain::Engine, "engine", true },
			{ EDomain::Game, "game", true },
			{ EDomain::User, "user", false },
			{ EDomain::Dev, "dev", false }
		};

		for (const SDomainDesc& desc : domain_descs)
		{
			const std::filesystem::path& root = _domain_roots.GetRoot(desc.Domain);

			if (root.empty())
			{
				PULVIS_WARNING_LOG("Domain {} has no root path configured, skipping mount.",
					static_cast<int>(desc.Domain));
				continue;
			}

			std::shared_ptr<IFileSource> source = std::make_shared<CFileSourceDisk>(root, desc.ReadOnly);
			Mount(desc.Domain, CFilePath(desc.VirtualPrefix), std::move(source));
		}
	}

	void CMountSystem::Mount(EDomain _domain, const CFilePath& _virtual_prefix, std::shared_ptr<IFileSource> _source, EMountTag _tag)
	{
		SDomainData& domain = GetDomain(_domain);

		std::unique_lock lock(domain.Mutex);

		domain.MountPoints.push_back({ _virtual_prefix, std::move(_source), _tag });

		PULVIS_INFO_LOG("Mounted {} at virtual path '{}' in domain {} with tag {}.",
			domain.MountPoints.back().Source->Name(), _virtual_prefix.Str(), static_cast<int>(_domain), static_cast<int>(_tag));
		SortMountPoints(domain.MountPoints);
	}

	void CMountSystem::Unmount(EDomain _domain, const CFilePath& _virtual_prefix)
	{
		SDomainData& domain = GetDomain(_domain);
		std::unique_lock lock(domain.Mutex);
		domain.MountPoints.erase(std::remove_if(domain.MountPoints.begin(), domain.MountPoints.end(),
			[&](const SMountPoint& mp) { return mp.VirtualPrefix == _virtual_prefix; }), 
		domain.MountPoints.end());
		PULVIS_INFO_LOG("Unmounted virtual path '{}' in domain {}.", _virtual_prefix.Str(), static_cast<int>(_domain));
	}

	CMountSystem::SResolvedPath CMountSystem::Resolve(EDomain _domain, const CFilePath& _virtual_path, bool check_writable) const
	{
		const SDomainData& domain = GetDomain(_domain);
		std::shared_lock lock(domain.Mutex);

		for (const SMountPoint& mount_point : domain.MountPoints)
		{
			if (check_writable && mount_point.Source->IsReadOnly())
			{
				continue;
			}

			if (!_virtual_path.StartsWith(mount_point.VirtualPrefix.Str()))
			{
				continue;
			}

			const CFilePath local_path = StripPrefix(_virtual_path, mount_point.VirtualPrefix);
			if (mount_point.Source->Exists(local_path))
			{
				return { mount_point.Source.get(), local_path };
			}
		}

		return {};
	}

	std::vector<CMountSystem::SResolvedPath> CMountSystem::ResolveAll(EDomain _domain, const CFilePath& _virtual_path) const
	{
		const SDomainData& domain = GetDomain(_domain);

		std::shared_lock lock(domain.Mutex);
		std::vector<SResolvedPath> resolved_paths;
		for (const SMountPoint& mount_point : domain.MountPoints)
		{
			if (!_virtual_path.StartsWith(mount_point.VirtualPrefix.Str()))
			{
				continue;
			}

			const CFilePath local_path = StripPrefix(_virtual_path, mount_point.VirtualPrefix);
			if (mount_point.Source->Exists(local_path))
			{
				resolved_paths.push_back({ mount_point.Source.get(), local_path });
			}
		}

		return resolved_paths;
	}

	CMountSystem::SResolvedPath CMountSystem::ResolveAny(const CFilePath& _virtual_path) const
	{
		constexpr EDomain domain_order[] = { EDomain::Game, EDomain::Engine, EDomain::User, EDomain::Dev };

		for (EDomain domain : domain_order)
		{
			SResolvedPath resolved = Resolve(domain, _virtual_path, false);
			if (resolved)
			{
				return resolved;
			}
		}

		return {};
	}

	bool CMountSystem::HasMount(EDomain _domain, const CFilePath& _virtual_prefix) const
	{
		const SDomainData& domain = GetDomain(_domain);

		std::shared_lock lock(domain.Mutex);

		const bool contains = std::any_of(domain.MountPoints.begin(), domain.MountPoints.end(), [&](const SMountPoint& mp) 
		{ 
				return mp.VirtualPrefix == _virtual_prefix; 
		});

		return contains;
	}

	size_t CMountSystem::MountCount(EDomain _domain) const
	{
		const SDomainData& domain = GetDomain(_domain);
		std::shared_lock lock(domain.Mutex);
		return domain.MountPoints.size();
	}



	CMountSystem::SDomainData& CMountSystem::GetDomain(EDomain _domain)
	{
		return m_Domains[static_cast<unsigned int>(_domain)];
	}

	const CMountSystem::SDomainData& CMountSystem::GetDomain(EDomain _domain) const
	{
		return m_Domains[static_cast<unsigned int>(_domain)];
	}

	void CMountSystem::SortMountPoints(std::vector<SMountPoint>& _mount_points)
	{
		std::stable_sort(_mount_points.begin(), _mount_points.end(), [](const SMountPoint& a, const SMountPoint& b) 
		{
				return a.Tag > b.Tag;
		});
	}

	CFilePath CMountSystem::StripPrefix(const CFilePath& _path, const CFilePath& _prefix)
	{
		const std::string_view path_str = _path.Str();
		const std::string_view prefix_str = _prefix.Str();

		if (path_str.size() <= prefix_str.size())
		{
			return CFilePath("/");
		}

		std::string_view remainder = path_str.substr(prefix_str.size());
		if (!remainder.empty() && remainder.front() == '/')
		{
			remainder = remainder.substr(1);
		}

		return CFilePath(remainder);
	}
}