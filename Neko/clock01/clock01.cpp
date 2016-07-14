// WinMain.cpp : Win32 Template

#include <windows.h>

#define	ID_MYTIMER	100

ATOM InitApp(HINSTANCE hInst);
BOOL InitInstance(HINSTANCE hInst, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

HFONT MyCreateFont(int nHeight, DWORD dwCharSet, LPCTSTR lpName);

TCHAR szClassName[] = TEXT("clock01");	    // Window Class

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
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;		// UINT style;
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
			TEXT(""),				// LPCTSTR lpWindowName, // �E�B���h�E��
			WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~ WS_MAXIMIZEBOX,    // DWORD dwStyle,        // �E�B���h�E�X�^�C��
			CW_USEDEFAULT,			// int x,                // �E�B���h�E�̉������̈ʒu
			CW_USEDEFAULT,			// int y,                // �E�B���h�E�̏c�����̈ʒu
			200,					// int nWidth,           // �E�B���h�E�̕�
			100,					// int nHeight,          // �E�B���h�E�̍���
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
	int id;
	HDC hdc;
	PAINTSTRUCT ps;
	static TCHAR szTime[64], szDate[64];
	SYSTEMTIME st;
	HFONT hFont1, hFont2;

    switch(msg) {
	case WM_CREATE:
		SetTimer(hWnd, ID_MYTIMER, 500, NULL);
		break;
	case WM_TIMER:
		if(wp != ID_MYTIMER)
			return DefWindowProc(hWnd, msg, wp, lp);
		GetLocalTime(&st);
		wsprintf(szTime, TEXT("%02d:%02d:%02d"), st.wHour, st.wMinute, st.wSecond);
		wsprintf(szDate, TEXT("%d�N:%02d��:%02d��"), st.wYear, st.wMonth, st.wDay);

		// �N���C�A���g�̈�S�̂��X�V
		InvalidateRect(hWnd, NULL, TRUE);
		SetWindowText(hWnd, szTime);
		break;
	case WM_LBUTTONDBLCLK:
		SendMessage(hWnd, WM_CLOSE, 0, 0);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		hFont1 = MyCreateFont(12, SHIFTJIS_CHARSET, TEXT("�l�r�@�S�V�b�N"));
		SelectObject(hdc, hFont1);
		TextOut(hdc, 5, 5, szDate, lstrlen(szDate));

		hFont2 = MyCreateFont(30, ANSI_CHARSET, TEXT("�l�r�@�S�V�b�N"));
		SelectObject(hdc, hFont2);
		SetTextColor(hdc, RGB(0, 0, 255));
		TextOut(hdc, 10, 20, szTime, lstrlen(szTime));

		DeleteObject(hFont1);
		DeleteObject(hFont2);

		EndPaint(hWnd, &ps);
		break;
	case WM_CLOSE:
		id = MessageBox(hWnd, TEXT("�I�����Ă���낵���ł���"), TEXT("�m�F"), MB_YESNO | MB_ICONQUESTION);
		if(IDYES == id) {
			if(!KillTimer(hWnd, ID_MYTIMER)) {
				id = MessageBox(hWnd, TEXT("KillTimer Error!"), TEXT("Error"), MB_OK | MB_ICONEXCLAMATION);
			}
			DestroyWindow(hWnd);
		}
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


HFONT MyCreateFont(int nHeight, DWORD dwCharSet, LPCTSTR lpName)
{
	return CreateFont(nHeight, 0, 0, 0,		// �����A���A�p�x
			FW_DONTCARE,					// ����
			FALSE, FALSE, FALSE,			// �C�^���b�N�A�����A�ŏ���
			dwCharSet,						// �����Z�b�g
			OUT_DEFAULT_PRECIS,				// �o�͐��x
			CLIP_DEFAULT_PRECIS,			// �N���b�s���O���x
			DEFAULT_QUALITY,				// �o�͕i��
			DEFAULT_PITCH | FF_DONTCARE,	// �s�b�`�ƃt�@�~��
			lpName);						// �^�C�v�t�F�C�X:w
}



