#pragma once

#include "ArchiveBackend.hpp"
#include "Endian.hpp"

#include <vector>
#include <cstdint>

/*
* CBinaryArchiveBackend
	All multi-byte values are stored in little-endian.
	Strings are length-prefixed (uint32_t length + raw bytes, no null terminator).
	Arrays are count-prefixed (uint32_t count).
	Objects have no special framing - fields are written sequentially.
	Field names are NOT stored (binary relies on consistent field order).

	Format is NOT self-describing. Reader and writer must agree on schema.
	This is the fast path for saves, network packets, and .pak entries.
*/

namespace pulvis::fs::serialization
{
	class CBinaryArchiveBackend : public IArchiveBackend
	{
		public:

			void WriteInt(const char* _name, int64_t _value) override;
			void WriteUInt(const char* _name, uint64_t _value) override;
			void WriteFloat(const char* _name, double _value) override;
			void WriteBool(const char* _name, bool _value) override;
			void WriteString(const char* _name, const std::string& _value) override;
			void WriteRaw(const char* _name, const void* _data, size_t _size) override;

			void ReadInt(const char* _name, int64_t& _out) override;
			void ReadUInt(const char* _name, uint64_t& _out) override;
			void ReadFloat(const char* _name, double& _out) override;
			void ReadBool(const char* _name, bool& _out) override;
			void ReadString(const char* _name, std::string& _out) override;
			void ReadRaw(const char* _name, void* _out, size_t _size) override;

			void BeginObject(const char* _name) override;
			void EndObject() override;

			void BeginArray(const char* _name, uint32_t& _count, bool _writing) override;
			void EndArray() override;

			CFileBuffer GetResult() override;
			void SetInput(const CFileBuffer& _buffer) override;

		private:

			void WriteBytes(const void* _data, size_t _size);
			void ReadBytes(void* _out, size_t _size);

			template<typename T>
			void WriteLittleEndian(const T& _value)
			{
				T le_value = serialization::ToLittleEndian(_value);
				WriteBytes(&le_value, sizeof(T));
			}

			template<typename T>
			void ReadLittleEndian(T& _out)
			{
				T le_value;
				ReadBytes(&le_value, sizeof(T));
				_out = serialization::ToLittleEndian(le_value);
			}

	private:

		std::vector<uint8_t> m_Buffer;
		const uint8_t* m_ReadPtr = nullptr;
		const uint8_t* m_ReadEnd = nullptr;
	};
}