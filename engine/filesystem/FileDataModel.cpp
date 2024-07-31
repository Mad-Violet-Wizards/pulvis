#include "engine/engine_pch.hpp"
#include "FileDataModel.hpp"

namespace engine
{
namespace fs
{
IFileDataModel::IFileDataModel(const std::string& _absolute_path)
	: m_AbsolutePath(_absolute_path)
{
}

const std::string& IFileDataModel::GetAbsolutePath() const
{
	return m_AbsolutePath;
}

const std::string& IFileDataModel::GetRelativePath() const
{
	return m_RelativePath;
}

const std::string& IFileDataModel::GetFilename() const
{
	return m_Filename;
}
}
}

