// WinMain.cpp : Win32 Template

#include <windows.h>

ATOM InitApp(HINSTANCE hInst);
BOOL InitInstance(HINSTANCE hInst, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void DrawRect(HWND hWnd, POINTS beg, POINTS end);

TCHAR szClassName[] = TEXT("mouse01");	    // Window Class

POINTS start, end, old_end;					// 矩形の始点・終点

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
    wc.hIcon = NULL;						// HICON hIcon;
    wc.hCursor = (HCURSOR)LoadImage(NULL,
	    MAKEINTRESOURCE(IDC_ARROW),
	    IMAGE_CURSOR,
	    0,
	    0,
	    LR_DEFAULTSIZE | LR_SHARED);		// HCURSOR hCursor;
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = NULL;					// Menu 
    wc.lpszClassName = szClassName;
    wc.hIconSm = NULL;
 
    return RegisterClassEx(&wc);
}


// ウィンドウの生成
BOOL InitInstance(HINSTANCE hInst, int nCmdShow)
{
    HWND hWnd;

    hWnd = CreateWindow(
			szClassName,			// LPCTSTR lpClassName,  // 登録されているクラス名
			TEXT("猫でもわかるマウス入力"),		// LPCTSTR lpWindowName, // ウィンドウ名
			WS_OVERLAPPEDWINDOW,    // DWORD dwStyle,        // ウィンドウスタイル
			CW_USEDEFAULT,			// int x,                // ウィンドウの横方向の位置
			CW_USEDEFAULT,			// int y,                // ウィンドウの縦方向の位置
			384,					// int nWidth,           // ウィンドウの幅
			384,					// int nHeight,          // ウィンドウの高さ
			NULL,					// HWND hWndParent,      // 親ウィンドウまたはオーナーウィンドウのハンドル
			NULL,					// HMENU hMenu,          // メニューハンドルまたは子ウィンドウ ID クラスメニューを使う時は NULL
			hInst,					// HINSTANCE hInstance,  // アプリケーションインスタンスのハンドル
			NULL);					// LPVOID lpParam        // ウィンドウ作成データ

    if(!hWnd)	return FALSE;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}


// ウィンドウプロシージャ
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	TCHAR szBuf[32];
	HDC hdc;
	HBRUSH hBrush;
	static BOOL bDraw;

    switch(msg) {
	case WM_LBUTTONDOWN:
		bDraw = TRUE;
		old_end = start = MAKEPOINTS(lParam);
		DrawRect(hWnd, start, old_end);
		break;
	case WM_MOUSEMOVE:
		if(bDraw) {
			end = MAKEPOINTS(lParam);
			DrawRect(hWnd, start, old_end);
			DrawRect(hWnd, start, end);
			old_end = end;
		} else {
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
		break;
	case WM_LBUTTONUP:
		if(bDraw) {
			SetCursor(LoadCursor(NULL, IDC_ARROW));
			DrawRect(hWnd, start, end);
			bDraw = FALSE;

			wsprintf(szBuf, TEXT("(%d, %d) - (%d, %d)"), start.x, start.y, end.x, end.y);
			SetWindowText(hWnd, szBuf);
			hdc = GetDC(hWnd);
			hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
			SelectObject(hdc, hBrush);
			Ellipse(hdc, start.x, start.y, end.x, end.y);
			ReleaseDC(hWnd, hdc);
		} else {
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
		break;
	case WM_DESTROY:
	    PostQuitMessage(0);
	    break;
	case WM_CREATE:
	case WM_TIMER:
	case WM_LBUTTONDBLCLK:
	case WM_PAINT:
	case WM_CLOSE:
	default:
	    return DefWindowProc(hWnd, msg, wParam, lParam);
	    break;
    }

	return 0;
}


void DrawRect(HWND hWnd, POINTS beg, POINTS end)
{
	HDC hdc;

	hdc = GetDC(hWnd);
	SetROP2(hdc, R2_NOT);

	MoveToEx(hdc, beg.x, beg.y, NULL);
	LineTo(hdc, end.x, beg.y);
	LineTo(hdc, end.x, end.y);
	LineTo(hdc, beg.x, end.y);
	LineTo(hdc, beg.x, beg.y);
	LineTo(hdc, end.x, end.y);
	
	ReleaseDC(hWnd, hdc);

	return;
}
