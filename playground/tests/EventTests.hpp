#pragma once

#include "engine/core/Application.hpp"
#include "engine/core/Setup.hpp"
#include "engine/events/Event.hpp"
#include "engine/events/EventBus.hpp"
#include "engine/events/EventController.hpp"
#include "engine/events/EventTypeTraits.hpp"

namespace engine::events
{
//////////////////////////////////////////////////////////////////////////
	namespace tests
	{
//////////////////////////////////////////////////////////////////////////
		class MocEventBool : public IEvent
		{
			EVENT_CLASS_API(Tests);

		public:
			bool m_Handled = false;
			bool m_Handled2 = false;
		};

//////////////////////////////////////////////////////////////////////////
		class MocEventInt : public IEvent
		{
			EVENT_CLASS_API(Tests);

		public:
			int m_Value = 0;
		};

//////////////////////////////////////////////////////////////////////////
		class MocEventNextFrameDelay : public IEvent
		{
			EVENT_CLASS_API(Tests);

			public:

				virtual EProcessTime GetProcessTime() const override
				{
					return EProcessTime::NextFrame;
				}
				bool m_Handled = false;
		};

		//////////////////////////////////////////////////////////////////////////
		class MocEventDifferentBus : public IEvent
		{
			EVENT_CLASS_API(Input);

		public:

			bool m_Handled = false;
		};

//////////////////////////////////////////////////////////////////////////
		class MocEventListener : public IEventListener
		{
			public:
				void OnEvent(IEvent* _event) override
				{
					if (_event->GetEventType() == MocEventBool::GetStaticEventType())
					{
						MocEventBool* event = static_cast<MocEventBool*>(_event);
						event->m_Handled = true;
					}
					else if (_event->GetEventType() == MocEventInt::GetStaticEventType())
					{
						MocEventInt* event = static_cast<MocEventInt*>(_event);
						event->m_Value = 42;
					}
					else if (_event->GetEventType() == MocEventNextFrameDelay::GetStaticEventType())
					{
						MocEventNextFrameDelay* event = static_cast<MocEventNextFrameDelay*>(_event);
						event->m_Handled = true;
					}
					else if (_event->GetEventType() == MocEventDifferentBus::GetStaticEventType())
					{
						MocEventDifferentBus* event = static_cast<MocEventDifferentBus*>(_event);
						event->m_Handled = true;
					}
				}
		};

//////////////////////////////////////////////////////////////////////////
		class MocEventListener2 : public IEventListener
		{			
		public:
			void OnEvent(IEvent* _event) override
			{
				if (_event->GetEventType() == MocEventBool::GetStaticEventType())
				{
					MocEventBool* event = static_cast<MocEventBool*>(_event);
					event->m_Handled2 = true;
				}
			}
		};

//////////////////////////////////////////////////////////////////////////
		class EventTestsFixture
		{
		public:

			EventTestsFixture()
			{
				engine::core::SApplicationSetup app_setup;
				app_setup.m_ClientApp = core::EClientApp::Playground;
				m_Application = std::make_unique<engine::core::Application>(app_setup);
				m_Listener = std::make_unique<MocEventListener>();
				m_Listener2 = std::make_unique<MocEventListener2>();
			}

			MocEventListener* GetListener()
			{
				return m_Listener.get();
			}

			MocEventListener2* GetListener2()
			{
				return m_Listener2.get();
			}

			std::unique_ptr<engine::core::Application> m_Application;
			std::unique_ptr<MocEventListener> m_Listener;
			std::unique_ptr<MocEventListener2> m_Listener2;
		};
	}
}