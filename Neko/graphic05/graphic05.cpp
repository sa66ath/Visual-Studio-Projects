// WinMain.cpp : Win32 Template

#include <windows.h>


ATOM InitApp(HINSTANCE hInst);
BOOL InitInstance(HINSTANCE hInst, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

TCHAR szClassName[] = TEXT("graphic05");	    // Window Class

int WINAPI WinMain(HINSTANCE hCurInst, HINSTANCE hPrevInst, LPSTR lpsCmdLine, int nCmdShow)
{
    MSG msg;
    BOOL bRet;

    if(!InitApp(hCurInst))
		return FALSE;

    if(!InitInstance(hCurInst, nCmdShow))
		return FALSE;
	
    while((bRet = GetMessage(&msg, NULL, 0, 0)) != 0) {
		if(bRet == -1) {
			MessageBox(NULL, TEXT("GetMessage Error"), TEXT("Error"), MB_OK);
			break;
		} else {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
    }

    return (int)msg.wParam;
}


// Registering Window Class
ATOM InitApp(HINSTANCE hInst)
{
    WNDCLASSEX wc;

    wc.cbSize = sizeof(WNDCLASSEX);			// UINT cbSize;
    wc.style = CS_HREDRAW | CS_VREDRAW;		// UINT style;
    wc.lpfnWndProc = WndProc;				// WNDPROC lpfnWndProc;
    wc.cbClsExtra = 0;						// int cbClsExtra;
    wc.cbWndExtra = 0;						// int cbWndExtra;
    wc.hInstance = hInst;					// HINSTANCE hInstance;
    wc.hIcon = (HICON)LoadImage(NULL,
	    MAKEINTRESOURCE(IDI_APPLICATION),
	    IMAGE_ICON,
	    0,
	    0,
	    LR_DEFAULTSIZE | LR_SHARED);		// HICON hIcon;
    wc.hCursor = (HCURSOR)LoadImage(NULL,
	    MAKEINTRESOURCE(IDC_ARROW),
	    IMAGE_CURSOR,
	    0,
	    0,
	    LR_DEFAULTSIZE | LR_SHARED);		// HCURSOR hCursor;
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = NULL;					// Menu 
    wc.lpszClassName = szClassName;
    wc.hIconSm = (HICON)LoadImage(NULL,
	    MAKEINTRESOURCE(IDI_APPLICATION),
	    IMAGE_ICON,
	    0,
	    0,
	    LR_DEFAULTSIZE | LR_SHARED);


 
    return RegisterClassEx(&wc);
}


// �E�B���h�E�̐���
BOOL InitInstance(HINSTANCE hInst, int nCmdShow)
{
    HWND hWnd;

    hWnd = CreateWindow(
			szClassName,			// LPCTSTR lpClassName,  // �o�^����Ă���N���X��
			TEXT("My Window"),		// LPCTSTR lpWindowName, // �E�B���h�E��
			WS_OVERLAPPEDWINDOW,    // DWORD dwStyle,        // �E�B���h�E�X�^�C��
			CW_USEDEFAULT,			// int x,                // �E�B���h�E�̉������̈ʒu
			CW_USEDEFAULT,			// int y,                // �E�B���h�E�̏c�����̈ʒu
			256,					// int nWidth,           // �E�B���h�E�̕�
			192,					// int nHeight,          // �E�B���h�E�̍���
			NULL,					// HWND hWndParent,      // �e�E�B���h�E�܂��̓I�[�i�[�E�B���h�E�̃n���h��
			NULL,					// HMENU hMenu,          // ���j���[�n���h���܂��͎q�E�B���h�E ID �N���X���j���[���g������ NULL
			hInst,					// HINSTANCE hInstance,  // �A�v���P�[�V�����C���X�^���X�̃n���h��
			NULL);					// LPVOID lpParam        // �E�B���h�E�쐬�f�[�^

    if(!hWnd)	return FALSE;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}


// �E�B���h�E�v���V�[�W��
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	HDC hdc;
	PAINTSTRUCT ps;
	
	HBRUSH hBrush, hOldBrush;

	// Polygon �֐��̂��߂̔z��
	POINT pt[] = {{ 20, 20 }, { 20, 80 }, { 120, 50 }, { 100, 100 }};


	// PolyPolygon �֐��̂��߂̔z��
	POINT poly[] = {{ 150, 20 }, { 200, 25 }, { 210, 70 }, { 145, 65 },
					{ 180, 10 }, { 160, 90 }, { 205, 100 }};
	// �����̑��p�`�̒��_�̐��̔z��
	INT nC[2] = { 4, 3 };


    switch(msg) {
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		hBrush = CreateSolidBrush(RGB(255, 0, 0));
		hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

		// Polygon(hdc, pt, 4);
		Polygon(hdc, pt, sizeof(pt)/sizeof(POINT));
		PolyPolygon(hdc, poly, nC, 2);

		// �������ɖ߂��Ă���
		DeleteObject(hBrush);
		SelectObject(hdc, hOldBrush);

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
	    PostQuitMessage(0);
	    break;
	default:
	    return DefWindowProc(hWnd, msg, wp, lp);
	    break;
    }

	return 0;
}
