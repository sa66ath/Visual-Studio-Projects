/* cset.c */

#include <windows.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	TCHAR	szBuf[] = TEXT("abc");

#if UNICODE
	printf("UNICODE����`����Ă��܂�\n");
#else
	printf("MBCS�ł�\n");
#endif

	printf("szBuf��%d����\nszBuf��%d�o�C�g\n", lstrlen(szBuf), sizeof(szBuf));

	printf("�ŏ��̕�����[%c]�@���̕�����[%c] �Ō�̕�����[%c]\n",
			szBuf[0], szBuf[1], szBuf[2]);

	return 0;
}
