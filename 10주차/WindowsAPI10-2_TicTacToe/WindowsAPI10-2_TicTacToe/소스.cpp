#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("AnimWin");

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
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	hWndMain = hWnd; //추가했음.
	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}


int pan[3][3]; // 3x3칸
int turn = 1; // 최초의 턴을  1로 고정
int iCount; //현 게임에서 바둑을 둔 횟수 세기


/* 승패 판단 함수  */
int winpoint(int a) {  // a는 turn
	if (pan[0][0] == a && pan[1][0] == a && pan[2][0] == a)
		return 1;
	else if (pan[0][1] == a && pan[1][1] == a && pan[2][1] == a)
		return 1;
	else if (pan[0][2] == a && pan[1][2] == a && pan[2][2] == a)
		return 1;
	else if (pan[0][0] == a && pan[0][1] == a && pan[0][2] == a)
		return 1;
	else if (pan[1][0] == a && pan[1][1] == a && pan[1][2] == a)
		return 1;
	else if (pan[2][0] == a && pan[2][1] == a && pan[2][2] == a)
		return 1;
	else if (pan[0][0] == a && pan[1][1] == a && pan[2][2] == a)
		return 1;
	else if (pan[2][0] == a && pan[1][1] == a && pan[0][2] == a)
		return 1;
	else
		return 0;
}

/* 게임 연속 진행시 초기화 함수*/
void init() {
	int i, j;
	turn = 1; //게임이 연속 될때를 대비해서
	iCount = 0;
	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++) {
			pan[i][j] = 0;
		}
	InvalidateRect(hWndMain, NULL, TRUE);
}




LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
	HBRUSH hBrush, oldBrush;
	TCHAR* Mes = TEXT("클릭 시 애니메이션이 동작함");

	int x, y;
	TCHAR buf[128]; // 메시지박스 문구

	switch (iMessage) {
	case WM_LBUTTONDOWN:
		GetClientRect(hWnd, &rect);
		SetRect(&rect, rect.left, rect.top, rect.right, rect.bottom);

		/*  3x3 영역 분할하기*/
		x = (LOWORD(lParam)) / (rect.right / 3);
		y = (HIWORD(lParam)) / (rect.bottom / 3);

		/* 빈곳에 대해 색상 순서 바꾸기*/
		if (pan[x][y] == 0) { 
			pan[x][y] = turn;
			iCount++;
			// turn = (turn == 1 ? 2 : 1);  // 컴퓨터와 승부할 것이니 이제 필요없음
			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);

			//메세지박스 응답선택에 따른 반응
			if (winpoint(1)) { // 컴퓨터vs플레이어 이므로, 플레이어1만 따지자.
				wsprintf(buf, TEXT("사용자 win. \n 새로 시작하겠습니까?"));
				if (MessageBox(hWnd, buf, TEXT("TicTacToe"), MB_YESNO | MB_ICONEXCLAMATION) == IDYES) 
					init();
				else
					SendMessage(hWnd, WM_CLOSE, 0L, 0L);
				return 0;
			}
			else if (iCount == 9) {
				if (MessageBox(hWnd, TEXT("Game Over. \n 새로 시작하겠습니까?"), TEXT("TicTacToe"),
					MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
					init();
				else
					SendMessage(hWnd, WM_CLOSE, 0L, 0L);
				return 0;
			}

			while (1) {
				/* (컴퓨터) 무작위 위치 찾기*/
				int tx = rand() % 3;
				int ty = rand() % 3;

				/* 무작위 위치에서 빈 곳을 찾으면*/
				if (pan[tx][ty] == 0) {
					pan[tx][ty] = 2;
					iCount++;
					InvalidateRect(hWnd, NULL, TRUE);
					UpdateWindow(hWnd);

					//메세지박스 응답선택에 따른 반응
					if (winpoint(2)) { // 컴퓨터vs플레이어 이므로, 2는 컴퓨터 고정
						wsprintf(buf, TEXT("컴퓨터 win. \n 새로 시작하겠습니까?"));
						if (MessageBox(hWnd, buf, TEXT("TicTacToe"), MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
							init();
						else
							SendMessage(hWnd, WM_CLOSE, 0L, 0L);
						return 0;
					}
					else if (iCount == 9) {
						if (MessageBox(hWnd, TEXT("Game Over. \n 새로 시작하겠습니까?"), TEXT("TicTacToe"),
							MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
							init();
						else
							SendMessage(hWnd, WM_CLOSE, 0L, 0L);
						return 0;
					}

					break; //바둑을 두었다면 제어문 탈출
				}
			}
		}
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rect);
		SetRect(&rect, rect.left, rect.top, rect.right, rect.bottom);


		/* 판의 윤곽선 그리기 */
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				Rectangle(hdc, i * rect.right / 3, j * rect.bottom / 3, (i + 1) * rect.right / 3, (j + 1) * rect.bottom / 3);
			}
		}

		/* 바둑알 그리기 */
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				/* 영역 크기에 맞게 */
				if (pan[i][j] ==1) {  // 플레이어1
					hBrush = (HBRUSH)CreateSolidBrush(RGB(255, 0, 0));
					oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
					Ellipse(hdc, i * rect.right / 3, j * rect.bottom / 3, (i + 1) * rect.right / 3, (j + 1) * rect.bottom / 3);
					SelectObject(hdc, oldBrush);
					DeleteObject(hBrush);
				}
				else if (pan[i][j] == 2) { // 플레이어2
					hBrush = (HBRUSH)CreateSolidBrush(RGB(0, 255, 0));
					oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
					Ellipse(hdc, i * rect.right / 3, j * rect.bottom / 3, (i + 1) * rect.right / 3, (j + 1) * rect.bottom / 3);
					SelectObject(hdc, oldBrush);
					DeleteObject(hBrush);
				}
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
