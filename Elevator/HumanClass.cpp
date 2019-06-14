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
	stayTime = INT_MAX;
	arrivedTime = INT_MAX;
	
	state = (targetFloor > nowFloor) ? HumanState::WaitingUpside
								     : HumanState::WaitingDownSide;
}

void Human::setOnElevator()
{
	state = HumanState::OnElevator;
}

void Human::setArrivedTime(int nowTime)
{
	stayTime = rand() % 81 + 20;
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
	if (nowTime - arrivedTime >= stayTime) {
		targetFloor = rand() % 12 + 1;
		state = (targetFloor > nowFloor) ? HumanState::WaitingUpside
										 : HumanState::WaitingDownSide;
	}
	return { state, targetFloor };
}

bool Human::operator==(const Human& obj) const
{
	return obj.id == id;
}


int Human::getRandomTargetFloor()
{
	return 0;
}
