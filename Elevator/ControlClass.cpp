
#include "ControlClass.h"
#include <iostream>
#include <queue>
#include <unordered_map>
#include <thread>
#include <chrono>
#include <algorithm>

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
	elevator_order.clear();
	for (int i = 0; i <= floor_count; i++) {
		std::vector<int> vec;
		elevator_order.push_back(vec);

	}
	TotalRun = 0;
	TotalWaitTime = 0;
}

void ControlClass::StartSimulate(onStatusChangedCallBack osc)
{
	this->pStatusChanged = osc;
	Time = 0;
	std::queue<int> floorQuery;
	std::unordered_map<int, bool> floorQueryHandled;
	int new_order =0;

	while (true) {
		//1�Ӳ��ͷs�u�@��
		if (rand() % 100 < humanGenRate && allUsers.size() <= max_user) {
			int humanGenThisRound = rand() % 3;
			std::cout << "Generate " << humanGenThisRound << " at Time:\t" << Time << std::endl;
			for (int i = 0; i < humanGenThisRound; i++) {
				User* h = new User(1, rand() % 11 + 2, Time);
				allUsers.insert(h);
				floors[1].push_back(h);
			}
		}
		bool purge_find = true;
		while (purge_find)
		{
			purge_find = false;
			for (User* u : allUsers)
				if (u->getState() == UserState::ForPurge)
				{
					purge_find = true;
					auto it = allUsers.find(u);
					auto pUser = *it;
					allUsers.erase(it);
					if (pUser->getNowElevatorId() != -1) {
						auto& _ele = elevators[pUser->getNowElevatorId()];
						_ele.users.erase(_ele.users.find(pUser));
						_ele.setState(ElevatorState::Idle);
					}
					else {
						auto& _floor = floors[pUser->getNowFloor()];
						_floor.erase(std::remove(_floor.begin(), _floor.end(), pUser), _floor.end());
					}
					delete pUser;
					break;
				}
		}
		//�{�ɶ��I�ˬd�ò���2F~12F�����u�@���H�A���s�H�N�Ʊ��1F(80%)�Ψ�2F~12F(20%)
		for (User* h : allUsers)
			h->checkStayFinished(Time);

		int NewOrderCount = ResetOrder();
		//�R�O: ����1���ɶ���줧�q�貾��
		for (Elevator& elt : elevators)
		{
			//�Y�q��i�X�������ɶ����i�i�X���H��
			int LoadCount = (int)(((Time - (double)elt.getBoardStartTime()) - elt.getBoardedTime()) / board_speed);
			switch (elt.getElevatorState())
			{
			case ElevatorState::Idle: //��F�ؼЩε��ݤ�
				//�s�b�ϥΪ̷Q�U�q��
				if (elt.HaveUnloadableUser() != nullptr)
				{
					elt.setBroadStartTime(Time);
					elt.setState(ElevatorState::UnLoading);
					continue;
				}
				//�s�b�ϥΪ̷Q�W�q��B������V
				if (floors_order[elt.getFloor()].size() > 0 && !elt.isFull() &&
					CheckDirectionOrderExist(&elt))
				{
					elt.setBroadStartTime(Time);
					elt.setState(ElevatorState::Loading);
					continue;
				}
				// �q�褺�s�b�ϥΪ�
				if (elt.userCount() > 0) 
				{
					elt.setPreferTarget();
					elt.setState(ElevatorState::Moving);
				}
				//�q�褺�S���ϥΪ�
				else
				{
					ResetOrder();
					int target = -1;
					//�M��̪񪺥ؼЭ���
					for (int shift = 1; shift < floor_count; ++shift)
					{
						int upfloor = elt.getFloor() + shift, // ���W��
							downfloor = elt.getFloor() - shift; // ���U��
						if (upfloor <= floor_count && floors_order[upfloor].size() > 0)
						{
							target = upfloor;
							break;
						}
						else if (downfloor >= 1 && floors_order[downfloor].size() > 0)
						{
							target = downfloor;
							break;
						}
					}
					if (target > 0) // ���ؼмӼh���ϥΪ�
					{
						elt.Reset_PreferDirection();
						elt.setTarget(target);
						elt.setState(ElevatorState::Moving);
					}
					else //�L�ؼШϥΪ̹q�褺�]�S���ϥιq��
						elt.setState(ElevatorState::Idle);
				}
				break;
			case ElevatorState::Moving:
				//���ʤ����H���q���̪��V���Ӽh
				new_order = CheckNewOrder(&elt);
				if (!elt.isFull() && new_order > 0)
					elt.setTarget(new_order);
				elt.MoveOnce();
				//��F�ؼмӼh
				if (elt.isTargetReached()) 
				{
					elt.Reset_PreferDirection();
					elt.setState(ElevatorState::Idle);
				}
				break;
			case ElevatorState::Loading:
				//�����ɶ��i���H��
				for (int i = 0; i < LoadCount; ++i)
					if (!LoadUserOnce(&elt)) //�L�H�i�H�A�����q��
					{
						elt.setPreferTarget();
						elt.setState(ElevatorState::Idle);
					}
				break;
			case ElevatorState::UnLoading:
				
				int unloadCnt = 0;
				//�����ɶ��i���}���ϥΪ�
				while (unloadCnt < LoadCount) {
					UnLoadUserOnce(&elt, Time);
					unloadCnt++;
				}
				elt.setState(ElevatorState::Idle);
				break;
			}
			NewOrderCount = ResetOrder();
		}
		Time++;
		(*pStatusChanged)(this, Time);
		//Check Elvator in floor have target or 
		std::this_thread::sleep_for(sync_period * 1ms); //sleep and wait till next cycle
	}
}
bool ControlClass::LoadUserOnce(Elevator* p_elt)
{
	int floor_id = p_elt->getFloor();
	if (p_elt->isFull()) 
		return false;
	p_elt->Reset_PreferDirection();
	bool loaded = false;
	for (int i = 0; i < floors[floor_id].size(); ++i)
		if (p_elt->Load(floors[floor_id][i]))
		{
			floors[floor_id].erase(floors[floor_id].begin() + i);
			loaded = true;
		}
	return loaded;
}

bool ControlClass::UnLoadUserOnce(Elevator* p_elt, int time)
{
	bool unloaded = false;
	User* user = p_elt->HaveUnloadableUser();
	if (user != nullptr)
	{
		p_elt->Unload(user, time);
		if(user->getState() != UserState::ForPurge)
			floors[user->getNowFloor()].push_back(user);
		unloaded = true;
		TotalRun++;
		TotalWaitTime += time - user->JoinTime;
		user->JoinTime = time;
	}
	return unloaded;
}
bool ControlClass::CheckDirectionOrderExist(Elevator* p_elt)
{
	int elt_floor = p_elt -> getFloor();
	if (p_elt->userCount() == 0 && floors[elt_floor].size() > 0) //�q�褺�S�H�B���Ӽh���H�n�f�q��
		return true;
	UserState TargetState = (p_elt->getPreferDirection() > 0)
								? UserState::WaitingUpside : UserState::WaitingDownside;
	for (User* u : floors[elt_floor])
		if (u->getState() == TargetState)
			return true;
	return false;
}
int ControlClass::ResetOrder()
{
	int count = 0;
	for (int i = 1; i <= floor_count; i++) {
		floors_order[i].clear();
	}
	for (int i = 1; i <= floor_count; i++) {
		elevator_order[i].clear();
	}
	//���ͥؼмӼh�ݨD��
	for (User* ph : allUsers) {
		auto state = ph->getState();
		if (state == UserState::WaitingUpside ||
			state == UserState::WaitingDownside) //���H�Q�f�q��
		{
			floors_order[ph->getNowFloor()].push_back(ph->getTargetFloor());
			count++;
		}
		else if (state == UserState::OnElevator) {
			elevator_order[ph->getTargetFloor()].push_back(ph->getTargetFloor());
		}
	}
	return count;
}
int ControlClass::CheckNewOrder(Elevator* elt)
{
	//�q��ؼФ����Y�s�b�s�ݨD���H�s�ݨD�u������
	//�U��
	if(elt->direction() < 0)
		for (int i = elt->getFloor() - 1; i >= 1; --i)
			if (floors_order[i].size() > 0 && elt->getTarget() < i)
				return i;
	//�W��
	if (elt->direction() > 0)
		for (int i = elt->getFloor() + 1; i <= floor_count; ++i)
			if (floors_order[i].size() > 0 && elt->getTarget() > i)
				return i;
	return -1; // no new user
}
User* ControlClass::GetHumanByID(int id)
{
	return nullptr;
}

Elevator* ControlClass::GetElevatorByID(int id)
{
	return &elevators[id];
}

std::deque<User*>* ControlClass::GetFloorByID(int id)
{
	return &floors[id];
}

