#pragma once

#include "engine/core/Application.hpp"
#include "engine/core/Setup.hpp"
#include "engine/events/Event.hpp"
#include "engine/events/EventListener.hpp"
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
		};

//////////////////////////////////////////////////////////////////////////
		class MocEventInt : public IEvent
		{
			EVENT_CLASS_API(Tests);
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
		};

		//////////////////////////////////////////////////////////////////////////
		class MocEventDifferentBus : public IEvent
		{
			EVENT_CLASS_API(Input);
		};

//////////////////////////////////////////////////////////////////////////
		class MocEventListener : public IEventListener
		{
			public:
				void OnEvent(const IEvent* _event) override;
				int GetChecksum() const
				{
					return m_Checksum;
				}

			private:

				int m_Checksum = 0;
		};

//////////////////////////////////////////////////////////////////////////
		class MocEventListener2 : public IEventListener
		{			
		public:
			void OnEvent(const IEvent* _event) override;
			int GetChecksum() const
			{
				return m_Checksum;
			}

		private:

			int m_Checksum = 0;
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