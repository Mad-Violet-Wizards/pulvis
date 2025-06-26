#include "TestsThreadPoolMock.hpp"


void tests::threads::MocVoid()
{
}

int tests::threads::MocAddTwoValues(int _a, int _b)
{
	return _a + _b;
}

void tests::threads::RefAddTwoValues(int& _a, int& _b, int& _res)
{
	_res = _a + _b;
}

int tests::threads::MocNoSleepReturn()
{
	return s_MocReturnValue;
}

int tests::threads::MocSleepThreadMiliseconds(int _milliseconds)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(_milliseconds));
	return s_MocReturnValue;
}

int tests::threads::MocSleepThreadSeconds(int _seconds)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(_seconds));
	return s_MocReturnValue;
}
