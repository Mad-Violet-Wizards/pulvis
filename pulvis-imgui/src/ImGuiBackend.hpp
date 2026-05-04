#pragma once

namespace pulvis::imgui
{
	class IImGuiBackend
	{
		public:

			virtual ~IImGuiBackend() = default;

			[[nodiscard]] virtual bool Initialize() = 0;
			virtual void Shutdown() = 0;

			virtual void BeginFrame() = 0;
			virtual void Render() = 0;
	};
}