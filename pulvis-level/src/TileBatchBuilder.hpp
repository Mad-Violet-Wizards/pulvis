#pragma once

#include "Chunk.hpp"
#include "ChunkCoord.hpp"

#include <vector>
#include <unordered_map>
#include <cstdint>
#include <string>

namespace pulvis::fs::assets { class CAssetRegistry; }

namespace pulvis::rendering
{
	class CRenderQueue;
	namespace gl { class CGLRenderDevice; struct SGLBufferHandle; }
}

namespace pulvis::level
{
	class CLevel;

	/*
		Converts chunks into tile draw commands for the render queue.

		Maintains a VBO cache per chunk - only rebuilds when chunk is dirty.
		On unload, the VBO is destroyed.

		Tile size is configurable (default 16px).
	*/
	class CTileBatchBuilder
	{
		public:

			CTileBatchBuilder(pulvis::rendering::gl::CGLRenderDevice& _renderDevice, 
												pulvis::fs::assets::CAssetRegistry& _assetRegistry);

			~CTileBatchBuilder();

			void SetTileSize(float _tileSize) { m_TileSize = _tileSize; }
			float GetTileSize() const { return m_TileSize; }

			void BuildAndSubmit(const CLevel& _level,
				pulvis::rendering::CRenderQueue& _queue,
				float _cameraPosX, float _cameraPosY,
				float _viewportW, float _viewportH);

			void InvalidateChunk(const SChunkCoord& _coord);

			void InvalidateAll();

	private:

		struct SCachedChunkVBO
		{
			uint32_t VboID = 0;
			uint32_t VertexCount = 0;
			uint32_t TextureID = 0;
			float AtlasW = 0.f;
			float AtlasH = 0.f;
			int32_t Layer = 0;
		};

		void RebuildChunkVBOs(const CLevel& _level, const SChunk& _chunk);
		bool IsChunkVisible(const SChunkCoord& _coord, float _camX, float _camY, float _vpW, float _vpH) const;

	private:

		pulvis::rendering::gl::CGLRenderDevice& m_Device;
		pulvis::fs::assets::CAssetRegistry& m_Registry;
		float m_TileSize = 16.f;

		// Cache: chunk coord -> list of VBOs (one per tileset per layer)
		std::unordered_map<SChunkCoord, std::vector<SCachedChunkVBO>> m_VBOCache;
		// Track which chunks were dirty last frame
		std::unordered_map<SChunkCoord, uint64_t> m_LastBuildFrame;
		uint64_t m_FrameCounter = 0;
	};
}