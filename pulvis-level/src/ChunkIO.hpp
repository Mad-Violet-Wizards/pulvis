#pragma once

#include "ChunkCoord.hpp"
#include "Chunk.hpp"
#include "FileBuffer.hpp"
#include "FileTypes.hpp"

#include <string>
#include <vector>
#include <mutex>
#include <memory>

namespace pulvis::fs { class CMountSystem; }
namespace pulvis::threads { class CMessageBus; }

namespace pulvis::level
{
    class CLevel;

    struct SChunkLoadMsg
    {
        int32_t ChunkX;
        int32_t ChunkY;
        char LevelPath[128];
    };

    struct SChunkSaveMsg
    {
        int32_t ChunkX;
        int32_t ChunkY;
        char LevelPath[128];
    };

    struct SChunkLoadResultMsg
    {
        int32_t ChunkX;
        int32_t ChunkY;
        bool Success;
    };

    class CChunkIO
    {
        public:

			CChunkIO(pulvis::fs::CMountSystem& _mountSystem, pulvis::threads::CMessageBus& _messageBus,
				uint32_t _ioChannelId, uint32_t _mainChannelId);
			~CChunkIO() = default;

            void RegisterHandlers();

            bool LoadChunkSync(const std::string& _levelPath, SChunkCoord _coord, SChunk& _outChunk);
			bool SaveChunkSync(const std::string& _levelPath, const SChunk& _chunk);
			bool ChunkExistsOnDisk(const std::string& _levelPath, SChunkCoord _coord);

		    uint32_t LoadChunkBatchSync(const std::string& _levelPath, const std::vector<SChunkCoord>& _coords, CLevel& _level);
		    uint32_t SaveDirtyChunksSync(const std::string& _levelPath, CLevel& _level);

		    // Async
		    void RequestLoadChunk(const std::string& _levelPath, SChunkCoord _coord);
		    void RequestLoadChunks(const std::string& _levelPath, const std::vector<SChunkCoord>& _coords);

		    uint32_t ApplyPendingChunks(CLevel& _level);
		    size_t PendingCount() const { return m_PendingChunks.size(); }

        private:

            static std::string MakeChunkFileName(const std::string& _levelPath, SChunkCoord _coord);

        private:

            pulvis::fs::CMountSystem& m_MountSystem;
            pulvis::threads::CMessageBus& m_MessageBus;
            uint32_t m_IOChannelId;
            uint32_t m_MainChannelId;

            std::vector<std::unique_ptr<SChunk>> m_PendingChunks;
            std::mutex m_PendingMutex;
    };
}