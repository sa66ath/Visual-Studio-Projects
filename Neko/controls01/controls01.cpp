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


// �E�B���h�E�̐���
BOOL InitInstance(HINSTANCE hInst, int nCmdShow)
{
    HWND hWnd;

    hWnd = CreateWindow(
			szClassName,			// LPCTSTR lpClassName,  // �o�^����Ă���N���X��
			TEXT("�L�ł��킩��R���g���[��"),		// LPCTSTR lpWindowName, // �E�B���h�E��
			WS_OVERLAPPEDWINDOW,    // DWORD dwStyle,        // �E�B���h�E�X�^�C��
			CW_USEDEFAULT,			// int x,                // �E�B���h�E�̉������̈ʒu
			CW_USEDEFAULT,			// int y,                // �E�B���h�E�̏c�����̈ʒu
			270,					// int nWidth,           // �E�B���h�E�̕�
			200,					// int nHeight,          // �E�B���h�E�̍���
			NULL,					// HWND hWndParent,      // �e�E�B���h�E�܂��̓I�[�i�[�E�B���h�E�̃n���h��
			NULL,					// HMENU hMenu,          // ���j���[�n���h���܂��͎q�E�B���h�E ID �N���X���j���[���g������ NULL
			hInst,					// HINSTANCE hInstance,  // �A�v���P�[�V�����C���X�^���X�̃n���h��
			NULL);					// LPVOID lpParam        // �E�B���h�E�쐬�f�[�^

    if(!hWnd)	return FALSE;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}


// �E�B���h�E�v���V�[�W��
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
		TEXT("�����Ă��Ȃ�"),
		TEXT("��"),
		TEXT("�L"),
		TEXT("�˂���"),
		TEXT("�n"),
	};

	TCHAR szCombo[][16] = {
		TEXT("�k�C��"),
		TEXT("�{�B"),
		TEXT("�l��"),
		TEXT("��B"),
		TEXT("����"),
		TEXT("���̑�"),
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
		wsprintf(szText[5], TEXT("�X�N���[���o�[�̈ʒu = %03d"), nPos);
		InvalidateRect(hMain, NULL, TRUE);

		return TRUE;
	case WM_INITDIALOG:
			// �e�E�B���h�E�̃n���h���擾
			hMain = GetParent(hDlg);

			// �e�R���g���[���̃n���h�����擾
			hRadio1 = GetDlgItem(hDlg, IDC_RADIO1);
			hRadio2 = GetDlgItem(hDlg, IDC_RADIO2);
			hRadio3 = GetDlgItem(hDlg, IDC_RADIO3);
			hRadio4 = GetDlgItem(hDlg, IDC_RADIO4);
			hCheck1 = GetDlgItem(hDlg, IDC_CHECK1);
			hCombo1 = GetDlgItem(hDlg, IDC_COMBO1);
			hList1 = GetDlgItem(hDlg, IDC_LIST1);
			hEdit1 = GetDlgItem(hDlg, IDC_EDIT1);
			hScrol1 = GetDlgItem(hDlg, IDC_SCROLLBAR1);

			// �X�N���[���o�[�̏����ݒ�
			ScrollBar_SetRange(hScrol1, 0, 100, TRUE);
			ScrollBar_SetPos(hScrol1, nPos, TRUE);
			
			wsprintf(szBuf, TEXT("%03d"), nPos);
			Edit_SetText(hEdit1, szBuf);

			// ���W�I�O���[�v�̏����ݒ�
			Button_SetCheck((!nSex)? hRadio1:hRadio2, BST_CHECKED);
			Button_SetCheck((!nNation)? hRadio3:hRadio4, BST_CHECKED);

			// �`�F�b�N�{�b�N�X�̏����ݒ�
			if(n20) {
				Button_SetCheck(hCheck1, BST_CHECKED);
			} else {
				Button_SetCheck(hCheck1, BST_UNCHECKED);
			}

			// �R���{�{�b�N�X�ɕ������������
			for(n = 0; n < sizeof(szCombo)/sizeof(szCombo[0]); n++) {
				ComboBox_AddString(hCombo1, szCombo[n]);
			}
			ComboBox_SetCurSel(hCombo1, nAddress);

			// ���X�g�{�b�N�X�ɕ������������
			for(n = 0; n < sizeof(szList)/sizeof(szList[0]); n++) {
				ListBox_AddString(hList1, szList[n]);
			}
			ListBox_SetCurSel(hList1, nPet);
			
			return TRUE;
			break;
	case WM_COMMAND:
		switch(LOWORD(wParam)) {
		case IDOK:
			lstrcpy(szText[0], (!nSex)? TEXT("�j���I������܂���"):TEXT("�����I������܂���"));
			lstrcpy(szText[1], (!nNation)? TEXT("���{���I������܂���"):TEXT("���{�ȊO���I������܂���"));
			lstrcpy(szText[2], (!n20)? TEXT("20�Ζ����ł�"):TEXT("20�Έȏ�ł�"));
			wsprintf(szText[3], TEXT("%s ���I������܂���"), szCombo[nAddress]);
			wsprintf(szText[4], TEXT("%s ���I������܂���"), szList[nPet]);
			wsprintf(szText[5], TEXT("�X�N���[���o�[�̈ʒu = %03d"), nPos);

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
			lstrcpy(szText[0], (!nSex)? TEXT("�j���I������܂���"):TEXT("�����I������܂���"));
			InvalidateRect(hMain, NULL, TRUE);
			return TRUE;
		case IDC_RADIO3:
		case IDC_RADIO4:
			nNation = (BST_CHECKED != Button_GetCheck(hRadio3));
			lstrcpy(szText[1], (!nNation)? TEXT("���{���I������܂���"):TEXT("���{�ȊO���I������܂���"));
			InvalidateRect(hMain, NULL, TRUE);
			return TRUE;
		case IDC_CHECK1:
			n20 = (BST_CHECKED == Button_GetCheck(hCheck1));
			lstrcpy(szText[2], (!n20)? TEXT("20�Ζ����ł�"):TEXT("20�Έȏ�ł�"));
			InvalidateRect(hMain, NULL, TRUE);
			return TRUE;
		case IDC_COMBO1:
			id = ComboBox_GetCurSel(hCombo1);
			wsprintf(szText[3], TEXT("%s ���I������܂���"), szCombo[id]);
			InvalidateRect(hMain, NULL, TRUE);
			nAddress = id;
			return TRUE;
		case IDC_LIST1:
			id = ListBox_GetCurSel(hList1);
			wsprintf(szText[4], TEXT("%s ���I������܂���"), szList[id]);
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
			wsprintf(szText[5], TEXT("�X�N���[���o�[�̈ʒu = %03d"), nPos);
			InvalidateRect(hMain, NULL, TRUE);
			return TRUE;
		default:
			break;
		}
		return FALSE;
	}
	return FALSE;
}
