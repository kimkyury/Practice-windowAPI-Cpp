
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
RECT rt = { 100,100, };
//일단 제일 처음에는 제법 큰영역을 지운다
// 이건 필요없는 동작이다.제일 처음엔 지울 게 아예 없다.
SIZE	size;
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	SYSTEMTIME st;
	static TCHAR sTime[128];

	switch (iMessage) {
	case WM_CREATE:
		SetTimer(hWnd, 1, 1000, NULL);
		SendMessage(hWnd, WM_TIMER, 1, 0);
		return 0;
	case WM_RBUTTONDOWN:
		hdc = GetDC(hWnd);
		Ellipse(hdc, 60, 60, 200, 300);
		ReleaseDC(hWnd, hdc);
		return 0;
	case WM_TIMER:
		GetLocalTime(&st);
		hdc = GetDC(hWnd);
		//wsprintf(sTime,TEXT("지금 시간은 %d:%d:%02d 입니다."),
		wsprintf(sTime, TEXT("지금 시간은 %d:%d:%d 입니다."),
			st.wHour, st.wMinute, st.wSecond);

		GetTextExtentPoint(hdc, sTime, lstrlen(sTime), &size); //sTime의 크기 -> size에 대입
		rt.bottom = 100 + size.cy;  //문자열 높이
		rt.right = 100 + size.cx;  // 문자열 너비
		//	InvalidateRect(hWnd,NULL,TRUE);
		InvalidateRect(hWnd, &rt, TRUE);
		ReleaseDC(hWnd, hdc);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 100, 100, sTime, lstrlen(sTime));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

