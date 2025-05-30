#pragma once

#include "engine/rendering/opengl/RenderTargetOpenGL.hpp"

namespace engine::rendering::opengl::pipeline
{
	class IRenderPipelineOpenGL
	{
		public:
			
			virtual ~IRenderPipelineOpenGL() = default;
			virtual void Prepare() = 0;
			virtual void Render(CRenderTargetOpenGL& _target) = 0;
	};
}