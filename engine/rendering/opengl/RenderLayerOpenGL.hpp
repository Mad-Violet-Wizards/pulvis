#pragma once


namespace engine::rendering::opengl
{
	class RenderSubLayerOpenGL;

	enum class ERenderLayerOpenGL
	{
		Unknown,
		GamePrefix,
		GameEntities,
		GameSuffix,
		HUD,
		UI
	};

	class RenderLayerOpenGL
	{
		public:

			RenderLayerOpenGL(ERenderLayerOpenGL _layer);

			void CreateSublayer(unsigned int _sublayer_index);

			ERenderLayerOpenGL GetLayerType() const;

	private:

			ERenderLayerOpenGL m_Layer;
			std::vector<RenderSubLayerOpenGL*> m_Sublayers;

	};
}