/* cset.c */

#include <windows.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	TCHAR	szBuf[] = TEXT("abc");

#if UNICODE
	printf("UNICODEが定義されています\n");
#else
	printf("MBCSです\n");
#endif

	printf("szBufは%d文字\nszBufは%dバイト\n", lstrlen(szBuf), sizeof(szBuf));

	printf("最初の文字は[%c]　次の文字は[%c] 最後の文字は[%c]\n",
			szBuf[0], szBuf[1], szBuf[2]);

	return 0;
}
