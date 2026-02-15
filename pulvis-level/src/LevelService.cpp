#include "LevelService.hpp"

namespace pulvis::level
{
	CLevelService& CLevelService::GetInstance()
	{
		static CLevelService instance;
		return instance;
	}

	void CLevelService::Initialize()
	{

	}

	void CLevelService::Shutdown()
	{

	}
}