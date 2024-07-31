#pragma once

#include <cereal/types/polymorphic.hpp>

namespace engine
{
namespace fs
{
    class IFileDataModel;

    #define REGISTER_SERIALIZABLE(class) \
        using namespace engine::fs; \
        CEREAL_REGISTER_TYPE(class) \
        CEREAL_REGISTER_POLYMORPHIC_RELATION(IFileDataModel, class) \

    class PULVIS_API ISerializable
    {
        public:

            ISerializable() = default;
            ~ISerializable() = default;

            ISerializable(const ISerializable&) = delete;
            ISerializable& operator=(const ISerializable&) = delete;

            virtual void Serialize(std::fstream& _file_stream) = 0;
            virtual void Deserialize(std::fstream& _file_stream) = 0;

            void SerializeBinary(std::fstream& _file_stream, std::unique_ptr<IFileDataModel> _file_data_model);
            void DeserializeBinary(std::fstream& _file_stream, std::unique_ptr<IFileDataModel> _file_data_model);

            void SerializeJSON(std::fstream& _file_stream, std::unique_ptr<IFileDataModel> _file_data_model);
            void DeserializeJSON(std::fstream& _file_stream, std::unique_ptr<IFileDataModel> _file_data_model);

            std::unique_ptr<IFileDataModel> Wrapper(IFileDataModel* _file_data_model) const;
    };
}
}