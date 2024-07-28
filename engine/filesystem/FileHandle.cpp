#include "engine/engine_pch.hpp"
#include "FileHandle.hpp"

namespace engine
{
namespace fs
{
template<class IFileModel>
class FileHandle
{
public:

    IFileModel* OpenFile();
    bool CloseFile();
};
}
}