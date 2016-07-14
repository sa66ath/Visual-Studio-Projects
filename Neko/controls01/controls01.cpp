// WinMain.cpp : Win32 Template

#include <windows.h>

#include <windowsx.h>
#include "resource.h"


ATOM InitApp(HINSTANCE hInst);
BOOL InitInstance(HINSTANCE hInst, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK MyDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);

TCHAR szClassName[] = TEXT("controls01");	    // Window Class

HINSTANCE hInst;

TCHAR szText[6][64];
int nSex, nNation, n20, nPet, nAddress, nPos = 50;


int WINAPI WinMain(HINSTANCE hCurInst, HINSTANCE hPrevInst, LPSTR lpsCmdLine, int nCmdShow)
{
    MSG msg;
    BOOL bRet;

	hInst = hCurInst;

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
			TEXT("猫でもわかるコントロール"),		// LPCTSTR lpWindowName, // ウィンドウ名
			WS_OVERLAPPEDWINDOW,    // DWORD dwStyle,        // ウィンドウスタイル
			CW_USEDEFAULT,			// int x,                // ウィンドウの横方向の位置
			CW_USEDEFAULT,			// int y,                // ウィンドウの縦方向の位置
			270,					// int nWidth,           // ウィンドウの幅
			200,					// int nHeight,          // ウィンドウの高さ
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
	int n;
	HDC hdc;
	PAINTSTRUCT ps;

    switch(msg) {
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		for(n = 0; n < sizeof(szText)/sizeof(szText[0]); n++) {
			TextOut(hdc, 10, 20 * n + 10, szText[n], lstrlen(szText[n]));
		}

		EndPaint(hWnd, &ps);
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam)) {
		case IDM_END:
			SendMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		case IDM_DLG:
			DialogBox(hInst, TEXT("MYDLG"), hWnd, (DLGPROC)MyDlgProc);
			break;
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
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


BOOL CALLBACK MyDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hRadio1, hRadio2, hRadio3, hRadio4;
	static HWND hCheck1, hCombo1, hList1, hEdit1, hScrol1;
	
	TCHAR szBuf[64];

	TCHAR szList[][16] = {
		TEXT("飼っていない"),
		TEXT("犬"),
		TEXT("猫"),
		TEXT("ねずみ"),
		TEXT("馬"),
	};

	TCHAR szCombo[][16] = {
		TEXT("北海道"),
		TEXT("本州"),
		TEXT("四国"),
		TEXT("九州"),
		TEXT("沖縄"),
		TEXT("その他"),
	};

	static HWND hMain;

	int id, n, x;

	switch(msg) {
	case WM_HSCROLL:
		if(lParam != (LPARAM)hScrol1)
			return FALSE;

		switch(LOWORD(wParam)) {
		case SB_LINELEFT:
			nPos--;
			if(nPos < 0)	nPos = 0;
			break;
		case SB_LINERIGHT:
			nPos++;
			if(100 < nPos)	nPos = 100;
			break;
		case SB_PAGELEFT:
			nPos -= 10;
			if(nPos < 0)	nPos = 0;
			break;
		case SB_PAGERIGHT:
			nPos += 10;
			if(100 < nPos)	nPos = 100;
			break;
		case SB_THUMBTRACK:
		case SB_THUMBPOSITION:
			nPos = HIWORD(wParam);
			break;
		}
		ScrollBar_SetPos(hScrol1, nPos, TRUE);
		wsprintf(szBuf, TEXT("%03d"), nPos);
		Edit_SetText(hEdit1, szBuf);
		wsprintf(szText[5], TEXT("スクロールバーの位置 = %03d"), nPos);
		InvalidateRect(hMain, NULL, TRUE);

		return TRUE;
	case WM_INITDIALOG:
			// 親ウィンドウのハンドル取得
			hMain = GetParent(hDlg);

			// 各コントロールのハンドルを取得
			hRadio1 = GetDlgItem(hDlg, IDC_RADIO1);
			hRadio2 = GetDlgItem(hDlg, IDC_RADIO2);
			hRadio3 = GetDlgItem(hDlg, IDC_RADIO3);
			hRadio4 = GetDlgItem(hDlg, IDC_RADIO4);
			hCheck1 = GetDlgItem(hDlg, IDC_CHECK1);
			hCombo1 = GetDlgItem(hDlg, IDC_COMBO1);
			hList1 = GetDlgItem(hDlg, IDC_LIST1);
			hEdit1 = GetDlgItem(hDlg, IDC_EDIT1);
			hScrol1 = GetDlgItem(hDlg, IDC_SCROLLBAR1);

			// スクロールバーの初期設定
			ScrollBar_SetRange(hScrol1, 0, 100, TRUE);
			ScrollBar_SetPos(hScrol1, nPos, TRUE);
			
			wsprintf(szBuf, TEXT("%03d"), nPos);
			Edit_SetText(hEdit1, szBuf);

			// ラジオグループの初期設定
			Button_SetCheck((!nSex)? hRadio1:hRadio2, BST_CHECKED);
			Button_SetCheck((!nNation)? hRadio3:hRadio4, BST_CHECKED);

			// チェックボックスの初期設定
			if(n20) {
				Button_SetCheck(hCheck1, BST_CHECKED);
			} else {
				Button_SetCheck(hCheck1, BST_UNCHECKED);
			}

			// コンボボックスに文字列を加える
			for(n = 0; n < sizeof(szCombo)/sizeof(szCombo[0]); n++) {
				ComboBox_AddString(hCombo1, szCombo[n]);
			}
			ComboBox_SetCurSel(hCombo1, nAddress);

			// リストボックスに文字列を加える
			for(n = 0; n < sizeof(szList)/sizeof(szList[0]); n++) {
				ListBox_AddString(hList1, szList[n]);
			}
			ListBox_SetCurSel(hList1, nPet);
			
			return TRUE;
			break;
	case WM_COMMAND:
		switch(LOWORD(wParam)) {
		case IDOK:
			lstrcpy(szText[0], (!nSex)? TEXT("男が選択されました"):TEXT("女が選択されました"));
			lstrcpy(szText[1], (!nNation)? TEXT("日本が選択されました"):TEXT("日本以外が選択されました"));
			lstrcpy(szText[2], (!n20)? TEXT("20歳未満です"):TEXT("20歳以上です"));
			wsprintf(szText[3], TEXT("%s が選択されました"), szCombo[nAddress]);
			wsprintf(szText[4], TEXT("%s が選択されました"), szList[nPet]);
			wsprintf(szText[5], TEXT("スクロールバーの位置 = %03d"), nPos);

			InvalidateRect(hMain, NULL, TRUE);	
			EndDialog(hDlg, IDOK);
			return TRUE;
			break;
		case IDCANCEL:
			EndDialog(hDlg, IDCANCEL);
			return TRUE;
		case IDC_RADIO1:
		case IDC_RADIO2:
			nSex = (BST_CHECKED != Button_GetCheck(hRadio1));
			lstrcpy(szText[0], (!nSex)? TEXT("男が選択されました"):TEXT("女が選択されました"));
			InvalidateRect(hMain, NULL, TRUE);
			return TRUE;
		case IDC_RADIO3:
		case IDC_RADIO4:
			nNation = (BST_CHECKED != Button_GetCheck(hRadio3));
			lstrcpy(szText[1], (!nNation)? TEXT("日本が選択されました"):TEXT("日本以外が選択されました"));
			InvalidateRect(hMain, NULL, TRUE);
			return TRUE;
		case IDC_CHECK1:
			n20 = (BST_CHECKED == Button_GetCheck(hCheck1));
			lstrcpy(szText[2], (!n20)? TEXT("20歳未満です"):TEXT("20歳以上です"));
			InvalidateRect(hMain, NULL, TRUE);
			return TRUE;
		case IDC_COMBO1:
			id = ComboBox_GetCurSel(hCombo1);
			wsprintf(szText[3], TEXT("%s が選択されました"), szCombo[id]);
			InvalidateRect(hMain, NULL, TRUE);
			nAddress = id;
			return TRUE;
		case IDC_LIST1:
			id = ListBox_GetCurSel(hList1);
			wsprintf(szText[4], TEXT("%s が選択されました"), szList[id]);
			InvalidateRect(hMain, NULL, TRUE);
			nPet = id;
			return TRUE;
		case IDC_EDIT1:
			Edit_GetText(hEdit1, szBuf, sizeof(szBuf) - 1);
			// x = atoi(szBuf);
			x = _wtoi(szBuf);
			nPos = x;
			if(nPos < 0)	nPos = 0;
			if(100 < nPos)	nPos = 100;
			ScrollBar_SetPos(hScrol1, nPos, TRUE);
			wsprintf(szText[5], TEXT("スクロールバーの位置 = %03d"), nPos);
			InvalidateRect(hMain, NULL, TRUE);
			return TRUE;
		default:
			break;
		}
		return FALSE;
	}
	return FALSE;
}
