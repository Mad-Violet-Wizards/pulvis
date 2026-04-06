#include "JsonArchiveBackend.hpp"

#include <iomanip>
#include <sstream>
#include <cstring>
#include <cassert>
#include <charconv>

namespace pulvis::fs::serialization
{
	void CJsonArchiveBackend::WriteInt(const char* _name, int64_t _value)
	{
		WriteValue(_name, _value);
	}

	void CJsonArchiveBackend::WriteUInt(const char* _name, uint64_t _value)
	{
		WriteValue(_name, _value);
	}

	void CJsonArchiveBackend::WriteFloat(const char* _name, double _value)
	{
		WriteValue(_name, _value);
	}

	void CJsonArchiveBackend::WriteBool(const char* _name, bool _value)
	{
		WriteValue(_name, _value);
	}

	void CJsonArchiveBackend::WriteString(const char* _name, const std::string& _value)
	{
		WriteValue(_name, _value);
	}

	void CJsonArchiveBackend::WriteRaw(const char* _name, const void* _data, size_t _size)
	{
		const auto* bytes = static_cast<const uint8_t*>(_data);
		std::ostringstream hex;
		for (size_t i = 0; i < _size; ++i)
			hex << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(bytes[i]);
		WriteValue(_name, hex.str());
	}

	void CJsonArchiveBackend::ReadInt(const char* _name, int64_t& _out)
	{
		_out = ReadValue(_name).get<int64_t>();
	}

	void CJsonArchiveBackend::ReadUInt(const char* _name, uint64_t& _out)
	{
		_out = ReadValue(_name).get<uint64_t>();
	}

	void CJsonArchiveBackend::ReadFloat(const char* _name, double& _out)
	{
		_out = ReadValue(_name).get<double>();
	}

	void CJsonArchiveBackend::ReadBool(const char* _name, bool& _out)
	{
		_out = ReadValue(_name).get<bool>();
	}

	void CJsonArchiveBackend::ReadString(const char* _name, std::string& _out)
	{
		_out = ReadValue(_name).get<std::string>();
	}

	void CJsonArchiveBackend::ReadRaw(const char* _name, void* _out, size_t _size)
	{
		std::string hex = ReadValue(_name).get<std::string>();
		auto* bytes = static_cast<uint8_t*>(_out);
		for (size_t i = 0; i < _size && (i * 2 + 1) < hex.size(); ++i)
		{
			char pair[3] = { hex[i * 2], hex[i * 2 + 1], '\0' };
			bytes[i] = static_cast<uint8_t>(std::strtol(pair, nullptr, 16));
		}
	}

	void CJsonArchiveBackend::BeginObject(const char* _name)
	{
		if (!m_ReadRoot.is_null())
		{
			// Read path
			const auto& parent = CurrentRead();
			const nlohmann::json* child = nullptr;
			if (_name)
				child = &parent.at(_name);
			else
			{
				assert(!m_ArrayIndex.empty());
				child = &parent.at(m_ArrayIndex.back()++);
			}
			m_ReadStack.push_back(child);
		}
		else
		{
			// Write path
			auto& parent = CurrentWrite();
			if (_name)
			{
				parent[_name] = nlohmann::json::object();
				m_WriteStack.push_back(&parent[_name]);
			}
			else
			{
				parent.push_back(nlohmann::json::object());
				m_WriteStack.push_back(&parent.back());
			}
		}
	}

	void CJsonArchiveBackend::EndObject()
	{
		if (!m_ReadRoot.is_null())
			m_ReadStack.pop_back();
		else
			m_WriteStack.pop_back();
	}

	void CJsonArchiveBackend::BeginArray(const char* _name, uint32_t& _count, bool _writing)
	{
		if (_writing)
		{
			auto& parent = CurrentWrite();
			if (_name)
			{
				parent[_name] = nlohmann::json::array();
				m_WriteStack.push_back(&parent[_name]);
			}
			else
			{
				parent.push_back(nlohmann::json::array());
				m_WriteStack.push_back(&parent.back());
			}
		}
		else
		{
			const auto& parent = CurrentRead();
			const nlohmann::json* arr = nullptr;
			if (_name)
				arr = &parent.at(_name);
			else
			{
				assert(!m_ArrayIndex.empty());
				arr = &parent.at(m_ArrayIndex.back()++);
			}
			_count = static_cast<uint32_t>(arr->size());
			m_ReadStack.push_back(arr);
			m_ArrayIndex.push_back(0);
		}
	}

	void CJsonArchiveBackend::EndArray()
	{
		if (!m_ReadRoot.is_null())
		{
			m_ReadStack.pop_back();
			m_ArrayIndex.pop_back();
		}
		else
		{
			m_WriteStack.pop_back();
		}
	}

	CFileBuffer CJsonArchiveBackend::GetResult()
	{
		std::string str = m_WriteRoot.dump(2);
		return CFileBuffer(str.data(), static_cast<file_size_t>(str.size()));
	}

	void CJsonArchiveBackend::SetInput(const CFileBuffer& _buffer)
	{
		std::string str = _buffer.ToString();
		m_ReadRoot = nlohmann::json::parse(str);

		if (m_ReadRoot.is_array() && m_ArrayIndex.empty())
		{
			m_ArrayIndex.push_back(0);
		}
	}

	nlohmann::json& CJsonArchiveBackend::CurrentWrite()
	{
		return m_WriteStack.empty() ? m_WriteRoot : *m_WriteStack.back();
	}

	const nlohmann::json& CJsonArchiveBackend::CurrentRead()
	{
		return m_ReadStack.empty() ? m_ReadRoot : *m_ReadStack.back();
	}

	void CJsonArchiveBackend::WriteValue(const char* _name, nlohmann::json _value)
	{
		auto& current = CurrentWrite();

		if (_name)
		{
			current[_name] = std::move(_value);
		}
		else
		{
			if (!current.is_array())
			{
				current = nlohmann::json::array();
			}
			current.push_back(std::move(_value));
		}
	}
	const nlohmann::json& CJsonArchiveBackend::ReadValue(const char* _name)
	{
		const auto& source = CurrentRead();
		if (_name)
			return source.at(_name);

		if (m_ArrayIndex.empty())
			m_ArrayIndex.push_back(0);

		return source.at(m_ArrayIndex.back()++);
	}
}