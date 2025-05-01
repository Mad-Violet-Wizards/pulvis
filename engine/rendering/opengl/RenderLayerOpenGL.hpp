#pragma once


namespace engine::rendering::opengl
{
	class CRenderSubLayerOpenGL;

	enum class ERenderLayerOpenGL
	{
		Unknown,
		GamePrefix,
		GameEntities,
		GameSuffix,
		HUD,
		UI,
		Count
	};

	class CRenderLayerOpenGL
	{
		public:

			CRenderLayerOpenGL(ERenderLayerOpenGL _layer);
			~CRenderLayerOpenGL();

			void CreateSublayer(unsigned int _sublayer_index);
			ERenderLayerOpenGL GetLayerType() const;

	private:

			ERenderLayerOpenGL m_Layer;
			std::vector<CRenderSubLayerOpenGL*> m_Sublayers;

	};
}