#include <windows.h>
#include "resource.h"  // 리소스 적용하기 위한 헤더파일 적용

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("20192608김규리");

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

/* 선 구조체 */
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
int newX, newY, oldX, oldY;  // 직선긋기


/* 점 구조체 */
typedef struct tagP
{
	int x;
	int y;
} pxy;
pxy p[1000];
int iCount = 0;



/* 다이얼로그 전역변수 */
HWND hRadioR, hRadioG, hRadioB; // radio
HWND hList, hCombo; // 리스트, 콤보 
TCHAR* Items[] = { TEXT("2"), TEXT("4"), TEXT("6"), TEXT("8") };
TCHAR* Items2[] = { TEXT("점찍기"), TEXT("선그리기")};
HWND hSRed, hSGreen, hSBlue; // 스크롤
HWND hWndMain;


// 선그리기와 점찍기만 구현해보자.
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps; //그려낼 화면
	HPEN hPen, hPen2;  // 선그리기용
	HBRUSH hBrush; //점찍기용

	static BOOL bNowDraw = FALSE;
	static int x, y;  // 위치값
	static int iShape; // 0이면 선그리기, 1이면 점찍기

	switch (iMessage) {

	case WM_CREATE:
		hWndMain = hWnd;
		return 0;

	case WM_COMMAND: //메뉴바 적용하기
		switch (LOWORD(wParam)) {
			case ID_LINE: // 선그리기
				iShape = 0;
				break;
			case ID_DOT: // 점찍기
				iShape = 1;
				break;
		}
		return 0;

	case WM_LBUTTONDOWN: // 선 그리기 시작
		if (iShape == 0) {  // 선그리기 라면

			/* 자유 곡선 그리기
			x = LOWORD(lParam);
			y = HIWORD(lParam);
			lines[iLineCount].p[lines[iLineCount].iPointCount].x = x;  
			lines[iLineCount].p[lines[iLineCount].iPointCount].y = y;
			lines[iLineCount].iPointCount++;
			lines[iLineCount].penColor = CurrentPenColor;
			return 0;
			*/

			/* 직선 그리기 */
			lines[iLineCount].penColor = CurrentPenColor;
			lines[iLineCount].iPenWidth = iCurrentPenWidth;
			newX = LOWORD(lParam);
			newY = HIWORD(lParam);
			oldX = newX;
			oldY = newY;
			lines[iLineCount].p[0].x = newX; //시작점
			lines[iLineCount].p[0].y = newY; //시작점
			bNowDraw = TRUE;

		}
		else if (iShape == 1) {   //점 찍기라면.
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

				/* 자유곡선 그리기 
				hPen = CreatePen(PS_SOLID, iCurrentPenWidth, CurrentPenColor);
				SelectObject(hdc, hPen);
				MoveToEx(hdc, x, y, NULL);
				x = LOWORD(lParam);
				y = HIWORD(lParam);
				LineTo(hdc, x, y);
				ReleaseDC(hWnd, hdc);
				// 선 객체 정보 저장 (움직이는 점)
				lines[iLineCount].p[lines[iLineCount].iPointCount].x = x;
				lines[iLineCount].p[lines[iLineCount].iPointCount].y = y;
				lines[iLineCount].iPointCount++; 
				ReleaseDC(hWnd, hdc);	*/


				/* 직선 그리기*/
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

			/* 자유곡선 그리기 
			iLineCount++; // 선 하나 증가
			*/

			/* 직선 그리기*/
			bNowDraw = FALSE;
			hdc = GetDC(hWnd);
			hPen = CreatePen(PS_SOLID, iCurrentPenWidth, CurrentPenColor);
			hPen = (HPEN)SelectObject(hdc, hPen);
			MoveToEx(hdc, newX, newY, NULL);
			LineTo(hdc, oldX, oldY);
			lines[iLineCount].p[1].x = oldX; // 끝점
			lines[iLineCount].p[1].y = oldY; // 끝점
			iLineCount++;
			ReleaseDC(hWnd, hdc);
		}
		else {

		}
		return 0;
	
	case WM_LBUTTONDBLCLK:
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	
	case WM_PAINT: // 저장된 선의 정보 그리기 실행
		if (iShape == 0) {

			/* 자유 곡선 그리기
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

			/* 직선 그리기 */
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
		else if (iShape == 1) { // 점찍기라면
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
