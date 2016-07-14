// WinMain.cpp : Win32 Template

#include <windows.h>
#include <windowsx.h>

#include "resource.h"


#define	ID_EDIT		100
#define	ID_EDIT2	101


ATOM InitApp(HINSTANCE hInst);
BOOL InitInstance(HINSTANCE hInst, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


int MyNew(HWND hEdit);
int MyOpen(HWND hEdit);
int MySave(HWND hEdit);
int MySaveAs(HWND hEdit);
int MyConfirm(HWND hEdit);

int MySetInfo(HWND hEdit, HWND hEdit2);
int MyYmd(HWND hEdit, HWND hEdit2, int nOption);
LRESULT CALLBACK MyEditProc(HWND hEdit, UINT msg, WPARAM wParam, LPARAM lParam);

TCHAR szClassName[] = TEXT("memo02");	    // Window Class

HINSTANCE	hAppInst;
HWND hMain;						// 親ウィンドウのハンドル

TCHAR	szTitle[128] = TEXT("猫でもわかるメモ帳もどき");
TCHAR	szTitle_org[128] = TEXT("猫でもわかるメモ帳もどき[%s](%dバイト)");
TCHAR	szFile[MAX_PATH];
TCHAR	szFileTitle[MAX_PATH];

HWND hSubEdit;
BOOL	bInfo = TRUE;	// 情報ウィンドウの表示・非表示
WNDPROC OrgEditProc;

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
			if(!TranslateAccelerator(hMain, hAccel, &msg)) {
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
    wc.hIcon = (HICON)LoadImage(hInst,
	    TEXT("MYICON"),
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
    wc.hIconSm = (HICON)LoadImage(hInst,
	    TEXT("MYICON"),
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
	
	hMain = hWnd;				// グローバル変数にウィンドウハンドルを格納

    return TRUE;
}


// ウィンドウプロシージャ
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HDC	hdc, hdc_mem;
	PAINTSTRUCT	ps;

	int id, info_h;

	static HWND	hEdit, hEdit2;
	static HMENU	hMenu;

	RECT	rc;
	HBITMAP	hBmp;
	BITMAP	bmp_info;
	int	w, h;
	HICON	hIcon;
	MENUITEMINFO	mi;
	int	nStart, nEnd;

    switch(msg) {
	case WM_CREATE:
		hMenu = GetMenu(hWnd);

		hEdit = CreateWindow(TEXT("EDIT"), NULL,
				WS_CHILD | WS_VISIBLE | ES_WANTRETURN |
				ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL |
				ES_AUTOHSCROLL | WS_HSCROLL,
				0, 0, 0, 0,
				hWnd, (HMENU)ID_EDIT, hAppInst, NULL);
		hEdit2 = CreateWindow(TEXT("EDIT"), NULL,
				WS_CHILD | WS_VISIBLE | ES_WANTRETURN |
				ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL |
				ES_AUTOHSCROLL,
				0, 0, 0, 0,
				hWnd, (HMENU)ID_EDIT2, hAppInst, NULL);
		hSubEdit = hEdit2;

		lstrcat(szTitle, TEXT("無題"));
		SetWindowText(hWnd, szTitle);
		Edit_LimitText(hEdit, 0);

		SendMessage(hEdit2, EM_SETMARGINS,
					EC_LEFTMARGIN | EC_RIGHTMARGIN,
					MAKELPARAM(20, 20));
		OrgEditProc = (WNDPROC)SetWindowLongPtr(hEdit, GWL_WNDPROC, (LONG)MyEditProc);
		break;
	case WM_SIZE:
		info_h = (bInfo)? 50:0;
		
		MoveWindow(hEdit, 0, 0, LOWORD(lParam), HIWORD(lParam) - info_h, TRUE);

		if(bInfo) {
			MoveWindow(hEdit2, 150, HIWORD(lParam) - info_h, LOWORD(lParam) - 150, info_h, TRUE);
		}
		break;
	case WM_PAINT:
		if(bInfo) {
			GetClientRect(hWnd, &rc);
			hdc = BeginPaint(hWnd, &ps);

			hBmp = (HBITMAP)LoadImage(hAppInst, TEXT("MYBMP"), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
			GetObject(hBmp, sizeof(BITMAP), &bmp_info);
			w = bmp_info.bmWidth;
			h = bmp_info.bmHeight;
			hdc_mem = CreateCompatibleDC(hdc);
			SelectObject(hdc_mem, hBmp);
			BitBlt(hdc, 0, rc.bottom - 50, w, h, hdc_mem, 0, 0, SRCCOPY);
			DeleteObject(hBmp);
			DeleteDC(hdc_mem);

			EndPaint(hWnd, &ps);
		}
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
		case IDM__UNDO:
			SendMessage(hEdit, EM_UNDO, 0, 0);
			break;
		case IDM__CUT:
			SendMessage(hEdit, WM_CUT, 0, 0);
			break;
		case IDM__COPY:
			SendMessage(hEdit, WM_COPY, 0, 0);
			break;
		case IDM__PASTE:
			SendMessage(hEdit, WM_PASTE, 0, 0);
			break;
		case IDM__DEL:
			SendMessage(hEdit, WM_CLEAR, 0, 0);
			break;
		case IDM__ALL:
			SendMessage(hEdit, EM_SETSEL, 0, 0);
			break;
		case IDM__QUIT:
			SendMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		case IDM__ABOUT:
			hIcon = (HICON)LoadImage(hAppInst, TEXT("MYICON"), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
			ShellAbout(hWnd, TEXT("猫でもわかるメモ帳もどき#")
							TEXT("猫でもわかるメモ帳もどき Ver.0.500"),
							TEXT("Copyright(C) 2004 Y.Kumei"), hIcon);
			break;
		case IDM__INFO:
//			GetWindowRect(hWnd, &rc);
			if(bInfo) {
				bInfo = FALSE;
				ShowWindow(hEdit2, SW_HIDE);
			} else {
				bInfo = TRUE;
				ShowWindow(hEdit2, SW_NORMAL);
			}
			GetClientRect(hWnd, &rc);
			SendMessage(hWnd, WM_SIZE, (WPARAM)SIZE_RESTORED, MAKELPARAM(rc.right, rc.bottom));
			UpdateWindow(hEdit);
			break;
		case IDM__YMD1:
			MyYmd(hEdit, hEdit2, 1);
			break;
		case IDM__YMD2:
			MyYmd(hEdit, hEdit2, 2);
			break;
		case IDM__HMS1:
			MyYmd(hEdit, hEdit2, 3);
			break;
		case IDM__HMS2:
			MyYmd(hEdit, hEdit2, 4);
			break;
		default:
			break;
		}
		if(lParam != (LPARAM)hEdit) {
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
		switch(HIWORD(wParam)) {
		case EN_CHANGE:
			MySetInfo(hEdit, hEdit2);
			break;
		default:
			break;
		}
		break;
	case WM_SETFOCUS:
		SetFocus(hEdit);
		break;
	case WM_INITMENU:
		if(SendMessage(hEdit, EM_CANUNDO, 0, 0)) {
			EnableMenuItem(hMenu, IDM__UNDO, MF_BYCOMMAND | MF_ENABLED);
		} else {
			EnableMenuItem(hMenu, IDM__UNDO, MF_BYCOMMAND | MF_GRAYED);
		}

		mi.cbSize = sizeof(MENUITEMINFO);
		mi.fMask = MIIM_STATE;
		if(bInfo) {
			mi.fState = MFS_CHECKED;
			SetMenuItemInfo(hMenu, IDM__INFO, FALSE, &mi);
		} else {
			mi.fState = MFS_UNCHECKED;
			SetMenuItemInfo(hMenu, IDM__INFO, FALSE, &mi);
		}

		SendMessage(hEdit, EM_GETSEL, (WPARAM)&nStart, (LPARAM)&nEnd);
		if(nStart == nEnd) {
			EnableMenuItem(hMenu, IDM__CUT, MF_BYCOMMAND | MF_GRAYED);
			EnableMenuItem(hMenu, IDM__COPY, MF_BYCOMMAND | MF_GRAYED);
			EnableMenuItem(hMenu, IDM__DEL, MF_BYCOMMAND | MF_GRAYED);
		} else {
			EnableMenuItem(hMenu, IDM__CUT, MF_BYCOMMAND | MF_ENABLED);
			EnableMenuItem(hMenu, IDM__COPY, MF_BYCOMMAND | MF_ENABLED);
			EnableMenuItem(hMenu, IDM__DEL, MF_BYCOMMAND | MF_ENABLED);
		}

		if(IsClipboardFormatAvailable(CF_TEXT)) {
			EnableMenuItem(hMenu, IDM__PASTE, MF_BYCOMMAND | MF_ENABLED);
		} else {
			EnableMenuItem(hMenu, IDM__PASTE, MF_BYCOMMAND | MF_GRAYED);
		}

		DrawMenuBar(hWnd);

		break;
	case WM_CLOSE:
		id = MyConfirm(hEdit);
		if(IDCANCEL == id) {
			break;
		}

		id = MessageBox(hWnd, TEXT("終了してもよろしいですか"),
							TEXT("確認"),
							MB_YESNO | MB_ICONQUESTION);
		switch(id) {
		case IDYES:
			DestroyWindow(hEdit);
			DestroyWindow(hEdit2);
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
		case IDNO:
			return id;
			break;
		default:
			break;
		}
	}

	return 0;
}



int MySetInfo(HWND hEdit, HWND hEdit2)
{
	TCHAR	szBuf[256], szBuf2[64];
	int nLen, nLine, nStart, nEnd;

	nLen = GetWindowTextLength(hEdit);
	nLine = (int)SendMessage(hEdit, EM_GETLINECOUNT, 0, 0);
	wsprintf(szBuf, TEXT("文字数 = %d 文字\t行 = %d"), nLen, nLine);

	SendMessage(hEdit, EM_GETSEL, (WPARAM)&nStart, (LPARAM)&nEnd);
	if(nStart != nEnd) {
		wsprintf(szBuf2, TEXT("\r\n選択開始位置 = %d, 終了位置 = %d"), nStart, nEnd);
		lstrcat(szBuf, szBuf2);
	}

	SetWindowText(hEdit2, szBuf);

	return 0;
}



LRESULT CALLBACK MyEditProc(HWND hEdit, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg) {
	case WM_LBUTTONUP:
	case WM_KEYUP:
		MySetInfo(hEdit, hSubEdit);
		break;
	}

	return CallWindowProc(OrgEditProc, hEdit, msg, wParam, lParam);
}


int MyYmd(HWND hEdit, HWND hEdit2, int nOption)
{
	SYSTEMTIME	st;
	TCHAR	szBuf[64];

	GetLocalTime(&st);

	switch(nOption) {
	case 1:
		wsprintf(szBuf, TEXT("%d 年 %02d 月 %02d 日"), st.wYear, st.wMonth, st.wDay);
		break;
	case 2:
		wsprintf(szBuf, TEXT("%d/%02d/%02d"), st.wYear, st.wMonth, st.wDay);
		break;
	case 3:
		wsprintf(szBuf, TEXT("%02d 時 %02d 分 %02d 秒"), st.wHour, st.wMinute, st.wSecond);
		break;
	case 4:
		wsprintf(szBuf, TEXT("%02d:%02d:%02d"), st.wHour, st.wMinute, st.wSecond);
		break;
	default:
		wsprintf(szBuf, TEXT(""));
		break;
	}

	SetWindowText(hEdit2, szBuf);

	SendMessage(hEdit2, EM_SETSEL, 0, -1);
	SendMessage(hEdit2, WM_COPY, 0, 0);
	SendMessage(hEdit,  WM_PASTE, 0, 0);

	return 0;
}
