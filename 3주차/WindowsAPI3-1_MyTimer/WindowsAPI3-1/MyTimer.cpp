#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("MyTimer");

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
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	SYSTEMTIME st;
	static TCHAR sTime[128];
	static RECT rt = { 100,100,400,120 };

	switch (iMessage) {
	case WM_CREATE: //window 한 번, 짠할 때 나타남 (p.99 중간)
		SetTimer(hWnd, 1, 1000, NULL); // (핸들, 타이머ID, 1초, WM_TIMER메시지를 WndProc에 전달(보통 NULL지정))
		//SendMessage(hWnd, WM_TIMER, 1, 0); //(전달할 메시지, wParam메시지추가정보, lParam메시지추가정보)
		return 0;
	case WM_TIMER:
		GetLocalTime(&st); // (현재로컬시갑을 대입받을 SYSTEMTIME 구조체)
		wsprintf(sTime, TEXT("지금 시간은 %d:%d:%d입니다"),   //sTime = buffer, %d= format(정수), 
			st.wHour, st.wMinute, st.wSecond);
		//InvalidateRect(hWnd, NULL, FALSE); // TRUE를 FALSE로 바꿔보는 것도 괜츈 
		InvalidateRect(hWnd,&rt,TRUE);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 100, 100, sTime, lstrlen(sTime)); //(hdc, x좌표, y좌표, 출력문자열, 문자열길이)
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		KillTimer(hWnd, 1); // (핸들, 타이머ID) 타이머는 종료시 KillTimer로 파괴해야 한다. 
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
