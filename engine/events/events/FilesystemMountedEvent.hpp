#pragma once

#include "engine/Events/Event.hpp"

namespace engine::events
{
	class CFilesystemMountedEvent : public IEvent
	{
	public:

		CFilesystemMountedEvent(engine::fs::Filesystem* _filesystem);

		EEventType GetType() const override { return EEventType::FilesystemMounted; }
		engine::fs::Filesystem* GetFilesystem() const { return m_Filesystem; }

	private:

		engine::fs::Filesystem* m_Filesystem;
	};
}