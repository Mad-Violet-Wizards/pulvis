#include "engine/engine_pch.hpp"
#include "FilesystemMountedEvent.hpp"

namespace engine::events
{
	CFilesystemMountedEvent::CFilesystemMountedEvent(engine::fs::Filesystem* _filesystem)
		: m_Filesystem(_filesystem)
	{
	}
	}