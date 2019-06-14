
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
		elevators.push_back(Elevator(0));
	}
	floors.clear();
	for (int i = 0; i < 12; i++) {
		std::deque<User*> vec;
		floors.push_back(vec);
	}
	floors_order.clear();
	for (int i = 0; i < 12; i++) {
		std::vector<int> vec;
		floors_order.push_back(vec);

	}
}

void ControlClass::StartSimulate(onHumanDestoryCallBack hdcb, onFloorChangedCallBack fccb, onElevatorBlockChangedCallBack efccb, onElevatorHumanChangedCallBack ehccb, int sleepTimems)
{
	int time = 0;
	const int humanGenRate = 60; //humanGenRate(%)
	std::queue<int> floorQuery;
	std::unordered_map<int, bool> floorQueryHandled;


	while (true) {
		//1�Ӳ��ͷs�u�@��
		if (rand() % 100 > humanGenRate) {
			int humanGenThisRound = rand() % 3;
			std::cout << "Generate " << humanGenThisRound << " at Time:\t" << time << std::endl;
			for (int i = 0; i < humanGenThisRound; i++) {
				User h = User(0, rand() % 12);
				humans.insert(h);
			}
		}

		//�{�ɶ��I�ˬd�ò���2F~12F�����u�@���H�A���s�H�N�Ʊ��1F(80%)�Ψ�2F~12F(20%)
		for (User h:humans)
			h.checkStayFinished(time);

		int NewOrderCount = 0;
		//���ͥؼмӼh�ݨD��
		for (int i = 0; i < floor_count; i++) 
		{	//Check floor for possible floor query
			bool have_order = false;
			floors_order[i].clear();
			for (User* ph : floors[i]) {
				auto state = ph->getState();
				if (state == UserState::WaitingUpside ||
					state == UserState::WaitingDownSide) //���H�Q�f�q��
				{
					have_order = true;
					floors_order[i].push_back(ph->getTargetFloor());
				}
			}
			if(have_order) 
				NewOrderCount++;
		}
		//�R�O: ����1���ɶ���줧�q�貾��
		for (Elevator elt : elevators)
		{
			if (NewOrderCount == 0) //�S���H�n�f�q�� 
				break;
			int LoadCount = (int)((((double)elt.boardStartTime - time) - elt.boardedTime) / board_speed);
			switch(elt.state)
			{
				case ElevatorState::Idle:
					//�ˬd�O�_�s�b�s�ݨD
				break;
				case ElevatorState::Moving:
					elt.MoveOnce();
				break;
				case ElevatorState::Loading:
					//�����ɶ��i���H��
					
					for (int i = 0; i < LoadCount; ++i)
					{
						if (floors[i].size() > 0)
						{
							elt.Load(floors[i][0]);
							floors[i].pop_front();
						}
						else
						{
						}
					}
				break;
				case ElevatorState::UnLoading:
					//�����ɶ��i�����
					
					for (int i = 0; i < LoadCount; ++i)
					{
						if (floors[i].size() > 0)
						{
							elt.Unload(floors[i][0], time);
							floors[i].pop_front();
						}
						else
						{
						}
					}
					break;
			}
		}

		//Check Elvator in floor have target or 
		std::this_thread::sleep_for(sync_period * 1ms); //sleep and wait till next cycle
	}
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

