#pragma once

namespace engine::rendering::opengl
{
	class CRenderTargetOpenGL
	{
		public:

			CRenderTargetOpenGL(int _buffer_id);

			void Clear(const engine::math::Vector4<float>& _color);
			void Bind();

		private:

			int m_FramebufferID;
	};
}