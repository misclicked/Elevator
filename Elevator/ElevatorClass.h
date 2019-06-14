#pragma once
#include <vector>
#include <cmath>
#include <unordered_set>
#include "HumanClass.h"
#include "Global.h"

class Elevator {
private:
	std::unordered_set< User*> users;
	ElevatorState _state;
	int _nowBlock;
	int _targetFloor;
	double _boardedTime;
	int _boardStartTime;
	int _prefer_direction;
	const int Capacity = 10;
	int _id;
public:
	Elevator(int id,int nowBlock);
	bool Load(User* user);
	bool Unload(User* user, int now_time);
	void setTarget(int target);
	const double& boardedTime = _boardedTime; // time that users have boarded
	const int& boardStartTime = _boardStartTime;
	const int& nowBlock = _nowBlock;
	const int prefer_direction = _prefer_direction;
	const ElevatorState& state = _state;
	const double& floorf = (double)_nowBlock / (double)floor_per_block + 1;
	const int floor = _nowBlock / floor_per_block + 1;
	void setBroadStartTime(int time) { _boardStartTime = time; _boardedTime = 0; }
	void setState(ElevatorState state_in) { _state = state_in; }
	int direction(); // target_block - _nowBlock  ( > 0) ? MovingUpward : ? ( <0 ) MovingDownward (==0) Idle
	bool MoveOnce();
	User* HaveUnloadableUser();
	bool isFull();
	int userCount();
};