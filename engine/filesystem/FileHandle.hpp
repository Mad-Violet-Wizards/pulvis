#pragma once

#include <fstream>
#include <format>
#include <string.h>
#include <errno.h>
#include <fmt/format.h>

#include "FileDataModel.hpp"
#include "Serializable.hpp"

namespace engine
{
	namespace fs
	{
		enum class EFileMode
		{
			Read = 1,
			Write = 2,
			ReadWrite = 4,
			Append = 8,
			Truncate = 16,
			Binary = 32
		};

		inline EFileMode operator|(EFileMode _lhs, EFileMode _rhs)
		{
			return static_cast<EFileMode>(static_cast<int>(_lhs) | static_cast<int>(_rhs));
		}

		inline int operator&(EFileMode _lhs, EFileMode _rhs)
		{
			return static_cast<int>(_lhs) & static_cast<int>(_rhs);
		}

		inline EFileMode operator~(EFileMode _mode)
		{
			return static_cast<EFileMode>(~static_cast<int>(_mode));
		}

		class CFileHandle
		{
		public:

			// Not sure if this cstor is needed (If FileHandle always needs a FileDataModel?)
			CFileHandle(const std::string& _relative_path);
			CFileHandle(const std::string& _relative_path, std::shared_ptr<IFileDataModel>* _file_data_model);

			bool Open(EFileMode _open_mode);
			bool Close();
			bool IsOpen() const;

			void Serialize();
			void Deserialize();

			void SetFileDataModel(std::shared_ptr<IFileDataModel>* _file_data_model);

		private:

			void Serialize_Internal();
			void Deserialize_Internal();


		private:
			
			std::shared_ptr<IFileDataModel> *m_FileDataModel;

			std::string m_AbsolutePath;
			std::string m_RelativePath;

			std::fstream m_FileStream;

			bool m_ReadOnly;
		};
	}
}