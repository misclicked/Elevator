#pragma once
#include <utility>
#include "Global.h"

class Human {
private:
	int waitTime;
	int arrivedTime;
	char nowFloor;
	char targetFloor;
	HumanState state;
public:
	int id;
	Human(char nowFloor, char targetFloor);
	void init(char nowFloor, char targetFloor);
	void setOnElevator();
	void setArrivedTime(int nowTime);
	int getTargetFloor();
	std::pair<HumanState, int> checkState(int nowTime);
	bool operator == (const Human& obj) const;
};

namespace std
{
	template<> 
	struct hash<Human>
	{
		size_t operator()(const Human& obj) const;
	};
	inline size_t hash<Human>::operator()(const Human& obj) const
	{
		return hash<int>()(obj.id);
	}
}
