#include "ElevatorClass.h"

Elevator::Elevator(int nowFloor)
{
	this->nowFloor = nowFloor;
	humanCount = 0;
	targetFloor = 0;
}

bool Elevator::Load(int humanID, int targetFloor, HumanState humanState)
{
	if ((state & ElevatorState::Boarding) != ElevatorState::Boarding) {
		state = ElevatorState::Boarding;
		moveCount = 0;
	}
	if (humanState == HumanState::WaitingUpside && moveState == ElevatorMoveState::DownSide)return true;
	if (humanState == HumanState::WaitingDownSide && moveState == ElevatorMoveState::Upside)return true;
	if (state == ElevatorState::Full)return false;
	IDs.push_back({ humanID, targetFloor });
	humanCount++;
	moveCount++;
	if (humanCount == Capacity) {
		state = state ^ ElevatorState::Boarding;
		state = state | ElevatorState::Full;
	}
	return state != ElevatorState::Full;
}

std::vector<int> Elevator::UnLoad(int Floor)
{
	std::vector<int> unloaded;
	for (int i = IDs.size() - 1; i >= 0; i--) {
		if (IDs[i].second == Floor) {
			unloaded.push_back(IDs[i].first);
			IDs.erase(IDs.begin() + i);
		}
	}
	moveCount += unloaded.size();
	return unloaded;
}

void Elevator::startWait(int Time)
{
	waitTime = (int)std::ceil(moveCount * 0.7);
	startWaitTime = Time;
	state = state | ElevatorState::Waiting;
}

int Elevator::getFloor()
{
	return nowBlock / 2;
}

