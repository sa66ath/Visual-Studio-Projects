// WinMain.cpp : Win32 Template

#include <windows.h>
#include <commctrl.h>

#include "resource.h"


#define	ID_TOOLBAR	100

ATOM InitApp(HINSTANCE hInst);
BOOL InitInstance(HINSTANCE hInst, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
HWND MyCreateToolbar(HWND hWnd);

#define	APP_NAME	TEXT("toolbar02")
#define	APP_VERSION	TEXT("0.00")

TCHAR szClassName[] = APP_NAME;					// Window Class
TCHAR szVersionStr[] = APP_NAME TEXT("\tVersion ") APP_VERSION;

int nType = 0;			// 0:旧タイプ	1:フラットタイプ	2:透明タイプ

// このように初期化するときは
// バージョンによる TBBBUTTON 構造体の大きさに注意
TBBUTTON tbb[] = {
	{ 0, IDM_MARU, TBSTATE_ENABLED, BTNS_BUTTON },
	{ 1, IDM_SANKAKU, TBSTATE_ENABLED, BTNS_BUTTON },
	{ 2, IDM_SHIKAKU, TBSTATE_ENABLED, BTNS_BUTTON },
	{ STD_FILENEW, IDM_NEW, TBSTATE_ENABLED, BTNS_BUTTON },
	{ STD_FILEOPEN, IDM_OPEN, TBSTATE_ENABLED, BTNS_BUTTON },
	{ STD_DELETE, IDM_DELETE, TBSTATE_ENABLED, BTNS_BUTTON },
	{ VIEW_PARENTFOLDER, IDM_PARENT, TBSTATE_ENABLED, BTNS_BUTTON },
};

TBBUTTON tb_sep = { 0, 0, TBSTATE_ENABLED, BTNS_SEP };

HINSTANCE	hAppInst;
HWND hMainWindow;						// 親ウィンドウのハンドル

int WINAPI WinMain(HINSTANCE hCurInst, HINSTANCE hPrevInst, LPSTR lpsCmdLine, int nCmdShow)
{
    MSG msg;
    BOOL bRet;
	
	HACCEL hAccel;

	hAppInst = hCurInst;

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
			if(!TranslateAccelerator(hMainWindow, hAccel, &msg)) {
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
			TEXT("猫でもわかるツールバー"),		// LPCTSTR lpWindowName, // ウィンドウ名
			WS_OVERLAPPEDWINDOW,    // DWORD dwStyle,        // ウィンドウスタイル
			CW_USEDEFAULT,			// int x,                // ウィンドウの横方向の位置
			CW_USEDEFAULT,			// int y,                // ウィンドウの縦方向の位置
			CW_USEDEFAULT,			// int nWidth,           // ウィンドウの幅
			CW_USEDEFAULT,			// int nHeight,          // ウィンドウの高さ
			NULL,					// HWND hWndParent,      // 親ウィンドウまたはオーナーウィンドウのハンドル
			NULL,					// HMENU hMenu,          // メニューハンドルまたは子ウィンドウ ID クラスメニューを使う時は NULL
			hInst,					// HINSTANCE hInstance,  // アプリケーションインスタンスのハンドル
			NULL);					// LPVOID lpParam        // ウィンドウ作成データ

    if(!hWnd)	return FALSE;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
	
	hMainWindow = hWnd;				// グローバル変数にウィンドウハンドルを格納

    return TRUE;
}


// ウィンドウプロシージャ
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HMENU	hMenu;
	INITCOMMONCONTROLSEX	cc;
	static HWND hTool;
	LONG	lStyle;
	MENUITEMINFO	mi;

    switch(msg) {
	case WM_CREATE:
		hMenu = GetMenu(hWnd);
		cc.dwSize = sizeof(INITCOMMONCONTROLSEX);
		cc.dwICC = ICC_BAR_CLASSES;
		InitCommonControlsEx(&cc);
		hTool = MyCreateToolbar(hWnd);
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam)) {
		case IDM_EXIT:
			SendMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		case IDM_NEW:
			MessageBox(hWnd, TEXT("New が選択されました"), TEXT("Option"), MB_OK);
			break;
		case IDM_OPEN:
			MessageBox(hWnd, TEXT("Open が選択されました"), TEXT("Option"), MB_OK);
			break;
		case IDM_DELETE:
			MessageBox(hWnd, TEXT("Delete が選択されました"), TEXT("Option"), MB_OK);
			break;
		case IDM_PARENT:
			MessageBox(hWnd, TEXT("View Parent Folder が選択されました"), TEXT("Option"), MB_OK);
			break;
		case IDM_MARU:
			MessageBox(hWnd, TEXT("丸が選択されました"), TEXT("Option"), MB_OK);
			break;
		case IDM_SANKAKU:
			MessageBox(hWnd, TEXT("三角が選択されました"), TEXT("Option"), MB_OK);
			break;
		case IDM_SHIKAKU:
			MessageBox(hWnd, TEXT("四角が選択されました"), TEXT("Option"), MB_OK);
			break;
		case IDM_OLD:
			lStyle = GetWindowLongPtr(hTool, GWL_STYLE);
			lStyle = lStyle & ~TBSTYLE_FLAT & ~TBSTYLE_TRANSPARENT;
			SetWindowLongPtr(hTool, GWL_STYLE, lStyle);
			InvalidateRect(hTool, NULL, TRUE);
			nType = 0;
			break;
		case IDM_FLAT:
			lStyle = GetWindowLongPtr(hTool, GWL_STYLE);
			lStyle = (lStyle | TBSTYLE_FLAT) & ~TBSTYLE_TRANSPARENT;
			SetWindowLongPtr(hTool, GWL_STYLE, lStyle);
			InvalidateRect(hTool, NULL, TRUE);
			nType = 1;
			break;
		case IDM_TRANSPARENT:
			lStyle = GetWindowLongPtr(hTool, GWL_STYLE);
			lStyle = lStyle | TBSTYLE_FLAT | TBSTYLE_TRANSPARENT;
			SetWindowLongPtr(hTool, GWL_STYLE, lStyle);
			InvalidateRect(hTool, NULL, TRUE);
			nType = 2;
			break;
		case IDM_ABOUT:
			ShellAbout(hWnd, szClassName,
							szVersionStr, NULL);
			break;
		}
		break;
	case WM_SIZE:
		// SendMessage(hTool, msg, wParam, lParam);		// を送っても良い
		SendMessage(hTool, TB_AUTOSIZE, 0, 0);
		break;
	case WM_INITMENU:
		mi.cbSize = sizeof(MENUITEMINFO);
		mi.fMask = MIIM_STATE;

		switch(nType) {
		case 0:
			mi.fState = MFS_CHECKED;
			SetMenuItemInfo(hMenu, IDM_OLD, FALSE, &mi);
			mi.fState = MFS_UNCHECKED;
			SetMenuItemInfo(hMenu, IDM_FLAT, FALSE, &mi);
			SetMenuItemInfo(hMenu, IDM_TRANSPARENT, FALSE, &mi);
			break;
		case 1:
			mi.fState = MFS_CHECKED;
			SetMenuItemInfo(hMenu, IDM_FLAT, FALSE, &mi);
			mi.fState = MFS_UNCHECKED;
			SetMenuItemInfo(hMenu, IDM_OLD, FALSE, &mi);
			SetMenuItemInfo(hMenu, IDM_TRANSPARENT, FALSE, &mi);
			break;
		case 2:
			mi.fState = MFS_CHECKED;
			SetMenuItemInfo(hMenu, IDM_TRANSPARENT, FALSE, &mi);
			mi.fState = MFS_UNCHECKED;
			SetMenuItemInfo(hMenu, IDM_OLD, FALSE, &mi);
			SetMenuItemInfo(hMenu, IDM_FLAT, FALSE, &mi);
			break;
		}
		DrawMenuBar(hWnd);
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



HWND MyCreateToolbar(HWND hWnd)
{
	HINSTANCE	hInst;
	HWND	hTool;
	TBADDBITMAP	tb;
	int stdid;

	hInst = (HINSTANCE)GetWindowLongPtr(hWnd, GWL_HINSTANCE);

	hTool = CreateToolbarEx(
				hWnd,					// ツールバーの親ウィンドウ
				WS_CHILD | WS_VISIBLE,	// ツールバーのウィンドウスタイル
				ID_TOOLBAR,				// ツールバーのコントロール識別子			
				3,						// ボタンイメージの数			
				hInst,					// モジュールのインスタンス			
				IDR_TOOLBAR1,			// ビットマップリソースの識別子	
				tbb,					// ボタン情報			
				3,						// ボタン数			
				0,						// ボタンの幅			
				0,						// ボタンの高さ			
				0,						// ボタンイメージの幅			
				0,						// ボタンイメージの高さ			
				sizeof(TBBUTTON)		// 構造体のサイズ
			);

	// TBADDBITMAP 構造体のメンバをセットして TB_ADDBITMAP メッセージを送る
	tb.hInst = HINST_COMMCTRL;
	tb.nID = IDB_STD_SMALL_COLOR;
	stdid = (int)SendMessage(hTool, TB_ADDBITMAP, 0, (LPARAM)&tb);
	stdid += 2;		// ?????	

	// TBBUTTON 構造体の iBitmap メンバを修正し、TB_ADDBUTTONS メッセージを送る。
	tbb[3].iBitmap += stdid;
	tbb[4].iBitmap += stdid;
	tbb[5].iBitmap += stdid;
	SendMessage(hTool, TB_ADDBUTTONS, (WPARAM)3, (LPARAM)&tbb[3]);

	// ボタンの系統(VIEW系）が違うので、再度同様の操作をする
	tb.hInst = HINST_COMMCTRL;
	tb.nID = IDB_VIEW_SMALL_COLOR;
	stdid = SendMessage(hTool, TB_ADDBITMAP, 0, (LPARAM)&tb);
	stdid += 2;		// ?????	
	tbb[6].iBitmap += stdid;
	SendMessage(hTool, TB_ADDBUTTONS, (WPARAM)1, (LPARAM)&tbb[6]);

	// セパレーターの挿入
	SendMessage(hTool, TB_INSERTBUTTON, (WPARAM)3, (LPARAM)&tb_sep);
	
	return hTool;
}
