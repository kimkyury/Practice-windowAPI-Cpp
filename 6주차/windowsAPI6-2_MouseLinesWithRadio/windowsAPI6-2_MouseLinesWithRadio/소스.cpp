#include <windows.h>
#include "resource.h"  // 리소스 적용하기 위한 헤더파일 적용

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
	COLORREF penColor; // 색상 속성
} line;  //선 객체 한 개의 표현

line lines[500];
int iLineCount; // 선 객체의 총 수
COLORREF CurrentPenColor = RGB(255, 0, 0); // 현재 선 객체의 색상


/* radio를 위한 전역변수 */
enum { ID_R1 = 101, ID_R2, ID_R3};
HWND r1, r2, r3;
/* radio를 위한 전역변수 */


LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps; //그려낼 화면
	static int x;
	static int y;
	HPEN hPen; // 색상조절을 위한 펜 객체
	//static BOOL bNowDraw=FALSE;

	switch (iMessage) {
	/* radio를 위한 코드 */
	case WM_CREATE: 
		CreateWindow(TEXT("button"), TEXT("Color"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 5, 5, 120, 110, hWnd, (HMENU)1, g_hInst, NULL); // 그룹박스 생성
		r1 = CreateWindow(TEXT("button"), TEXT("RED"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP, 10, 20, 100, 30, hWnd, (HMENU)ID_R1, g_hInst, NULL);
		r2 = CreateWindow(TEXT("button"), TEXT("GREEN"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP, 10, 50, 100, 30, hWnd, (HMENU)ID_R2, g_hInst, NULL);
		r3 = CreateWindow(TEXT("button"), TEXT("BLUE"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP, 10, 80, 100, 30, hWnd, (HMENU)ID_R3, g_hInst, NULL);
		CheckRadioButton(hWnd, ID_R1, ID_R3, ID_R1);// (hWnd, 그룹내 첫번째 라디오 버튼, 그룹 내 마지막 라디오 버튼, 체크할 라디오 버튼)
		break;
	/* radio를 위한 코드 */

	case WM_KEYDOWN:
		switch (wParam) {
		case VK_BACK: // 선 객체 지우기
			if (iLineCount > 0) {
				iLineCount--;
				lines[iLineCount].iPointCount = 0; // 초기화ver1 : 백스페이스를 누를 때마다 카운트를 초기화시키기
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
		}
		return 0;
	case WM_COMMAND: //메뉴바 적용하기
		switch (LOWORD(wParam)) {
		case IDM_RED:
		case ID_R1:
			CheckRadioButton(hWnd, ID_R1, ID_R3, ID_R1);
			CurrentPenColor = RGB(255, 0, 0);
			SetFocus(hWnd); //강제로 포커스 이동시키기 (라디오버튼에 포커스가 가있으므로)
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
	case WM_LBUTTONDOWN: // 선 그리기 시작
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		//	bNowDraw=TRUE;

		// lines[iLineCount].iPointCount = 0;   // 초기화.ver2 : 왼쪽버튼을 누를 때마다 카운트 초기화하기

		/* 구조체에 선 정보 (시작점) 저장하기 */
		lines[iLineCount].p[lines[iLineCount].iPointCount].x = x;
		lines[iLineCount].p[lines[iLineCount].iPointCount].y = y;
		//lines[iLineCount].iPointCount++;

		lines[iLineCount].penColor = CurrentPenColor;

		return 0;
	case WM_MOUSEMOVE:
		if (wParam & MK_LBUTTON) {  // & = 비트연산자  // C언어에서 0 = False, 0이외의 수 = True
			// MK_LBUTTON   0x0001 ->00000....0001
			// MK_RBUTTON   0x0002 ->00000....0010 

			hdc = GetDC(hWnd);

			/* 펜 설정 */
			hPen = CreatePen(PS_SOLID, 1, lines[iLineCount].penColor);
			SelectObject(hdc, hPen);
			MoveToEx(hdc, x, y, NULL);
			x = LOWORD(lParam);
			y = HIWORD(lParam);
			LineTo(hdc, x, y);
			ReleaseDC(hWnd, hdc);

			/* 선 객체 정보 저장 (움직이는 점) */
			lines[iLineCount].p[lines[iLineCount].iPointCount].x = x;
			lines[iLineCount].p[lines[iLineCount].iPointCount].y = y;
			lines[iLineCount].iPointCount++;

		}
		return 0;
	case WM_LBUTTONUP:
		//	bNowDraw=FALSE;

		/* 선 객체의 정보 저장 ( 끝 점) */
		iLineCount++; // 선 하나 증가

		return 0;
	case WM_LBUTTONDBLCLK:
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	case WM_PAINT: // 저장된 선의 정보 그리기 실행
		hdc = BeginPaint(hWnd, &ps);
		for (int i = 0; i < iLineCount; i++) {

			hPen = CreatePen(PS_SOLID, 1, lines[i].penColor);
			hPen = (HPEN)SelectObject(hdc, hPen); // 펜 객체 선택

			MoveToEx(hdc, lines[i].p[0].x, lines[i].p[0].y, NULL);
			for (int j = 1; j < lines[i].iPointCount; j++) {
				LineTo(hdc, lines[i].p[j].x, lines[i].p[j].y);
			}
			DeleteObject(SelectObject(hdc, hPen)); //생성한 것은 없애라. 삭제했음.
		}
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
