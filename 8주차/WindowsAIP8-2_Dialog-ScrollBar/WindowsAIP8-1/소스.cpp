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


/* �޴��� ���� 
// �� ����ü
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

// �� ����ü
typedef struct tagP{
	int x;
	int y;
	int iColor;
	int iShape;
} pxy;
pxy p[1000];
int iCount = 0;
*/


typedef struct _Rect {  // �簢�� �ٷ��
	POINT p;
	POINT oldp;
	int iColor;
	int iShape;
}Rect;

Rect Rects[500];
int iRects;
int iCurrentwidth = 1;
int iCurrentShape = 0;
int iTempCount;
COLORREF iCurrentRGB = RGB(0, 0, 0);
TCHAR* Items1[] = { TEXT("5"), TEXT("10"), TEXT("15"), TEXT("20") };
TCHAR* Items2[] = { TEXT("���׸���"), TEXT("�����") };
HWND hRed, hGreen, hBlue;
HWND hSRed, hSGreen, hSBlue;
HWND hCombo1, hCombo2;
HWND hWndMain;

void PaintWindow(HWND hwnd, int iColor, int iShape) {
	HPEN          hPen;
	HBRUSH	      hBrush;
	HDC             hdc;
	RECT            rect;
	hdc = GetDC(hwnd);
	GetClientRect(hwnd, &rect);
	hBrush = CreateSolidBrush(iColor);
	hBrush = (HBRUSH)SelectObject(hdc, hBrush);
	hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	hPen = (HPEN)SelectObject(hdc, hPen);
	if (iShape == 0)
		Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
	else
		Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);
	DeleteObject(SelectObject(hdc, hBrush));
	DeleteObject(SelectObject(hdc, hPen));
	ReleaseDC(hwnd, hdc);

}

void PaintTheBlock(HWND hCtrl, int iColor, int iShape) {
	InvalidateRect(hCtrl, NULL, TRUE);
	UpdateWindow(hCtrl);
	PaintWindow(hCtrl, iColor, iShape);
}



BOOL CALLBACK InfoDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam) {

	int i;
	int iWidth;
	int TempPos;
	static int Red, Green, Blue;
	static int dWidth;
	static HWND hCtrlBlock;
	TCHAR str[128];
	switch (iMessage) {
	case WM_INITDIALOG:
		hCtrlBlock = GetDlgItem(hDlg, IDC_STATIC4); //�̸����� â

		hRed = GetDlgItem(hDlg, IDC_SCROLLBAR1); //��ũ��1
		hGreen = GetDlgItem(hDlg, IDC_SCROLLBAR2); //��ũ��2
		hBlue = GetDlgItem(hDlg, IDC_SCROLLBAR3); // ��ũ��3
		SetScrollRange(hRed, SB_CTL, 0, 255, TRUE);
		SetScrollPos(hRed, SB_CTL, Red, TRUE);
		SetScrollRange(hGreen, SB_CTL, 0, 255, TRUE);
		SetScrollPos(hGreen, SB_CTL, Green, TRUE);
		SetScrollRange(hBlue, SB_CTL, 0, 255, TRUE);
		SetScrollPos(hBlue, SB_CTL, Blue, TRUE);
		hSRed = GetDlgItem(hDlg, IDC_STATIC1); //��ũ��1
		hSGreen = GetDlgItem(hDlg, IDC_STATIC2); //��ũ��2
		hSBlue = GetDlgItem(hDlg, IDC_STATIC3); //��ũ��3

		Red = GetRValue(iCurrentRGB);
		Green = GetGValue(iCurrentRGB);
		Blue = GetBValue(iCurrentRGB);

		wsprintf(str, TEXT("%d"), Red);
		SetWindowText(hSRed, str);
		wsprintf(str, TEXT("%d"), Green);
		SetWindowText(hSGreen, str);
		wsprintf(str, TEXT("%d"), Blue);
		SetWindowText(hSBlue, str);

		hCombo1 = GetDlgItem(hDlg, IDC_COMBO1); // �׸����� (�����, ���׸���)
 		//hCombo2 = GetDlgItem(hDlg, IDC_COMBO2); //�� ���� 

		for (i = 0; i < 2; i++) { //�׸����� �׸� �߰�
			SendMessage(hCombo1, CB_ADDSTRING, 0, (LPARAM)Items2[i]);
		}
		SendMessage(hCombo1, CB_SETCURSEL, iCurrentShape, 0);

		/* for (iWidth = 0; iWidth < 4; iWidth++) {
			SendMessage(hCombo2, CB_ADDSTRING, 0, (LPARAM)Items1[iWidth]);
		}
		SendMessage(hCombo2, CB_SETCURSEL, iCurrentwidth, 0);
		return TRUE;
		*/

	case WM_HSCROLL:
		PaintTheBlock(hCtrlBlock, RGB(Red, Green, Blue), SendMessage(hCombo1, CB_GETCURSEL, 0, 0));
		TempPos = GetScrollPos((HWND)lParam, SB_CTL);
		switch (LOWORD(wParam)) {
		case SB_LINELEFT:
			TempPos = max(0, TempPos - 1);
			break;
		case SB_LINERIGHT:
			TempPos = min(255, TempPos + 1);
			break;
		case SB_PAGELEFT:
			TempPos = max(0, TempPos - 10);
			break;
		case SB_PAGERIGHT:
			TempPos = min(255, TempPos + 10);
			break;
		case SB_THUMBTRACK:
			TempPos = HIWORD(wParam);
			break;
		}
		if ((HWND)lParam == hRed) Red = TempPos;
		if ((HWND)lParam == hGreen) Green = TempPos;
		if ((HWND)lParam == hBlue) Blue = TempPos;
		SetScrollPos((HWND)lParam, SB_CTL, TempPos, TRUE);
		wsprintf(str, TEXT("%d"), Red);
		SetWindowText(hSRed, str);
		wsprintf(str, TEXT("%d"), Green);
		SetWindowText(hSGreen, str);
		wsprintf(str, TEXT("%d"), Blue);
		SetWindowText(hSBlue, str);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			EndDialog(hDlg, IDOK);
			i = SendMessage(hCombo1, CB_GETCURSEL, 0, 0);
			iCurrentShape = i;
			iCurrentRGB = RGB(Red, Green, Blue);
			return TRUE;
		case IDCANCEL:
			EndDialog(hDlg, IDCANCEL);
			return TRUE;
		case IDC_COMBO1:
			switch (HIWORD(wParam)) {
			case CBN_SELCHANGE:
				i = SendMessage(hCombo1, CB_GETCURSEL, 0, 0);
				PaintTheBlock(hCtrlBlock, RGB(Red, Green, Blue), i);
				break;
			}
			return TRUE;
		}
		break;
	case WM_PAINT:
		PaintTheBlock(hCtrlBlock, RGB(Red, Green, Blue), SendMessage(hCombo1, CB_GETCURSEL, 0, 0));
		break;
	}
	return FALSE;
}



// �޴��ٸ� ���� ���׸���� ����⸸ ����
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	
	HDC hdc;
	PAINTSTRUCT ps;
	static int x, y, oldx, oldy;
	HBRUSH MyBrush;
	HPEN MyPen;
	switch (iMessage) {
	case WM_CREATE:
		hWndMain = hWnd;
		return 0;
	case WM_LBUTTONDOWN:
		iTempCount = 0;
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		oldx = x;
		oldy = y;
		Rects[iRects].p.x = x;
		Rects[iRects].p.y = y;
		iTempCount++;
		return 0;
	case WM_MOUSEMOVE:
		if (wParam & MK_LBUTTON) {
			hdc = GetDC(hWnd);
			SetROP2(hdc, R2_NOT);
			MyBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
			MyBrush = (HBRUSH)SelectObject(hdc, MyBrush);
			MyPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
			MyPen = (HPEN)SelectObject(hdc, MyPen);
			MoveToEx(hdc, x, y, NULL);
			if (iCurrentShape == 0) Rectangle(hdc, x, y, oldx, oldy);
			else Ellipse(hdc, x, y, oldx, oldy);
			oldx = LOWORD(lParam);
			oldy = HIWORD(lParam);
			MoveToEx(hdc, x, y, NULL);
			if (iCurrentShape == 0) Rectangle(hdc, x, y, oldx, oldy);
			else Ellipse(hdc, x, y, oldx, oldy);
			DeleteObject(SelectObject(hdc, MyBrush));
			DeleteObject(SelectObject(hdc, MyPen));
			ReleaseDC(hWnd, hdc);
			iTempCount++;
		}
		return 0;
	case WM_LBUTTONUP:
		hdc = GetDC(hWnd);
		MyBrush = CreateSolidBrush(iCurrentRGB);
		MyBrush = (HBRUSH)SelectObject(hdc, MyBrush);
		if (iCurrentShape == 0) Rectangle(hdc, x, y, oldx, oldy);
		else Ellipse(hdc, x, y, oldx, oldy);
		DeleteObject(SelectObject(hdc, MyBrush));
		ReleaseDC(hWnd, hdc);
		Rects[iRects].iShape = iCurrentShape;
		Rects[iRects].iColor = iCurrentRGB;
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		Rects[iRects].oldp.x = x;
		Rects[iRects].oldp.y = y;
		iRects++;
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		for (int i = 0; i < iRects; i++) {
			MyBrush = CreateSolidBrush(Rects[i].iColor);
			MyBrush = (HBRUSH)SelectObject(hdc, MyBrush);
			MyPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
			MyPen = (HPEN)SelectObject(hdc, MyPen);
			if (Rects[i].iShape == 0) {
				Rectangle(hdc, Rects[i].p.x, Rects[i].p.y, Rects[i].oldp.x, Rects[i].oldp.y);
			}
			else Ellipse(hdc, Rects[i].p.x, Rects[i].p.y, Rects[i].oldp.x, Rects[i].oldp.y);
			DeleteObject(SelectObject(hdc, MyBrush));
			DeleteObject(SelectObject(hdc, MyPen));
		}
		EndPaint(hWnd, &ps);
		return 0;
	case WM_COMMAND:


		switch (LOWORD(wParam)) {
		case IDM_DIALOG:
			KillTimer(hWnd, 1);
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, InfoDlgProc);
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
		return 0;


	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));





	/* �޴��� ���� 
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

			// ���� � �׸���
			x = LOWORD(lParam);
			y = HIWORD(lParam);
			lines[iLineCount].p[lines[iLineCount].iPointCount].x = x;  
			lines[iLineCount].p[lines[iLineCount].iPointCount].y = y;
			lines[iLineCount].iPointCount++;
			lines[iLineCount].penColor = CurrentPenColor;
			return 0;
			

			// ���� �׸��� 
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

				// ����� �׸��� 
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
				ReleaseDC(hWnd, hdc);	


				// ���� �׸��� 
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

			// ����� �׸��� 
			iLineCount++; // �� �ϳ� ����
			

			// ���� �׸���
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

			// ���� � �׸���
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
			

			// ���� �׸��� 
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
	*/
}
