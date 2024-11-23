#include "engine/engine_pch.hpp"

#include <fmt/format.h>

#include "Filesystem.hpp"
#include "engine/events/events/FilesystemMountedEvent.hpp"

namespace engine::fs
{

	class Filesystem::Impl
	{
		public:

			bool m_Mounted;
			std::string m_Name;
			std::filesystem::path m_AbsolutePath;

			std::vector<std::string> m_MountedFilelist;
	};

	//////////////////////////////////////////////////////////////////////////
		Filesystem::Filesystem(const std::string& _name, const std::string& _absolute_path)
		{
			m_Impl = new Impl();
			m_Impl->m_Name = _name;
			m_Impl->m_AbsolutePath = _absolute_path;
			m_Impl->m_Mounted = false;
		}

		Filesystem::~Filesystem()
		{
			delete m_Impl;
		}

		void Filesystem::Mount()
		{
			if (!std::filesystem::exists(m_Impl->m_AbsolutePath))
			{
				std::cout << "[Filesystem] " << m_Impl->m_Name << " creating directory : " << m_Impl->m_AbsolutePath << "\n";
				std::filesystem::create_directory(m_Impl->m_AbsolutePath);
			}

			for (const auto& entry : std::filesystem::directory_iterator(m_Impl->m_AbsolutePath))
			{
				std::string path = entry.path().string();
				path.erase(0, m_Impl->m_AbsolutePath.string().size());

				std::cout << "[Filesystem] " << m_Impl->m_Name << " mounted: " << path << "\n";
				m_Impl->m_MountedFilelist.push_back(path);
			}

			m_Impl->m_Mounted = true;
		}

		void Filesystem::Unmount()
		{

		}

		bool Filesystem::IsMounted() const
		{
			return m_Impl->m_Mounted;
		}

		std::optional<CFileHandle> engine::fs::Filesystem::OpenFile(const std::string& _relative_path, std::shared_ptr<IFileDataModel>* _file_data_model, EFileMode _open_mode)
		{
			const std::filesystem::path file_path = m_Impl->m_AbsolutePath / _relative_path;

			ASSERT(_file_data_model, "[Filesystem] FileDataModel is nullptr.");

			const bool create_if_no_exists = GetCreateFileIfNotExists(_open_mode);

			if (!create_if_no_exists)
			{
				const std::string file_not_exists_error_msg = fmt::format("[Filesystem] File not found: {0}", file_path.string());
				ASSERT(std::filesystem::exists(file_path), file_not_exists_error_msg);
			}

			if (!std::filesystem::exists(file_path))
			{
				if (!create_if_no_exists)
					return std::nullopt;
			}
			
			CFileHandle file_handle(file_path.string(), _file_data_model);
			if (file_handle.Open(_open_mode))
			{
				return file_handle;
			}

			return std::nullopt;
		}

		std::optional<CFileHandle> engine::fs::Filesystem::OpenFile(const std::string& _relative_path, EFileMode _open_mode)
		{
			const std::filesystem::path file_path = m_Impl->m_AbsolutePath / _relative_path;
			
			const bool create_if_no_exists = GetCreateFileIfNotExists(_open_mode);

			if (!create_if_no_exists)
			{
				const std::string file_not_exists_error_msg = std::format("[Filesystem] File not found: {0}", file_path.string());
				ASSERT(std::filesystem::exists(file_path), file_not_exists_error_msg);
			}

			if (!std::filesystem::exists(file_path))
			{
				if (!create_if_no_exists)
					return std::nullopt;
			}

			CFileHandle file_handle(file_path.string());
			if (file_handle.Open(_open_mode))
			{
				return file_handle;
			}

			return std::nullopt;
		}
		bool Filesystem::GetCreateFileIfNotExists(EFileMode _open_mode) const
		{
			if (_open_mode & EFileMode::Write)
			{
				return true;
			}
			else if (_open_mode & EFileMode::ReadWrite)
			{
				return true;
			}
			else if (_open_mode & EFileMode::Append)
			{
				return true;
			}
			else if (_open_mode & EFileMode::Truncate)
			{
				return true;
			}

			return false;
	}
}
