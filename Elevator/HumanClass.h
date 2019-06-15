#pragma once
#include <utility>
#include "Global.h"

class User {
private:
	int stayTime;    //how many time this human stay in paticular floor
	int arrivedTime; 
	int nowFloor;
	int targetFloor;
	int nowElevatorId;
	UserState state;
public:
	int JoinTime;
	bool JoinTimeSetted;
	int id;
	User(char nowFloor, char targetFloor, int time);
	int getNowFloor() { return nowFloor; };
	int getNowElevatorId() { return nowElevatorId; };
	void init(char nowFloor, char targetFloor);
	void setOnElevator(int ele_id);
	void setArriveTarget(int nowTime);
	UserState getState();
	void setState(UserState state);
	int getTargetFloor();
	std::pair<UserState, int> checkStayFinished(int nowTime);
	bool operator == (const User& obj) const;
};

namespace std
{
	template<> 
	struct hash<User>
	{
		size_t operator()(const User& obj) const;
	};
	inline size_t hash<User>::operator()(const User& obj) const
	{
		return hash<int>()(obj.id);
	}
}
