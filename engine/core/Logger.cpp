#include "engine/engine_pch.hpp"
#include "Logger.hpp"

#include "engine/events/events/FilesystemMountedEvent.hpp"

namespace engine
{
namespace core
{
	CLogger::CLogger()
		: m_EngineFs(nullptr)
	{
		engine::events::CEventController::GetInstance().SubscribeEvent(events::EEventType::FilesystemMounted, this);
	}

	bool CLogger::OnEvent(events::IEvent* _event)
	{
		switch(_event->GetType())
			case events::EEventType::FilesystemMounted:
		{
			m_EngineFs = static_cast<engine::events::CFilesystemMountedEvent*>(_event)->GetFilesystem();

			LOG(ELogLevel::Debug, "Event: CLogger received filesystem mounted event.");
			return true;
		}

		return false;
	}
}
}