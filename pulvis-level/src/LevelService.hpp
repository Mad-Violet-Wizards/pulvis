#pragma once

namespace pulvis::level
{
	class CLevelService
	{
		public:

			~CLevelService() = default;

			static CLevelService& GetInstance();

			void Initialize();
			void Shutdown();

		private:

			CLevelService() = default;
	};
}