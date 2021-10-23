#include <windows.h>
#include "resource.h"  // ���ҽ� �����ϱ� ���� ������� ����

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("Mouse");

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
	//WndClass.lpszMenuName = NULL;
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;// | CS_DBLCLKS;
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

typedef struct _line {
	POINT p[1000];
	int iPointCount;
	COLORREF penColor; // ���� �Ӽ�
} line;  //�� ��ü �� ���� ǥ��

line lines[500];
int iLineCount; // �� ��ü�� �� ��
COLORREF CurrentPenColor = RGB(255, 0, 0); // ���� �� ��ü�� ����


/* radio�� ���� �������� */
enum { ID_R1 = 101, ID_R2, ID_R3};
HWND r1, r2, r3;
/* radio�� ���� �������� */


LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps; //�׷��� ȭ��
	static int x;
	static int y;
	HPEN hPen; // ���������� ���� �� ��ü
	//static BOOL bNowDraw=FALSE;

	switch (iMessage) {
	/* radio�� ���� �ڵ� */
	case WM_CREATE: 
		CreateWindow(TEXT("button"), TEXT("Color"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 5, 5, 120, 110, hWnd, (HMENU)1, g_hInst, NULL); // �׷�ڽ� ����
		r1 = CreateWindow(TEXT("button"), TEXT("RED"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP, 10, 20, 100, 30, hWnd, (HMENU)ID_R1, g_hInst, NULL);
		r2 = CreateWindow(TEXT("button"), TEXT("GREEN"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP, 10, 50, 100, 30, hWnd, (HMENU)ID_R2, g_hInst, NULL);
		r3 = CreateWindow(TEXT("button"), TEXT("BLUE"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP, 10, 80, 100, 30, hWnd, (HMENU)ID_R3, g_hInst, NULL);
		CheckRadioButton(hWnd, ID_R1, ID_R3, ID_R1);// (hWnd, �׷쳻 ù��° ���� ��ư, �׷� �� ������ ���� ��ư, üũ�� ���� ��ư)
		break;
	/* radio�� ���� �ڵ� */

	case WM_KEYDOWN:
		switch (wParam) {
		case VK_BACK: // �� ��ü �����
			if (iLineCount > 0) {
				iLineCount--;
				lines[iLineCount].iPointCount = 0; // �ʱ�ȭver1 : �齺���̽��� ���� ������ ī��Ʈ�� �ʱ�ȭ��Ű��
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
		}
		return 0;
	case WM_COMMAND: //�޴��� �����ϱ�
		switch (LOWORD(wParam)) {
		case IDM_RED:
		case ID_R1:
			CheckRadioButton(hWnd, ID_R1, ID_R3, ID_R1);
			CurrentPenColor = RGB(255, 0, 0);
			SetFocus(hWnd); //������ ��Ŀ�� �̵���Ű�� (������ư�� ��Ŀ���� �������Ƿ�)
			break;
		case IDM_GREEN:
		case ID_R2:
			CheckRadioButton(hWnd, ID_R1, ID_R3, ID_R2);
			CurrentPenColor = RGB(0, 255, 0);
			SetFocus(hWnd);
			break;
		case IDM_BLUE:
		case ID_R3:
			CheckRadioButton(hWnd, ID_R1, ID_R3, ID_R3);
			CurrentPenColor = RGB(0, 0, 255);
			SetFocus(hWnd);
			break;
		}
		return 0;
	case WM_LBUTTONDOWN: // �� �׸��� ����
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		//	bNowDraw=TRUE;

		// lines[iLineCount].iPointCount = 0;   // �ʱ�ȭ.ver2 : ���ʹ�ư�� ���� ������ ī��Ʈ �ʱ�ȭ�ϱ�

		/* ����ü�� �� ���� (������) �����ϱ� */
		lines[iLineCount].p[lines[iLineCount].iPointCount].x = x;
		lines[iLineCount].p[lines[iLineCount].iPointCount].y = y;
		//lines[iLineCount].iPointCount++;

		lines[iLineCount].penColor = CurrentPenColor;

		return 0;
	case WM_MOUSEMOVE:
		if (wParam & MK_LBUTTON) {  // & = ��Ʈ������  // C���� 0 = False, 0�̿��� �� = True
			// MK_LBUTTON   0x0001 ->00000....0001
			// MK_RBUTTON   0x0002 ->00000....0010 

			hdc = GetDC(hWnd);

			/* �� ���� */
			hPen = CreatePen(PS_SOLID, 1, lines[iLineCount].penColor);
			SelectObject(hdc, hPen);
			MoveToEx(hdc, x, y, NULL);
			x = LOWORD(lParam);
			y = HIWORD(lParam);
			LineTo(hdc, x, y);
			ReleaseDC(hWnd, hdc);

			/* �� ��ü ���� ���� (�����̴� ��) */
			lines[iLineCount].p[lines[iLineCount].iPointCount].x = x;
			lines[iLineCount].p[lines[iLineCount].iPointCount].y = y;
			lines[iLineCount].iPointCount++;

		}
		return 0;
	case WM_LBUTTONUP:
		//	bNowDraw=FALSE;

		/* �� ��ü�� ���� ���� ( �� ��) */
		iLineCount++; // �� �ϳ� ����

		return 0;
	case WM_LBUTTONDBLCLK:
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	case WM_PAINT: // ����� ���� ���� �׸��� ����
		hdc = BeginPaint(hWnd, &ps);
		for (int i = 0; i < iLineCount; i++) {

			hPen = CreatePen(PS_SOLID, 1, lines[i].penColor);
			hPen = (HPEN)SelectObject(hdc, hPen); // �� ��ü ����

			MoveToEx(hdc, lines[i].p[0].x, lines[i].p[0].y, NULL);
			for (int j = 1; j < lines[i].iPointCount; j++) {
				LineTo(hdc, lines[i].p[j].x, lines[i].p[j].y);
			}
			DeleteObject(SelectObject(hdc, hPen)); //������ ���� ���ֶ�. ��������.
		}
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
