#include <windows.h>
#include "resource.h"  // ���ҽ� �����ϱ� ���� ������� ����

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("20192608��Ը�");

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
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
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
int newX, newY, oldX, oldY;  // �����߱�


/* �� ����ü */
typedef struct tagP
{
	int x;
	int y;
} pxy;
pxy p[1000];
int iCount = 0;



/* ���̾�α� �������� */
HWND hRadioR, hRadioG, hRadioB; // radio
HWND hList, hCombo; // ����Ʈ, �޺� 
TCHAR* Items[] = { TEXT("2"), TEXT("4"), TEXT("6"), TEXT("8") };
TCHAR* Items2[] = { TEXT("�����"), TEXT("���׸���")};
HWND hSRed, hSGreen, hSBlue; // ��ũ��
HWND hWndMain;


// ���׸���� ����⸸ �����غ���.
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps; //�׷��� ȭ��
	HPEN hPen, hPen2;  // ���׸����
	HBRUSH hBrush; //������

	static BOOL bNowDraw = FALSE;
	static int x, y;  // ��ġ��
	static int iShape; // 0�̸� ���׸���, 1�̸� �����

	switch (iMessage) {

	case WM_CREATE:
		hWndMain = hWnd;
		return 0;

	case WM_COMMAND: //�޴��� �����ϱ�
		switch (LOWORD(wParam)) {
			case ID_LINE: // ���׸���
				iShape = 0;
				break;
			case ID_DOT: // �����
				iShape = 1;
				break;
		}
		return 0;

	case WM_LBUTTONDOWN: // �� �׸��� ����
		if (iShape == 0) {  // ���׸��� ���

			/* ���� � �׸���
			x = LOWORD(lParam);
			y = HIWORD(lParam);
			lines[iLineCount].p[lines[iLineCount].iPointCount].x = x;  
			lines[iLineCount].p[lines[iLineCount].iPointCount].y = y;
			lines[iLineCount].iPointCount++;
			lines[iLineCount].penColor = CurrentPenColor;
			return 0;
			*/

			/* ���� �׸��� */
			lines[iLineCount].penColor = CurrentPenColor;
			lines[iLineCount].iPenWidth = iCurrentPenWidth;
			newX = LOWORD(lParam);
			newY = HIWORD(lParam);
			oldX = newX;
			oldY = newY;
			lines[iLineCount].p[0].x = newX; //������
			lines[iLineCount].p[0].y = newY; //������
			bNowDraw = TRUE;

		}
		else if (iShape == 1) {   //�� �����.
			hdc = GetDC(hWnd);
			x = LOWORD(lParam);
			y = HIWORD(lParam);
			p[iCount].x = x;
			p[iCount].y = y;
			iCount++;
			InvalidateRect(hWnd, NULL, FALSE);
		}
		return 0;


	case WM_MOUSEMOVE:
		if (iShape == 0) {
			if (wParam & MK_LBUTTON) {  
				hdc = GetDC(hWnd);

				/* ����� �׸��� 
				hPen = CreatePen(PS_SOLID, iCurrentPenWidth, CurrentPenColor);
				SelectObject(hdc, hPen);
				MoveToEx(hdc, x, y, NULL);
				x = LOWORD(lParam);
				y = HIWORD(lParam);
				LineTo(hdc, x, y);
				ReleaseDC(hWnd, hdc);
				// �� ��ü ���� ���� (�����̴� ��)
				lines[iLineCount].p[lines[iLineCount].iPointCount].x = x;
				lines[iLineCount].p[lines[iLineCount].iPointCount].y = y;
				lines[iLineCount].iPointCount++; 
				ReleaseDC(hWnd, hdc);	*/


				/* ���� �׸���*/
				if (bNowDraw) {
					SetROP2(hdc, R2_NOTXORPEN);
					hPen = CreatePen(PS_SOLID, iCurrentPenWidth, CurrentPenColor);
					hPen = (HPEN)SelectObject(hdc, hPen);
					MoveToEx(hdc, newX, newY, NULL);
					LineTo(hdc, oldX, oldY);
					oldX = LOWORD(lParam);
					oldY = HIWORD(lParam);
					MoveToEx(hdc, newX, newY, NULL);
					LineTo(hdc, oldX, oldY);
					ReleaseDC(hWnd, hdc);
				}
			}
		} else if (iShape == 1) {

		}
		return 0;

	case WM_LBUTTONUP:
		if (iShape == 0) {

			/* ����� �׸��� 
			iLineCount++; // �� �ϳ� ����
			*/

			/* ���� �׸���*/
			bNowDraw = FALSE;
			hdc = GetDC(hWnd);
			hPen = CreatePen(PS_SOLID, iCurrentPenWidth, CurrentPenColor);
			hPen = (HPEN)SelectObject(hdc, hPen);
			MoveToEx(hdc, newX, newY, NULL);
			LineTo(hdc, oldX, oldY);
			lines[iLineCount].p[1].x = oldX; // ����
			lines[iLineCount].p[1].y = oldY; // ����
			iLineCount++;
			ReleaseDC(hWnd, hdc);
		}
		else {

		}
		return 0;
	
	case WM_LBUTTONDBLCLK:
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	
	case WM_PAINT: // ����� ���� ���� �׸��� ����
		if (iShape == 0) {

			/* ���� � �׸���
			hdc = BeginPaint(hWnd, &ps);
			for (int i = 0; i < iLineCount; i++) {
				hPen = CreatePen(PS_SOLID, 1, lines[i].penColor);
				SelectObject(hdc, hPen);
				MoveToEx(hdc, lines[i].p[0].x, lines[i].p[0].y, NULL);
				for (int j = 1; j < lines[i].iPointCount; j++) {
					LineTo(hdc, lines[i].p[j].x, lines[i].p[j].y);
				}
			}
			EndPaint(hWnd, &ps);
			*/

			/* ���� �׸��� */
			hdc = BeginPaint(hWnd, &ps);
			for (int i = 0; i < iLineCount; i++)
			{
				hPen = CreatePen(PS_SOLID, lines[i].iPenWidth, lines[i].penColor);
				hPen = (HPEN)SelectObject(hdc, hPen);
				MoveToEx(hdc, lines[i].p[0].x, lines[i].p[0].y, NULL);
				LineTo(hdc, lines[i].p[1].x, lines[i].p[1].y);
			}
			EndPaint(hWnd, &ps);
		}
		else if (iShape == 1) { // �������
			hdc = BeginPaint(hWnd, &ps);
			hPen2 = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
			SelectObject(hdc, hPen2);
			hBrush = CreateSolidBrush(RGB(0, 0, 255));
			SelectObject(hdc, hBrush);

			for (int i = 0; i < iCount; i++){
				Rectangle(hdc, p[i].x - 20, p[i].y - 20, p[i].x + 20, p[i].y + 20);
			}

			EndPaint(hWnd, &ps);
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
