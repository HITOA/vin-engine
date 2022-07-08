#pragma once

namespace Vin::Msgbox {

	void Show(const char* title, const char* msg);
	void ShowErr(const char* title, const char* msg);

#ifdef _WIN32
#include <Windows.h>
	void Show(const char* title, const char* msg) {
		MessageBoxA(NULL, msg, title, MB_OK);
	}

	void ShowErr(const char* title, const char* msg) {
		MessageBoxA(NULL, msg, title, MB_OK | MB_ICONERROR);
	}
#else
#include <stdio.h>
	void Show(const char* title, const char* msg) {
		printf("%s : %s", title, msg);
	}

	void ShowErr(const char* title, const char* msg) {
		printf("/!\\ %s : %s", title, msg);
	}
#endif

}
