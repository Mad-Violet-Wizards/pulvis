#pragma once

#include "Serializable.hpp"

namespace engine
{
namespace fs
{
	class PULVIS_API IFileDataModel : public ISerializable
	{
		public:

			IFileDataModel(const std::string& _absolute_path);
			virtual ~IFileDataModel() = default;

			IFileDataModel(const IFileDataModel&) = delete;
			IFileDataModel&operator=(const IFileDataModel&) = delete;

			virtual void Serialize(std::fstream& _file_stream) = 0;
			virtual void Deserialize(std::fstream& _file_stream) = 0;

			const std::string& GetAbsolutePath() const;
			const std::string& GetRelativePath() const;
			const std::string& GetFilename() const;

		private:

			std::string m_AbsolutePath;
			std::string m_RelativePath;
			std::string m_Filename;
	};
}
}
