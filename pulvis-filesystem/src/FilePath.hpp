#pragma once

#include <string>
#include <string_view>
#include <functional>

namespace pulvis::fs
{
	class CFilePath
	{
		public:

			CFilePath() = default;
			explicit CFilePath(const char* _path) : m_Path(_path) { Normalize(); }
			explicit CFilePath(std::string_view _path) : m_Path(_path) { Normalize(); }

			const std::string& Str() const { return m_Path; }
			std::string Str() { return m_Path; }
			const char* CStr() const { return m_Path.c_str(); }
			bool Empty() const { return m_Path.empty(); }

			std::string_view Extension() const;
			std::string_view Filename() const;
			std::string_view Directory() const;
			std::string_view Stem() const;

			bool StartsWith(const std::string_view& _prefix) const;

		public:

			CFilePath operator/(const std::string_view& _other) const;
			bool operator ==(const CFilePath& _other) const { return m_Path == _other.m_Path; }
			bool operator !=(const CFilePath& _other) const { return m_Path != _other.m_Path; }
			bool operator <(const CFilePath& _other) const { return m_Path < _other.m_Path; }
			bool operator >(const CFilePath& _other) const { return m_Path > _other.m_Path; }

		private:

			void Normalize();

		private:

			std::string m_Path;
	};
}

//////////////////////////////////////////////////////////////////////////
template<>
struct std::hash<pulvis::fs::CFilePath>
{
	size_t operator()(const pulvis::fs::CFilePath& _path) const noexcept
	{
		return std::hash<std::string>{}(_path.Str());
	}
};