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
} line;  //�� ��ü �� ���� ǥ��

line lines[500];
int iLineCount; // �� ��ü�� �� ��
int iTempPointCount; // �� ���� �� ��ü ���� ������ ī��Ʈ

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps; //�׷��� ȭ��
	static int x;
	static int y;
	//static BOOL bNowDraw=FALSE;

	switch (iMessage) {
	case WM_KEYDOWN:  // �� ��ü �����
		switch(wParam) {
		case VK_BACK:
			if (iLineCount > 0) {
				iLineCount--;
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
		}
		return 0;
	case WM_LBUTTONDOWN: // �� �׸��� ����
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		//	bNowDraw=TRUE;

		/* ����ü�� �� ���� (������) �����ϱ� */
		lines[iLineCount].p[iTempPointCount].x = x;
		lines[iLineCount].p[iTempPointCount].y = y;
		iTempPointCount++;
		return 0;
	case WM_MOUSEMOVE:
		if (wParam & MK_LBUTTON) {  // & = ��Ʈ������  // C���� 0 = False, 0�̿��� �� = True
			// MK_LBUTTON   0x0001 ->00000....0001
			// MK_RBUTTON   0x0002 ->00000....0010 

			hdc = GetDC(hWnd); 
			MoveToEx(hdc, x, y, NULL);
			x = LOWORD(lParam);
			y = HIWORD(lParam);
			LineTo(hdc, x, y);
			ReleaseDC(hWnd, hdc);

			/* �� ��ü ���� ���� (�����̴� ��) */
			ReleaseDC(hWnd, hdc);
			lines[iLineCount].p[iTempPointCount].x = x;
			lines[iLineCount].p[iTempPointCount].y = y;
			iTempPointCount++;
		}
		return 0;
	case WM_LBUTTONUP:
		//	bNowDraw=FALSE;
		/* �� ��ü�� ���� ���� ( �� ��) */
		lines[iLineCount].iPointCount = iTempPointCount;
		iLineCount++; // �� �ϳ� ����
		iTempPointCount = 0;  // �ʱ�ȭ
		return 0;
	case WM_LBUTTONDBLCLK:
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	case WM_PAINT: // ����� ���� ���� �׸��� ����
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
