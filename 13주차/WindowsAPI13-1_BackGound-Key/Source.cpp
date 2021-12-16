#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("RandGrp");

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


HWND hWndMain; //주 핸들
TCHAR str[256];

DWORD WINAPI ThreadFunc(LPVOID temp){
	HDC hdc = GetDC(hWndMain);
	for (;;){
		SetPixel(hdc, rand() % 500, rand()% 400,  //랜덤 위치에
			RGB(rand() % 256, rand() % 256, rand() % 256)); //랜덤 색상으로
	}
	ReleaseDC(hWndMain, hdc);
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam){
	HDC hdc;
	PAINTSTRUCT ps;
	int i, len; 

	DWORD ThreadID;
	HANDLE hThread;
	switch (iMessage){
		/* 키 입력 받기 */
	case WM_CHAR:
		len = lstrlen(str);
		str[len] = (TCHAR)wParam;
		str[len + 1] = 0;
		InvalidateRect(hWnd, NULL, FALSE);
		return 0;

	case WM_CREATE:
		hWndMain = hWnd;
		hThread = CreateThread(NULL, 0, ThreadFunc, NULL, 0, &ThreadID);
		CloseHandle(hThread);
		return 0;

		/* 점 찍기 */
	case WM_LBUTTONDOWN:
		hdc = GetDC(hWnd);
		Ellipse(hdc, LOWORD(lParam) - 10, HIWORD(lParam) - 10, LOWORD(lParam) + 10, HIWORD(lParam) + 10);
		ReleaseDC(hWnd, hdc);
		return 0;

		/* 입력된 문자 출력 */
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 100, 100, str, lstrlen(str));
		EndPaint(hWnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}