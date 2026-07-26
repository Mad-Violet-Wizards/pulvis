#include "ChunkIO.hpp"
#include "serialization/ChunkSerializeTrait.hpp"
#include "Level.hpp"
#include "FileSystem.hpp"
#include "FilePath.hpp"
#include "MessageBus.hpp"
#include "serialization/SerializationArchive.hpp"
#include "serialization/BinaryArchiveBackend.hpp"
#include "Logger.hpp"

#include <cstring>
#include <format>

namespace pulvis::level
{
    CChunkIO::CChunkIO(pulvis::fs::CFileSystem& _file_system, pulvis::threads::CMessageBus& _messageBus, uint32_t _ioChannelId, uint32_t _mainChannelId)
		: m_FileSystem(_file_system)
		, m_MessageBus(_messageBus)
		, m_IOChannelId(_ioChannelId)
		, m_MainChannelId(_mainChannelId)
		{
		}

    void CChunkIO::RegisterHandlers()
    {
	    CChunkIO* self = this;

	    m_MessageBus.RegisterHandler<SChunkLoadMsg>(m_IOChannelId,
		[self](const SChunkLoadMsg& _msg)
		{
			SChunkCoord coord{ _msg.ChunkX, _msg.ChunkY };
			std::string level_path(_msg.LevelPath);

			auto chunk = std::make_unique<SChunk>();
			bool success = self->LoadChunkSync(level_path, coord, *chunk);

			if (success)
			{
				std::lock_guard lock(self->m_PendingMutex);
				self->m_PendingChunks.push_back(std::move(chunk));
			}

			SChunkLoadResultMsg result;
			result.ChunkX = _msg.ChunkX;
			result.ChunkY = _msg.ChunkY;
			result.Success = success;
			self->m_MessageBus.Send<SChunkLoadResultMsg>(self->m_MainChannelId, result);
		});

	    m_MessageBus.RegisterHandler<SChunkLoadResultMsg>(m_MainChannelId, [](const SChunkLoadResultMsg& _msg)
		{
			if (!_msg.Success)
			{
				PULVIS_WARNING_LOG("Async chunk load failed: ({}, {})", _msg.ChunkX, _msg.ChunkY);
			}
		});
    }

    bool CChunkIO::LoadChunkSync(const std::string& _levelPath, SChunkCoord _coord, SChunk& _outChunk)
	{
			const pulvis::fs::CFilePath file_path(MakeChunkFileName(_levelPath, _coord));

			const pulvis::fs::EFileResult result = m_FileSystem.Load(pulvis::fs::EDomain::Game, file_path, _outChunk);

			if (result == pulvis::fs::EFileResult::NotFound)
			{
				return false;
			}

			if (result != pulvis::fs::EFileResult::Success)
			{
				PULVIS_ERROR_LOG("CChunkIO::LoadChunkSync - Failed to read: {} ({})", file_path.Str(), static_cast<int>(result));
				return false;
			}

			return true;
	}

    bool CChunkIO::SaveChunkSync(const std::string& _levelPath, const SChunk& _chunk)
    {
			const pulvis::fs::CFilePath file_path(MakeChunkFileName(_levelPath, _chunk.Coord));

			const pulvis::fs::EFileResult result = m_FileSystem.Save(pulvis::fs::EDomain::Game, file_path, _chunk);

			if (result != pulvis::fs::EFileResult::Success)
			{
				PULVIS_ERROR_LOG("CChunkIO::SaveChunkSync - Failed to write: {} ({})", file_path.Str(), static_cast<int>(result));
				return false;
			}

			return true;
    }

    bool CChunkIO::ChunkExistsOnDisk(const std::string& _levelPath, SChunkCoord _coord)
		{
			return m_FileSystem.FileExists(pulvis::fs::EDomain::Game, pulvis::fs::CFilePath(MakeChunkFileName(_levelPath, _coord)));

		}

    uint32_t CChunkIO::LoadChunkBatchSync(const std::string& _levelPath, const std::vector<SChunkCoord>& _coords, CLevel& _level)
		{
			uint32_t loaded = 0;

			for (const SChunkCoord& coord : _coords)
			{
				if (_level.HasChunk(coord))
				{
					continue;
				}

				SChunk* chunk = _level.GetOrCreateChunk(coord);

				if (LoadChunkSync(_levelPath, coord, *chunk))
				{
					chunk->IsDirty = false;
					++loaded;
				}
			}

			return loaded;
		}

    uint32_t CChunkIO::SaveDirtyChunksSync(const std::string& _levelPath, CLevel& _level)
		{
			uint32_t saved = 0;
			const std::vector<SChunkCoord> dirty = _level.GetDirtyChunkCoords();

			for (const SChunkCoord& coord : dirty)
			{
				const SChunk* chunk = _level.GetChunk(coord);
				if (!chunk)
				{
					continue;
				}

				if (SaveChunkSync(_levelPath, *chunk))
				{
					++saved;
				}
			}

			if (saved > 0)
			{
				_level.ClearDirtyFlags();
			}

			return saved;
		}

    void CChunkIO::RequestLoadChunk(const std::string& _levelPath, SChunkCoord _coord)
		{
			SChunkLoadMsg msg;
			msg.ChunkX = _coord.X;
			msg.ChunkY = _coord.Y;

			std::memset(msg.LevelPath, 0, sizeof(msg.LevelPath));
			const size_t copy_len = std::min(_levelPath.size(), sizeof(msg.LevelPath) - 1);
			std::memcpy(msg.LevelPath, _levelPath.c_str(), copy_len);

			m_MessageBus.Send<SChunkLoadMsg>(m_IOChannelId, msg);
		}

		void CChunkIO::RequestLoadChunks(const std::string& _levelPath, const std::vector<SChunkCoord>& _coords)
		{
			for (const SChunkCoord& coord : _coords)
			{
				RequestLoadChunk(_levelPath, coord);
			}
		}

    uint32_t CChunkIO::ApplyPendingChunks(CLevel& _level)
		{
			std::lock_guard lock(m_PendingMutex);

			uint32_t applied = 0;

			for (auto& pending_chunk : m_PendingChunks)
			{
				if (!pending_chunk)
				{
					continue;
				}

				SChunk* target = _level.GetOrCreateChunk(pending_chunk->Coord);
				*target = std::move(*pending_chunk);
				target->IsDirty = false;
				++applied;
			}

			m_PendingChunks.clear();
			return applied;
		}

	std::string CChunkIO::MakeChunkFileName(const std::string& _levelPath, SChunkCoord _coord)
	{
		return std::format("{}/{}.chunk", _levelPath, _coord.ToFileString());
	}
}