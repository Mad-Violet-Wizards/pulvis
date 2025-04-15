#pragma once

namespace engine::rendering::opengl
{
	class IRenderableGroupOpenGL
	{
		public:

			virtual ~IRenderableGroupOpenGL() = default;
			virtual void Render() const = 0;
			virtual void Clear() const = 0;
	};
}
