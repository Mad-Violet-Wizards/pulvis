#pragma once

#include <fstream>
#include <format>
#include <string.h>
#include <errno.h>


#include "FileDataModel.hpp"
#include "Serializable.hpp"

namespace engine
{
	namespace fs
	{
		enum class EFileMode
		{
			Read = 0x00,
			Write = 0x01,
			ReadWrite = 0x02,
			Append = 0x04,
			Truncate = 0x08,
			Binary = 0x10
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
			CFileHandle(const std::string& _relative_path)
				: m_ReadOnly(false)
				, m_RelativePath(_relative_path)
				, m_FileDataModel(nullptr)
			{
			}

			CFileHandle(const std::string& _relative_path, std::shared_ptr<IFileDataModel>* _file_data_model)
				: m_ReadOnly(false)
				, m_RelativePath(_relative_path)
				, m_FileDataModel(_file_data_model)
			{
			}

			bool Open(EFileMode _open_mode)
			{
				std::ios::openmode stream_open_mode = (std::ios_base::openmode)0x00;
				
				if (_open_mode & EFileMode::Read)
				{
					m_ReadOnly = true;
					stream_open_mode |= std::ios::in;
				}

				if (_open_mode & EFileMode::Write)
				{
					m_ReadOnly = false;
					stream_open_mode |= std::ios::out;
				}

				if (_open_mode & EFileMode::ReadWrite)
				{
					m_ReadOnly = false;
					stream_open_mode |= std::ios::out | std::ios::in;
				}

				if (_open_mode & EFileMode::Append)
				{
					m_ReadOnly = false;
					stream_open_mode |= std::ios::app;
				}

				if (_open_mode & EFileMode::Truncate)
				{
					m_ReadOnly = false;
					stream_open_mode |= std::ios::trunc;
				}

				if (_open_mode & EFileMode::Binary)
				{
					stream_open_mode |= std::ios::binary;
				}

				m_FileStream.open(m_RelativePath, stream_open_mode);
				
				if (!IsOpen())
				{
					char error_message_buffer[256];
					strerror_s(error_message_buffer, errno);
					const std::string error_message = std::format("Failed to open file: {} ({})", m_RelativePath, error_message_buffer);
					ASSERT(IsOpen(), error_message);
				}

				return IsOpen();
			}

			bool Close()
			{
				m_FileStream.close();
				return IsOpen();
			}

			bool IsOpen() const
			{
				return m_FileStream.is_open();
			}

			void Serialize()
			{
				ASSERT(m_FileDataModel != nullptr, "FileDataModel instance is nullptr.");
				m_FileStream.seekg(0, std::ios::beg);
				Serialize_Internal();
			}

			void Deserialize()
			{
				ASSERT(m_FileDataModel != nullptr, "FileDataModel instance is nullptr.");
				m_FileStream.seekg(0, std::ios::beg);
				Deserialize_Internal();
			}

			void SetFileDataModel(std::shared_ptr<IFileDataModel>* _file_data_model)
			{
				m_FileDataModel = _file_data_model;
			}

		private:

			void Serialize_Internal()
			{
				const EFileDataModelType data_model_type = (*m_FileDataModel)->GetFileDataModelType();

				switch(data_model_type)
				{
					case EFileDataModelType::Binary:
					{
						SerializeBinary(m_FileStream, *m_FileDataModel);
						break;
					}
					case EFileDataModelType::JSON:
					{
						SerializeJSON(m_FileStream, *m_FileDataModel);
						break;
					}
					default:
						ASSERT(true, "Unsupported data type.");
						break;
				}
			}

			void Deserialize_Internal()
			{
				const EFileDataModelType data_model_type = (*m_FileDataModel)->GetFileDataModelType();

				switch(data_model_type)
				{
					case EFileDataModelType::Binary:
					{
						DeserializeBinary(m_FileStream, *m_FileDataModel);
						break;
					}
					case EFileDataModelType::JSON:
					{
						DeserializeJSON(m_FileStream, *m_FileDataModel);
						break;
					}
					default:
						ASSERT(true, "Unsupported data type.");
						break;
				}
			}


		private:
			
			std::shared_ptr<IFileDataModel> *m_FileDataModel;

			std::string m_AbsolutePath;
			std::string m_RelativePath;

			std::fstream m_FileStream;

			bool m_ReadOnly;
		};
	}
}