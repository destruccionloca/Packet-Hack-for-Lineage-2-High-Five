#include <stdio.h>
#include <windows.h>
#include <io.h>
#include <fcntl.h>
DWORD* arr[3];

VOID WINAPI handler()
{	
	int offset = 0;
	int b_len = 0;
	printf_s("%s -> ", (CHAR*)*(arr[1]));
	for (int i = 0; *((CHAR*)*(arr[1]) + i) != 0x00; i++) 
	{
		switch (*((CHAR*)*(arr[1]) + i))
		{
		case 'c':
			printf_s("%02X ", *(arr[2] + offset));
			offset++;
			break;
		case 'h':
			printf_s("%04X ", *(arr[2] + offset));
			offset++;
			break;
		case 'd':
			printf_s("%08X ", *(arr[2] + offset));
			offset++;
			break;
		case 'b':
			printf_s("( ");
			b_len = *(arr[2] + offset);
			offset++;
			for (int j = 0;j < b_len; j++)
				printf_s("%02hhX ", *((CHAR*)*(arr[2] + offset) + j));
			offset++;
			printf_s(") ");
			break;
		case 'S':
			printf_s("%ls ", (WCHAR*)*(arr[2] + offset));
			offset++;
			break;
		case 'Q':
			printf_s("%08X%08X ", *(arr[2] + offset + 1), *(arr[2] + offset));
			offset+=2;
			break;
		}		
	}
	printf_s("\n");
}


inline void CreateConsole()
{
	int hConHandle = 0;
	long lStdHandle = 0;
	FILE* fp = 0;

	AllocConsole();
	lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

	fp = _fdopen(hConHandle, "w");
	*stdout = *fp;
	setvbuf(stdout, NULL, _IONBF, 0);
	FILE* pCout;
	freopen_s(&pCout, "conout$", "w", stdout);
}


DWORD WINAPI thread()
{
	CreateConsole();

	HINSTANCE module = GetModuleHandleA("unc1e.dll");
	FARPROC func_Addr = GetProcAddress(module, "unc1e_init");
	if (func_Addr)
	{
		typedef void(__stdcall *proto_unc1e_init)(DWORD**, DWORD*);
		((proto_unc1e_init)func_Addr)(&arr[0], (DWORD*)handler);
		*(arr[0]) = 1;
		return 1;
	}

	return 0;
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
		CreateThread(0, 0, thread, 0, 0, 0);
	return TRUE;
}
