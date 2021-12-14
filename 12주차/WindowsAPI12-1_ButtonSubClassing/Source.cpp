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
// WM_SORI = 0x401값을 가지는 메시지임을 정의. (WM_USER가 0x400값임) 
#define WM_SORI WM_USER +1

HWND hButton, hWndSori; //버튼 핸들
WNDPROC OldButtonProc;
// char buffer[20]; // 출력할 문구 공간

//UserMes에서 리턴값 가져오기 위한 추가 변수
int ret;
HWND hStatic;//출력할 레이블 핸들

//SubProc작성
LRESULT CALLBACK ButtonSubProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam){
	TCHAR buffer[20]; // 출력 문구 공간을 이번에는 내부에다 선언
	
	switch (iMessage){
	case WM_LBUTTONDOWN:
		CallWindowProc(OldButtonProc, hWnd, iMessage, wParam, lParam);
		hWndSori = FindWindow(NULL, TEXT("UserMes")); //캡션이 UserMes인 Window를 찾는다
		if (hWndSori == NULL){
			MessageBox(hWnd, TEXT(" 상대프로그램이 존재하지 않습니다."),
				TEXT("이런!"), MB_OK);
		}
		else{
			/* ret = SendMessage(hWndSori, WM_SORI, 0, 0); //찾으면 Message를 전달했었다
			sprintf(buffer, "%d", ret);
			SetFocus(NULL); // Focus를 없앤다, 없애지 않으면 모든 메시지는 hButton으로 전달된다
			hWndSori 윈도우의 윈도우 프록시져를 호출한다 */

			ret = PostMessage(hWndSori, WM_SORI, 0, 0);
			wsprintf(buffer, TEXT("리턴값: %d"), ret);
			SetWindowText(hStatic, buffer);
		}
		return 0; // 즉, break
	}
	return CallWindowProc(OldButtonProc, hWnd, iMessage, wParam, lParam);
}

//WndProc작성
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam){
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR *Mes = TEXT("UserMes 프로그램에게 메시지를 보냅니다.");

	switch (iMessage){
	case WM_CREATE:
		hButton = CreateWindow(TEXT("button"), TEXT("Click me"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 10, 200, 25, hWnd, (HMENU)101, g_hInst, NULL);
		OldButtonProc = (WNDPROC)SetWindowLongPtr(hButton, GWLP_WNDPROC, (LONG_PTR)ButtonSubProc);
		
		//레이블 컨트롤 작성
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