
#include "ControlClass.h"
#include <iostream>
#include <queue>
#include <unordered_map>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;
ControlClass::ControlClass()
{
	Initialize();
}

void ControlClass::Initialize()
{
	elevators.clear();
	for (int i = 0; i < 3; i++) {
		elevators.push_back(Elevator(1));
	}
	floors.clear();
	for (int i = 0; i < 12; i++) {
		std::vector<Human*> vec;
		floors.push_back(vec);
	}
}

void ControlClass::StartSimulate(onHumanDestoryCallBack hdcb, onFloorChangedCallBack fccb, onElevatorBlockChangedCallBack efccb, onElevatorHumanChangedCallBack ehccb, int sleepTimems)
{
	int time = 0;
	const int humanGenRate = 60; //humanGenRate(%)
	std::queue<int> floorQuery;
	std::unordered_map<int, bool> floorQueryHandled;
	while (true) {
		//1樓產生新工作者
		if (rand() % 100 > humanGenRate) {
			int humanGenThisRound = rand() % 3;
			std::cout << "Generate " << humanGenThisRound << " at Time:\t" << time << std::endl;
			for (int i = 0; i < humanGenThisRound; i++) {
				Human h = Human(1, rand() % 11 + 2);
				humans.insert(h);
			}
		}

		//現時間點檢查並產生2F~12F完成工作的人，此群人將希望到1F(80%)或到2F~12F(20%)
		for (Human h:humans)
			h.checkStayFinished(time);

		for (int i = 0; i < floor_count; i++) {	//Check floor for possible floor query
			for (Human* ph : floors[i]) {
				auto state = ph->getState();

			}
		}

		//Check Elvator in floor have target or 

		std::this_thread::sleep_for(sync_period * 1ms); //sleep and wait till next cycle
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

