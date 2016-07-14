// WinMain.cpp : Win32 Template

#include <windows.h>
#include <time.h>


ATOM InitApp(HINSTANCE hInst);
BOOL InitInstance(HINSTANCE hInst, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int TypeStart(HWND hWnd);


TCHAR szClassName[] = TEXT("key01");	    // Window Class

TCHAR szMondai[32], szInput[32], szCheck[32];
DWORD dwStart, dwEnd;
BOOL bStart = FALSE, bSeikai = TRUE;


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
			TEXT("猫でもわかるキー入力"),		// LPCTSTR lpWindowName, // ウィンドウ名
			WS_OVERLAPPEDWINDOW,    // DWORD dwStyle,        // ウィンドウスタイル
			CW_USEDEFAULT,			// int x,                // ウィンドウの横方向の位置
			CW_USEDEFAULT,			// int y,                // ウィンドウの縦方向の位置
			240,					// int nWidth,           // ウィンドウの幅
			180,					// int nHeight,          // ウィンドウの高さ
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
	HDC hdc;
	PAINTSTRUCT ps;
	MMTIME mm;

    switch(msg) {
	case WM_CREATE:
		srand((unsigned)time(NULL));
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		TextOut(hdc, 0, 0, szMondai, lstrlen(szMondai));
		TextOut(hdc, 0, 40, szInput, lstrlen(szInput));

		if(bSeikai) {
			SetTextColor(hdc, RGB(0, 0, 0));
		} else {
			SetTextColor(hdc, RGB(255, 0, 0));
		}

		TextOut(hdc, 0, 80, szCheck, lstrlen(szCheck));

		EndPaint(hWnd, &ps);

		break;
	case WM_CHAR:
		if((VK_SPACE == wParam) && !bStart) {
			bStart = TRUE;
			TypeStart(hWnd);
			break;
		}
		if(FALSE == bStart) 
			return DefWindowProc(hWnd, msg, wParam, lParam);
		if(VK_ESCAPE == wParam) {
			lstrcpy(szMondai, TEXT(""));
			lstrcpy(szInput, TEXT(""));
			lstrcpy(szCheck, TEXT(""));
			InvalidateRect(hWnd, NULL, TRUE);
			bStart = FALSE;
			break;
		}
		wsprintf(szInput, TEXT("あなたの入力=\"%c\""), (int)wParam);
		if(szMondai[4] == szInput[8]) {
			bSeikai = TRUE;
			mm.wType = TIME_MS;
			timeGetSystemTime(&mm, sizeof(MMTIME));
			dwEnd = mm.u.ms;
			wsprintf(szCheck, TEXT("反応時間[%dミリ秒]"), dwEnd - dwStart);
			TypeStart(hWnd);
		} else {
			bSeikai = FALSE;
			MessageBeep(MB_OK);
			lstrcpy(szCheck, TEXT("タイプミス!"));
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_DESTROY:
	    PostQuitMessage(0);
	    break;
	case WM_TIMER:
	case WM_LBUTTONDBLCLK:
	case WM_CLOSE:
	default:
	    return DefWindowProc(hWnd, msg, wParam, lParam);
	    break;
    }

	return 0;
}


int TypeStart(HWND hWnd)
{
	int n;
	MMTIME mm;

	n = rand() % 26;
	wsprintf(szMondai, TEXT("問題=\"%c\""), 'a' + n);
	mm.wType = TIME_MS;
	timeGetSystemTime(&mm, sizeof(MMTIME));
	dwStart = mm.u.ms;
	InvalidateRect(hWnd, NULL, TRUE);

	return 0;
}

