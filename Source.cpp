#include <windows.h>
#include <iostream>
#include <io.h>
#include <fcntl.h>
DWORD* arr[3];

VOID WINAPI handler()
{	
	int offset = 0;
	printf_s("%s -> ", (CHAR*)*(arr[1]));
	for (int i = 0; *(((CHAR*)*(arr[1]))+i) != 0x00; i++) 
	{
		if (*((CHAR*)*(arr[1]) + i) != 'b' && *((CHAR*)*(arr[1]) + i) != 'Q' && *((CHAR*)*(arr[1]) + i) != 'S')
		{
			printf_s("%X ", *(arr[2] + offset));
			offset++;
		}
		else if (*((CHAR*)*(arr[1]) + i) == 'S')
		{
			printf_s("%ls ", (WCHAR*)*(arr[2] + offset));
			offset++;

		}
		else if (*((CHAR*)*(arr[1]) + i) == 'Q')
		{
			printf_s("%X ", *(arr[2] + offset));
			offset++;
			printf_s("%X ", *(arr[2] + offset));
			offset++;
		}
		else if (*((CHAR*)*(arr[1]) + i) == 'b')
		{
			int len = *(arr[2] + offset);
			offset++;
			for (int j = 0;j < len; j++)
				printf_s("(%X) ", *(((CHAR*)*(arr[2] + offset))+j));
			offset++;
		}

			
	}
	printf_s("\n");
}


void CreateConsole()
{
	int hConHandle = 0;
	long lStdHandle = 0;
	FILE* fp = 0;

	AllocConsole();
	setlocale(LC_ALL, "RUS");
	lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

	fp = _fdopen(hConHandle, "w");
	*stdout = *fp;
	setvbuf(stdout, NULL, _IONBF, 0);
	FILE* pCout;
	freopen_s(&pCout, "conout$", "w", stdout);
}


DWORD WINAPI thread(LPVOID)
{
	CreateConsole();

	LPWSTR name = L"unc1e.dll";
	HINSTANCE le_module = GetModuleHandle(name);
	if (le_module == NULL) { std::cout << "\n dont have library!"; }

	LPCSTR func_name = "unc1e_init";
	FARPROC adresse_la_func = GetProcAddress(le_module, func_name);
	if (adresse_la_func != 0)
	{
		typedef void(__stdcall *proto)(DWORD**, DWORD*);
		((proto)adresse_la_func)(&arr[0], (DWORD*)handler);
	}
	*(arr[0]) = 1; 
	return TRUE;
}



BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
		CreateThread(0, 0, thread, 0, 0, 0);
	return TRUE;
}