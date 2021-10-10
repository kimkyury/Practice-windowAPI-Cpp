#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("Class");

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

//구조체로 변경
typedef struct tagP
{
	int x;
	int y;
} pxy;
pxy p[1000];
int iCount = 0;

//int p[2][1000]; 
//int iCount;  //iCount=0///사용자 누른 점 개수를 저장
//Window Procedure  <- OS가 호출하면서, 인자(매개변수)로 부가적인 정보를 전달한다. 
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{

	HDC hdc;
	HBRUSH hBrush;
	HPEN hPen;
	PAINTSTRUCT ps;
	int x, y;
	switch (iMessage) {
	case WM_CREATE:
		hWndMain = hWnd;
		return 0;

	case WM_LBUTTONDOWN:
		// 특정윈도우의 그리기 관련 정보
		hdc = GetDC(hWnd);	//모든 그리기 함수에세 cd를 전달

		//x = LOWORD(lParam); // Macro
		//x = (WORD)(lParam);  //casting
		x = (short)(lParam);  //casting
		//y = HIWORD(lParam);
		y = ((lParam) >> 16); //bitwise oprator

		p[iCount].x = x;
		p[iCount].y = y;
		iCount++;

		InvalidateRect(hWnd, NULL, FALSE);
		/*
		hPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
		SelectObject(hdc, hPen);
		hBrush = CreateSolidBrush(RGB(0, 0, 255));
		SelectObject(hdc, hBrush);
		Rectangle(hdc, x - 20, y - 20, x + 20, y + 20);
		return 0; */

	case WM_PAINT: //BeginPaint()로 DB열기
		hdc = BeginPaint(hWnd, &ps);

		hPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
		SelectObject(hdc, hPen);
		hBrush = CreateSolidBrush(RGB(0, 0, 255));
		SelectObject(hdc, hBrush);

		for (int i = 0; i < iCount; i++)
		{
			Rectangle(hdc, p[i].x - 20, p[i].y - 20, p[i].x + 20, p[i].y + 20);
		}

		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
