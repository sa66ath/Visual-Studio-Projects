// WinMain.cpp : Win32 Template

#include <windows.h>

#include "resource.h"

ATOM InitApp(HINSTANCE hInst);
BOOL InitInstance(HINSTANCE hInst, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK MyDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);

TCHAR szClassName[] = TEXT("dialog02");	    // Window Class

HINSTANCE hInst;				// �C���X�^���X�n���h��
HWND hDlg;						// ���[�h���X�_�C�A���O�{�b�N�X�̃n���h��
TCHAR szName[32];				// �_�C�A���O�{�b�N�X������͂��ꂽ������
HWND hMain;

int WINAPI WinMain(HINSTANCE hCurInst, HINSTANCE hPrevInst, LPSTR lpsCmdLine, int nCmdShow)
{
    MSG msg;
    BOOL bRet;
	HACCEL hAccel;

	hInst = hCurInst;

    if(!InitApp(hCurInst))
		return FALSE;

    if(!InitInstance(hCurInst, nCmdShow))
		return FALSE;

	hAccel = LoadAccelerators(hCurInst, TEXT("MYACCEL"));
	
    while((bRet = GetMessage(&msg, NULL, 0, 0)) != 0) {
		if(bRet == -1) {
			MessageBox(NULL, TEXT("GetMessage Error"), TEXT("Error"), MB_OK);
			break;
		} else {
			if(!hDlg || !IsDialogMessage(hDlg, &msg)) {	
				if(!TranslateAccelerator(hMain, hAccel, &msg)) {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
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


// �E�B���h�E�̐���
BOOL InitInstance(HINSTANCE hInst, int nCmdShow)
{
    HWND hWnd;

    hWnd = CreateWindow(
			szClassName,			// LPCTSTR lpClassName,  // �o�^����Ă���N���X��
			TEXT("�L�ł��킩�郂�[�h���X�_�C�A���O�{�b�N�X"),		// LPCTSTR lpWindowName, // �E�B���h�E��
			WS_OVERLAPPEDWINDOW,    // DWORD dwStyle,        // �E�B���h�E�X�^�C��
			CW_USEDEFAULT,			// int x,                // �E�B���h�E�̉������̈ʒu
			CW_USEDEFAULT,			// int y,                // �E�B���h�E�̏c�����̈ʒu
			256,			// int nWidth,           // �E�B���h�E�̕�
			128,			// int nHeight,          // �E�B���h�E�̍���
			NULL,					// HWND hWndParent,      // �e�E�B���h�E�܂��̓I�[�i�[�E�B���h�E�̃n���h��
			NULL,					// HMENU hMenu,          // ���j���[�n���h���܂��͎q�E�B���h�E ID �N���X���j���[���g������ NULL
			hInst,					// HINSTANCE hInstance,  // �A�v���P�[�V�����C���X�^���X�̃n���h��
			NULL);					// LPVOID lpParam        // �E�B���h�E�쐬�f�[�^

    if(!hWnd)	return FALSE;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

	hMain = hWnd;

    return TRUE;
}


// �E�B���h�E�v���V�[�W��
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HMENU hMenu;
	TCHAR szBuf[64];
	HDC hdc;
	PAINTSTRUCT ps;

    switch(msg) {
	case WM_CREATE:
		hMenu = GetMenu(hWnd);
		break;
	case WM_INITMENU:
		if(IsWindow(hDlg)) {
			EnableMenuItem(hMenu, IDM_DLG, MF_BYCOMMAND | MF_GRAYED);
			EnableMenuItem(hMenu, IDM_CLOSEDLG, MF_BYCOMMAND | MF_ENABLED);
		} else {
			EnableMenuItem(hMenu, IDM_DLG, MF_BYCOMMAND | MF_ENABLED);
			EnableMenuItem(hMenu, IDM_CLOSEDLG, MF_BYCOMMAND | MF_GRAYED);
		}
		DrawMenuBar(hWnd);
		break;
	case WM_PAINT:
		if(0 == lstrcmp(szName, TEXT(""))) {
			lstrcpy(szBuf, TEXT("�܂����O�̓��͂͂���܂���"));
		} else {
			wsprintf(szBuf, TEXT("���͂��ꂽ������ %s ����ł�"), szName);
		}

		hdc = BeginPaint(hWnd, &ps);

		TextOut(hdc, 10, 10, szBuf, lstrlen(szBuf));

		EndPaint(hWnd, &ps);
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam)) {
		case IDM_END:
			SendMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		case IDM_DLG:
			hDlg = CreateDialog(hInst, TEXT("MYDLG"), hWnd, (DLGPROC)MyDlgProc);
			ShowWindow(hDlg, SW_NORMAL);
			break;
		case IDM_CLOSEDLG:
			DestroyWindow(hDlg);
			break;
		case IDM_ABOUT:
			MessageBox(hWnd, TEXT("Copyright(C)2004 y.Kumei"),
							TEXT("about"), MB_OK);
			break;
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
		break;
	case WM_CLOSE:
		if(IsWindow(hDlg)) {
			MessageBox(hWnd, TEXT("�_�C�A���O��j�����܂�"),
							TEXT("�j��"),
							MB_OK);
			DestroyWindow(hDlg);
		}
		DestroyWindow(hWnd);
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
	static HWND hParent;
	
	switch(msg) {
	case WM_INITDIALOG:
		hParent = GetParent(hDlg);
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam)) {
		case IDOK:
			GetDlgItemText(hDlg, IDC_EDIT1, szName, sizeof(szName) - 1);
			InvalidateRect(hParent, NULL, TRUE);
			return TRUE;
			break;
		case IDCANCEL:
			SetDlgItemText(hDlg, IDC_EDIT1, TEXT(""));
			return TRUE;
		case IDC_CLOSE:
			DestroyWindow(hDlg);
			return TRUE;
		}
		return FALSE;
	}
	return FALSE;
}
