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
	typedef void (*onStatusChangedCallBack)(void*, int); //self, time
	std::unordered_set<User> allUsers;
	std::vector<Elevator> elevators;
	std::vector<std::deque<User*> > floors;
	onStatusChangedCallBack pStatusChanged; //on time chnaged
	std::vector<std::vector<int>> floors_order;
public:
	ControlClass();
	const std::unordered_set<User>& AllUsers = allUsers;
	void Initialize();
	void StartSimulate(onStatusChangedCallBack);
	int CheckNewOrder(Elevator* elt);
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