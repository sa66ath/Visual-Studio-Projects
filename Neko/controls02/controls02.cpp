// WinMain.cpp : Win32 Template

#include <windows.h>

#include "resource.h"

#define	ID_BUTTON	100
#define	ID_EDIT	101

ATOM InitApp(HINSTANCE hInst);
BOOL InitInstance(HINSTANCE hInst, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

TCHAR szClassName[] = TEXT("controls02");	    // Window Class

HINSTANCE	hInst;
HWND hParent;						// 親ウィンドウのハンドル

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
			TEXT("猫でもわかるコントロール２"),		// LPCTSTR lpWindowName, // ウィンドウ名
			WS_OVERLAPPEDWINDOW,    // DWORD dwStyle,        // ウィンドウスタイル
			CW_USEDEFAULT,			// int x,                // ウィンドウの横方向の位置
			CW_USEDEFAULT,			// int y,                // ウィンドウの縦方向の位置
			230,			// int nWidth,           // ウィンドウの幅
			150,			// int nHeight,          // ウィンドウの高さ
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
	int	id;
	static HWND hBtn, hEdit;
	TCHAR	szBuf[64];

    switch(msg) {
	case WM_CREATE:
		hEdit = CreateWindow(TEXT("EDIT"), TEXT(""),
				WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
				10, 10, 200, 25,
				hWnd, (HMENU)ID_EDIT, hInst, NULL);
		SendMessage(hEdit, EM_SETLIMITTEXT, (WPARAM)63, 0);

		hBtn = CreateWindow(TEXT("BUTTON"), TEXT("押す"),
				WS_CHILD | WS_VISIBLE,
				85, 45, 50, 30,
				hWnd, (HMENU)ID_BUTTON, hInst, NULL);
		break;
	case WM_SETFOCUS:
		SetFocus(hEdit);
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam)) {
		case ID_BUTTON:
			GetWindowText(hEdit, szBuf, sizeof(szBuf));
			MessageBox(hWnd, szBuf, TEXT("猫でもわかるコントロール2"), MB_OK);
			SetWindowText(hEdit, TEXT(""));
			break;
		case IDM__QUIT:
			SendMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		case IDM__ABOUT:
			MessageBox(NULL, TEXT("MENU Help->About"), TEXT("OK ?"), MB_OK);
			break;
		default:
			break;
		}
		break;
	case WM_CLOSE:
		id = MessageBox(hWnd,
				TEXT("終了してもよろしいですか？"),
				TEXT("確認"),
				MB_YESNO | MB_ICONQUESTION);

		switch(id) {
		case IDYES:
			DestroyWindow(hBtn);
			DestroyWindow(hEdit);
			DestroyWindow(hWnd);
			break;
		default:
			break;
		}

		break;
	case WM_DESTROY:
	    PostQuitMessage(0);
	    break;
	default:
	    return DefWindowProc(hWnd, msg, wParam, lParam);
	    break;
    }

	return 0;
}
