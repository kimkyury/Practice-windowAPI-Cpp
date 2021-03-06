#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("RandGrp");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow){
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

TCHAR str[256];
HWND hWndMain;

//스레드로 넘길 인수 구조체
struct Threadparam {
	int x, y;
	TCHAR ch;
} Param[4] = {
	{ 10, 10, TCHAR('a') },
	{ 10, 30, TCHAR('b') },
	{ 10, 50, TCHAR('c') },
	{ 10, 70, TCHAR('d') }
};

DWORD WINAPI ThreadFunc(LPVOID temp) {
	int iCount = 0;
	TCHAR buf[256];
	HDC hdc = GetDC(hWndMain);

	/* 각 문자에 대하여 문자 수 카운팅 */
	Threadparam* tp = (Threadparam*)temp; 
	for (int i = 0; i < lstrlen(str); i++){
		if (str[i] == tp->ch) iCount++;
		Sleep(10);
	}

	wsprintf(buf, TEXT("%c의 개수 ; %d"), tp->ch, iCount);
	TextOut(hdc, tp->x, tp->y, buf, lstrlen(buf));

	ReleaseDC(hWndMain, hdc);
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam){
	HDC hdc;
	PAINTSTRUCT ps;
	DWORD ThreadID;
	HANDLE hThread;
	int i, len;
	static int iCount;

	switch (iMessage){
	case WM_CHAR:
		len = lstrlen(str);
		str[len] = (TCHAR)wParam;
		str[len + 1] = 0;

		//한 번 입력 > 문자 각각에 대한 스레드 발생
		for (i = 0; i < 4; i++){
			CloseHandle(CreateThread(NULL, 0, ThreadFunc, &Param[i], 0, &ThreadID));
		}

		InvalidateRect(hWnd, NULL, FALSE);
		return 0;

	case WM_CREATE:
		hWndMain = hWnd;
		return 0;

	case WM_LBUTTONDOWN:
		hdc = GetDC(hWnd);
		Ellipse(hdc, LOWORD(lParam) - 10, HIWORD(lParam) - 10, LOWORD(lParam) + 10, HIWORD(lParam) + 10);
		ReleaseDC(hWnd, hdc);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 100, 100, str, lstrlen(str));
		EndPaint(hWnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}