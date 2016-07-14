// WinMain.cpp : Win32 Template

#include <windows.h>
#include <commctrl.h>

#include "resource.h"

#define	MY_STATUS	100

ATOM InitApp(HINSTANCE hInst);
BOOL InitInstance(HINSTANCE hInst, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
HWND MyCreateStatusbar(HWND hWnd);

#define	APP_NAME	TEXT("status01")
#define	APP_VERSION	TEXT("0.00")

TCHAR szClassName[] = APP_NAME;					// Window Class
TCHAR szVersionStr[] = APP_NAME TEXT("\tVersion ") APP_VERSION;



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
			TEXT("�L�ł��킩��X�e�[�^�X�o�["),		// LPCTSTR lpWindowName, // �E�B���h�E��
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

	INITCOMMONCONTROLSEX	ic;
	static HWND	hStatus;

    switch(msg) {
	case WM_CREATE:
		ic.dwICC = ICC_BAR_CLASSES;
		ic.dwSize = sizeof(INITCOMMONCONTROLSEX);
		InitCommonControlsEx(&ic);
		hStatus = MyCreateStatusbar(hWnd);
		break;
	case WM_SIZE:
		SendMessage(hStatus, msg, wParam, lParam);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam)) {
		case IDM_EXIT:
			SendMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		case IDM_ABOUT:
			ShellAbout(hWnd, szClassName,
							szVersionStr, NULL);
			break;
		}
		break;
	case WM_DESTROY:
		DestroyWindow(hStatus);
	    PostQuitMessage(0);
	    break;
	default:
	    return DefWindowProc(hWnd, msg, wParam, lParam);
	    break;
    }

	return 0;
}


HWND MyCreateStatusbar(HWND hWnd)
{
	HWND	hStatus;


    hStatus = CreateWindowEx(
			0,						// �g���X�^�C��
			STATUSCLASSNAME,		// LPCTSTR lpClassName,  // �o�^����Ă���N���X��
			NULL,					// LPCTSTR lpWindowName, // �E�B���h�E��
			WS_CHILD | SBARS_SIZEGRIP | CCS_BOTTOM | WS_VISIBLE,    // DWORD dwStyle,        // �E�B���h�E�X�^�C��
			0,						// int x,                // �E�B���h�E�̉������̈ʒu
			0,						// int y,                // �E�B���h�E�̏c�����̈ʒu
			0,						// int nWidth,           // �E�B���h�E�̕�
			0,						// int nHeight,          // �E�B���h�E�̍���
			hWnd,					// HWND hWndParent,      // �e�E�B���h�E�܂��̓I�[�i�[�E�B���h�E�̃n���h��
			(HMENU)MY_STATUS,		// HMENU hMenu,          // ���j���[�n���h���܂��͎q�E�B���h�E ID �N���X���j���[���g������ NULL
			hAppInst,				// HINSTANCE hInstance,  // �A�v���P�[�V�����C���X�^���X�̃n���h��
			NULL);					// LPVOID lpParam        // �E�B���h�E�쐬�f�[�^

	SendMessage(hStatus, SB_SIMPLE, TRUE, 0L);
	SendMessage(hStatus, SB_SETTEXT, 0 | 0, (LPARAM)TEXT("�L�ł��킩��X�e�[�^�X�o�["));
	

	return hStatus;
}
