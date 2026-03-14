#pragma once

#include "SerializeTrait.hpp"
#include "ArchiveBackend.hpp"

#include <memory>
#include <cstdint>

namespace pulvis::fs::serialization
{
	enum class EArchiveMode : uint8_t
	{ 
		Write,
		Read
	};

	class CSerializationArchive
	{
		public:

			CSerializationArchive(std::unique_ptr<IArchiveBackend> _backend, EArchiveMode _mode)
				: m_Backend(std::move(_backend))
				, m_Mode(_mode)
			{

			}

			bool IsWriteArchive() const { return m_Mode == EArchiveMode::Write; }
			bool IsReadArchive() const { return m_Mode == EArchiveMode::Read; }

			template<typename T>
			void Process(const char* _name, T& _value)
			{
				if (IsWriteArchive())
				{
					TSerializeTrait<T>::Serialize(*this, _name, _value);
				}
				else
				{
					TSerializeTrait<T>::Deserialize(*this, _name, _value);
				}
			}

			template<typename T>
			void Process(const char* _name, const T& _value)
			{
				TSerializeTrait<T>::Serialize(*this, _name, _value);
			}

			void WriteInt(const char* _n, int64_t _v) { m_Backend->WriteInt(_n, _v); }
			void WriteUInt(const char* _n, uint64_t _v) { m_Backend->WriteUInt(_n, _v); }
			void WriteFloat(const char* _n, double _v) { m_Backend->WriteFloat(_n, _v); }
			void WriteBool(const char* _n, bool _v) { m_Backend->WriteBool(_n, _v); }
			void WriteString(const char* _n, const std::string& _v) { m_Backend->WriteString(_n, _v); }
			void WriteRaw(const char* _n, const void* _d, size_t _s) { m_Backend->WriteRaw(_n, _d, _s); }

			void ReadInt(const char* _n, int64_t& _o) { m_Backend->ReadInt(_n, _o); }
			void ReadUInt(const char* _n, uint64_t& _o) { m_Backend->ReadUInt(_n, _o); }
			void ReadFloat(const char* _n, double& _o) { m_Backend->ReadFloat(_n, _o); }
			void ReadBool(const char* _n, bool& _o) { m_Backend->ReadBool(_n, _o); }
			void ReadString(const char* _n, std::string& _o) { m_Backend->ReadString(_n, _o); }
			void ReadRaw(const char* _n, void* _d, size_t _s) { m_Backend->ReadRaw(_n, _d, _s); }

			void BeginObject(const char* _n) { m_Backend->BeginObject(_n); }
			void EndObject() { m_Backend->EndObject(); }

			void BeginArray(const char* _n, uint32_t& _c, bool _w) { m_Backend->BeginArray(_n, _c, _w); }
			void EndArray() { m_Backend->EndArray(); }

			CFileBuffer GetResult() { return m_Backend->GetResult(); }
			void SetInput(const CFileBuffer& _buf) { m_Backend->SetInput(_buf); }

			IArchiveBackend& GetBackend() { return *m_Backend; }
	private:
			std::unique_ptr<IArchiveBackend> m_Backend;
			EArchiveMode m_Mode;
	};
}

#include "SerializeTrait.inl"