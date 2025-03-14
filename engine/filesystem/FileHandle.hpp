#pragma once

#include <fstream>
#include <format>
#include <string.h>
#include <errno.h>
#include <fmt/format.h>

#include "FileDataModel.hpp"
#include "Serializable.hpp"
#include "TextFileDataModel.hpp"
#include "ShaderFileDataModel.hpp"
#include "ScriptFileDataModel.hpp"

namespace engine
{
	namespace fs
	{
		enum class EFileMode
		{
			Read = 0x01,
			Write = 0x02,
			ReadWrite = 0x04,
			Append = 0x08,
			Truncate = 0x16,
			Binary = 0x32
		};

		inline EFileMode operator|(EFileMode _lhs, EFileMode _rhs)
		{
			return static_cast<EFileMode>(static_cast<int>(_lhs) | static_cast<int>(_rhs));
		}

		inline int operator&(EFileMode _lhs, EFileMode _rhs)
		{
			return static_cast<int>(_lhs) & static_cast<int>(_rhs);
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