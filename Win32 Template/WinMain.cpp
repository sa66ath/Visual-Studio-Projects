// WinMain.cpp : Win32 Template

#include <windows.h>

#include "resource.h"

#define MAX_LOADSTRING 100

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// グローバル変数:
HINSTANCE hInst;								// 現在のインターフェイス
TCHAR szTitle[MAX_LOADSTRING];					// タイトル バーのテキスト
TCHAR szWindowClass[MAX_LOADSTRING];			// メイン ウィンドウ クラス名
TCHAR szVersionStr[MAX_LOADSTRING];				// Version String
HWND hMainWindow;								// 親ウィンドウのハンドル

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
    MSG msg;
	HACCEL hAccelTable;

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpszCmdLine);

	LoadString(hInstance, IDC_MYAPP, szWindowClass, MAX_LOADSTRING);
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDS_APP_VERSION, szVersionStr, MAX_LOADSTRING);

    MyRegisterClass(hInstance);


    if(!InitInstance(hInstance, nCmdShow))
		return FALSE;
	
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYAPP));
	
    while(GetMessage(&msg, NULL, 0, 0)) {
		if(!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
    }

    return (int)msg.wParam;
}


// Registering Window Class
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);			// UINT cbSize;
    wcex.style = CS_HREDRAW | CS_VREDRAW;		// UINT style;
    wcex.lpfnWndProc = WndProc;					// WNDPROC lpfnWndProc;
    wcex.cbClsExtra = 0;						// int cbClsExtra;
    wcex.cbWndExtra = 0;						// int cbWndExtra;
    wcex.hInstance = hInstance;					// HINSTANCE hInstance;
    wcex.hIcon = (HICON)LoadImage(hInstance,
	    MAKEINTRESOURCE(IDI_MYAPP), IMAGE_ICON,
	    0,
	    0,
	    LR_DEFAULTSIZE | LR_SHARED);		// HICON hIcon;
    wcex.hCursor = (HCURSOR)LoadImage(NULL,
	    MAKEINTRESOURCE(IDC_ARROW), IMAGE_CURSOR,
	    0,
	    0,
	    LR_DEFAULTSIZE | LR_SHARED);		// HCURSOR hCursor;
    wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wcex.lpszMenuName = MAKEINTRESOURCE(IDC_MYAPP);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = (HICON)LoadImage(hInstance,
	    MAKEINTRESOURCE(IDI_SMALL), IMAGE_ICON,
	    0,
	    0,
	    LR_DEFAULTSIZE | LR_SHARED);
 
    return RegisterClassEx(&wcex);
}


// ウィンドウの生成
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    HWND hWnd;
	
	hInst = hInstance;

    hWnd = CreateWindow(
			szWindowClass,			// LPCTSTR lpClassName,  // 登録されているクラス名
			szTitle,				// LPCTSTR lpWindowName, // ウィンドウ名
			WS_OVERLAPPEDWINDOW,    // DWORD dwStyle,        // ウィンドウスタイル
			CW_USEDEFAULT,			// int x,                // ウィンドウの横方向の位置
			CW_USEDEFAULT,			// int y,                // ウィンドウの縦方向の位置
			CW_USEDEFAULT,			// int nWidth,           // ウィンドウの幅
			CW_USEDEFAULT,			// int nHeight,          // ウィンドウの高さ
			NULL,					// HWND hWndParent,      // 親ウィンドウまたはオーナーウィンドウのハンドル
			NULL,					// HMENU hMenu,          // メニューハンドルまたは子ウィンドウ ID クラスメニューを使う時は NULL
			hInstance,					// HINSTANCE hInstance,  // アプリケーションインスタンスのハンドル
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

    switch(msg) {
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam)) {
		case IDM_NEW:
			MessageBox(NULL, TEXT("MENU File->New"), TEXT("OK ?"), MB_OK);
			break;
		case IDM_OPEN:
			MessageBox(NULL, TEXT("MENU File->Open..."), TEXT("OK ?"), MB_OK);
			break;
		case IDM_SAVE:
			MessageBox(NULL, TEXT("MENU File->Save"), TEXT("OK ?"), MB_OK);
			break;
		case IDM_SAVE_AS:
			MessageBox(NULL, TEXT("MENU File->Save As..."), TEXT("OK ?"), MB_OK);
			break;
		case IDM_EXIT:
			SendMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		case IDM_UNDO:
			MessageBox(NULL, TEXT("MENU Edit->Undo"), TEXT("OK ?"), MB_OK);
			break;
		case IDM_CUT:
			MessageBox(NULL, TEXT("MENU Edit->Cut"), TEXT("OK ?"), MB_OK);
			break;
		case IDM_COPY:
			MessageBox(NULL, TEXT("MENU Edit->Copy"), TEXT("OK ?"), MB_OK);
			break;
		case IDM_PASTE:
			MessageBox(NULL, TEXT("MENU Edit->Paste"), TEXT("OK ?"), MB_OK);
			break;
		case IDM_DELETE:
			MessageBox(NULL, TEXT("MENU Edit->DELETE"), TEXT("OK ?"), MB_OK);
			break;
		case IDM_SELECT_ALL:
			MessageBox(NULL, TEXT("MENU Edit->Select All"), TEXT("OK ?"), MB_OK);
			break;
		case IDM_ABOUT:
			ShellAbout(hWnd, szTitle,
							szVersionStr, NULL);
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
