#pragma once

#include <cereal/types/polymorphic.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/json.hpp>
#include <fstream>

#include "FileDataModel.hpp"

namespace engine
{
namespace fs
{
    #define REGISTER_SERIALIZABLE(class) \
        using namespace engine::fs; \
        CEREAL_REGISTER_TYPE(class) \
        CEREAL_REGISTER_POLYMORPHIC_RELATION(IFileDataModel, class) \

    inline void SerializeBinary(std::fstream& _file_stream, const std::shared_ptr<IFileDataModel>& _file_data_model)
    {
        cereal::PortableBinaryOutputArchive archive(_file_stream);
	 	archive(_file_data_model);   
    }
    inline void DeserializeBinary(std::fstream& _file_stream, std::shared_ptr<IFileDataModel>& _file_data_model)
    {
        cereal::PortableBinaryInputArchive archive(_file_stream);
        archive(_file_data_model);
    }

    inline void SerializeJSON(std::fstream& _file_stream, const std::shared_ptr<IFileDataModel>& _file_data_model)
    {
        cereal::JSONOutputArchive archive(_file_stream);
        archive(_file_data_model);
    }

    inline void DeserializeJSON(std::fstream& _file_stream, std::shared_ptr<IFileDataModel>& _file_data_model)
    {
        cereal::JSONInputArchive archive(_file_stream);
        archive(_file_data_model);
    }
}
}