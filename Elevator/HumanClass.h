#pragma once
#include <utility>
#include "Global.h"

class Human {
private:
	int waitTime;
	int arrivedTime;
	char nowFloor;
	char targetFloor;
	HumanState state;
	int id;
public:
	Human(char nowFloor, char targetFloor);
	void init(char nowFloor, char targetFloor);
	void setOnElevator();
	void setArrivedTime(int nowTime);
	int getTargetFloor();
	std::pair<HumanState, int> checkState(int nowTime);
};
