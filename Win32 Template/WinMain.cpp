// WinMain.cpp : Win32 Template

#include <windows.h>

#include "resource.h"

#define MAX_LOADSTRING 100

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// �O���[�o���ϐ�:
HINSTANCE hInst;								// ���݂̃C���^�[�t�F�C�X
TCHAR szTitle[MAX_LOADSTRING];					// �^�C�g�� �o�[�̃e�L�X�g
TCHAR szWindowClass[MAX_LOADSTRING];			// ���C�� �E�B���h�E �N���X��
TCHAR szVersionStr[MAX_LOADSTRING];				// Version String
HWND hMainWindow;								// �e�E�B���h�E�̃n���h��

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


// �E�B���h�E�̐���
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    HWND hWnd;
	
	hInst = hInstance;

    hWnd = CreateWindow(
			szWindowClass,			// LPCTSTR lpClassName,  // �o�^����Ă���N���X��
			szTitle,				// LPCTSTR lpWindowName, // �E�B���h�E��
			WS_OVERLAPPEDWINDOW,    // DWORD dwStyle,        // �E�B���h�E�X�^�C��
			CW_USEDEFAULT,			// int x,                // �E�B���h�E�̉������̈ʒu
			CW_USEDEFAULT,			// int y,                // �E�B���h�E�̏c�����̈ʒu
			CW_USEDEFAULT,			// int nWidth,           // �E�B���h�E�̕�
			CW_USEDEFAULT,			// int nHeight,          // �E�B���h�E�̍���
			NULL,					// HWND hWndParent,      // �e�E�B���h�E�܂��̓I�[�i�[�E�B���h�E�̃n���h��
			NULL,					// HMENU hMenu,          // ���j���[�n���h���܂��͎q�E�B���h�E ID �N���X���j���[���g������ NULL
			hInstance,					// HINSTANCE hInstance,  // �A�v���P�[�V�����C���X�^���X�̃n���h��
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
