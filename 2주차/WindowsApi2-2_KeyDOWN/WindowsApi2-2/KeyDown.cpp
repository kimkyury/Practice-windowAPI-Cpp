#include <windows.h>

BOOL static toggle;
TCHAR static* str;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("KeyDown");

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
	static int x = 100;
	static int y = 100;

	switch (iMessage) {
	case WM_KEYDOWN : // 0x0100:
		switch (wParam) {
		case VK_LEFT: // 37: // 0x25: 
			x -= 8;
			break;
		case VK_RIGHT:
			x += 8;
			break;
		case VK_UP:
			y -= 8;
			break;
		case VK_DOWN:
			y += 8;
			break;

		case VK_SPACE:
			if (toggle == TRUE) {
				toggle = FALSE;
			}
			else	toggle = TRUE;
			break;
		}
		//InvalidateRect(hWnd,NULL,TRUE); // A가 하나씩만 생김
		InvalidateRect(hWnd, NULL, FALSE); // hWnd handle을 가진 윈도우로 WM_PAINT message를 발생시키기, A가 연속으로 생김
		return 0;
	case WM_PAINT:
		if (toggle == TRUE) {
			str = TEXT("#");
		}
		else str = TEXT("A");
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, x, y, str, 1); // 출력하는 문자(A)의 개수 = 1 표시
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
