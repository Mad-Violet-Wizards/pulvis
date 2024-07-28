#pragma once

#include "FileDataModel.hpp"

namespace engine
{
	namespace fs
	{
		enum class EFileMode
		{
			None = 0x00,
			Read = 0x01,
			Write = 0x02,
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

		template<class CFileDataModel>
		class CFileHandle
		{
		public:

			template<typename... Args>
			CFileHandle(const std::string& absolute_path, Args&&... args)
				: m_ReadOnly(false)
			{
				m_FileDataModel = new CFileDataModel(absolute_path, std::forward<Args>(args)...);
			}

			bool Open(EFileMode _open_mode)
			{
				std::ios_base::openmode stream_open_mode = (std::ios_base::openmode)0x00;
				
				if (_open_mode & EFileMode::Read)
				{
					m_ReadOnly = true;
					stream_open_mode |= std::ios_base::in;
				}

				if (_open_mode & EFileMode::Write)
				{
					m_ReadOnly = false;
					stream_open_mode |= std::ios_base::out;
				}

				if (_open_mode & EFileMode::Append)
				{
					m_ReadOnly = false;
					stream_open_mode |= std::ios_base::app;
				}

				if (_open_mode & EFileMode::Truncate)
				{
					m_ReadOnly = false;
					stream_open_mode |= std::ios_base::trunc;
				}

				if (_open_mode & EFileMode::Binary)
				{
					stream_open_mode |= std::ios_base::binary;
				}

				m_FileStream.open(m_FileDataModel->GetAbsolutePath(), stream_open_mode);

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
				m_FileDataModel->Serialize(m_FileStream);
			}

			void Deserialize()
			{
				ASSERT(m_FileDataModel != nullptr, "FileDataModel instance is nullptr.");
				m_FileDataModel->Deserialize(m_FileStream);
			}

		private:
			
			IFileDataModel* m_FileDataModel;
			std::fstream m_FileStream;

			bool m_ReadOnly;
		};
	}
}