#pragma once
#include<vector>
#include<unordered_set>
#include "HumanClass.h"
#include "ElevatorClass.h"
#include "Global.h"
class ControlClass
{
private:
	typedef void onHumanDestoryCallBack(void*, int); //self, humanID
	typedef void onFloorChangedCallBack(void*, int); //self, floorID
	typedef void onElevatorFloorChangedCallBack(void*, int); //self, elevatorID
	typedef void onElevatorHumanChangedCallBack(void*, int); //self, elevatorID
	std::unordered_set<Human> humans;
	std::vector<Elevator> elevators;
	std::vector<std::vector<Human*> > floor;
	onHumanDestoryCallBack pHDCB;
	onFloorChangedCallBack pFCCB;
	onElevatorFloorChangedCallBack pEFCCB;
	onElevatorHumanChangedCallBack pEHCCB;
public:
	ControlClass();
	void Initialize();
	void StartSimulate(onHumanDestoryCallBack, onFloorChangedCallBack, onElevatorFloorChangedCallBack, onElevatorHumanChangedCallBack, int sleepTimems = 1000);
	Human* GetHumanByID(int id);
	Elevator* GetElevatorByID(int id);
	std::vector<Human*> GetFloorByID(int id);
};

