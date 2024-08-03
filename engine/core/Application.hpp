#pragma once

namespace engine
{
	namespace core
	{
		struct ApplicationSetup;

		class PULVIS_API Application
		{
		public:

			Application(const ApplicationSetup& _app_setup);
			~Application() = default;
		};
	}
}
