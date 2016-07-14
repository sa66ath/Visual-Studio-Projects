// WinMain.cpp : Win32 Template

#include <windows.h>

ATOM InitApp(HINSTANCE hInst);
BOOL InitInstance(HINSTANCE hInst, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

HFONT MyCreateFont(int nHeight, DWORD dwCharSet, LPCTSTR lpName);

TCHAR szClassName[] = TEXT("font01");	    // Window Class

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
    wc.lpszMenuName = NULL;					// Menu 
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
			TEXT("文字列を表示する"),		// LPCTSTR lpWindowName, // ウィンドウ名
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
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	static HFONT hFont1, hFont2, hFont3;

	PAINTSTRUCT ps;
	HDC hdc;

	LPCTSTR lpszName1 = TEXT("粂井康孝");
	LPCTSTR lpszName2 = TEXT("Yasutaka Kumei");


    switch(msg) {
	case WM_CREATE:
		hFont1 = MyCreateFont(40, SHIFTJIS_CHARSET, TEXT("HG行書体"));
		hFont2 = MyCreateFont(40, ANSI_CHARSET, TEXT("Fraktur JS"));
		hFont3 = MyCreateFont(40, SHIFTJIS_CHARSET, TEXT("ＭＳ　明朝"));
		break;
	case WM_PAINT:
		hdc =BeginPaint(hWnd, &ps);

		SelectObject(hdc, hFont1);
		TextOut(hdc, 0,   0, lpszName1, lstrlen(lpszName1));
		SelectObject(hdc, hFont2);
		TextOut(hdc, 0,  60, lpszName2, lstrlen(lpszName2));
		SelectObject(hdc, hFont2);
		TextOut(hdc, 0, 120, lpszName1, lstrlen(lpszName1));

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		DeleteObject(hFont1);
		DeleteObject(hFont2);
		DeleteObject(hFont3);

	    PostQuitMessage(0);
	    break;
	default:
	    return DefWindowProc(hWnd, msg, wp, lp);
	    break;
    }

	return 0;
}



HFONT MyCreateFont(int nHeight, DWORD dwCharSet, LPCTSTR lpName)
{
	return CreateFont(nHeight, 0, 0, 0,		// 高さ、幅、角度
			FW_DONTCARE,					// 太さ
			FALSE, FALSE, FALSE,			// イタリック、下線、打消し
			dwCharSet,						// 文字セット
			OUT_DEFAULT_PRECIS,				// 出力精度
			CLIP_DEFAULT_PRECIS,			// クリッピング精度
			DEFAULT_QUALITY,				// 出力品質
			DEFAULT_PITCH | FF_DONTCARE,	// ピッチとファミリ
			lpName);						// タイプフェイス:w
}



