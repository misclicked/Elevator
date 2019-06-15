#pragma once
#include <vector>
#include <cmath>
#include <unordered_set>
#include "HumanClass.h"
#include "Global.h"


class Elevator {
private:
	struct PointedObjEq {
		bool operator () (User const* lhs, User const* rhs) const {
			return lhs->id == rhs->id;
		}
	};
	struct PointedObjHash {
		int operator () (User const* user) const {
			return user->id;
		}
	};
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
	std::unordered_set< User*, PointedObjHash, PointedObjEq> users;
	bool Load(User* user);
	bool Unload(User* user, int now_time);
	void setTarget(int target);
	int getTarget() { return _targetFloor; }
	int getBoardedTime() { return _boardedTime; } // time that users have boarded
	int getBoardStartTime() { return _boardStartTime; }
	int getNowBlock() { return _nowBlock; }
	int getPreferDirection() { return _prefer_direction; }
	ElevatorState getElevatorState() { return _state; };
	void Reset_PreferDirection();
	bool setPreferTarget();
	bool isTargetReached();
	int getFloor() const;
	void setFloor(int floor);
	double getFloorRF() const;
	void setBroadStartTime(int time) { _boardStartTime = time; _boardedTime = 0; }
	void setState(ElevatorState state_in) { _state = state_in; }
	double direction(); // target_block - _nowBlock  ( > 0) ? MovingUpward : ? ( <0 ) MovingDownward (==0) Idle
	bool MoveOnce();
	User* HaveUnloadableUser();
	bool isFull();
	int userCount();
	int ThroughPut;
};