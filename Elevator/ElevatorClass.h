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
	const int Capacity = 10;
public:
	Elevator(int nowBlock); 
	bool Load(User* user);
	bool Unload(User* user, int now_time);
	const double& boardedTime  = _boardedTime; // time that users have boarded
	const int& boardStartTime  = _boardStartTime;
	const int& nowBlock		   = _nowBlock;
	const ElevatorState& state = _state;
	void setBroadStartTime(int time) { _boardStartTime = time; }
	void SetState(ElevatorState state_in) { _state = state_in; }

	int direction(); // target_block - _nowBlock  ( > 0) ? MovingUpward : ? ( <0 ) MovingDownward (==0) Idle
	bool MoveOnce();
	double getFloor();
	bool isFull();
	int userCount();
};