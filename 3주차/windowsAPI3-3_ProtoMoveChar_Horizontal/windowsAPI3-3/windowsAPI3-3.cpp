#include <windows.h>
#include <time.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("PROG");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	hWndMain = hWnd;

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}


int x, y;
int iTry = 0; // 키보드를 누른 횟수
int iCorrect = 0; // 키보드 눌러서 맞춘 횟수

TCHAR str[26]; // 알파벳 공간
TCHAR sTry[128]; 
TCHAR sCorrect[128]; 


LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	SYSTEMTIME st;
	static RECT rect;

	switch (iMessage) {
	case WM_CREATE:
		srand((unsigned int)time(NULL));
		GetClientRect(hWnd, &rect);
		SetTimer(hWnd, 1, 250, NULL);
		//	SendMessage(hWnd, WM_TIMER, 1, 0);
		str[0] = rand() % 26 + 97;
		y = rand() % (rect.bottom - 40) + 20;
		return 0;

	case WM_SIZE:
		GetClientRect(hWnd, &rect);
		return 0;

	case WM_TIMER:
		if (x >= rect.right) // 글자가 다 지나간 상태, 즉 실패했다면
		{
			str[0] = rand() % 26 + 97;
			y = rand() % (rect.bottom - 40) + 20; // 수평움직임
			x = 0; // 수평움직임
			iTry++;
		}
		x += 60; // 수평움직임
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;

	case WM_CHAR:  // 키보드를 눌렀을 때
		if (wParam == str[0]) { // 현재 str과 같다면
			iCorrect++; // 맞춘 횟수 증가
		}
		str[0] = rand() % 26 + 97; // 맞췄든 말든 새로운 글자 발생
		y = rand() % (rect.bottom - 40) + 20; // 시작점 설정
		x = 0; //시작점 설정
		iTry++; //시작 횟수 설정

		InvalidateRect(hWnd, NULL, TRUE);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		wsprintf(sTry, TEXT("모든 문자수 : %d"), iTry);
		wsprintf(sCorrect, TEXT("맞춘 문자수: %d"), iCorrect);
		TextOut(hdc, 30, 30, sTry, lstrlen(sTry));
		TextOut(hdc, 30, 60, sCorrect, lstrlen(sCorrect));
		TextOut(hdc, x, y, str, lstrlen(str));
		EndPaint(hWnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
