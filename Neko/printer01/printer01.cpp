// WinMain.cpp : Win32 Template

#include <windows.h>

#include "resource.h"

ATOM InitApp(HINSTANCE hInst);
BOOL InitInstance(HINSTANCE hInst, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
int MyPrint(HWND hWnd);

TCHAR szClassName[] = TEXT("printer01");	    // Window Class

HINSTANCE	hAppInst;
HWND hMainWindow;						// �e�E�B���h�E�̃n���h��

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


// �E�B���h�E�̐���
BOOL InitInstance(HINSTANCE hInst, int nCmdShow)
{
    HWND hWnd;

    hWnd = CreateWindow(
			szClassName,			// LPCTSTR lpClassName,  // �o�^����Ă���N���X��
			TEXT("�L�ł��킩��v�����^"),		// LPCTSTR lpWindowName, // �E�B���h�E��
			WS_OVERLAPPEDWINDOW,    // DWORD dwStyle,        // �E�B���h�E�X�^�C��
			CW_USEDEFAULT,			// int x,                // �E�B���h�E�̉������̈ʒu
			CW_USEDEFAULT,			// int y,                // �E�B���h�E�̏c�����̈ʒu
			CW_USEDEFAULT,			// int nWidth,           // �E�B���h�E�̕�
			CW_USEDEFAULT,			// int nHeight,          // �E�B���h�E�̍���
			NULL,					// HWND hWndParent,      // �e�E�B���h�E�܂��̓I�[�i�[�E�B���h�E�̃n���h��
			NULL,					// HMENU hMenu,          // ���j���[�n���h���܂��͎q�E�B���h�E ID �N���X���j���[���g������ NULL
			hInst,					// HINSTANCE hInstance,  // �A�v���P�[�V�����C���X�^���X�̃n���h��
			NULL);					// LPVOID lpParam        // �E�B���h�E�쐬�f�[�^

    if(!hWnd)	return FALSE;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
	
	hMainWindow = hWnd;				// �O���[�o���ϐ��ɃE�B���h�E�n���h�����i�[

    return TRUE;
}


// �E�B���h�E�v���V�[�W��
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
		case IDM__PRINT:
			MyPrint(hWnd);
			break;
		case IDM__QUIT:
			SendMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		case IDM__ABOUT:
			ShellAbout(hWnd, szClassName,
							TEXT("Win32 Template Version 0.00"), NULL);
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


int MyPrint(HWND hWnd)
{
	int i;
	PRINTDLG	pd;
	DOCINFO		di;
	TEXTMETRIC	tm;

	LPTSTR	lpszBuf = TEXT("�������������܂�");

	memset(&pd, 0, sizeof(PRINTDLG));
	pd.lStructSize = sizeof(PRINTDLG);
	pd.hwndOwner = hWnd;
	pd.hDevMode = NULL;
	pd.hDevNames = NULL;
	// pd.hDC;
	pd.Flags = PD_USEDEVMODECOPIESANDCOLLATE |
				PD_RETURNDC |
				PD_NOPAGENUMS |
				PD_NOSELECTION |
				PD_HIDEPRINTTOFILE;
	pd.nFromPage = 1;
	pd.nToPage = 1;
	pd.nMinPage = 1;
	pd.nMaxPage = 1;
	pd.nCopies = 1;
	// pd.hInstance;
	// pd.lCustData;
	// pd.lpfnPrintHook;
	// pd.lpfnSetupHook;
	// pd.lpPrintTemplateName;
	// pd.lpSetupTemplateName;
	// pd.hPrintTemplate;
	// pd.hSetupTemplate;
	
	if(!PrintDlg(&pd)) {
		return -1;
	}

	memset(&di, 0, sizeof(DOCINFO));
	di.cbSize = sizeof(DOCINFO);   
	di.lpszDocName = TEXT("Text");;
	//	di.lpszOutput;
	//	di.lpszDatatype;
	//	di.fwType;
	
	StartDoc(pd.hDC, &di);
	StartPage(pd.hDC);

	GetTextMetrics(pd.hDC, &tm);
	for(i = 0; i < 10; i++) {
		SetTextColor(pd.hDC, RGB(255, 0, 0));
		TextOut(pd.hDC, 10, 10 + (tm.tmHeight * i +20), lpszBuf, lstrlen(lpszBuf));
	}

	EndPage(pd.hDC);
	EndDoc(pd.hDC);
	DeleteDC(pd.hDC);

	
	return 0;
}
