#pragma once

#include <vector>
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

		int32_t Layer = 0;
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
		int32_t Layer = 0;
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
		Commands are sorted by layer before rendering.

		Layer convention:
			-100 - Parallax
			0 - Ground tiles
			10 - Detail/decoration tiles
			20 - Entites/sprites
			30 - Foreground tiles (roofs, treetops), has opacity to allow sprites to be visible behind them.
			50 - Particles/effects
			100 - UI overlay
	*/
	class CRenderQueue
	{
		public:

			void PushSprite(const SSpriteDrawCmd& _cmd);
			void PushTileBatch(const STileDrawCmd& _cmd);

			void SetFrameState(const SFrameRenderState& _state) { m_FrameState = _state; }
			const SFrameRenderState& GetFrameState() const { return m_FrameState; }

			const std::vector<SSpriteDrawCmd>& GetSpriteDraws() const { return m_SpriteDrawCommands; }
			const std::vector<STileDrawCmd>& GetTileDraws() const { return m_TileDrawCommands; }

			void SortAll();
			void Clear();

			bool Empty() const { return m_SpriteDrawCommands.empty() && m_TileDrawCommands.empty(); }

		private:

			std::vector<SSpriteDrawCmd> m_SpriteDrawCommands;
			std::vector<STileDrawCmd> m_TileDrawCommands;
			SFrameRenderState m_FrameState;
	};
}