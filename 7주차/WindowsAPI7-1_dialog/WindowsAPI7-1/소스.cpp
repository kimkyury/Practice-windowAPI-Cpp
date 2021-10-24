#include <windows.h>
#include "resource.h"  // 리소스 적용하기 위한 헤더파일 적용

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("infoDlg");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // ?
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	//WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1); //메뉴바 제거
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
int x, y;
TCHAR str[128];

BOOL CALLBACK InfoDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage) {
	case WM_INITDIALOG: // WM_CREATE
		SetDlgItemText(hDlg, IDC_STR, str);
		SetDlgItemInt(hDlg, IDC_X, x, FALSE);
		SetDlgItemInt(hDlg, IDC_Y, y, FALSE);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			GetDlgItemText(hDlg, IDC_STR, str, 128);
			x = GetDlgItemInt(hDlg, IDC_X, NULL, FALSE);
			y = GetDlgItemInt(hDlg, IDC_Y, NULL, FALSE);
			EndDialog(hDlg, IDOK); //OK버튼을 누르면 종료할 수 있게 만듦
			// IODK는 InfoDlgpro() 대화상자 procedure의 return값
			return TRUE;
		case IDCANCEL:
			EndDialog(hDlg, IDCANCEL);
			return TRUE;
		}
		break;
	}
	return FALSE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam){
	HDC hdc;
	PAINTSTRUCT ps; 

	switch (iMessage) {
	case WM_CREATE:
		hWndMain = hWnd;
		x = 100;
		y = 100;
		lstrcpy(str, TEXT("String"));
		return 0;

	case WM_PAINT: 
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, x, y, str, lstrlen(str));
		EndPaint(hWnd, &ps);
		return 0;

	case WM_LBUTTONDOWN:
		if (DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, InfoDlgProc) == IDOK)
			InvalidateRect(hWnd, NULL, TRUE);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
