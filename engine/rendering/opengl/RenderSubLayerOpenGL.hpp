#pragma once

namespace engine::rendering::opengl
{
	class IRenderableGroupOpenGL;
	enum class ERenderLayerOpenGL;

	class CRenderSubLayerOpenGL
	{
	public:

		CRenderSubLayerOpenGL();
		CRenderSubLayerOpenGL(ERenderLayerOpenGL _parent_layer, unsigned int _sublayer_index);

	private:

		ERenderLayerOpenGL m_ParentLayer;
		unsigned int m_SublayerIndex;
	};
}