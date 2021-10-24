#include <windows.h>
#include "resource.h"  // ���ҽ� �����ϱ� ���� ������� ����

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("Mouse");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {
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
	//WndClass.lpszMenuName = NULL;
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1); // ��ȭ���� �޴���
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

/* �� ����ü */
typedef struct _line {
	POINT p[1000];
	int iPointCount;
	COLORREF penColor;
	int iPenWidth;
} line;
line lines[500];
int iLineCount;

COLORREF CurrentPenColor = RGB(255, 0, 0);
int iCurrentPenWidth = 2;
TCHAR* Items[] = { TEXT("2"), TEXT("4"), TEXT("6"), TEXT("8") };
HWND hRadioR, hRadioG, hRadioB, hList;

BOOL CALLBACK DlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	static COLORREF iColor; // ��ȭ���� ���� ���õ� �� ��
	static int iWidth; // ��ȭ���� ���� ���õ� �� ����
	int i;

	switch (iMessage) {
	case WM_INITDIALOG: // WM_CREATE
		iColor = CurrentPenColor;
		iWidth = iCurrentPenWidth;

		hRadioR = GetDlgItem(hDlg, IDC_RADIO1);
		hRadioG = GetDlgItem(hDlg, IDC_RADIO2);
		hRadioB = GetDlgItem(hDlg, IDC_RADIO3);
		hList = GetDlgItem(hDlg, IDC_LIST1);

		if (CurrentPenColor == RGB(255, 0, 0))
			CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO3, IDC_RADIO1);
		else if (CurrentPenColor == RGB(0, 255, 0))
			CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO3, IDC_RADIO2);
		else if (CurrentPenColor == RGB(0, 0, 255))
			CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO3, IDC_RADIO3);

		for (i = 0; i < 4; i++) {
			SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)Items[i]);
		}
		SendMessage(hList, LB_SETCURSEL, iWidth / 2 - 1, 0);
		return TRUE;


	case WM_COMMAND:
		switch (LOWORD(wParam)) {

			/* ���� ��ư ����*/
		case IDC_RADIO1:
			iColor = RGB(255, 0, 0);
			break;
		case IDC_RADIO2:
			iColor = RGB(0, 255, 0);
			break;
		case IDC_RADIO3:
			iColor = RGB(0, 0, 255);
			break;

			/* listBox ����*/
		case IDC_LIST1:
			switch (HIWORD(wParam)) {
			case LBN_SELCHANGE: //����Ʈ ���ý�
				i = SendMessage(hList, LB_GETCURSEL, 0, 0); //���� ������ i�� ����
				iWidth = (i + 1) * 2;
				break;
			}
			break;

			/* ��ȭ���� ���� ���� ����*/
		case IDOK:
			CurrentPenColor = iColor;
			iCurrentPenWidth = iWidth;
			EndDialog(hDlg, IDOK);
			return TRUE;
		case IDCANCEL:
			EndDialog(hDlg, IDCANCEL);
			return TRUE;
		}
		break;
	}
	return FALSE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	static int x, y;
	HPEN hPen;

	switch (iMessage) {
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_BACK:
			if (iLineCount > 0) {
				iLineCount--;
				lines[iLineCount].iPointCount = 0;
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
		}
		return 0;

	case WM_COMMAND:
		switch(LOWORD(wParam)) {
		case IDM_DIALOG:
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DlgProc);
		}
		return 0;
	
	case WM_LBUTTONDOWN:
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		lines[iLineCount].p[lines[iLineCount].iPointCount].x = x;
		lines[iLineCount].p[lines[iLineCount].iPointCount].y = y;
		lines[iLineCount].iPointCount++;
		lines[iLineCount].penColor = CurrentPenColor;
		lines[iLineCount].iPenWidth = iCurrentPenWidth;
		return 0;

	case WM_MOUSEMOVE:
		if (wParam & MK_LBUTTON) {
			hdc = GetDC(hWnd);
			hPen = CreatePen(PS_SOLID, lines[iLineCount].iPenWidth, lines[iLineCount].penColor);
			SelectObject(hdc, hPen);
			MoveToEx(hdc, x, y, NULL);
			x = LOWORD(lParam);
			y = HIWORD(lParam);
			LineTo(hdc, x, y);
			ReleaseDC(hWnd, hdc);
			lines[iLineCount].p[lines[iLineCount].iPointCount].x = x;
			lines[iLineCount].p[lines[iLineCount].iPointCount].y = y;
			lines[iLineCount].iPointCount++;
		}
		return 0;

	case WM_LBUTTONUP:
		iLineCount++;
		return 0;
	
	case WM_LBUTTONDBLCLK:
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		for (int i = 0; i < iLineCount; i++) {
			hPen = CreatePen(PS_SOLID, lines[i].iPenWidth, lines[i].penColor);
			hPen = (HPEN)SelectObject(hdc, hPen);
			MoveToEx(hdc, lines[i].p[0].x, lines[i].p[0].y, NULL);
			for (int j = 1; j < lines[i].iPointCount; j++) {
				LineTo(hdc, lines[i].p[j].x, lines[i].p[j].y);
			}
			DeleteObject(SelectObject(hdc, hPen));
		}
		EndPaint(hWnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
