#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("SubEdit");

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

#define ID_EDIT1 100
#define ID_EDIT2 101
HWND hEdit1, hEdit2;
WNDPROC OldEditProc;

LRESULT CALLBACK EditSubProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	switch (iMessage) {
	case WM_KEYDOWN:
		if (wParam == VK_RETURN) {
			MessageBox(hWnd, TEXT("엔터키가 입력되었음."), TEXT("Edit"), MB_OK);
			SetFocus(hWnd);
		}
	}
	return CallWindowProc(OldEditProc, hWnd, iMessage, wParam, lParam); // oldEditProc에 파라미터들을 넣어준다 (이벤트를 이쪽으로 실행시킨다)
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR* Mes = TEXT("ENTER KEY 입력을 감시함");

	switch (iMessage) {
	case WM_CREATE:
		hEdit1 = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 10, 10, 200, 25, hWnd, (HMENU)ID_EDIT1, g_hInst, NULL);
		hEdit2 = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 10, 50, 200, 25, hWnd, (HMENU)ID_EDIT1, g_hInst, NULL);
		SetFocus(hEdit1);
		OldEditProc = (WNDPROC)SetWindowLongPtr(hEdit1, GWLP_WNDPROC, (LONG_PTR)EditSubProc); //처음에는 hEdit1 윈도우에 EditSubProc 함수를 작동시킨다
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 10, 100, Mes, lstrlen(Mes));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		SetWindowLongPtr(hEdit1, GWLP_WNDPROC, (LONG_PTR)OldEditProc);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));

}