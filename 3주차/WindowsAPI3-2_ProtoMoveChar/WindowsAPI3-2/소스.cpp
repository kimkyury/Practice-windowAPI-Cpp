#include <windows.h>
#include <time.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("PROG");

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
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	hWndMain = hWnd;

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}
int x, y;
TCHAR str[26];

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	SYSTEMTIME st;
	static RECT rect;

	switch (iMessage) {
	case WM_CREATE:
		srand((unsigned int)time(NULL)); // ���� ��ġ ����
		GetClientRect(hWnd, &rect); // (hWnd, Rect����ü������), �������� �۾����� ũ�⸦ �����

		SetTimer(hWnd, 1, 100, NULL); // 0.1�� ������ ���������� ��ġ ��ȯ
		//	SendMessage(hWnd, WM_TIMER, 1, 0);
		str[0] = rand() % 26 + 97; // ����� ���ĺ� �ڵ� ���
		y = rand() % (rect.right - 40) + 20; // �����ġ�� ���� x��ǥ ���

		return 0;

	case WM_SIZE:
		GetClientRect(hWnd, &rect);
		return 0;

	case WM_TIMER:

		/* �¿� �������� �ٲٰ� �ʹٸ�, (1) rect.right ���� �ٲ۴�.*/
		if (y >= rect.bottom){ //���ڰ� �ٴ� ��ġ�� ������
			str[0] = rand() % 26 + 97; //���ο� ���� ����

			/* �¿� �������� �ٲٰ� �ʹٸ�, (2) rect.botom���� �ٲ۴�.*/

			x = rand() % (rect.right - 40) + 20; //  x��(������ġ) . 40�� �� ���� �� ����, �������� 20���� Ŀ�� ��.
			y = 0; // y��(������ġ)
		}
		y += 60; // ��ġ ����
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		/* �¿� �������� �ٲٰ� �ʹٸ�, (3) x�� y�� ��ġ�� �ٲ۴�.*/
		TextOut(hdc, x, y, str, lstrlen(str)); // (hdc, x��ǥ, y��ǥ, ���ڿ�, ���ڿ�����)
		EndPaint(hWnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
