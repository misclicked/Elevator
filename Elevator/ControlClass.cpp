
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
		elevators.push_back(Elevator(i, 1));
	}
	floors.clear();
	for (int i = 0; i <= floor_count; i++) {
		std::deque<User*> vec;
		floors.push_back(vec);
	}
	floors_order.clear();
	for (int i = 0; i <= floor_count; i++) {
		std::vector<int> vec;
		floors_order.push_back(vec);

	}
}

void ControlClass::StartSimulate(onStatusChangedCallBack osc)
{
	this->pStatusChanged = osc;
	int time = 0;
	const int humanGenRate = 60; //humanGenRate(%)
	std::queue<int> floorQuery;
	std::unordered_map<int, bool> floorQueryHandled;
	int new_order =0;

	while (true) {
		//1樓產生新工作者
		if (rand() % 100 > humanGenRate) {
			int humanGenThisRound = rand() % 3;
			std::cout << "Generate " << humanGenThisRound << " at Time:\t" << time << std::endl;
			for (int i = 0; i < humanGenThisRound; i++) {
				User h = User(1, rand() % 12 + 1);
				allUsers.insert(h);
				floors[1].push_back(&h);
			}
		}
		bool purge_find = true;
		while (purge_find)
		{
			purge_find = false;
			for (User u : allUsers)
				if (u.getState() == UserState::ForPurge)
				{
					purge_find = true;
					allUsers.erase(allUsers.find(u));
					break;
				}
		}
		//現時間點檢查並產生2F~12F完成工作的人，此群人將希望到1F(80%)或到2F~12F(20%)
		for (User h : allUsers)
			h.checkStayFinished(time);

		int NewOrderCount = 0;
		//產生目標樓層需求表
		for (int i = 1; i <= floor_count; i++)
		{	//Check floor for possible floor query
			bool have_order = false;
			floors_order[i].clear();
			for (User* ph : floors[i]) {
				auto state = ph->getState();
				if (state == UserState::WaitingUpside ||
					state == UserState::WaitingDownSide) //有人想搭電梯
				{
					have_order = true;
					floors_order[i].push_back(ph->getTargetFloor());
				}
			}
			if (have_order)
				NewOrderCount++;
		}
		//命令: 執行1次時間單位之電梯移動
		for (Elevator elt : elevators)
		{
			if (NewOrderCount == 0) //沒有人要搭電梯 
				break;
			//若電梯進出中此次時間內可進出的人數
			int LoadCount = (int)((((double)elt.boardStartTime - time) - elt.boardedTime) / board_speed);
			switch (elt.state)
			{
			case ElevatorState::Idle:
				if (elt.HaveUnloadableUser() != nullptr)
				{
					elt.setBroadStartTime(time);
					elt.setState(ElevatorState::UnLoading);
					continue;
				}
				if (floors_order[elt.floor].size() > 0)
				{
					elt.setBroadStartTime(time);
					elt.setState(ElevatorState::Loading);
				}
				//檢查上行是否存在新需求，若是Idle以往上乘客優先乘載
				if (elt.prefer_direction >= 0)
					for (int i = elt.floor + 1; i <= floor_count; ++i)
						if (floors_order[i].size() > 0)
						{
							elt.setTarget(i);
							elt.setState(ElevatorState::Moving);
							continue;
						}

				//檢查下行是否存在新需求
				if (elt.prefer_direction < 0)
				for (int i = elt.floor - 1; i >=1 ; --i)
				{
					if (floors_order[i].size() > 0)
					{
						elt.setTarget(i);
						elt.setState(ElevatorState::Moving);
						continue;
					}
				}
				break;
			case ElevatorState::Moving:
				//移動中有人按電梯於最近方向之樓層
				new_order = CheckNewOrder(&elt);
				if (!elt.isFull() && new_order > 0)
					elt.setTarget(new_order);
				elt.MoveOnce();
				if (elt.direction() == 0)//到達目標樓層
					elt.setState(ElevatorState::Idle);
				break;
			case ElevatorState::Loading:
				//此單位時間可載人數
				for (int i = 0; i < LoadCount; ++i)
				{
					bool loaded = false;
					if(!elt.isFull())
						for(int load_id =0; load_id < floors[elt.floor].size(); ++load_id)
						{
							if(elt.Load(floors[elt.floor][load_id++])) //找到指定方向可乘載人員
								floors[i].erase(floors[i].begin() + load_id);
						}
					if(!loaded) //無人可以再乘坐電梯
						elt.setState(ElevatorState::Idle);
				}
				break;
			case ElevatorState::UnLoading:
				//此單位時間可釋放數
				for (int i = 0; i < LoadCount; ++i)
				{
					User* user = elt.HaveUnloadableUser();
					if (user !=nullptr)
						elt.Unload(user, time);
					else
						elt.setState(ElevatorState::Idle);
				}
				break;
			}
		}
		time++;
		(*pStatusChanged)(this, time);
		//Check Elvator in floor have target or 
		std::this_thread::sleep_for(sync_period * 1ms); //sleep and wait till next cycle
	}
}
int ControlClass::CheckNewOrder(Elevator* elt)
{
	//下行
	if(elt->direction() < 0)
		for (int i = elt->floor - 1; i >= 1; --i)
			if (floors_order[i].size() > 0)
				return i;
	//上行
	if (elt->direction() > 0)
		for (int i = elt->floor; i <= floor_count; ++i)
			if (floors_order[i].size() > 0)
				return i;
	return -1; // no new user
}
User* ControlClass::GetHumanByID(int id)
{
	return nullptr;
}

Elevator* ControlClass::GetElevatorByID(int id)
{
	return nullptr;
}

std::vector<User*> ControlClass::GetFloorByID(int id)
{
	return std::vector<User*>();
}

