#pragma once

#include "ArchiveBackend.hpp"

#include <nlohmann/json.hpp>

#include <string>
#include <vector>

/*
	CJsonArchiveBackend - Human-readable JSON serialization format.

	Produces valid JSON with indentation. Field names ARE stored.
	Suitable for config files, level data, save files (debug builds),
	and any data that needs to be human-inspectable or hand-editable.
*/

namespace pulvis::fs::serialization
{
	class CJsonArchiveBackend : public IArchiveBackend
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

			nlohmann::json& CurrentWrite();
			const nlohmann::json& CurrentRead();
			void WriteValue(const char* _name, nlohmann::json _value);
			const nlohmann::json& ReadValue(const char* _name);

		private:

			nlohmann::json m_WriteRoot;
			std::vector<nlohmann::json*> m_WriteStack;

			// Read: stack of json pointers + array index tracking
			nlohmann::json m_ReadRoot;
			std::vector<const nlohmann::json*> m_ReadStack;
			std::vector<uint32_t> m_ArrayIndex;
	};
}