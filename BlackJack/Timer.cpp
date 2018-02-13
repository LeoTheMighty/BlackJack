#include "Timer.h"

Timer::Timer()
{
}


Timer::~Timer()
{
}

int Timer::addStartTime() {
	int id = startTimes.size();
	startTimes.push_back(getTime());
	return id;
}
