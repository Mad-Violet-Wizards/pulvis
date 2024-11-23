#pragma once

#include <filesystem>
#include <optional>

#include "FileHandle.hpp"

namespace engine
{
namespace fs
{
	class PULVIS_API Filesystem
	{
		public:

			Filesystem(const std::string& _name, const std::string& _absolute_path);
			~Filesystem();

			void Mount();
			void Unmount();
			bool IsMounted() const;

			std::optional<CFileHandle> OpenFile(const std::string& _relative_path, std::shared_ptr<IFileDataModel>* _file_data_model, EFileMode _open_mode);
			std::optional<CFileHandle> OpenFile(const std::string& _relative_path, EFileMode _open_mode);

		private:

			bool GetCreateFileIfNotExists(EFileMode _open_mode) const;

		private:

			class Impl;
			Impl* m_Impl = nullptr;
	};
}
}
