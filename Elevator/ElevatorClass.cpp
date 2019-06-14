#include "ElevatorClass.h"

Elevator::Elevator(int id,int nowFloor)
{
	_id = id;
	_state = ElevatorState::Idle;
	_boardedTime = 0;
	_boardStartTime = 0;
	_nowBlock = nowFloor / 2;
	_targetFloor = target_null;
	_prefer_direction = 0;
}

bool Elevator::Load(User*& user)
{
	if (_prefer_direction != 0) // 使用者方向與電梯遵循方向不符
		if ((user->getTargetFloor() > getFloor() && _prefer_direction < 0) ||
			(user->getTargetFloor() < getFloor() && _prefer_direction > 0))
			return false;
	if (isFull()) //滿員
		return false;
	user->setOnElevator(_id);
	_boardedTime += board_speed;
	users.insert(user);
	double min_target = floor_count + 1.0;
	double max_target = -1.0;
	for (auto h : users)
	{
		if ((h->getTargetFloor()) > max_target)
			max_target = h->getTargetFloor();
		if ((h->getTargetFloor()) < min_target)
			min_target = h->getTargetFloor();
	}
	if (getFloorRF() > max_target)
		_targetFloor = max_target;//Only Have Users: Move Downward 
	else
		_targetFloor = min_target;//Only Have Users: Move Upward

	_prefer_direction = _targetFloor - getFloor();
	return true;
}

int Elevator::getFloor() const
{
	return _nowBlock / floor_per_block + 1;
}

double Elevator::getFloorRF() const
{
	return (double)_nowBlock / (double)floor_per_block + 1;
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

User* Elevator::HaveUnloadableUser()
{
	for (User* user : users)
		if (user->getTargetFloor() == _nowBlock / floor_per_block)
			return user;
	return nullptr;
}
bool Elevator::Unload(User*& user, int now_time)
{
	if (users.count(user) > 0 && user->getTargetFloor() == getFloor()) {
		user->setArriveTarget(now_time);
		users.erase(user);
		return true;
	}
	if (users.size() == 0)
		_prefer_direction = 0;
	return false;
}
void Elevator::setTarget(int target)
{
	_prefer_direction = _targetFloor - getFloor();
	_targetFloor = target;
}
bool Elevator::isFull()
{
	return userCount() == Capacity;
}
int Elevator::userCount()
{
	return (int)users.size();
}