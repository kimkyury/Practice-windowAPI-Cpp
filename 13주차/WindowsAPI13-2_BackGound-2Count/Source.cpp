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

HWND hWndMain;
TCHAR str[256]; //�Է��� ���� ����

/* 'ī��Ʈ �� ī��Ʈ ���' ������, ������ ��׶��� ������� */
DWORD WINAPI ThreadFunc(LPVOID temp){
	int iCount;
	TCHAR buf[256];
	HDC hdc = GetDC(hWndMain);

	// a�� ���Ͽ� ī��Ʈ ��Ű��
	iCount = 0;
	for (int i = 0; i < lstrlen(str); i++){
		if (str[i] == TCHAR('a')) iCount++;
		Sleep(10); //Complex BackGorund Job (��?) 
	}

	// ī��Ʈ �� ���� ���
	wsprintf(buf, TEXT("%c�� ���� : %d"), TCHAR('a'), iCount);
	TextOut(hdc, 10, 10, buf, lstrlen(buf));

	ReleaseDC(hWndMain, hdc);
	return 0;
}

/* ������ */
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam){
	HDC hdc;
	PAINTSTRUCT ps;
	int i, len;

	DWORD ThreadID;
	HANDLE hThread;

	/* �Է��� ���� ��� */
	switch (iMessage){
	case WM_CHAR:
		len = lstrlen(str);
		str[len] = (TCHAR)wParam;
		str[len + 1] = 0;

		/* Ű �Է� �߻� �� ������ �۵�*/
		hThread = CreateThread(NULL, 0, ThreadFunc, NULL, 0, &ThreadID); 
		InvalidateRect(hWnd, NULL, FALSE);
		return 0;

	case WM_CREATE:
		hWndMain = hWnd;
		//CloseHandle(hThread); //�� ��������.
		return 0;

	case WM_LBUTTONDOWN: //�����
		hdc = GetDC(hWnd);
		Ellipse(hdc, LOWORD(lParam) - 10, HIWORD(lParam) - 10, LOWORD(lParam) + 10, HIWORD(lParam) + 10);
		ReleaseDC(hWnd, hdc);
		return 0;

	case WM_PAINT: // �Է� ���� ����ϱ�
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