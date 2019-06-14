
#include "ControlClass.h"
#include <iostream>
#include <queue>
#include <unordered_map>

ControlClass::ControlClass()
{
	Initialize();
}

void ControlClass::Initialize()
{
	elevators.clear();
	for (int i = 0; i < 3; i++) {
		elevators.push_back(Elevator(0));
	}
	floor.clear();
	for (int i = 0; i < 12; i++) {
		std::vector<Human*> vec;
		floor.push_back(vec);
	}
}


void ControlClass::StartSimulate(onHumanDestoryCallBack hdcb, onFloorChangedCallBack fccb, onElevatorBlockChangedCallBack efccb, onElevatorHumanChangedCallBack ehccb, int sleepTimems)
{
	int time = 0;
	const int humanGenRate = 60; //humanGenRate(%)
	std::queue<int> floorQuery;
	std::unordered_map<int, bool> floorQueryHandled;
	while (true) {
		if (rand() % 100 > humanGenRate) {
			int humanGenThisRound = rand() % 3;
			std::cout << "Generate " << humanGenThisRound << " at Time:\t" << time << std::endl;
			for (int i = 0; i < humanGenThisRound; i++) {
				Human h = Human(0, rand() % 12);
				humans.insert(h);
			}
		}
		for (int i = 0; i < 12; i++) {	//Check floor for possible floor query

		}
	}
}

Human* ControlClass::GetHumanByID(int id)
{
	return nullptr;
}

Elevator* ControlClass::GetElevatorByID(int id)
{
	return nullptr;
}

std::vector<Human*> ControlClass::GetFloorByID(int id)
{
	return std::vector<Human*>();
}

