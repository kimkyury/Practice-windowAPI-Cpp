#include <windows.h>
#include <string>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("ButtonSubClassing");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow){
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
// WM_SORI = 0x401���� ������ �޽������� ����. (WM_USER�� 0x400����) 
#define WM_SORI WM_USER +1

HWND hButton, hWndSori; //��ư �ڵ�
WNDPROC OldButtonProc;
// char buffer[20]; // ����� ���� ����

//UserMes���� ���ϰ� �������� ���� �߰� ����
int ret;
HWND hStatic;//����� ���̺� �ڵ�

//SubProc�ۼ�
LRESULT CALLBACK ButtonSubProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam){
	TCHAR buffer[20]; // ��� ���� ������ �̹����� ���ο��� ����
	
	switch (iMessage){
	case WM_LBUTTONDOWN:
		CallWindowProc(OldButtonProc, hWnd, iMessage, wParam, lParam);
		hWndSori = FindWindow(NULL, TEXT("UserMes")); //ĸ���� UserMes�� Window�� ã�´�
		if (hWndSori == NULL){
			MessageBox(hWnd, TEXT(" ������α׷��� �������� �ʽ��ϴ�."),
				TEXT("�̷�!"), MB_OK);
		}
		else{
			/* ret = SendMessage(hWndSori, WM_SORI, 0, 0); //ã���� Message�� �����߾���
			sprintf(buffer, "%d", ret);
			SetFocus(NULL); // Focus�� ���ش�, ������ ������ ��� �޽����� hButton���� ���޵ȴ�
			hWndSori �������� ������ ���Ͻ����� ȣ���Ѵ� */

			ret = PostMessage(hWndSori, WM_SORI, 0, 0);
			wsprintf(buffer, TEXT("���ϰ�: %d"), ret);
			SetWindowText(hStatic, buffer);
		}
		return 0; // ��, break
	}
	return CallWindowProc(OldButtonProc, hWnd, iMessage, wParam, lParam);
}

//WndProc�ۼ�
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam){
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR *Mes = TEXT("UserMes ���α׷����� �޽����� �����ϴ�.");

	switch (iMessage){
	case WM_CREATE:
		hButton = CreateWindow(TEXT("button"), TEXT("Click me"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 10, 200, 25, hWnd, (HMENU)101, g_hInst, NULL);
		OldButtonProc = (WNDPROC)SetWindowLongPtr(hButton, GWLP_WNDPROC, (LONG_PTR)ButtonSubProc);
		
		//���̺� ��Ʈ�� �ۼ�
		hStatic = CreateWindow(TEXT("static"), TEXT("-"), WS_CHILD | WS_VISIBLE, 10, 100, 200, 25, hWnd, (HMENU)102, g_hInst, NULL);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 50, 50, Mes, lstrlen(Mes));
		/* TextOut(hdc, 50, 100, TEXT(buffer), lstrlen(buffer)); */

		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0); 
		return 0;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}