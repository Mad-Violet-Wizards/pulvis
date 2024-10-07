#pragma once

#include "engine/Events/Event.hpp"

namespace engine::events
{
	class CFilesystemMountedEvent : public IEvent
	{
		EVENT_CLASS_API(Engine);

	public:

		CFilesystemMountedEvent(engine::fs::Filesystem* _filesystem);

	private:

		engine::fs::Filesystem* m_Filesystem;
	};
}