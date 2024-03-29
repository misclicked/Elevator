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
	typedef void (*onStatusChangedCallBack)(void*, int); //self, time
	onStatusChangedCallBack pStatusChanged; //on time chnaged
	std::vector<std::vector<int>> floors_order;
	std::vector<std::vector<int>> elevator_order;
public:
	ControlClass();
	std::vector<Elevator> elevators;
	std::vector<std::deque<User*> > floors;
	std::unordered_set<User*, PointedObjHash, PointedObjEq> getAllUsers() { return allUsers; }
	void Initialize();
	void StartSimulate(onStatusChangedCallBack);
	bool LoadUserOnce(Elevator* p_elt, int time);
	bool UnLoadUserOnce(Elevator* p_elt, int time);
	bool CheckDirectionOrderExist(Elevator* p_elt);
	int ResetOrder();
	int CheckNewOrder(Elevator* elt);
	User* GetHumanByID(int id);
	Elevator* GetElevatorByID(int id);
	std::deque<User*>* GetFloorByID(int id);
	std::unordered_set<User*, PointedObjHash, PointedObjEq> allUsers;
	int TotalRun;
	int TotalWaitTime;
	int Time;
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