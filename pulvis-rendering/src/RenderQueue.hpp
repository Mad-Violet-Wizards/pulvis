#pragma once

#include "RenderLayer.hpp"

#include <vector>
#include <unordered_map>
#include <cstdint>
#include <glm/glm.hpp>

namespace pulvis::rendering
{
	//////////////////////////////////////////////////////////////////////////
	struct SSpriteDrawCmd
	{
		glm::vec2 Position = { 0.f, 0.f };
		glm::vec2 Size = { 0.f, 0.f };
		uint32_t TextureID = 0;

		glm::vec4 UVRect = { 0.f, 0.f, 1.f, 1.f };
		glm::vec4 Color = { 1.f, 1.f, 1.f, 1.f };
	
		SRenderLayerHandle Layer;
		int32_t SortOrder = 0;
	};

	struct STileDrawCmd
	{
		uint32_t TextureID = 0;
		uint32_t VboID = 0;
		uint32_t VertexCount = 0;
		glm::vec2 TextureSize = { 0.f, 0.f };

		float AnimationTime = 0.f;
		glm::vec4 AmbientColor = { 1.f, 1.f, 1.f, 1.f };
		SRenderLayerHandle Layer;
	};

	struct SFrameRenderState
	{
		float TimeOfDay = 0.f;
		float AnimationTime = 0.f;
		glm::vec4 AmbientLight = { 1.f, 1.f, 1.f, 1.f };
	};

//////////////////////////////////////////////////////////////////////////
	/*
		Acumulates draw commands from any system during the frame.
		Commands are put in buckets based on their render layer.
		Layer and respective render targets sorting are performed by CRenderLayerCache.
	*/
	class CRenderQueue
	{
		public:

			void PushSprite(const SSpriteDrawCmd& _cmd);
			void PushTileBatch(const STileDrawCmd& _cmd);

			void SetFrameState(const SFrameRenderState& _state) { m_FrameState = _state; }
			const SFrameRenderState& GetFrameState() const { return m_FrameState; }

			const std::vector<SSpriteDrawCmd>& GetSpritesForLayer(SRenderLayerHandle _layer) const;
			const std::vector<STileDrawCmd>& GetTilesForLayer(SRenderLayerHandle _layer) const;

			void SortAll();
			void Clear();

			bool Empty() const { return m_SpriteBuckets.empty() && m_TileBuckets.empty(); }

		private:

			std::unordered_map<SRenderLayerHandle, std::vector<SSpriteDrawCmd>> m_SpriteBuckets;
			std::unordered_map<SRenderLayerHandle, std::vector<STileDrawCmd>> m_TileBuckets;
			SFrameRenderState m_FrameState;
	};
}