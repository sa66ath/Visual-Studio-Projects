// WinMain.cpp : Win32 Template

#include <windows.h>

#include "resource.h"

ATOM InitApp(HINSTANCE hInst);
BOOL InitInstance(HINSTANCE hInst, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

TCHAR szClassName[] = TEXT("bitmap02");	    // Window Class

HINSTANCE	hInst;
HWND hParent;						// 親ウィンドウのハンドル

HDC	hdc_mem1, hdc_mem2;
int show_no = 1;

int WINAPI WinMain(HINSTANCE hCurInst, HINSTANCE hPrevInst, LPSTR lpsCmdLine, int nCmdShow)
{
    MSG msg;
    BOOL bRet;
	
	HACCEL hAccel;

	hInst = hCurInst;

    if(!InitApp(hCurInst))
		return FALSE;

    if(!InitInstance(hCurInst, nCmdShow))
		return FALSE;
	
	hAccel = LoadAccelerators(hCurInst, TEXT("ACCELERATOR_MAIN"));
	
    while((bRet = GetMessage(&msg, NULL, 0, 0)) != 0) {
		if(bRet == -1) {
			MessageBox(NULL, TEXT("GetMessage Error"), TEXT("Error"), MB_OK);
			break;
		} else {
			if(!TranslateAccelerator(hParent, hAccel, &msg)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
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
    wc.lpszMenuName = TEXT("MENU_MAIN");					// Menu 
    wc.lpszClassName = szClassName;
    wc.hIconSm = (HICON)LoadImage(NULL,
	    MAKEINTRESOURCE(IDI_APPLICATION),
	    IMAGE_ICON,
	    0,
	    0,
	    LR_DEFAULTSIZE | LR_SHARED);


 
    return RegisterClassEx(&wc);
}


// ウィンドウの生成
BOOL InitInstance(HINSTANCE hInst, int nCmdShow)
{
    HWND hWnd;

    hWnd = CreateWindow(
			szClassName,			// LPCTSTR lpClassName,  // 登録されているクラス名
			TEXT("猫でもわかるデバイスコンテキスト"),		// LPCTSTR lpWindowName, // ウィンドウ名
			WS_OVERLAPPEDWINDOW,    // DWORD dwStyle,        // ウィンドウスタイル
			CW_USEDEFAULT,			// int x,                // ウィンドウの横方向の位置
			CW_USEDEFAULT,			// int y,                // ウィンドウの縦方向の位置
			256,					// int nWidth,           // ウィンドウの幅
			192,					// int nHeight,          // ウィンドウの高さ
			NULL,					// HWND hWndParent,      // 親ウィンドウまたはオーナーウィンドウのハンドル
			NULL,					// HMENU hMenu,          // メニューハンドルまたは子ウィンドウ ID クラスメニューを使う時は NULL
			hInst,					// HINSTANCE hInstance,  // アプリケーションインスタンスのハンドル
			NULL);					// LPVOID lpParam        // ウィンドウ作成データ

    if(!hWnd)	return FALSE;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
	
	hParent = hWnd;				// グローバル変数にウィンドウハンドルを格納

    return TRUE;
}


// ウィンドウプロシージャ
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HDC	hdc;
	PAINTSTRUCT	ps;
	HBITMAP	hBmp;
	HDC hdc_memx;


    switch(msg) {
	case WM_CREATE:
		hdc = GetDC(hWnd);

		hBmp = LoadBitmap(hInst, TEXT("MYBMP1"));
		hdc_mem1 = CreateCompatibleDC(hdc);
		SelectObject(hdc_mem1, hBmp);
		DeleteObject(hBmp);
		
		hBmp = LoadBitmap(hInst, TEXT("MYBMP2"));
		hdc_mem2 = CreateCompatibleDC(hdc);
		SelectObject(hdc_mem2, hBmp);
		DeleteObject(hBmp);

		ReleaseDC(hWnd, hdc);
		break;
	case WM_PAINT:
		BeginPaint(hWnd, &ps);

		switch(show_no) {
		case 1:
			hdc_memx = hdc_mem1;
			break;
		case 2:
			hdc_memx = hdc_mem2;
			break;
		default:
			break;
		}

		BitBlt(ps.hdc,
				ps.rcPaint.left, ps.rcPaint.top,
				ps.rcPaint.right - ps.rcPaint.left, 
				ps.rcPaint.bottom - ps.rcPaint.top,
				hdc_memx, 
				ps.rcPaint.left, ps.rcPaint.top,
				SRCCOPY);

		EndPaint(hWnd, &ps);
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam)) {
		case IDM_CAT1:
			show_no = 1;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case IDM_CAT2:
			show_no = 2;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case IDM__QUIT:
			SendMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		case IDM__ABOUT:
			MessageBox(NULL, TEXT("Copyright(c) 2009  Shoichi Ikeda"), TEXT("OK ?"), MB_OK);
			break;
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		DeleteDC(hdc_mem1);
		DeleteDC(hdc_mem2);
	    PostQuitMessage(0);
	    break;
	case WM_TIMER:
	case WM_LBUTTONDBLCLK:
	default:
	    return DefWindowProc(hWnd, msg, wParam, lParam);
	    break;
    }

	return 0;
}
