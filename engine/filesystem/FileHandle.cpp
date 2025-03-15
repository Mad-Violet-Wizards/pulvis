#include "engine/engine_pch.hpp"
#include "FileHandle.hpp"

#include "engine/filesystem/data_models/TextFileDataModel.hpp"
#include "engine/filesystem/data_models/ShaderFileDataModel.hpp"
#include "engine/filesystem/data_models/ScriptFileDataModel.hpp"
#include "engine/filesystem/data_models/images/PngFileDataModel.hpp"

namespace engine::fs
{
	CFileHandle::CFileHandle(const std::string& _relative_path)
		: m_ReadOnly(false)
		, m_RelativePath(_relative_path)
		, m_FileDataModel(nullptr)
	{
	}

	CFileHandle::CFileHandle(const std::string& _relative_path, std::shared_ptr<IFileDataModel>* _file_data_model)
		: m_ReadOnly(false)
		, m_RelativePath(_relative_path)
		, m_FileDataModel(_file_data_model)
	{
	}

	bool CFileHandle::Open(EFileMode _open_mode)
	{
		std::ios::openmode stream_open_mode = (std::ios_base::openmode)0x00;
		EFileMode mode = _open_mode;

		if (mode & EFileMode::Read)
		{
			m_ReadOnly = true;
			stream_open_mode |= std::ios::in;
			mode = static_cast<EFileMode>(mode & ~EFileMode::Read);
		}

		if (mode & EFileMode::Write)
		{
			m_ReadOnly = false;
			stream_open_mode |= std::ios::out;
			mode = static_cast<EFileMode>(mode & ~EFileMode::Write);
		}

		if (mode & EFileMode::ReadWrite)
		{
			m_ReadOnly = false;
			stream_open_mode |= std::ios::out | std::ios::in;
			mode = static_cast<EFileMode>(mode & ~EFileMode::ReadWrite);
		}

		if (mode & EFileMode::Append)
		{
			m_ReadOnly = false;
			stream_open_mode |= std::ios::app;
			mode = static_cast<EFileMode>(mode & ~EFileMode::Append);
		}

		if (mode & EFileMode::Truncate)
		{
			m_ReadOnly = false;
			stream_open_mode |= std::ios::trunc;
			mode = static_cast<EFileMode>(mode & ~EFileMode::Truncate);
		}

		if (mode & EFileMode::Binary)
		{
			stream_open_mode |= std::ios::binary;
			mode = static_cast<EFileMode>(mode & ~EFileMode::Binary);
		}

		m_FileStream.open(m_RelativePath, stream_open_mode);

		if (!IsOpen())
		{
			char error_message_buffer[256];
#if defined(_WIN32) || defined(_WIN64)
			strerror_s(error_message_buffer, errno);
#else
			strcpy(error_message_buffer, strerror(errno));
#endif
			const std::string error_message = fmt::format("Failed to open file: {} ({})", m_RelativePath, error_message_buffer);
			ASSERT(IsOpen(), error_message);
		}

		return IsOpen();
	}

	bool CFileHandle::Close()
	{
		m_FileStream.close();
		return IsOpen();
	}

	bool CFileHandle::IsOpen() const
	{
		return m_FileStream.is_open();
	}

	void CFileHandle::Serialize()
	{
		ASSERT(m_FileDataModel != nullptr, "FileDataModel instance is nullptr.");
		m_FileStream.seekg(0, std::ios::beg);
		Serialize_Internal();
	}

	void CFileHandle::Deserialize()
	{
		ASSERT(m_FileDataModel != nullptr, "FileDataModel instance is nullptr.");
		m_FileStream.seekg(0, std::ios::beg);
		Deserialize_Internal();
	}

	void CFileHandle::SetFileDataModel(std::shared_ptr<IFileDataModel>* _file_data_model)
	{
		m_FileDataModel = _file_data_model;
	}

	void CFileHandle::Serialize_Internal()
	{
		const EFileDataModelType data_model_type = (*m_FileDataModel)->GetFileDataModelType();

		switch (data_model_type)
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
		case EFileDataModelType::Text:
		{
			data_models::CTextFileDataModel* text_model = dynamic_cast<data_models::CTextFileDataModel*>(m_FileDataModel->get());
			text_model->Serialize(m_FileStream);
			break;
		}
		default:
			ASSERT(true, "Unsupported data type.");
			break;
		}
	}

	void CFileHandle::Deserialize_Internal()
	{
		const EFileDataModelType data_model_type = (*m_FileDataModel)->GetFileDataModelType();

		switch (data_model_type)
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
		case EFileDataModelType::Text:
		{
			data_models::CTextFileDataModel* text_model = dynamic_cast<data_models::CTextFileDataModel*>(m_FileDataModel->get());
			text_model->Deserialize(m_FileStream);
			break;
		}
		case EFileDataModelType::Shader:
		{
			data_models::CShaderFileDataModel* shader_model = dynamic_cast<data_models::CShaderFileDataModel*>(m_FileDataModel->get());
			shader_model->Deserialize(m_FileStream);
			break;
		}
		case EFileDataModelType::Script:
		{
			data_models::CScriptFileDataModel* script_model = dynamic_cast<data_models::CScriptFileDataModel*>(m_FileDataModel->get());
			script_model->Deserialize(m_FileStream);
			break;
		}
		case EFileDataModelType::Png:
		{
			data_models::CPngFileDataModel* png_model = dynamic_cast<data_models::CPngFileDataModel*>(m_FileDataModel->get());
			png_model->Deserialize(m_FileStream);
			break;

		}
		default:
			ASSERT(true, "Unsupported data type.");
			break;
		}
	}

}