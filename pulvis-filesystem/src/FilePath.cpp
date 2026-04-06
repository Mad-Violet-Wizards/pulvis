#include "FilePath.hpp"

namespace pulvis::fs
{
	std::string_view CFilePath::Extension() const
	{
		const auto filename = Filename();
		const auto dot = filename.rfind('.');

		if (dot == std::string::npos || dot == 0)
			return std::string_view();

		return std::string_view(filename).substr(dot);
	}

	std::string_view CFilePath::Filename() const
	{
		const auto slash = m_Path.rfind('/');

		if (slash == std::string::npos)
			return std::string_view(m_Path);

		return std::string_view(m_Path).substr(slash + 1);
	}

	std::string_view CFilePath::Directory() const
	{
		const auto slash = m_Path.rfind('/');
		if (slash == std::string::npos)
			return std::string_view();
		return std::string_view(m_Path).substr(0, slash);
	}

	std::string_view CFilePath::Stem() const
	{
		auto filename = Filename();
		const auto dot = filename.rfind('.');

		if (dot == std::string::npos)
			return filename;

		return filename.substr(0, dot);
	}

	bool CFilePath::StartsWith(const std::string_view& _prefix) const
	{
		return m_Path.compare(0, _prefix.size(), _prefix) == 0;
	}

	CFilePath CFilePath::operator/(const std::string_view& _other) const
	{
		std::string new_path = m_Path;
		if (!new_path.empty() && new_path.back() != '/')
			new_path += '/';

		new_path += _other;
		return CFilePath(new_path);
	}

	void CFilePath::Normalize()
	{
		for (char& c : m_Path)
		{
			if (c == '\\')
				c = '/';
		}

		size_t pos = m_Path.find("//");
		while (pos != std::string::npos)
		{
			m_Path.replace(pos, 2, "/");
			pos = m_Path.find("//");
		}

		while(m_Path.size() > 1 && m_Path.back() == '/')
			m_Path.pop_back();
	}
}

