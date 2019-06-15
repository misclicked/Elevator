// Elevator.cpp : 定義應用程式的進入點。
//

#define _CRT_SECURE_NO_WARNINGS
#include "framework.h"
#include "Elevator.h"
#include <iostream>
#include <CommCtrl.h>
#include <string>
#include "ControlClass.h"
#include <thread>


using namespace std;

#define MAX_LOADSTRING 100

// 全域變數:
HINSTANCE hInst;                                // 目前執行個體
WCHAR szTitle[MAX_LOADSTRING];                  // 標題列文字
WCHAR szWindowClass[MAX_LOADSTRING];            // 主視窗類別名稱
HWND hBtnSim, hBtnAddProp, hBtnKillProp, hBtnResetElevator;		//按鈕
HWND hListView;			//ListViews										
HHOOK hHook;
ControlClass Simulator;

// 這個程式碼模組所包含之函式的向前宣告:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// Utilities

LPWSTR GetWC(const char* c)
{
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs(wc, c, cSize);

	return wc;
}

string IntToFloor(const int floor) {
	char ch = 'a';
	if (floor >= 10)
		ch = 'A' + floor - 10;
	else
		ch = to_string(floor)[0];
	auto iStr = string(1, ch);
	return iStr;
}

inline const char* ToString(ElevatorState es)
{
	switch (es)
	{
	case ElevatorState::Idle:		return "Idle";
	case ElevatorState::Loading:		return "Loading";
	case ElevatorState::UnLoading:	return "UnLoading";
	case ElevatorState::Moving:		return "Moving";
	default:      return "[Unknown ElevatorState]";
	}
}


// ListView Setting Functions

void setText(int col, int row, const char* text) {
	ListView_SetItemText(hListView, row, col, GetWC(text));
}

void setElevatorText(int elevatorID, int block, const char* text) {
	setText(elevatorID + 2, 44 - (block) * 2, text);
}

void setWaitingText(int floor, const char* text) {
	setText(1, 44 - (floor - 1) * 4, text);
}

vector<string> WaitingText;
vector<string> ElevatorText[3];
// Main CallBack
void onStatusChanged(void* sender, int time) {
	ControlClass* Simulator = static_cast<ControlClass*>(sender);
	WaitingText.clear();
	WaitingText.resize(13);
	for (int i = 0; i < 3; i++) {
		ElevatorText[i].clear();
		ElevatorText[i].resize(24);
	}
	ElevatorText[0][Simulator->GetElevatorByID(0)->getNowBlock()].push_back('|');
	ElevatorText[1][Simulator->GetElevatorByID(1)->getNowBlock()].push_back('|');
	ElevatorText[2][Simulator->GetElevatorByID(2)->getNowBlock()].push_back('|');
	for (User* user : Simulator->getAllUsers()) {
		if (user->NowElevatorId < 0) {
			WaitingText[user->NowFloor].push_back(IntToFloor(user->getTargetFloor())[0]);
		}
		else {
			ElevatorText[user->NowElevatorId][Simulator->GetElevatorByID(user->NowElevatorId)->getNowBlock()].push_back(IntToFloor(user->getTargetFloor())[0]);
		}
	}
	for (int i = 1; i <= 12; i++) {
		setWaitingText(i, WaitingText[i].c_str());
	}
	for (int i = 0; i <3; i++) {
		for (int j = 0; j < 24; j++) {
			if (ElevatorText[i][j].size()) {
				ElevatorText[i][j].push_back('|');
				ElevatorText[i][j] += ToString(Simulator->GetElevatorByID(i)->getElevatorState());
			}
			setElevatorText(i, j, ElevatorText[i][j].c_str());
		}
	}
}

// Button CallBack

LRESULT CALLBACK BtnMsgProc(int iCode, WPARAM wParam, LPARAM lParam)
{
	MSG* msg = reinterpret_cast<MSG*>(lParam);
	if (msg->hwnd == hBtnSim) {
		switch (msg->message) {
		case WM_LBUTTONUP:
			ShowWindow(hBtnSim, SW_HIDE);
			ShowWindow(hBtnAddProp, SW_SHOW);
			ShowWindow(hBtnKillProp, SW_SHOW);
			ShowWindow(hBtnResetElevator, SW_SHOW);
			//MessageBox(0, L"And text here", L"MessageBox caption", MB_OK);
			thread t([&]() {Simulator.StartSimulate(onStatusChanged); });
			t.detach();
			break;
		}
	}
	else if (msg->hwnd == hBtnAddProp) {
		switch (msg->message) {
		case WM_LBUTTONUP:
			setElevatorText(rand() % 3, rand() % 12 + 1, "Full");
			break;
		}
	}
	else if (msg->hwnd == hBtnKillProp) {
		switch (msg->message) {
		case WM_LBUTTONUP:
			break;
		}
	}
	else if (msg->hwnd == hBtnResetElevator) {
		switch (msg->message) {
		case WM_LBUTTONUP:
			break;
		}
	}

	return CallNextHookEx(hHook, iCode, wParam, lParam);
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    // TODO: 在此放置程式碼。
	// Simulater Start Button

	
    // 將全域字串初始化
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_ELEVATOR, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 執行應用程式初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ELEVATOR));

    MSG msg;

    // 主訊息迴圈:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  函式: MyRegisterClass()
//
//  用途: 註冊視窗類別。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ELEVATOR));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_ELEVATOR);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函式: InitInstance(HINSTANCE, int)
//
//   用途: 儲存執行個體控制代碼並且建立主視窗
//
//   註解:
//
//        在這個函式中，我們將執行個體控制代碼儲存在全域變數中，
//        並建立及顯示主程式視窗。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
	{
	hInst = hInstance; // 將執行個體控制代碼儲存在全域變數中

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, 1650, 900, nullptr, nullptr, hInstance, nullptr);


	RECT windowArea;
	GetClientRect(hWnd, &windowArea);
	int width = windowArea.right - windowArea.left;
	int height = windowArea.bottom - windowArea.top;

	hBtnSim = CreateWindowEx(NULL, L"button", L"Start", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_DEFPUSHBUTTON,
		(int)(width * 0.01), (int)(height * 0.04), (int)(width * 0.05), (int)(height * 0.92), hWnd, (HMENU)200, hInstance, NULL);

	hBtnAddProp = CreateWindowEx(NULL, L"button", L"Add prop", WS_CHILD | WS_TABSTOP | BS_DEFPUSHBUTTON,
		(int)(width * 0.01), (int)(height * 0.04), (int)(width * 0.05), (int)(height * 0.2), hWnd, (HMENU)200, hInstance, NULL);

	hBtnKillProp = CreateWindowEx(NULL, L"button", L"Kill prop", WS_CHILD | WS_TABSTOP | BS_DEFPUSHBUTTON,
		(int)(width * 0.01), (int)(height * 0.346), (int)(width * 0.05), (int)(height * 0.2), hWnd, (HMENU)200, hInstance, NULL);

	hBtnResetElevator = CreateWindowEx(NULL, L"button", L"Reset", WS_CHILD | WS_TABSTOP | BS_DEFPUSHBUTTON,
		(int)(width * 0.01), (int)(height * 0.652), (int)(width * 0.05), (int)(height * 0.2), hWnd, (HMENU)200, hInstance, NULL);

	hListView = CreateWindowEx(NULL, WC_LISTVIEW, L"", WS_CHILD | LVS_REPORT | WS_VISIBLE,
		(int)(width * 0.08), (int)(height * 0.04), (int)(width * 0.9), (int)(height * 0.92), hWnd, NULL, hInstance, NULL);

	LVCOLUMN lvc;
	// Initialize LVCOLUMN members that are common to all items.
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	auto InsertColumn = [&](auto id, auto width, auto text)
	{
		lvc.iSubItem = id;
		lvc.pszText = GetWC(text);
		lvc.cx = int(width);

		lvc.fmt = LVCFMT_LEFT;
		ListView_InsertColumn(hListView, id, &lvc);
	};

	InsertColumn(0, width * 0.02, "F");
	InsertColumn(1, width * 0.48, "Waiting");
	InsertColumn(2, width * 0.10, "Elevator A");
	InsertColumn(3, width * 0.10, "Elevator B");
	InsertColumn(4, width * 0.10, "Elevator C");
	InsertColumn(5, width * 0.10, "Statistic");

	LVITEM lvI;
	// Initialize LVITEM members that are common to all items.
	lvI.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
	lvI.stateMask = 0;
	lvI.state = 0;

	// Fill floor column
	lvI.iSubItem = 0;
	for (int i = 0; i < 45; i++) {
		lvI.iItem = i;
		if (i % 4 == 0) {
			lvI.pszText = GetWC(IntToFloor(12 - i / 4).c_str());
		}
		else {
			lvI.pszText = GetWC("");
		}
		ListView_InsertItem(hListView, &lvI);
	}

	// Fill rest columns 1: Waiting Line 2: Elevator A 3: Elevator B 4: Elevator C
	for (int i = 1; i < 5; i++) {
		lvI.iSubItem = i;
		for (int j = 0; j < 45; j++) {

			lvI.iItem = j;
			lvI.pszText = GetWC("");
			ListView_SetItem(hListView, &lvI);
		}
	}

	//Fill statistic column
	lvI.iSubItem = 5;
	auto SetStatisticItem = [&](auto id, auto text) {lvI.iItem = id; lvI.pszText = GetWC(text); ListView_SetItem(hListView, &lvI); };
	SetStatisticItem(0, "Finished Run:");
	SetStatisticItem(2, "Total Used Time:");
	SetStatisticItem(4, "Wait Time Per User:");
	SetStatisticItem(6, "Elevator A Through Put");
	SetStatisticItem(8, "Elevator B Through Put");
	SetStatisticItem(10, "Elevator C Through Put");

	hHook = SetWindowsHookEx(WH_GETMESSAGE, (HOOKPROC)BtnMsgProc, NULL, GetCurrentThreadId());

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  函式: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  用途: 處理主視窗的訊息。
//
//  WM_COMMAND  - 處理應用程式功能表
//  WM_PAINT    - 繪製主視窗
//  WM_DESTROY  - 張貼結束訊息然後傳回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 剖析功能表選取項目:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
				UnhookWindowsHookEx(hHook);
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此新增任何使用 hdc 的繪圖程式碼...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// [關於] 方塊的訊息處理常式。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
