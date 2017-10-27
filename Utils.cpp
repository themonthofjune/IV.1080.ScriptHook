#include "Utils.h"

const char* va(const char* string, ...)
{
	static thread_local int currentBuffer;
	static thread_local char* buffer;

	if (!buffer)
	{
		buffer = new char[BUFFER_COUNT * BUFFER_LENGTH];
	}

	int thisBuffer = currentBuffer;

	va_list ap;
	va_start(ap, string);
	int length = vsnprintf(&buffer[thisBuffer * BUFFER_LENGTH], BUFFER_LENGTH, string, ap);
	va_end(ap);

	if (length >= BUFFER_LENGTH)
	{
		exit(1);
	}

	buffer[(thisBuffer * BUFFER_LENGTH) + BUFFER_LENGTH - 1] = '\0';

	currentBuffer = (currentBuffer + 1) % BUFFER_COUNT;

	return &buffer[thisBuffer * BUFFER_LENGTH];
}


const wchar_t* va(const wchar_t* string, ...)
{
	static thread_local int currentBuffer;
	static thread_local wchar_t* buffer;

	if (!buffer)
	{
		buffer = new wchar_t[BUFFER_COUNT * BUFFER_LENGTH];
	}

	int thisBuffer = currentBuffer;

	va_list ap;
	va_start(ap, string);
	int length = vswprintf(&buffer[thisBuffer * BUFFER_LENGTH], BUFFER_LENGTH, string, ap);
	va_end(ap);

	if (length >= BUFFER_LENGTH)
	{
		exit(1);
	}

	buffer[(thisBuffer * BUFFER_LENGTH) + BUFFER_LENGTH - 1] = '\0';

	currentBuffer = (currentBuffer + 1) % BUFFER_COUNT;

	return &buffer[thisBuffer * BUFFER_LENGTH];
}