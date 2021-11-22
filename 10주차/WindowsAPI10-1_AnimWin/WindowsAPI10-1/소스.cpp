#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("AnimWin");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
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
	TCHAR* Mes = TEXT("마우스 왼쪽 버튼을 누르시면 애니메이션을 볼 수 있습니다");
	int i;

	switch (iMessage) {
	case WM_LBUTTONDOWN:
		i = (LOWORD(lParam) - 10) / 50;   // x(LOWORD(lParam)) = 132라고 쳐보자.
		ShowWindow(hWnd, SW_HIDE);
		switch (i) {
		case 0:
			AnimateWindow(hWnd, 500, AW_SLIDE | AW_HOR_POSITIVE); //AnimateWindow(hWnd, dwTime,dwFlags)
			break;
		case 1:
			AnimateWindow(hWnd, 500, AW_SLIDE | AW_HOR_NEGATIVE);
			break;
		case 2:
			AnimateWindow(hWnd, 500, AW_SLIDE | AW_VER_POSITIVE);
			break;
		case 3:
			AnimateWindow(hWnd, 500, AW_SLIDE | AW_VER_NEGATIVE);
			break;
		case 4:
			AnimateWindow(hWnd, 500, AW_SLIDE | AW_HOR_POSITIVE | AW_VER_POSITIVE);
			break;
		case 5:
			AnimateWindow(hWnd, 500, AW_SLIDE | AW_HOR_POSITIVE | AW_VER_NEGATIVE);
			break;
		case 6:
			AnimateWindow(hWnd, 500, AW_BLEND);
			break;
		case 7:
		default:
			AnimateWindow(hWnd, 500, AW_CENTER);
			break;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		SetForegroundWindow(hWnd); //SetForegroundWindow(hWnd) 
		return TRUE;
	case WM_PAINT:  // 50x50사이즈의 사각형을 그린다
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 10, 10, Mes, lstrlen(Mes));
		for (i = 10; i <= 360; i += 50)
			Rectangle(hdc, i, 50, i + 50, 100); // 시작점의 Y좌표 고정(50), 끝점의 Y좌표 고정(100)
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0); //PostQuitMessage(nExitCode)
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam)); //DefWindowProc(hWnd, Msg, wParam, lParam)
}