#ifndef FILEIO_H
#define FILEIO_H

#include <windows.h>
#include <stdint.h>
#include <stdio.h>

//#include "../win32main.h"

/*
typedef struct Asset
{
	char* filepath;
}Asset;
*/

class File
{
public:
	bool OpenToRead(char* filepath);
	void* Read(uint64_t sizeToRead = 0);
	bool OpenToWrite(char* filepath);
	bool Write(void* buffer, size_t sizeToWrite);
	void Close();
	
	
private:
	void* fileHandle = INVALID_HANDLE_VALUE;
	uint64_t m_size = 0;
	void* buffer = nullptr;
};

#endif