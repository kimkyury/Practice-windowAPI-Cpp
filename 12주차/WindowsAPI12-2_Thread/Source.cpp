#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("Thread");

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

DWORD WINAPI ThreadFunc(LPVOID temp){ //DWORD = 32비트 부호 없는 정수, LPVOID:모든 형식에 대한 포인터
	HDC hdc;
	BYTE Blue = 0; //
	HBRUSH hBrush, hOldBrush;
	hdc = GetDC(hWndMain);
	for (;;) {
		Blue += 5;
		Sleep(20);
		hBrush = CreateSolidBrush(RGB(0, 0, Blue)); // rgb색상 값중 b에 대한 값만 5씩 변화하도록 함
		hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		Rectangle(hdc, 10, 10, 400, 200);
		SelectObject(hdc, hOldBrush);
		DeleteObject(hBrush);
	}
	ReleaseDC(hWndMain, hdc);
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam){
	DWORD ThreadID; 
	HANDLE hThread;

	switch (iMessage) {
	case WM_CREATE:
		hWndMain = hWnd;
		hThread = CreateThread(NULL, 0, ThreadFunc, NULL, 0, &ThreadID); // 프로세스에 대한 새 스레드 생성
		CloseHandle(hThread); // 열려진 핸들을 닫음, 실패시 0을 리턴(성공은 0이 아닌 값), 이 함수로 핸들을 닫는단들 스레드가 파괴되는 건 아님
		return TRUE;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}