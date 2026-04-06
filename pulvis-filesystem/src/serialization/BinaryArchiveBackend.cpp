#include "BinaryArchiveBackend.hpp"

#include <cstring>
#include <cassert>

namespace pulvis::fs::serialization
{
	void CBinaryArchiveBackend::WriteInt(const char* _name, int64_t _value)
	{
		WriteLittleEndian(_value);
	}

	void CBinaryArchiveBackend::WriteUInt(const char* _name, uint64_t _value)
	{
		WriteLittleEndian(_value);
	}

	void CBinaryArchiveBackend::WriteFloat(const char* _name, double _value)
	{
		WriteLittleEndian(_value);
	}

	void CBinaryArchiveBackend::WriteBool(const char* _name, bool _value)
	{
		WriteBytes(&_value, sizeof(bool));
	}

	void CBinaryArchiveBackend::WriteString(const char* _name, const std::string& _value)
	{
		const uint32_t length = static_cast<uint32_t>(_value.size());
		WriteLittleEndian(length);
		WriteBytes(_value.data(), length);
	}

	void CBinaryArchiveBackend::WriteRaw(const char* _name, const void* _data, size_t _size)
	{
		WriteBytes(_data, _size);
	}

	void CBinaryArchiveBackend::ReadInt(const char* _name, int64_t& _out)
	{
		ReadLittleEndian(_out);
	}

	void CBinaryArchiveBackend::ReadUInt(const char* _name, uint64_t& _out)
	{
		ReadLittleEndian(_out);
	}

	void CBinaryArchiveBackend::ReadFloat(const char* _name, double& _out)
	{
		ReadLittleEndian(_out);
	}

	void CBinaryArchiveBackend::ReadBool(const char* _name, bool& _out)
	{
		ReadBytes(&_out, sizeof(bool));
	}

	void CBinaryArchiveBackend::ReadString(const char* _name, std::string& _out)
	{
		uint32_t length = 0;
		ReadLittleEndian(length);
		_out.resize(length);
		ReadBytes(_out.data(), length);
	}

	void CBinaryArchiveBackend::ReadRaw(const char* _name, void* _out, size_t _size)
	{
		ReadBytes(_out, _size);
	}

	void CBinaryArchiveBackend::BeginObject(const char* _name)
	{
		// No special framing for objects in binary format.
	}

	void CBinaryArchiveBackend::EndObject()
	{
		// No special framing for objects in binary format.
	}

	void CBinaryArchiveBackend::BeginArray(const char* _name, uint32_t& _count, bool _writing)
	{
		if (_writing)
		{
			WriteLittleEndian(_count);
		}
		else
		{
			ReadLittleEndian(_count);
		}
	}

	void CBinaryArchiveBackend::EndArray()
	{
		// No special framing for arrays in binary format.
	}

	CFileBuffer CBinaryArchiveBackend::GetResult()
	{
		return CFileBuffer(m_Buffer.data(), static_cast<file_size_t>(m_Buffer.size()));
	}

	void CBinaryArchiveBackend::SetInput(const CFileBuffer& _buffer)
	{
		m_Buffer = std::vector<uint8_t>(_buffer.Data(), _buffer.Data() + _buffer.Size());
		m_ReadPtr = m_Buffer.data();
		m_ReadEnd = m_Buffer.data() + m_Buffer.size();
	}

	void CBinaryArchiveBackend::WriteBytes(const void* _data, size_t _size)
	{
		const uint8_t* byte_data = static_cast<const uint8_t*>(_data);
		m_Buffer.insert(m_Buffer.end(), byte_data, byte_data + _size);
	}

	void CBinaryArchiveBackend::ReadBytes(void* _out, size_t _size)
	{
		assert(m_ReadPtr + _size <= m_ReadEnd);
		std::memcpy(_out, m_ReadPtr, _size);
		m_ReadPtr += _size;
	}
}