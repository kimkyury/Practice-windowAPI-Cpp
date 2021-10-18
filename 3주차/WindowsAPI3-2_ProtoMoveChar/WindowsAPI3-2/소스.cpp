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
TCHAR str[26];

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	SYSTEMTIME st;
	static RECT rect;

	switch (iMessage) {
	case WM_CREATE:
		srand((unsigned int)time(NULL)); // 랜덤 위치 설정
		GetClientRect(hWnd, &rect); // (hWnd, Rect구조체포인터), 윈도우의 작업영역 크기를 계산함

		SetTimer(hWnd, 1, 100, NULL); // 0.1초 단위로 수직방향의 위치 변환
		//	SendMessage(hWnd, WM_TIMER, 1, 0);
		str[0] = rand() % 26 + 97; // 출력할 알파벳 코드 얻기
		y = rand() % (rect.right - 40) + 20; // 출력위치의 랜덤 x좌표 얻기

		return 0;

	case WM_SIZE:
		GetClientRect(hWnd, &rect);
		return 0;

	case WM_TIMER:

		/* 좌우 방향으로 바꾸고 싶다면, (1) rect.right 으로 바꾼다.*/
		if (y >= rect.bottom){ //문자가 바닥 위치를 넘으면
			str[0] = rand() % 26 + 97; //새로운 문자 지정

			/* 좌우 방향으로 바꾸고 싶다면, (2) rect.botom으로 바꾼다.*/

			x = rand() % (rect.right - 40) + 20; //  x축(시작위치) . 40은 양 끝의 총 길이, 시작점은 20보다 커야 함.
			y = 0; // y축(시작위치)
		}
		y += 60; // 위치 하향
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		/* 좌우 방향으로 바꾸고 싶다면, (3) x와 y의 위치를 바꾼다.*/
		TextOut(hdc, x, y, str, lstrlen(str)); // (hdc, x좌표, y좌표, 문자열, 문자열길이)
		EndPaint(hWnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
