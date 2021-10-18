#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("MyTimer");

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
	SYSTEMTIME st;
	static TCHAR sTime[128];
	static RECT rt = { 100,100,400,120 };

	switch (iMessage) {
	case WM_CREATE: //window �� ��, §�� �� ��Ÿ�� (p.99 �߰�)
		SetTimer(hWnd, 1, 1000, NULL); // (�ڵ�, Ÿ�̸�ID, 1��, WM_TIMER�޽����� WndProc�� ����(���� NULL����))
		//SendMessage(hWnd, WM_TIMER, 1, 0); //(������ �޽���, wParam�޽����߰�����, lParam�޽����߰�����)
		return 0;
	case WM_TIMER:
		GetLocalTime(&st); // (������ýð��� ���Թ��� SYSTEMTIME ����ü)
		wsprintf(sTime, TEXT("���� �ð��� %d:%d:%d�Դϴ�"),   //sTime = buffer, %d= format(����), 
			st.wHour, st.wMinute, st.wSecond);
		//InvalidateRect(hWnd, NULL, FALSE); // TRUE�� FALSE�� �ٲ㺸�� �͵� ���� 
		InvalidateRect(hWnd,&rt,TRUE);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 100, 100, sTime, lstrlen(sTime)); //(hdc, x��ǥ, y��ǥ, ��¹��ڿ�, ���ڿ�����)
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		KillTimer(hWnd, 1); // (�ڵ�, Ÿ�̸�ID) Ÿ�̸Ӵ� ����� KillTimer�� �ı��ؾ� �Ѵ�. 
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
