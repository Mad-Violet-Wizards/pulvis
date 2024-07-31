#include "engine/engine_pch.hpp"
#include "Serializable.hpp"

#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/json.hpp>
#include <fstream>
#include "FileDataModel.hpp"

namespace engine
{
namespace fs
{

    void ISerializable::SerializeBinary(std::fstream& _file_stream, std::unique_ptr<IFileDataModel> _file_data_model)
    {
    	cereal::PortableBinaryOutputArchive archive(_file_stream);
		archive(_file_data_model);
    }

    void ISerializable::DeserializeBinary(std::fstream& _file_stream, std::unique_ptr<IFileDataModel> _file_data_model)
    {
        cereal::PortableBinaryInputArchive archive(_file_stream);
        archive(_file_data_model);
    }

    void ISerializable::SerializeJSON(std::fstream& _file_stream, std::unique_ptr<IFileDataModel> _file_data_model)
    {
        cereal::JSONOutputArchive archive(_file_stream);
        archive(_file_data_model);
    }

    void ISerializable::DeserializeJSON(std::fstream& _file_stream, std::unique_ptr<IFileDataModel> _file_data_model)
    {
        cereal::JSONOutputArchive archive(_file_stream);
        archive(_file_data_model);
    }

    std::unique_ptr<IFileDataModel> ISerializable::Wrapper(IFileDataModel* _file_data_model) const
    {
        return std::unique_ptr<IFileDataModel>{ _file_data_model };
    }

}
}