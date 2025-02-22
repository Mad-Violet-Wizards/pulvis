#include "engine/engine_pch.hpp"

#include <fmt/format.h>

#include "Filesystem.hpp"
#include "engine/events/events/FilesystemMountedEvent.hpp"

namespace engine::fs
{

	//////////////////////////////////////////////////////////////////////////
		Filesystem::Filesystem(const std::string& _name, const std::string& _absolute_path)
		{
			m_Name = _name;
			m_AbsolutePath = _absolute_path;
			m_Mounted = false;
		}

		Filesystem::~Filesystem()
		{
		}

		void Filesystem::Mount()
		{
			if (!std::filesystem::exists(m_AbsolutePath))
			{
				std::cout << "[Filesystem] " << m_Name << " creating directory : " << m_AbsolutePath << "\n";
				std::filesystem::create_directory(m_AbsolutePath);
			}

			for (const auto& entry : std::filesystem::recursive_directory_iterator(m_AbsolutePath))
			{
				std::string path = entry.path().string();
				path.erase(0, m_AbsolutePath.string().size());

				if (path[0] == '\\' || path[0] == '/')
					path.erase(0, 1);

				std::cout << "[Filesystem] " << m_Name << " mounted: " << path << "\n";
				m_MountedFilelist.push_back(path);
			}

			m_Mounted = true;
		}

		void Filesystem::Unmount()
		{

		}

		bool Filesystem::IsMounted() const
		{
			return m_Mounted;
		}

		std::optional<CFileHandle> engine::fs::Filesystem::OpenFile(const std::string& _relative_path, std::shared_ptr<IFileDataModel>* _file_data_model, EFileMode _open_mode)
		{
			std::filesystem::path file_path = m_AbsolutePath;
			file_path /= _relative_path;

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
			const std::filesystem::path file_path = m_AbsolutePath / _relative_path;
			
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

		void Filesystem::GetFilenamesInDirectory(const std::string& _relative_path, std::vector<std::string>& _out_filenames_list) const
		{
			const std::filesystem::path directory_path = m_AbsolutePath / _relative_path;

			for (const auto& entry : std::filesystem::directory_iterator(directory_path))
			{
				std::string path = entry.path().string();
				path.erase(0, directory_path.string().size());
				if (path.find("\\") == 0 || path.find("/") == 0)
				{
					path.erase(0, 1);
				}

				_out_filenames_list.push_back(path);
			}
		}

		bool Filesystem::GetFileExists(const std::string& _relative_path) const
		{
			const std::filesystem::path file_path = m_AbsolutePath / _relative_path;

			return std::filesystem::exists(file_path);
		}

		std::vector<std::string>::const_iterator Filesystem::FileListBegin() const
		{
			return m_MountedFilelist.cbegin();
		}

		std::vector<std::string>::const_iterator Filesystem::FileListEnd() const
		{
			return m_MountedFilelist.cend();
		}

		std::string Filesystem::GetFilename(const std::string& _filepath)
		{
			const size_t last_slash = _filepath.find_last_of("/\\");
			if (last_slash == std::string::npos)
			{
				return _filepath;
			}

			return _filepath.substr(last_slash + 1);
		}

		std::string Filesystem::GetFileExtension(const std::string& _filename)
		{
			const size_t last_dot = _filename.find_last_of('.');
			if (last_dot == std::string::npos)
			{
				return "";
			}

			return _filename.substr(last_dot);
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
