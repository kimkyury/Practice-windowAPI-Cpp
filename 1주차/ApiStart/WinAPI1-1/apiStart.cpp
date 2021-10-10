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


/*
int p[2][200];// ������ ��ǥ ����
int iCount; // �簢���� ���� ����
*/

/* ����ü ������ ���� */
typedef struct tagP {
	int x;
	int y;
} pxy;

pxy p[1000]; // ����ü �迭 ����
int iCount;
int x, y; // ���� ��ǥ
HBRUSH hBrush; 

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;


	switch (iMessage) {
	case WM_CREATE:
		hWndMain = hWnd;
		return 0;
	case WM_LBUTTONDOWN:
		/* ���콺 ��Ŭ�� �̺�Ʈ �ۼ� */
		hdc = GetDC(hWnd);
		x = LOWORD(lParam); //��ũ��(Mecro), ĳ������ ���ؼ��� ����(x = (WORD)(lParam) )
		y = HIWORD(lParam); 
		/* p[0][iCount] = x;
		p[1][iCount] = y; */
		p[iCount].x = x;
		p[iCount].y = y;
		iCount++;
		hBrush = CreateSolidBrush(RGB(0, 0, 255));
		SelectObject(hdc, hBrush);
		Rectangle(hdc, x - 10, y - 10, x + 60, y + 60); // ���콺�κ��� (10,10) ������ ��ġ�� 60x60 ������ ���
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		for (int i = 0; i < iCount; i++) {
			hBrush = CreateSolidBrush(RGB(0, 0, 255));
			SelectObject(hdc, hBrush);
			Rectangle(hdc, p[i].x - 10, p[i].y - 10, p[i].x + 60, p[i].y + 60);
		}
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
