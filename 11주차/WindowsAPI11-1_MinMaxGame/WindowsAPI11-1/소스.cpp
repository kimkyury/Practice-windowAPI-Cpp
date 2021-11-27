#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("MinMax");

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

	hWndMain = hWnd; //�߰�����.
	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}


int pan[3][3]; // 3x3ĭ
int turn = 1; // ������ ����  1�� ����
int iCount; //�� ���ӿ��� �ٵ��� �� Ƚ�� ����

// POINT moves[9] = { 1, 1, 0, 0, 2, 0, 0, 2, 2, 2, 1, 0, 0, 1, 2, 1, 1, 2 };
// (1,1), (0,0), (2,0), (0,2), (2,2), (1,0), (0,1), (2,1), (1,2)


/* ���� �Ǵ� �Լ�  */
int winpoint(int a) {  // a�� turn
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

/* ���� ���� ����� �ʱ�ȭ �Լ�*/
void init(HWND hWnd) {  //�Ķ���͸� �������� ����
	int i, j;
	turn = 1; //������ ���� �ɶ��� ����ؼ�
	iCount = 0;
	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++) {
			pan[i][j] = 0;
		}
	InvalidateRect(hWndMain, NULL, TRUE);
}

int evaluate(int depth = 0) {
	if (winpoint(2)) // ��ǻ�Ͱ� �̱�� ����� ���� ����
		return 10 + depth;
	else if (winpoint(1)) // ��ǻ�Ͱ� ���� ����� ���� ����
		return -10 - depth;
	else
		return 0;
}

int minmax(int* pos, int depth, int turn) {
	int score;
	int best;
	int position;

	if (turn == 1)
		best = 99;
	else
		best = -99;

	if (depth == 0 || winpoint(1) || winpoint(2))
		return evaluate(depth);

	for (int i = 0; i < 9; i++) {
		if (pan[i % 3][i / 3] == 0) {// ������ �Ŀ���,  �� ĭ�̶��
			pan[i % 3][i / 3] = turn; // Ž���� ���� ���� ���ʿ� ��ǻ���� ���� ���� (����)
			if (turn == 1)
				/*����Լ�*/
				score = minmax(pos, depth - 1, 2); // ��ǻ�� ���ʷ� �Ѱܼ� ���� ĭ�� minmax�� ã��
			else
				score = minmax(pos, depth - 1, 1); // �÷��̾� ���ʷ� �Ѱܼ� ���� ĭ�� minmax�� ã��

			pan[i % 3][i / 3] = 0; // ����� ���� ��, �� �ڸ��� �ʱ�ȭ �Ѵ�
			if (turn == 1) { // �÷��̾� �����, �������� '�ּ�'�� �Ǵ� ���� ã�� ����Ѵ�
				if (score < best) {  
					best = score;
					position = i; // i������ ������(�ּ�) ��ġ ���
				}
			}
			else { // ��ǻ�� �����, �������� '�ִ�'�� �Ǵ� ���� ã�� ����Ѵ�
				if (score > best) { 
					best = score;
					position = i; // i������ ������(�ִ�) ��ġ ���
				}
			}
		}
	}
	*pos = position; // ���� ��ġ�� ����(�ּҰ��̴ϱ� ����st)
	return best; //������ ��ȯ
}

int computer() {
	int depth = 0;
	int pos = 0;
	for (int i = 0; i < 9; i++) 
		if(pan[i%3][i/3] == 0)
			depth++; //depth�� ���� ����ִ� ĭ�� ���� ����
	
	minmax(&pos, depth, 2); // ��ǻ���� ������ ã���� ��
	pan[pos % 3][pos / 3] = 2;
	if (winpoint(2)) // �̰�ٸ� 1�� ����(true)
		return 1;
	else // ���ٸ�
		return 0;
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
	HBRUSH hBrush, oldBrush;
	TCHAR* Mes = TEXT("Ŭ�� �� �ִϸ��̼��� ������");

	int x, y;
	TCHAR buf[128]; // �޽����ڽ� ����

	switch (iMessage) {
	case WM_LBUTTONDOWN:
		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rect);
		SetRect(&rect, rect.left, rect.top, rect.right, rect.bottom);

		/*  3x3 ���� �����ϱ�*/
		x = (LOWORD(lParam)) / (rect.right / 3);
		y = (HIWORD(lParam)) / (rect.bottom / 3);

		/* ����� ���� ���� ���� �ٲٱ�*/
		if (pan[x][y] == 0) {
			pan[x][y] = turn;
			iCount++;
			// turn = (turn == 1 ? 2 : 1);  // ��ǻ�Ϳ� �º��� ���̴� ���� �ʿ����
			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);

			//�޼����ڽ� ���伱�ÿ� ���� ����
			if (winpoint(1)) { // 1�� �̰�ٸ�
				wsprintf(buf, TEXT("����� win. \n ���� �����ϰڽ��ϱ�?"));
				if (MessageBox(hWnd, buf, TEXT("TicTacToe"), MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
					init(hWnd);
				else
					SendMessage(hWnd, WM_CLOSE, 0L, 0L);
				return 0;
			}
			else if (iCount == 9) { // �� �̻� �� ���� ���� ���ų� �÷��̾ ���ٸ�
				if (MessageBox(hWnd, TEXT("Game Over. \n ���� �����ϰڽ��ϱ�?"), TEXT("TicTacToe"),
					MB_YESNO | MB_ICONEXCLAMATION) == IDYES) {
					init(hWnd);
					//turn =1; //�� ��� �� �� ���Ƽ� ������
				}
				else
					SendMessage(hWnd, WM_CLOSE, 0L, 0L);
				return 0;
			}
			else { //������ �������̶��, 
				int ret = computer(); // ��ǻ���� ��(1)��(0)���� �޾ƿ�
				iCount++;
				InvalidateRect(hWnd, NULL, TRUE);
				UpdateWindow(hWnd);
				if (ret) { //��ǻ�Ͱ� �̰�ٸ�
					wsprintf(buf, TEXT(" ��ǻ�� win. \n ���� �����ϰڽ��ϱ�?"));
					if (MessageBox(hWnd, buf, TEXT("MinMax"), MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
						init(hWnd);
					else
						SendMessage(hWnd, WM_CLOSE, 0L, 0L);
					return 0;
				}
				return 0;
			}
		}
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rect);
		SetRect(&rect, rect.left, rect.top, rect.right, rect.bottom);


		/* ���� ������ �׸��� */
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				Rectangle(hdc, i * rect.right / 3, j * rect.bottom / 3, (i + 1) * rect.right / 3, (j + 1) * rect.bottom / 3);
			}
		}

		/* �ٵϾ� �׸��� */
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				/* ���� ũ�⿡ �°� */
				if (pan[i][j] == 1) {  // �÷��̾�1
					hBrush = (HBRUSH)CreateSolidBrush(RGB(255, 0, 0));
					oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
					Ellipse(hdc, i * rect.right / 3, j * rect.bottom / 3, (i + 1) * rect.right / 3, (j + 1) * rect.bottom / 3);
					SelectObject(hdc, oldBrush);
					DeleteObject(hBrush);
				}
				else if (pan[i][j] == 2) { // �÷��̾�2
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
