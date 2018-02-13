#include <vector>
#include <SDL.h>
#pragma once
class Timer
{
public:
	Timer();
	~Timer();

	//getters
	unsigned int getTime() { return SDL_GetTicks(); }
	unsigned getStartTime() { return startTime; }
	unsigned int getStartTime(int id) { return startTimes[id]; }
	unsigned int getFullStartTime() { return fullStartTime; }
	unsigned int getElapsedTime() { return getTime() - startTime; }
	unsigned int getElapsedTime(int id) { return SDL_GetTicks() - startTimes[id]; }
	unsigned int getFullElapsedTime() { return getTime() - fullStartTime; }

	//setters
	void setEndTime(unsigned int totalTime) { endTime = getTime() + totalTime; }
	void setStartTime() { startTime = getTime(); }
	void clearStartTimes() { startTimes.clear(); }

	//return id of the startTime
	int addStartTime();
	void addFullStartTime() { fullStartTime = getTime(); }



private:
	//Start times for objects

	//Start time for animation
	unsigned int fullStartTime;
	//Start time for textures
	std::vector <unsigned int> startTimes;

	unsigned int startTime = getTime();
	unsigned endTime = getTime();


};

