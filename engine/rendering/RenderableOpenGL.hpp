#pragma once

namespace engine::rendering::opengl
{
	class IRenderableOpenGL
	{
		public:

			virtual ~IRenderableOpenGL() = default;
			virtual void Render() const = 0;
	};
}