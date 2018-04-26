#include "StdAfx.h"
#include "FileIO.h"


bool File::OpenToRead(char* filepath)
{
	if(fileHandle != INVALID_HANDLE_VALUE)
	{
		return false;
	}
	
	fileHandle = CreateFileA(filepath, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);

	if(fileHandle != INVALID_HANDLE_VALUE)
	{
		LARGE_INTEGER fileSize;
		if(GetFileSizeEx(fileHandle, &fileSize))
		{
			Assert(fileSize.QuadPart < 0xffffffff);
			m_size = fileSize.QuadPart;
			return true;
		}
		else
		{
			// mauvaise taille ?
			// GetLastError 
		}
	}
	else
	{
		LOG_ERROR("Le fichier %s n'existe pas\n", filepath);
	}
	
	return false;
}

void* File::Read(uint64_t sizeToRead)
{
	if(fileHandle == INVALID_HANDLE_VALUE)
	{
		return nullptr;
	}
	
	if(sizeToRead == 0)
	{
		sizeToRead = m_size;
	}
	
	assert(sizeToRead <= m_size);
	// TODO changer le VirtualAlloc !!! malloc, HeapAlloc ...
	buffer = VirtualAlloc(0, sizeToRead, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if(buffer)
	{
		DWORD bytesToRead;
		if(ReadFile(fileHandle, buffer, (uint32_t)sizeToRead, &bytesToRead, 0))
		{
			if(bytesToRead == (uint32_t)sizeToRead)
			{
				return buffer;
			}
			else
			{
				// TODO pas bonne taille lue, fichier modifié ?
				this->Close();
			}
		}
		else
		{
			this->Close();
		}
	}
	else
	{
		// plus assez de memoire dispo ??
		this->Close();
	}
	
	return nullptr;
}

bool File::OpenToWrite(char* filepath)
{
	if(fileHandle)
	{
		return false;
	}
	
	
	return false;
}

void File::Close()
{
	if(buffer)
	{
		VirtualFree(buffer, 0, MEM_RELEASE);
		buffer = nullptr;
	}
	m_size = 0;
	CloseHandle(fileHandle);
}



/*
bool FileWriteFull(char* filepath, uint32_t filesize, void* memory)
{
	bool result = false;
	HANDLE fileHandle = CreateFileA(filepath, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
	if(fileHandle != INVALID_HANDLE_VALUE)
	{
		DWORD bytesWritten;
		if(WriteFile(fileHandle, memory, filesize, &bytesWritten, 0))
		{
			result = (bytesWritten == filesize);
		}
		else
		{
		
		}
		CloseHandle(fileHandle);
	}
	else
	{
		// TODO
	}
	return result;
}
*/