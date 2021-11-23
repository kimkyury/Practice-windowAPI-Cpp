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


int pan[3][3]; // 3x3칸

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
	HBRUSH hBrush, oldBrush;
	TCHAR* Mes = TEXT("클릭 시 애니메이션이 동작함");

	int x, y;
	static int turn = 1; // 최초의 턴을  1로 고정

	switch (iMessage) {
	case WM_LBUTTONDOWN:
		GetClientRect(hWnd, &rect);

		/*  3x3 영역 분할하기*/
		x = (LOWORD(lParam)) / (rect.right / 3);
		y = (HIWORD(lParam)) / (rect.bottom / 3);

		/* 빈곳에 대해 색상 순서 바꾸기*/
		if (pan[x][y] == 0) { 
			turn = (turn == 1 ? 2 : 1);
			InvalidateRect(hWnd, NULL, TRUE);
		}
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rect);


		/* 판의 윤곽선 그리기 */
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				Rectangle(hdc, i * rect.right / 3, j * rect.bottom / 3, (i + 1) * rect.right / 3, (j + 1) * rect.bottom / 3);
			}
		}

		/* 바둑알 그리기 */
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				/* 영역 크기에 맞게 */
				if (pan[i][j] ==1) {  // 플레이어1
					hBrush = (HBRUSH)CreateSolidBrush(RGB(255, 0, 0));
					oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
					Ellipse(hdc, i * rect.right / 3, j * rect.bottom / 3, (i + 1) * rect.right / 3, (j + 1) * rect.bottom / 3);
					SelectObject(hdc, oldBrush);
					DeleteObject(hBrush);
				}
				else if (pan[i][j] == 2) { // 플레이어2
					hBrush = (HBRUSH)CreateSolidBrush(RGB(0, 255, 0));
					oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
					Ellipse(hdc, i * rect.right / 3, j * rect.bottom / 3, (i + 1) * rect.right / 3, (j + 1) * rect.bottom / 3);
					SelectObject(hdc, oldBrush);
					DeleteObject(hBrush);
				}
			}
		}
		EndPaint(hWnd, &ps);
		return 0;
	
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
