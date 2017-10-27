#pragma once

#include <windows.h>
#include <string>

#define BUFFER_COUNT 8
#define BUFFER_LENGTH 32768

const char* va(const char* string, ...);
const wchar_t* va(const wchar_t* string, ...);