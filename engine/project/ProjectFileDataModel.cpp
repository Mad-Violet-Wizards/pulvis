#include "engine/engine_pch.hpp"
#include "ProjectFileDataModel.hpp"

namespace engine::projects
{
	CProjectFileDataModel::CProjectFileDataModel()
		: m_Name("Unkown")
		, m_Path("Unkown")
		, m_VersionMajor(0)
		, m_VersionMinor(0)
		, m_VersionPatch(0)
		, m_EngineVersionHash(0)
	{
	}

	CProjectFileDataModel::CProjectFileDataModel(const std::string& _name, const std::string& _path, unsigned int _version_major, unsigned int _version_minor, unsigned int _version_patch, unsigned int _engine_version_hash)
		: m_Name(_name)
		, m_Path(_path)
		, m_VersionMajor(_version_major)
		, m_VersionMinor(_version_minor)
		, m_VersionPatch(_version_patch)
		, m_EngineVersionHash(_engine_version_hash)
	{
	}
}
