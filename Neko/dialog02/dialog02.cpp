// WinMain.cpp : Win32 Template

#include <windows.h>

#include "resource.h"

ATOM InitApp(HINSTANCE hInst);
BOOL InitInstance(HINSTANCE hInst, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK MyDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);

TCHAR szClassName[] = TEXT("dialog02");	    // Window Class

HINSTANCE hInst;				// インスタンスハンドル
HWND hDlg;						// モードレスダイアログボックスのハンドル
TCHAR szName[32];				// ダイアログボックスから入力された文字列
HWND hMain;

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

	hAccel = LoadAccelerators(hCurInst, TEXT("MYACCEL"));
	
    while((bRet = GetMessage(&msg, NULL, 0, 0)) != 0) {
		if(bRet == -1) {
			MessageBox(NULL, TEXT("GetMessage Error"), TEXT("Error"), MB_OK);
			break;
		} else {
			if(!hDlg || !IsDialogMessage(hDlg, &msg)) {	
				if(!TranslateAccelerator(hMain, hAccel, &msg)) {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
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
    wc.lpszMenuName = TEXT("MYMENU");					// Menu 
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
			TEXT("猫でもわかるモードレスダイアログボックス"),		// LPCTSTR lpWindowName, // ウィンドウ名
			WS_OVERLAPPEDWINDOW,    // DWORD dwStyle,        // ウィンドウスタイル
			CW_USEDEFAULT,			// int x,                // ウィンドウの横方向の位置
			CW_USEDEFAULT,			// int y,                // ウィンドウの縦方向の位置
			256,			// int nWidth,           // ウィンドウの幅
			128,			// int nHeight,          // ウィンドウの高さ
			NULL,					// HWND hWndParent,      // 親ウィンドウまたはオーナーウィンドウのハンドル
			NULL,					// HMENU hMenu,          // メニューハンドルまたは子ウィンドウ ID クラスメニューを使う時は NULL
			hInst,					// HINSTANCE hInstance,  // アプリケーションインスタンスのハンドル
			NULL);					// LPVOID lpParam        // ウィンドウ作成データ

    if(!hWnd)	return FALSE;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

	hMain = hWnd;

    return TRUE;
}


// ウィンドウプロシージャ
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HMENU hMenu;
	TCHAR szBuf[64];
	HDC hdc;
	PAINTSTRUCT ps;

    switch(msg) {
	case WM_CREATE:
		hMenu = GetMenu(hWnd);
		break;
	case WM_INITMENU:
		if(IsWindow(hDlg)) {
			EnableMenuItem(hMenu, IDM_DLG, MF_BYCOMMAND | MF_GRAYED);
			EnableMenuItem(hMenu, IDM_CLOSEDLG, MF_BYCOMMAND | MF_ENABLED);
		} else {
			EnableMenuItem(hMenu, IDM_DLG, MF_BYCOMMAND | MF_ENABLED);
			EnableMenuItem(hMenu, IDM_CLOSEDLG, MF_BYCOMMAND | MF_GRAYED);
		}
		DrawMenuBar(hWnd);
		break;
	case WM_PAINT:
		if(0 == lstrcmp(szName, TEXT(""))) {
			lstrcpy(szBuf, TEXT("まだ名前の入力はありません"));
		} else {
			wsprintf(szBuf, TEXT("入力された氏名は %s さんです"), szName);
		}

		hdc = BeginPaint(hWnd, &ps);

		TextOut(hdc, 10, 10, szBuf, lstrlen(szBuf));

		EndPaint(hWnd, &ps);
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam)) {
		case IDM_END:
			SendMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		case IDM_DLG:
			hDlg = CreateDialog(hInst, TEXT("MYDLG"), hWnd, (DLGPROC)MyDlgProc);
			ShowWindow(hDlg, SW_NORMAL);
			break;
		case IDM_CLOSEDLG:
			DestroyWindow(hDlg);
			break;
		case IDM_ABOUT:
			MessageBox(hWnd, TEXT("Copyright(C)2004 y.Kumei"),
							TEXT("about"), MB_OK);
			break;
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
		break;
	case WM_CLOSE:
		if(IsWindow(hDlg)) {
			MessageBox(hWnd, TEXT("ダイアログを破棄します"),
							TEXT("破棄"),
							MB_OK);
			DestroyWindow(hDlg);
		}
		DestroyWindow(hWnd);
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


BOOL CALLBACK MyDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hParent;
	
	switch(msg) {
	case WM_INITDIALOG:
		hParent = GetParent(hDlg);
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam)) {
		case IDOK:
			GetDlgItemText(hDlg, IDC_EDIT1, szName, sizeof(szName) - 1);
			InvalidateRect(hParent, NULL, TRUE);
			return TRUE;
			break;
		case IDCANCEL:
			SetDlgItemText(hDlg, IDC_EDIT1, TEXT(""));
			return TRUE;
		case IDC_CLOSE:
			DestroyWindow(hDlg);
			return TRUE;
		}
		return FALSE;
	}
	return FALSE;
}
