#include "TileBatchBuilder.hpp"
#include "Level.hpp"

#include "assets/AssetRegistry.hpp"
#include "assets/AssetEntry.hpp"
#include "payloads/TilesetPayload.hpp"
#include "payloads/TexturePayload.hpp"
#include "RenderQueue.hpp"
#include "opengl/GLRenderDevice.hpp"
#include "opengl/GLTileRenderer.hpp"

#include <cmath>

namespace pulvis::level
{
	CTileBatchBuilder::CTileBatchBuilder(pulvis::rendering::gl::CGLRenderDevice& _device,
		pulvis::fs::assets::CAssetRegistry& _registry)
		: m_Device(_device)
		, m_Registry(_registry)
	{
	}

	CTileBatchBuilder::~CTileBatchBuilder()
	{
		InvalidateAll();
	}

	void CTileBatchBuilder::BuildAndSubmit(const CLevel& _level, pulvis::rendering::CRenderQueue& _queue, float _cameraPosX, float _cameraPosY, float _viewportW, float _viewportH)
	{
		++m_FrameCounter;

		for (const auto& [coord, chunk_ptr] : _level.GetLoadedChunks())
		{
			if (!IsChunkVisible(coord, _cameraPosX, _cameraPosY, _viewportW, _viewportH))
			{
				continue;
			}

			chunk_ptr->LastAccessTime = m_FrameCounter;

			auto cache_it = m_VBOCache.find(coord);
			if (cache_it == m_VBOCache.end() || chunk_ptr->IsDirty)
			{
				RebuildChunkVBOs(_level, *chunk_ptr);
				chunk_ptr->IsDirty = false;
			}

			cache_it = m_VBOCache.find(coord);
			if (cache_it == m_VBOCache.end())
			{
				continue;
			}

			const SRegion* region = _level.GetRegionAt(coord);

			for (const SCachedChunkVBO& vbo : cache_it->second)
			{
				if (vbo.VboID == 0 || vbo.VertexCount == 0)
				{
					continue;
				}

				const SLevelLayerDesc* layer_desc = _level.GetLayer(static_cast<uint32_t>(vbo.Layer));
				if (!layer_desc || !layer_desc->Visible || !layer_desc->Render.IsValid())
				{
					continue;
				}

				pulvis::rendering::STileDrawCmd cmd;
				cmd.TextureID = vbo.TextureID;
				cmd.VboID = vbo.VboID;
				cmd.VertexCount = vbo.VertexCount;
				cmd.TextureSize = { vbo.AtlasW, vbo.AtlasH };
				cmd.Layer = layer_desc->Render;

				if (region)
				{
					const float r = region->Properties.GetFloat("ambient_r", 1.f);
					const float g = region->Properties.GetFloat("ambient_g", 1.f);
					const float b = region->Properties.GetFloat("ambient_b", 1.f);
					const float a = region->Properties.GetFloat("ambient_a", 1.f);
					cmd.AmbientColor = { r, g, b, a };
				}

				_queue.PushTileBatch(cmd);
			}
		}
	}

	void CTileBatchBuilder::RebuildChunkVBOs(const CLevel& _level, const SChunk& _chunk)
	{
		InvalidateChunk(_chunk.Coord);

		std::vector<SCachedChunkVBO> new_vbos;

		struct SBatchKey
		{
			uint16_t TilesetIndex;
			uint32_t Layer;
			bool operator==(const SBatchKey& _o) const { return TilesetIndex == _o.TilesetIndex && Layer == _o.Layer; }
		};

		struct SBatchKeyHash
		{
			size_t operator()(const SBatchKey& _k) const
			{
				return std::hash<uint32_t>{}(_k.TilesetIndex) ^ (std::hash<uint32_t>{}(_k.Layer) << 16);
			}
		};

		std::unordered_map<SBatchKey, std::vector<pulvis::rendering::gl::STileVertex>, SBatchKeyHash> vertex_groups;

		const float chunk_origin_x = static_cast<float>(_chunk.Coord.X) * static_cast<float>(CHUNK_SIZE) * m_TileSize;
		const float chunk_origin_y = static_cast<float>(_chunk.Coord.Y) * static_cast<float>(CHUNK_SIZE) * m_TileSize;

		const uint32_t level_layer_count = _level.GetLayerCount();

		for (uint32_t layer = 0; layer < MAX_TILE_LAYERS; ++layer)
		{
			if (!_chunk.HasLayer(layer)) { continue; }
			if (layer >= level_layer_count) { continue; }

			for (uint32_t y = 0; y < CHUNK_SIZE; ++y)
			{
				for (uint32_t x = 0; x < CHUNK_SIZE; ++x)
				{
					const STile* tile_ptr = _chunk.TryGetTile(layer, x, y);
					if (!tile_ptr) { continue; }

					const STile& tile = *tile_ptr;
					if (tile.TilesetIndex == 0xFFFF) { continue; }

					const std::string& tileset_path = _level.GetTilesetPath(tile.TilesetIndex);
					if (tileset_path.empty()) { continue; }

					auto tileset_handle = m_Registry.Find(tileset_path);
					if (!tileset_handle.IsValid()) { continue; }

					const auto* tileset_entry = m_Registry.Get(tileset_handle);
					if (!tileset_entry || tileset_entry->State != pulvis::fs::assets::EAssetState::Ready)
					{
						continue;
					}

					const auto* tileset = tileset_entry->GetPayload<pulvis::level::STilesetPayload>();
					if (tile.TileIndex >= tileset->Tiles.size()) { continue; }

					const auto& tile_info = tileset->Tiles[tile.TileIndex];

					const float px = chunk_origin_x + static_cast<float>(x) * m_TileSize;
					const float py = chunk_origin_y + static_cast<float>(y) * m_TileSize;

					const float u0 = tile_info.UV_X;
					const float v0 = tile_info.UV_Y;
					const float u1 = tile_info.UV_X + tile_info.UV_W;
					const float v1 = tile_info.UV_Y + tile_info.UV_H;

					SBatchKey key{ tile.TilesetIndex, layer };
					auto& verts = vertex_groups[key];

					verts.push_back({ { px,              py },              { u0, v0 } });
					verts.push_back({ { px + m_TileSize, py },              { u1, v0 } });
					verts.push_back({ { px + m_TileSize, py + m_TileSize }, { u1, v1 } });

					verts.push_back({ { px,              py },              { u0, v0 } });
					verts.push_back({ { px + m_TileSize, py + m_TileSize }, { u1, v1 } });
					verts.push_back({ { px,              py + m_TileSize }, { u0, v1 } });
				}
			}
		}

		for (auto& [key, vertices] : vertex_groups)
		{
			if (vertices.empty()) { continue; }

			auto vbo = m_Device.CreateVertexBuffer(
				vertices.data(),
				vertices.size() * sizeof(pulvis::rendering::gl::STileVertex),
				pulvis::rendering::gl::EBufferUsage::Dynamic
			);

			uint32_t gpu_tex_id = 0;
			float atlas_w = 0.f;
			float atlas_h = 0.f;

			const std::string& tileset_path = _level.GetTilesetPath(key.TilesetIndex);
			auto tileset_handle = m_Registry.Find(tileset_path);
			if (tileset_handle.IsValid())
			{
				const auto* tileset_entry = m_Registry.Get(tileset_handle);
				if (tileset_entry)
				{
					const auto* tileset = tileset_entry->GetPayload<pulvis::level::STilesetPayload>();

					auto tex_handle = tileset->TextureHandle;
					const auto* tex_entry = m_Registry.Get(tex_handle);
					if (tex_entry && tex_entry->State == pulvis::fs::assets::EAssetState::Ready)
					{
						const auto* tex = tex_entry->GetPayload<pulvis::rendering::STexturePayload>();
						gpu_tex_id = tex->GPUHandle;
						atlas_w = static_cast<float>(tex->Width);
						atlas_h = static_cast<float>(tex->Height);
					}
				}
			}

			SCachedChunkVBO cached;
			cached.VboID = vbo.ID;
			cached.VertexCount = static_cast<uint32_t>(vertices.size());
			cached.TextureID = gpu_tex_id;
			cached.AtlasW = atlas_w;
			cached.AtlasH = atlas_h;
			cached.Layer = key.Layer;

			new_vbos.push_back(cached);
		}

		m_VBOCache[_chunk.Coord] = std::move(new_vbos);
	}

	bool CTileBatchBuilder::IsChunkVisible(const SChunkCoord& _coord,
		float _camX, float _camY, float _vpW, float _vpH) const
	{
		const float chunk_world_size = static_cast<float>(CHUNK_SIZE) * m_TileSize;

		const float chunk_x = static_cast<float>(_coord.X) * chunk_world_size;
		const float chunk_y = static_cast<float>(_coord.Y) * chunk_world_size;

		const float half_vp_w = _vpW * 0.5f;
		const float half_vp_h = _vpH * 0.5f;

		return !(chunk_x + chunk_world_size < _camX - half_vp_w
			|| chunk_x > _camX + half_vp_w
			|| chunk_y + chunk_world_size < _camY - half_vp_h
			|| chunk_y > _camY + half_vp_h);
	}

	void CTileBatchBuilder::InvalidateChunk(const SChunkCoord& _coord)
	{
		auto it = m_VBOCache.find(_coord);
		if (it == m_VBOCache.end()) { return; }

		for (SCachedChunkVBO& vbo : it->second)
		{
			if (vbo.VboID != 0)
			{
				pulvis::rendering::gl::SGLBufferHandle handle;
				handle.ID = vbo.VboID;
				m_Device.DestroyBuffer(handle);
			}
		}

		m_VBOCache.erase(it);
	}

	void CTileBatchBuilder::InvalidateAll()
	{
		for (auto& [coord, vbos] : m_VBOCache)
		{
			for (SCachedChunkVBO& vbo : vbos)
			{
				if (vbo.VboID != 0)
				{
					pulvis::rendering::gl::SGLBufferHandle handle;
					handle.ID = vbo.VboID;
					m_Device.DestroyBuffer(handle);
				}
			}
		}

		m_VBOCache.clear();
	}
}