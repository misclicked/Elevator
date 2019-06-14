#include "HumanClass.h"

Human::Human(char nowFloor, char targetFloor)
{
	init(nowFloor, targetFloor);
	id = HID++;
}

void Human::init(char nowFloor, char targetFloor)
{
	this->nowFloor = nowFloor;
	this->targetFloor = targetFloor;
	waitTime = INT_MAX;
	arrivedTime = INT_MAX;
	state = HumanState::Waiting;
}

void Human::setOnElevator()
{
	state = HumanState::OnElevator;
}

void Human::setArrivedTime(int nowTime)
{
	waitTime = rand() % 81 + 20;
	arrivedTime = nowTime;
	state = HumanState::Staying;
	nowFloor = targetFloor;
}

int Human::getTargetFloor()
{
	return targetFloor;
}

std::pair<HumanState, int> Human::checkState(int nowTime)
{
	if (nowTime - arrivedTime >= waitTime) {
		state = HumanState::Waiting;
		targetFloor = rand() % 12 + 1;
	}
	return { state, targetFloor };
}
