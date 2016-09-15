// WinMain.cpp : Win32 Template

#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>

#include "resource.h"

#define	ID_MYTIMER	100
#define	ID_MYTIMER2	200
#define	ID_MYTRAY	300

#define	MYTRAY_MESSAGE	(WM_APP + 1)

#define	INIFILENAME	TEXT("mydesktop.ini")



ATOM InitApp(HINSTANCE hInst);
BOOL InitInstance(HINSTANCE hInst, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK MyDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
int MyMoveCenter(HWND hWnd);
int MyInitNotifyIcon(HWND hWnd);
int MakeTrayMenu(HWND hWnd);
int MyShowPlan(HWND *hEdit, LPTSTR lpszApp);
int MyWritePlan(HWND *hEdit, LPTSTR lpszApp);


#define	APP_NAME	TEXT("desktop01")
#define	APP_VERSION	TEXT("0.00")

TCHAR szClassName[] = APP_NAME;					// Window Class
TCHAR szVersionStr[] = APP_NAME TEXT("\tVersion ") APP_VERSION;



HINSTANCE	hAppInst;
HWND hMainWindow;						// 親ウィンドウのハンドル

BOOL	bPlan;
NOTIFYICONDATA	ni;
BOOL	bTray;
int	nDlg;




int WINAPI WinMain(HINSTANCE hCurInst, HINSTANCE hPrevInst, LPSTR lpsCmdLine, int nCmdShow)
{
    MSG msg;
    BOOL bRet;
	
	HACCEL hAccel;


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
    wc.lpszMenuName = NULL;					// Menu 
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
	
	hAppInst = hInst;

    hWnd = CreateWindow(
			szClassName,			// LPCTSTR lpClassName,  // 登録されているクラス名
			TEXT("猫でもわかる予定表"),		// LPCTSTR lpWindowName, // ウィンドウ名
			WS_POPUP,				// DWORD dwStyle,        // ウィンドウスタイル
			CW_USEDEFAULT,			// int x,                // ウィンドウの横方向の位置
			CW_USEDEFAULT,			// int y,                // ウィンドウの縦方向の位置
			150,					// int nWidth,           // ウィンドウの幅
			150,					// int nHeight,          // ウィンドウの高さ
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
	HDC	hdc;
	PAINTSTRUCT	ps;

	INITCOMMONCONTROLSEX	cc;
	static HRGN	hRgn, hRgn1, hRgn2, hRgn3, hRgn4;
	HPEN	hPen;
	static TCHAR	szBuf[32];
	HBITMAP	hBmp;
	BITMAP	bmp_info;
	int	w, h;
	HDC	hdc_mem;
	static RECT	rc = { 20, 120, 130, 140 };
	HMENU	hMenu, hSubMenu;
	POINT	pt;
	SYSTEMTIME	st;
	int	nPlan, i;
	TCHAR	szKey[8], szApp[32], szPlan[256];
	static RECT	rcBmp = { 20, 40, 130, 115 };
	int	id;

    switch(msg) {
	case WM_CREATE:
		MyMoveCenter(hWnd);
		cc.dwSize = sizeof(INITCOMMONCONTROLSEX);
		cc.dwICC = ICC_DATE_CLASSES;
		InitCommonControlsEx(&cc);

		hRgn = CreateRectRgn(0, 0, 1, 1);
		hRgn4 = CreateRectRgn(0, 0, 1, 1);
		hRgn1 = CreateRoundRectRgn(0, 25, 150, 150, 40, 40);
		hRgn2 = CreateEllipticRgn(50, 0, 100, 50);
		hRgn3 = CreateEllipticRgn(60, 10, 90, 40);

		CombineRgn(hRgn4, hRgn2, hRgn3, RGN_DIFF);
		CombineRgn(hRgn, hRgn4, hRgn1, RGN_OR);

		SetWindowRgn(hWnd, hRgn, TRUE);

		SetTimer(hWnd, ID_MYTIMER, 500, NULL);
		SetTimer(hWnd, ID_MYTIMER2, 5000, NULL);

		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		hPen = CreatePen(PS_SOLID, 20, RGB(0, 0, 255));
		SelectObject(hdc, hPen);

		Ellipse(hdc, 50, 0, 100, 50);
		RoundRect(hdc, 0, 25, 150, 150, 40, 40);
		SetBkMode(hdc, TRANSPARENT);
		DrawText(hdc, szBuf, -1, &rc, DT_CENTER);
		if(bPlan) {
			hBmp = (HBITMAP)LoadImage(hAppInst, TEXT("MYPLAN"), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
		} else {
			hBmp = (HBITMAP)LoadImage(hAppInst, TEXT("MYTITLE"), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
		}
		GetObject(hBmp, sizeof(BITMAP), &bmp_info);
		w = bmp_info.bmWidth;
		h = bmp_info.bmHeight;
		hdc_mem = CreateCompatibleDC(hdc);
		SelectObject(hdc_mem, hBmp);
		BitBlt(hdc, 20, 40, w, h, hdc_mem, 0, 0, SRCCOPY);

		DeleteDC(hdc_mem);
		DeleteObject(hBmp);
		DeleteObject(hPen);

		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONDOWN:
		PostMessage(hWnd, WM_NCLBUTTONDOWN, (WPARAM)HTCAPTION, lParam);
		break;
	case WM_RBUTTONDOWN:
		hMenu = LoadMenu(hAppInst, TEXT("MYMENU"));
		hSubMenu = GetSubMenu(hMenu, 0);
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);
		ClientToScreen(hWnd, &pt);
		TrackPopupMenu(hSubMenu, TPM_LEFTALIGN, pt.x, pt.y, 0, hWnd, NULL);
		DestroyMenu(hMenu);
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam)) {
		case IDM_PLAN:
			DialogBox(hAppInst, TEXT("MYDLG"), hWnd, (DLGPROC)MyDlgProc);
			break;
		case IDM_TASK:
			CloseWindow(hWnd);
			break;
		case IDM_TRAY:
			MyInitNotifyIcon(hWnd);
			break;
		case IDM_OUT:
			Shell_NotifyIcon(NIM_DELETE, &ni);
			ShowWindow(hWnd, SW_SHOWNORMAL);
			bTray = FALSE;
			break;
		case IDM_EXIT:
			SendMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		case IDM_ABOUT:
			ShellAbout(hWnd, szClassName,
							szVersionStr, NULL);
			break;
		}
		break;
	case WM_TIMER:
		switch(wParam) {
		case ID_MYTIMER:
			GetLocalTime(&st);
			wsprintf(szBuf, TEXT("%02d/%02d %02d:%02d:%02d"), st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
			InvalidateRect(hWnd, &rc, TRUE);
			if(IsIconic(hWnd)) {
				SetWindowText(hWnd, szBuf);
			} else {
				SetWindowText(hWnd, TEXT("猫でもわかる予定表"));
			}
			break;
		case ID_MYTIMER2:
			nPlan = 0;
			GetLocalTime(&st);
			wsprintf(szApp, TEXT("%d-%02d-%02d"), st.wYear, st.wMonth, st.wDay);
			for(i = 0; i < 5; i++) {
				wsprintf(szKey, TEXT("%d"), i);
				GetPrivateProfileString(szApp, szKey, TEXT("Nothing"),
						szPlan, (DWORD)sizeof(szPlan), INIFILENAME);
				if(0 == lstrcmp(szPlan, TEXT("Nothing"))) {
					nPlan++;
				}
			}
			if(5 == nPlan) {
				bPlan = FALSE;
			} else {
				bPlan = TRUE;
			}
			InvalidateRect(hWnd, &rcBmp, TRUE);
			break;
		default:
			break;
		}
		break;
	case MYTRAY_MESSAGE:
		switch(lParam) {
		case WM_RBUTTONDOWN:
			MakeTrayMenu(hWnd);
			break;
		case WM_LBUTTONDBLCLK:
			Shell_NotifyIcon(NIM_DELETE, &ni);
			ShowWindow(hWnd, SW_SHOW);
			bTray = TRUE;
			break;
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
		break;
	case WM_CLOSE:
		if(0 != nDlg) {
			MessageBox(hWnd, TEXT("予定表ダイアログを閉じてから終了してください"),
					TEXT("猫でもわかる予定表"),
					MB_OK | MB_ICONEXCLAMATION);
			break;
		}

		id = MessageBox(hWnd, TEXT("終了してもよろしいですか"),
				TEXT("猫でもわかる予定表"),
				MB_YESNO | MB_ICONQUESTION);

		if(IDYES == id) {
			if(bTray) {
				Shell_NotifyIcon(NIM_DELETE, &ni);
			}
			KillTimer(hWnd, ID_MYTIMER);
			DeleteObject(hRgn1);
			DeleteObject(hRgn2);
			DeleteObject(hRgn3);
			DeleteObject(hRgn4);
			DeleteObject(hRgn);
			DestroyWindow(hWnd);
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


int MyMoveCenter(HWND hWnd)
{
	int	w, h, x, y;
	RECT	rc;

	w = GetSystemMetrics(SM_CXSCREEN);
	h = GetSystemMetrics(SM_CYSCREEN);

	GetWindowRect(hWnd, &rc);

	x = (w - (rc.right - rc.left))/2;
	y = (h - (rc.bottom - rc.top))/2;

	MoveWindow(hWnd, x, y, rc.right - rc.left, rc.bottom - rc.top, TRUE);

	return 0;
}


BOOL CALLBACK MyDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND	hMon, hStatic, hEdit[5];
	static SYSTEMTIME	st;
	int	i;
	TCHAR	szBuf[256], szApp[32];
	LPNMSELCHANGE	lpSChange;

	switch(msg) {
	case WM_INITDIALOG:
		nDlg++;
		if(1 < nDlg) {
			MessageBox(hDlg,
					TEXT("すでに予定表は出ています"),
					TEXT("猫でもわかる予定表"), MB_OK);
			EndDialog(hDlg, IDOK);
			nDlg--;
			return TRUE;
		}

		hMon = GetDlgItem(hDlg, IDC_MONTHCALENDAR1);
		hStatic = GetDlgItem(hDlg, IDC_MYSTATIC);
		hEdit[0] = GetDlgItem(hDlg, IDC_EDIT1);
		hEdit[1] = GetDlgItem(hDlg, IDC_EDIT2);
		hEdit[2] = GetDlgItem(hDlg, IDC_EDIT3);
		hEdit[3] = GetDlgItem(hDlg, IDC_EDIT4);
		hEdit[4] = GetDlgItem(hDlg, IDC_EDIT5);

		for(i = 0; i < sizeof(hEdit)/sizeof(hEdit[0]); i++) {
			SendMessage(hEdit[i], EM_LIMITTEXT, (WPARAM)256, 0);
		}

		MonthCal_GetCurSel(hMon, &st);
		wsprintf(szBuf, TEXT("%d年%02d月%02d日の予定"), st.wYear, st.wMonth, st.wDay);
		SetWindowText(hStatic, szBuf);

		wsprintf(szApp, TEXT("%d-%02d-%02d"), st.wYear, st.wMonth, st.wDay);
		MyShowPlan(hEdit, szApp);
		SetFocus(hEdit[0]);

		return TRUE;
	case WM_COMMAND:
		switch(LOWORD(wParam)) {
		case IDOK:
			MonthCal_GetCurSel(hMon, &st);
			wsprintf(szApp, TEXT("%d-%02d-%02d"), st.wYear, st.wMonth, st.wDay);
			MyWritePlan(hEdit, szApp);
			return TRUE;
		case IDCANCEL:
			nDlg--;
			EndDialog(hDlg, IDCANCEL);
			return TRUE;
		case IDC_TODAY:
			GetLocalTime(&st);
			MonthCal_SetCurSel(hMon, &st);
			wsprintf(szApp, TEXT("%d-%02d-%02d"), st.wYear, st.wMonth, st.wDay);
			MyShowPlan(hEdit, szApp);

			// Static Text 更新の為、追加
			wsprintf(szBuf, TEXT("%d年%02d月%02d日の予定"), st.wYear, st.wMonth, st.wDay);	
			SetWindowText(hStatic, szBuf);
			return TRUE;
		default:
			break;
		}
		return FALSE;
	case WM_NOTIFY:
		lpSChange = (LPNMSELCHANGE)lParam;
		if((lpSChange->nmhdr).hwndFrom != hMon || (lpSChange->nmhdr).code != MCN_SELCHANGE) {
			return FALSE;
		}

		MonthCal_GetCurSel(hMon, &st);
		wsprintf(szBuf, TEXT("%d年%02d月%02d日の予定"), st.wYear, st.wMonth, st.wDay);
		SetWindowText(hStatic, szBuf);

		wsprintf(szApp, TEXT("%d-%02d-%02d"), st.wYear, st.wMonth, st.wDay);
		MyShowPlan(hEdit, szApp);
		SetFocus(hEdit[0]);

		return FALSE;
	default:
		break;
	}

	return FALSE;
}


int MyInitNotifyIcon(HWND hWnd)
{
	memset(&ni, 0, sizeof(NOTIFYICONDATA));

	ni.cbSize = sizeof(NOTIFYICONDATA);
	ni.hWnd = hWnd;
	ni.uID = ID_MYTRAY;
	ni.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	ni.hIcon = (HICON)LoadImage(hAppInst, TEXT("MYICON"), IMAGE_ICON, 0, 0, 0);
	ni.uCallbackMessage = MYTRAY_MESSAGE;
	lstrcpy(ni.szTip, TEXT("猫でもわかる予定表"));

	Shell_NotifyIcon(NIM_ADD, &ni);

	ShowWindow(hWnd, SW_HIDE);

	bTray = TRUE;

	return 0;
}



int MakeTrayMenu(HWND hWnd)
{
	HMENU	hTrayMenu, hSubMenu;
	POINT	pt;

	hTrayMenu = LoadMenu(hAppInst, TEXT("MYTRAY"));
	hSubMenu = GetSubMenu(hTrayMenu, 0);

	GetCursorPos(&pt);
	SetForegroundWindow(hWnd);
	TrackPopupMenu(hSubMenu, TPM_BOTTOMALIGN, pt.x, pt.y, 0, hWnd, NULL);
	DestroyMenu(hTrayMenu);

	return 0;
}


int MyShowPlan(HWND *hEdit, LPTSTR lpszApp)
{
	int	n;
	TCHAR	szKey[8], szBuf[256];

	for(n = 0; n < 5; n++) {
		SetWindowText(hEdit[n], TEXT(""));
		wsprintf(szKey, TEXT("%d"), n);
		GetPrivateProfileString(lpszApp,
				szKey,
				TEXT("Nothing"),
				szBuf, (DWORD)sizeof(szBuf),
				INIFILENAME);
		if(0 != lstrcmp(szBuf, TEXT("Nothing"))) {
			SetWindowText(hEdit[n], szBuf);
		}
	}

	return 0;
}


int MyWritePlan(HWND *hEdit, LPTSTR lpszApp)
{
	int	n;
	TCHAR	szKey[8], szBuf[256];

	for(n = 0; n < 5; n++) {
		wsprintf(szKey, TEXT("%d"), n);
		Edit_GetText(hEdit[n], szBuf, sizeof(szBuf) - 1);
		if(0 != lstrcmp(szBuf, TEXT(""))) {
			WritePrivateProfileString(lpszApp, szKey, szBuf, INIFILENAME);
		} else {
			WritePrivateProfileString(lpszApp, szKey, NULL, INIFILENAME);
		}
	}

	return 0;
}
