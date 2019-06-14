#pragma once
#include <vector>
#include <deque>
#include <unordered_set>
#include "HumanClass.h"
#include "ElevatorClass.h"
#include "Global.h"
class ControlClass
{
private:
	typedef void (*onHumanDestoryCallBack)(void*, int); //self, humanID
	typedef void (*onFloorChangedCallBack)(void*, int); //self, floorID
	typedef void (*onElevatorBlockChangedCallBack)(void*, int); //self, elevatorID
	typedef void (*onElevatorHumanChangedCallBack)(void*, int); //self, elevatorID
	std::unordered_set<User> humans;
	std::vector<Elevator> elevators;
	std::vector<std::deque<User*> > floors;
	onHumanDestoryCallBack pHDCB; //once human fulfill target
	onFloorChangedCallBack pFCCB; //once floor populance changed
	onElevatorBlockChangedCallBack pEFCCB; //block changed
	onElevatorHumanChangedCallBack pEHCCB; //once elavator populance changed
	std::vector<std::vector<int>> floors_order;
public:
	ControlClass();
	void Initialize();
	void StartSimulate(onHumanDestoryCallBack, onFloorChangedCallBack, onElevatorBlockChangedCallBack, onElevatorHumanChangedCallBack, int sleepTimems);
	User* GetHumanByID(int id);
	Elevator* GetElevatorByID(int id);
	std::vector<User*> GetFloorByID(int id);
};
class CallBackDemo {
private:
	typedef int (*myCallBack)(void*, int);
	myCallBack mcb;
public:
	void setCallBack(myCallBack cb) {
		mcb = cb;
	}
	void Run() {
		int result = (*mcb)(this, 5);
	}
};