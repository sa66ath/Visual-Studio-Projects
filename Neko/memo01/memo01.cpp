// WinMain.cpp : Win32 Template

#include <windows.h>
#include <windowsx.h>

#include "resource.h"


#define	ID_EDIT	100


ATOM InitApp(HINSTANCE hInst);
BOOL InitInstance(HINSTANCE hInst, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


int MyNew(HWND hEdit);
int MyOpen(HWND hEdit);
int MySave(HWND hEdit);
int MySaveAs(HWND hEdit);
int MyConfirm(HWND hEdit);


TCHAR szClassName[] = TEXT("memo01");	    // Window Class

HINSTANCE	hAppInst;
HWND hParent;						// 親ウィンドウのハンドル

TCHAR	szTitle[128] = TEXT("猫でもわかるメモ帳もどき");
TCHAR	szTitle_org[128] = TEXT("猫でもわかるメモ帳もどき[%s](%dバイト)");
TCHAR	szFile[MAX_PATH];
TCHAR	szFileTitle[MAX_PATH];


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
			szTitle,				// LPCTSTR lpWindowName, // ウィンドウ名
			WS_OVERLAPPEDWINDOW,    // DWORD dwStyle,        // ウィンドウスタイル
			CW_USEDEFAULT,			// int x,                // ウィンドウの横方向の位置
			CW_USEDEFAULT,			// int y,                // ウィンドウの縦方向の位置
			512,					// int nWidth,           // ウィンドウの幅
			384,					// int nHeight,          // ウィンドウの高さ
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

	int id;

	static HWND	hEdit;

    switch(msg) {
	case WM_CREATE:
		hEdit = CreateWindow(TEXT("EDIT"), NULL,
				WS_CHILD | WS_VISIBLE | ES_WANTRETURN |
				ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL |
				ES_AUTOHSCROLL | WS_HSCROLL,
				0, 0, 0, 0,
				hWnd, (HMENU)ID_EDIT, hAppInst, NULL);
		lstrcat(szTitle, TEXT("無題"));
		SetWindowText(hWnd, szTitle);
		Edit_LimitText(hEdit, 0);
		break;
	case WM_SIZE:
		MoveWindow(hEdit, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam)) {
		case IDM__NEW:
			MyNew(hEdit);
			break;
		case IDM__OPEN:
			MyOpen(hEdit);
			break;
		case IDM__SAVE:
			MySave(hEdit);
			break;
		case IDM__SAVE_AS:
			MySaveAs(hEdit);
			break;
		case IDM__QUIT:
			SendMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		case IDM__ABOUT:
			MessageBox(NULL, TEXT("MENU Help->About"), TEXT("OK ?"), MB_OK);
			break;
		}
		break;
	case WM_CLOSE:
		id = MessageBox(hWnd, TEXT("終了してもよろしいですか"),
							TEXT("確認"),
							MB_YESNO | MB_ICONQUESTION);
		switch(id) {
		case IDYES:
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


int MyNew(HWND hEdit)
{

	switch(MyConfirm(hEdit)) {
	case IDCANCEL:
			return -1;
		break;
	default:
		break;
	}

	Edit_SetText(hEdit, TEXT(""));
	SetWindowText(GetParent(hEdit), TEXT("猫でもわかるメモ帳もどき[無題]"));
	lstrcpy(szFile, TEXT(""));

	return 0;
}


int MyOpen(HWND hEdit)
{
	HWND	hMain;
	DWORD	dwSize = 0L;
	OPENFILENAME	ofn;
	HANDLE	hFile;
	DWORD	dwAccBytes;
	LPTSTR	lpszBuf;

	HGLOBAL	hMem;

	switch(MyConfirm(hEdit)) {
	case IDCANCEL:
			return -1;
		break;
	default:
		break;
	}

	memset(&ofn, 0, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hEdit;
	ofn.lpstrFilter = TEXT("text(*.txt)\0*.txt\0All files(*.*)\0*.*\0\0");
	ofn.lpstrFile = szFile;
	ofn.lpstrFileTitle = szFileTitle;
	ofn.nMaxFile = MAX_PATH;
	ofn.nMaxFileTitle = MAX_PATH;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = TEXT("txt");
	ofn.lpstrTitle = TEXT("猫でもわかるファイルオープン");

	if(!GetOpenFileName(&ofn))	return -1;
	hFile = CreateFile(szFile,			// ファイル名
				GENERIC_READ,			// 読み取り／書込みアクセス
				0,						// 共有設定 0 だと共有対象としない
				NULL,					// セキュリティ属性
				OPEN_ALWAYS,			// ファイルがすでに存在しているかどうか
				FILE_ATTRIBUTE_NORMAL,	// ファイル属性
				NULL);
	dwSize = GetFileSize(hFile, NULL);

	hMem = GlobalAlloc(GHND, dwSize + sizeof(TCHAR));
	if(NULL == hMem) {
		MessageBox(hEdit,
				TEXT("メモリを確保できません"),
				TEXT("猫でもわかるメモ帳もどき"),
				MB_ICONEXCLAMATION | MB_OK);
		return -1;
	}
	lpszBuf = (LPTSTR)GlobalLock(hMem);

	ReadFile(hFile, lpszBuf, dwSize, &dwAccBytes, NULL);
	lpszBuf[dwAccBytes] = TEXT('\0');
	Edit_SetText(hEdit, lpszBuf);

	wsprintf(szTitle, szTitle_org, szFileTitle, dwSize);
	hMain = GetParent(hEdit);	
	SetWindowText(hMain, szTitle);

	CloseHandle(hFile);
	GlobalUnlock(hMem);
	GlobalFree(hMem);

	return 0;
}


int MySaveAs(HWND hEdit)
{
	OPENFILENAME	ofn;
	HANDLE	hFile;
	DWORD	dwAccBytes;

	LPTSTR	lpszBuf;
	int	nLen;

	HGLOBAL	hMem;


	nLen = GetWindowTextLength(hEdit);
	hMem = GlobalAlloc(GHND, sizeof(TCHAR)*(nLen + 1));
	lpszBuf = (LPTSTR)GlobalLock(hMem);
	
	GetWindowText(hEdit, lpszBuf, nLen + 1);

	memset(&ofn, 0, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hEdit;
	ofn.lpstrFilter = TEXT("text(*.txt)\0*.txt\0All files(*.*)\0*.*\0\0");
	ofn.lpstrFile = szFile;
	ofn.lpstrFileTitle = szFileTitle;
	ofn.nFilterIndex = 1;
	ofn.nMaxFile = MAX_PATH;
	ofn.nMaxFileTitle = MAX_PATH;
	ofn.Flags = OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = TEXT("txt");
	ofn.lpstrTitle = TEXT("名前を付けて保存する");

	if(!GetSaveFileName(&ofn))	return -1;

	hFile = CreateFile(szFile,			// ファイル名
				GENERIC_WRITE,			// 読み取り／書込みアクセス
				0,						// 共有設定 0 だと共有対象としない
				NULL,					// セキュリティ属性
				CREATE_ALWAYS,			// ファイルがすでに存在しているかどうか
				FILE_ATTRIBUTE_NORMAL,	// ファイル属性
				NULL);
	WriteFile(hFile, lpszBuf, (DWORD)lstrlen(lpszBuf), &dwAccBytes, NULL);
	// 	WriteFile(hFile, lpszBuf, sizeof(TCHAR)*(DWORD)lstrlen(lpszBuf), &dwAccBytes, NULL);

	wsprintf(szTitle, szTitle_org, szFileTitle, nLen);
	// wsprintf(szTitle, szTitle_org, szFileTitle, dwAccBytes);
	SetWindowText(GetParent(hEdit), szTitle);

	if(0 == CloseHandle(hFile)) {
		MessageBox(hEdit, TEXT("Error CloseHandle"),
						TEXT("Error"),
						MB_OK);
	}
	SendMessage(hEdit, EM_SETMODIFY, FALSE, 0);
	GlobalUnlock(hMem);
	GlobalFree(hMem);

	return 0;
}


int MySave(HWND hEdit)
{
	HANDLE	hFile;
	DWORD	dwAccBytes;
	int	nLen;
	LPTSTR	lpszBuf;
	HGLOBAL	hMem;

	if(0 == lstrcmp(szFile, TEXT(""))) {
		MessageBox(hEdit, TEXT("ファイル名が付けられていません"),
						TEXT("猫でもわかるメモ帳もどき"),
						MB_OK);
		MySaveAs(hEdit);
		return -1;
	}

	nLen = GetWindowTextLength(hEdit);
	hMem = GlobalAlloc(GHND, sizeof(TCHAR)*(nLen + 1));
	// hMem = GlobalAlloc(GHND, nLen + sizeof(TCHAR));
	lpszBuf = (LPTSTR)GlobalLock(hMem);
	
	GetWindowText(hEdit, lpszBuf, nLen + 1);

	hFile = CreateFile(szFile,			// ファイル名
				GENERIC_WRITE,			// 読み取り／書込みアクセス
				0,						// 共有設定 0 だと共有対象としない
				NULL,					// セキュリティ属性
				TRUNCATE_EXISTING,			// ファイルがすでに存在しているかどうか
				FILE_ATTRIBUTE_NORMAL,	// ファイル属性
				NULL);
	WriteFile(hFile, lpszBuf, (DWORD)lstrlen(lpszBuf), &dwAccBytes, NULL);
	// WriteFile(hFile, lpszBuf, sizeof(TCHAR)*(DWORD)lstrlen(lpszBuf), &dwAccBytes, NULL);
	
	GlobalUnlock(hMem);
	GlobalFree(hMem);

	SendMessage(hEdit, EM_SETMODIFY, FALSE, 0);

	wsprintf(szTitle, szTitle_org, szFileTitle, nLen);
	// wsprintf(szTitle, szTitle_org, szFileTitle, dwAccBytes);
	SetWindowText(GetParent(hEdit), szTitle);

	if(0 == CloseHandle(hFile)) {
		MessageBox(hEdit, TEXT("Error CloseHandle"),
						TEXT("Error"),
						MB_OK);
	}

	return 0;
}




int MyConfirm(HWND hEdit)
{
	if(TRUE == SendMessage(hEdit, EM_GETMODIFY, 0, 0)) {
		int id;

		id = MessageBox(hEdit, TEXT("文書が更新されています。\n変更を保存しますか？"),
						TEXT("猫でもわかるメモ帳もどき"),
						MB_YESNOCANCEL | MB_ICONEXCLAMATION);

		switch(id) {
		case IDYES:
			MySave(hEdit);
			break;
		case IDCANCEL:
			return IDCANCEL;
			break;
		case IDNO:
			return IDNO;
			break;
		default:
			break;
		}
	}

	return 0;
}
