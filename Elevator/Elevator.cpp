// Elevator.cpp : 定義應用程式的進入點。
//

#define _CRT_SECURE_NO_WARNINGS
#include "framework.h"
#include "Elevator.h"
#include <iostream>
#include <CommCtrl.h>
#include <string>

using namespace std;

#define MAX_LOADSTRING 100

// 全域變數:
HINSTANCE hInst;                                // 目前執行個體
WCHAR szTitle[MAX_LOADSTRING];                  // 標題列文字
WCHAR szWindowClass[MAX_LOADSTRING];            // 主視窗類別名稱
HWND hBtnSim, hBtnAddProp, hBtnKillProp, hBtnResetElevator;		//按鈕
HWND hListViewFloor, hListViewElevator;			//ListViews										
HHOOK hHook;

// 這個程式碼模組所包含之函式的向前宣告:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

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
			break;
		}
	}
	else if (msg->hwnd == hBtnAddProp) {
		switch (msg->message) {
		case WM_LBUTTONUP:
			MessageBox(0, L"Click", L"MessageBox caption", MB_OK);
			break;
		}
	}
	else if (msg->hwnd == hBtnKillProp) {
		switch (msg->message) {
		case WM_LBUTTONUP:
			MessageBox(0, L"Click", L"MessageBox caption", MB_OK);
			break;
		}
	}
	else if (msg->hwnd == hBtnResetElevator) {
		switch (msg->message) {
		case WM_LBUTTONUP:
			MessageBox(0, L"Click", L"MessageBox caption", MB_OK);
			break;
		}
	}

	return CallNextHookEx(hHook, iCode, wParam, lParam);
}

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
	auto iStr = string(1,ch);
	return iStr;
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
		width * 0.01, height * 0.04, width * 0.05, height * 0.92, hWnd, (HMENU)200, hInstance, NULL);

	hBtnAddProp = CreateWindowEx(NULL, L"button", L"Add prop", WS_CHILD | WS_TABSTOP | BS_DEFPUSHBUTTON,
		width * 0.01, height * 0.04, width * 0.05, height * 0.2, hWnd, (HMENU)200, hInstance, NULL);

	hBtnKillProp = CreateWindowEx(NULL, L"button", L"Kill prop", WS_CHILD | WS_TABSTOP | BS_DEFPUSHBUTTON,
		width * 0.01, height * 0.346, width * 0.05, height * 0.2, hWnd, (HMENU)200, hInstance, NULL);

	hBtnResetElevator = CreateWindowEx(NULL, L"button", L"Reset", WS_CHILD | WS_TABSTOP | BS_DEFPUSHBUTTON,
		width * 0.01, height * 0.652, width * 0.05, height * 0.2, hWnd, (HMENU)200, hInstance, NULL);

	hListViewFloor = CreateWindowEx(NULL, WC_LISTVIEW, L"", WS_CHILD | LVS_REPORT | WS_VISIBLE,
		width * 0.08, height * 0.04, width * 0.85, height * 0.92, hWnd, NULL, hInstance, NULL);

	WCHAR szText[256];     // Temporary buffer.
	LVCOLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	lvc.iSubItem = 0;
	lvc.pszText = GetWC("F");
	lvc.cx = width * 0.02;               // Width of column in pixels.

	lvc.fmt = LVCFMT_LEFT;  // Left-aligned column.
	ListView_InsertColumn(hListViewFloor, 0, &lvc);

	lvc.iSubItem = 1;
	lvc.pszText = GetWC("Waiting");
	lvc.cx = width * 0.48;               // Width of column in pixels.

	lvc.fmt = LVCFMT_LEFT;  // Left-aligned column.
	ListView_InsertColumn(hListViewFloor, 1, &lvc);


	lvc.iSubItem = 2;
	lvc.pszText = GetWC("Elevator A");
	lvc.cx = width * 0.10;               // Width of column in pixels.

	lvc.fmt = LVCFMT_LEFT;  // Left-aligned column.
	ListView_InsertColumn(hListViewFloor, 2, &lvc);

	lvc.iSubItem = 3;
	lvc.pszText = GetWC("Elevator B");
	lvc.cx = width * 0.10;               // Width of column in pixels.

	lvc.fmt = LVCFMT_LEFT;  // Left-aligned column.
	ListView_InsertColumn(hListViewFloor, 3, &lvc);

	lvc.iSubItem = 4;
	lvc.pszText = GetWC("Elevator C");
	lvc.cx = width * 0.10;               // Width of column in pixels.

	lvc.fmt = LVCFMT_LEFT;  // Left-aligned column.
	ListView_InsertColumn(hListViewFloor, 4, &lvc);

	lvc.iSubItem = 5;
	lvc.pszText = GetWC("Statistic");
	lvc.cx = width * 0.05;               // Width of column in pixels.

	lvc.fmt = LVCFMT_LEFT;  // Left-aligned column.
	ListView_InsertColumn(hListViewFloor, 5, &lvc);
	


	LVITEM lvI;

	// Initialize LVITEM members that are common to all items.
	lvI.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
	lvI.stateMask = 0;
	lvI.iSubItem = 0;
	lvI.state = 0;


	for (int i = 0; i < 45; i++) {
		lvI.iItem = i;
		lvI.iImage = i;
		if (i % 4 == 0) {
			lvI.pszText = GetWC(IntToFloor(12 - i / 4).c_str());
		}
		else {
			lvI.pszText = GetWC("");
		}
		ListView_InsertItem(hListViewFloor, &lvI);
	}
	for (int i = 1; i < 5; i++) {
		lvI.iSubItem = i;
		for (int j = 0; j < 45; j++) {
			lvI.iItem = j;
			lvI.iImage = j;
			lvI.pszText = GetWC("2");
			ListView_SetItem(hListViewFloor, &lvI);
		}
	}
	ListView_SetItemText(hListViewFloor, 24, 2, GetWC("I'm Cunt"));

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
