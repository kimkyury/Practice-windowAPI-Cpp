#include <windows.h>

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
	WndClass.lpszMenuName = NULL;
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

typedef struct _line{
	POINT p[1000];
	int iPointCount;
} line;  //선 객체 한 개의 표현

line lines[500];
int iLineCount; // 선 객체의 총 수
int iTempPointCount; // 한 개의 선 객체 내의 점개수 카운트

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps; //그려낼 화면
	static int x;
	static int y;
	//static BOOL bNowDraw=FALSE;

	switch (iMessage) {
	case WM_KEYDOWN:  // 선 객체 지우기
		switch(wParam) {
		case VK_BACK:
			if (iLineCount > 0) {
				iLineCount--;
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
		}
		return 0;
	case WM_LBUTTONDOWN: // 선 그리기 시작
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		//	bNowDraw=TRUE;

		/* 구조체에 선 정보 (시작점) 저장하기 */
		lines[iLineCount].p[iTempPointCount].x = x;
		lines[iLineCount].p[iTempPointCount].y = y;
		iTempPointCount++;
		return 0;
	case WM_MOUSEMOVE:
		if (wParam & MK_LBUTTON) {  // & = 비트연산자  // C언어에서 0 = False, 0이외의 수 = True
			// MK_LBUTTON   0x0001 ->00000....0001
			// MK_RBUTTON   0x0002 ->00000....0010 

			hdc = GetDC(hWnd); 
			MoveToEx(hdc, x, y, NULL);
			x = LOWORD(lParam);
			y = HIWORD(lParam);
			LineTo(hdc, x, y);
			ReleaseDC(hWnd, hdc);

			/* 선 객체 정보 저장 (움직이는 점) */
			ReleaseDC(hWnd, hdc);
			lines[iLineCount].p[iTempPointCount].x = x;
			lines[iLineCount].p[iTempPointCount].y = y;
			iTempPointCount++;
		}
		return 0;
	case WM_LBUTTONUP:
		//	bNowDraw=FALSE;
		/* 선 객체의 정보 저장 ( 끝 점) */
		lines[iLineCount].iPointCount = iTempPointCount;
		iLineCount++; // 선 하나 증가
		iTempPointCount = 0;  // 초기화
		return 0;
	case WM_LBUTTONDBLCLK:
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	case WM_PAINT: // 저장된 선의 정보 그리기 실행
		hdc = BeginPaint(hWnd, &ps);
		for (int i = 0; i < iLineCount; i++){
			for (int j = 0; j < lines[i].iPointCount - 1; j++){
				MoveToEx(hdc, lines[i].p[j].x, lines[i].p[j].y, NULL);
				LineTo(hdc, lines[i].p[j + 1].x, lines[i].p[j + 1].y);
			}
		}
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
