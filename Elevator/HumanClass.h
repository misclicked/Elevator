#pragma once
#include <utility>
#include "Global.h"

class Human {
private:
	int stayTime;    //how many time this human stay in paticular floor
	int arrivedTime; 
	char nowFloor;
	char targetFloor;
	HumanState state;
public:
	int id;
	Human(char nowFloor, char targetFloor);
	void init(char nowFloor, char targetFloor);
	void setOnElevator();
	void setArriveTarget(int nowTime);
	HumanState getState();
	int getTargetFloor();
	std::pair<HumanState, int> checkStayFinished(int nowTime);
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
