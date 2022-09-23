#pragma once

#include <new>

void* __cdecl operator new[](size_t size, const char* pName,
	int flags, unsigned debugFlags, const char* file, int line);
void* __cdecl operator new[](size_t size, size_t alignment,
	size_t alignmentOffset, const char* pName, int flags,
	unsigned debugFlags, const char* file, int line);
