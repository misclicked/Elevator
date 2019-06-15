#include "HumanClass.h"
#include <iostream>

User::User(char nowFloor, char targetFloor, int time)
{
	init(nowFloor, targetFloor);
	id = HID++;
	JoinTime = time;
}

void User::init(char nowFloor, char targetFloor)
{
	this->nowFloor = nowFloor;
	this->targetFloor = targetFloor;
	stayTime = rand() % 81 + 20;
	arrivedTime = INT_MAX;
	nowElevatorId = -1;
	JoinTimeSetted = true;
	state = (targetFloor > nowFloor) ? UserState::WaitingUpside
								     : UserState::WaitingDownside;
}

void User::setOnElevator(int ele_id)
{
	nowElevatorId = ele_id;
	state = UserState::OnElevator;
}
///
void User::setArriveTarget(int nowTime)
{
	nowElevatorId = -1;
	arrivedTime = nowTime;
	nowFloor = targetFloor;
	if (nowFloor == 1) // to 1F
	{
		state = UserState::ForPurge;
		printf("Human:%d arrived floor:%d and leave building", id, targetFloor);
	}
	else
	{
		state = UserState::Staying;
		printf("Human:%d arrived floor:%d and start working", id, targetFloor);
	}
}

UserState User::getState()
{
	return state;
}

void User::setState(UserState state)
{
	this->state = state;
}

int User::getTargetFloor()
{
	return targetFloor;
}
///當使用者到達目標樓層後檢查其等待時間是否完成並產生下一個目標樓層
std::pair<UserState, int> User::checkStayFinished(int nowTime)
{
	if (nowTime - arrivedTime >= stayTime && state == UserState::Staying) {
		//80% to 1F  |||  20% to 2F~12F 
		targetFloor = (rand() % 100 < 80) ? 1 : rand() % 11 + 2;
		//Set new State
		state = (targetFloor > nowFloor) ? UserState::WaitingUpside
										 : UserState::WaitingDownside;
		printf("Human:%d Finish his job and hope to go to floor:%d",id, targetFloor);
	}
	return { state, targetFloor };
}

bool User::operator==(const User& obj) const
{
	return obj.id == id;
}
