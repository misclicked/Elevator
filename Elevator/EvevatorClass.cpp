#include "ElevatorClass.h"

Elevator::Elevator(int nowFloor)
{
	this->nowFloor = nowFloor;
	humanCount = 0;
	targetFloor = 0;
}

ElevatorState Elevator::Load(int humanID, int targetFloor)
{
	if ((state & ElevatorState::Boarding) != ElevatorState::Boarding) {
		state = ElevatorState::Boarding;
		moveCount = 0;
	}
	IDs.push_back({ humanID, targetFloor });
	humanCount++;
	moveCount++;
	if (humanCount == Capacity) {
		state = state ^ ElevatorState::Boarding;
		state = state | ElevatorState::Full;
	}
	return state;
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

int Elevator::getNowFloor()
{
	return nowFloor;
}

void Elevator::startWait(int Time)
{
	waitTime = std::ceil(moveCount * 0.7);
	startWaitTime = Time;
	state = state | ElevatorState::Waiting;
}

