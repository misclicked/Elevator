#include "HumanClass.h"
#include <iostream>

Human::Human(char nowFloor, char targetFloor)
{
	init(nowFloor, targetFloor);
	id = HID++;
}

void Human::init(char nowFloor, char targetFloor)
{
	this->nowFloor = nowFloor;
	this->targetFloor = targetFloor;
	stayTime = rand() % 81 + 20;
	arrivedTime = INT_MAX;
	
	state = (targetFloor > nowFloor) ? HumanState::WaitingUpside
								     : HumanState::WaitingDownSide;
}

void Human::setOnElevator()
{
	state = HumanState::OnElevator;
}

void Human::setArriveTarget(int nowTime)
{
	arrivedTime = nowTime;
	state = HumanState::Staying;
	nowFloor = targetFloor;
	printf("Human:%d arrived floor:%d and start working", id, targetFloor);
}

HumanState Human::getState()
{
	return state;
}

int Human::getTargetFloor()
{
	return targetFloor;
}

std::pair<HumanState, int> Human::checkStayFinished(int nowTime)
{
	if (nowTime - arrivedTime >= stayTime) {
		//80% to 1F  |||  20% to 2F~12F 
		targetFloor = (rand() % 100 < 80) ? 1 : rand() % 11 + 2;
		//Set new State
		state = (targetFloor > nowFloor) ? HumanState::WaitingUpside
										 : HumanState::WaitingDownSide;
		printf("Human:%d Finish his job and hope to go to floor:%d",id, targetFloor);
	}
	return { state, targetFloor };
}

bool Human::operator==(const Human& obj) const
{
	return obj.id == id;
}