#pragma once

#include "engine/filesystem/Serializable.hpp"
#include "engine/filesystem/FileDataModel.hpp"

namespace engine::game
{
	class CGameFileDataModel : public engine::fs::IFileDataModel
	{
		public:

			CGameFileDataModel();
			CGameFileDataModel(const std::string& _name, const std::string& _path, unsigned int _version_major, unsigned int _version_minor, unsigned int _version_patch, unsigned int _engine_version_hash);

			template<class Archive>
			void serialize(Archive& ar)
			{
				ar(m_Name, m_Path, m_VersionMajor, m_VersionMinor, m_VersionPatch, m_EngineVersionHash);
			}

			const std::string& GetName() const { return m_Name; }
			const std::string& GetPath() const { return m_Path; }

			engine::fs::EFileDataModelType GetFileDataModelType() const override final { return engine::fs::EFileDataModelType::JSON; }

		private:

			std::string m_Name;
			std::string m_Path;
			unsigned int m_VersionMajor;
			unsigned int m_VersionMinor;
			unsigned int m_VersionPatch;
			unsigned int m_EngineVersionHash;
	};
}

using namespace engine::game;
REGISTER_SERIALIZABLE(CGameFileDataModel);