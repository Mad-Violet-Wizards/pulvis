#pragma once

#include "FileBuffer.hpp"

#include <string>
#include <cstdint>

namespace pulvis::fs::serialization
{
	class IArchiveBackend
	{
		public:

			virtual ~IArchiveBackend() = default;

			virtual void WriteInt(const char* _name, int64_t _value) = 0;
			virtual void WriteUInt(const char* _name, uint64_t _value) = 0;
			virtual void WriteFloat(const char* _name, double _value) = 0;
			virtual void WriteBool(const char* _name, bool _value) = 0;
			virtual void WriteString(const char* _name, const std::string& _value) = 0;
			virtual void WriteRaw(const char* _name, const void* _data, size_t _size) = 0;

			virtual void ReadInt(const char* _name, int64_t& _out) = 0;
			virtual void ReadUInt(const char* _name, uint64_t& _out) = 0;
			virtual void ReadFloat(const char* _name, double& _out) = 0;
			virtual void ReadBool(const char* _name, bool& _out) = 0;
			virtual void ReadString(const char* _name, std::string& _out) = 0;
			virtual void ReadRaw(const char* _name, void* _out, size_t _size) = 0;

			virtual void BeginObject(const char* _name) = 0;
			virtual void EndObject() = 0;

			virtual void BeginArray(const char* _name, uint32_t& _count, bool _writing) = 0;
			virtual void EndArray() = 0;

			virtual CFileBuffer GetResult() = 0;
			virtual void SetInput(const CFileBuffer& _buffer) = 0;
	};
}