#include "ElevatorClass.h"

Elevator::Elevator(int nowFloor)
{
	_state = ElevatorState::Idle;
	_boardedTime = 0;
	_boardStartTime = 0;
	_nowBlock = nowFloor / 2;
	_targetFloor = target_null;
}

bool Elevator::Load(User* human)
{
	if (isFull())
		return false;
	_boardedTime += board_speed;
	users.insert(human);
	double min_target = floor_count; 
	double max_target = -1.0;
	for (auto h : users)
	{
		if ((h->getTargetFloor()) > max_target)
			max_target = h->getTargetFloor();
		if ((h->getTargetFloor()) < min_target)
			min_target = h->getTargetFloor();
	}
	
	if (getFloor() > max_target) 
		_targetFloor = min_target;//Only User Move Downward 
 	else 
		_targetFloor = max_target;//Only User Move Upward
	return true;
}

int Elevator::direction()
{
	return _targetFloor - _nowBlock;
}

///按照方向執行移動一次: 若是未移動則回傳false
bool Elevator::MoveOnce()
{
	if (_targetFloor == target_null)
		return false;
	if (direction() > 0)
		_nowBlock += elevator_speed;
	else if (direction() < 0)
		_nowBlock -= elevator_speed;
	else 
		return false;
	return true;
}
bool Elevator::Unload(User* human, int now_time)
{
	if (users.count(human) > 0) {
		human->setArriveTarget(now_time);
		users.erase(human);
		return true;
	}
	return false;
}

double Elevator::getFloor()
{
	return _nowBlock / 2.0;
}

bool Elevator::isFull()
{
	return userCount() == Capacity;
}
int Elevator::userCount()
{
	return users.size();
}