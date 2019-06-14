#pragma once
#include <vector>
#include <cmath>
#include "HumanClass.h"
#include "Global.h"

class Elevator {
private:
	std::vector<std::pair<int, int> > IDs;
	ElevatorState state;
	int nowBlock;
	int nowFloor;
	int humanCount;
	int moveCount;
	int startWaitTime;
	int waitTime;
	int targetFloor;
	const int Capacity = 10;
public:
	Elevator(int nowFloor); 
	ElevatorState Load(int humanID, int targetFloor);
	std::vector<int> UnLoad(int Floor);
	void startWait(int Time);
	int getFloor();
	
};