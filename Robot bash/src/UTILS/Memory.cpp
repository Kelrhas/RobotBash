#include "stdafx.h"
#include "Memory.h"

#include "Debug.h"

Memory* Memory::ms_instance = nullptr;
MemoryPool Memory::m_mainMemoryPool;
MemoryPool Memory::m_displayMemoryPool;


Memory* Memory::GetInstance()
{
	if (ms_instance == nullptr)
	{
		ms_instance = new Memory(); // TODO change the location
	}
	return ms_instance;
}

bool Memory::CreateMainMemory(size_t size)
{
	if (m_mainMemoryPool.memory == nullptr)
	{
		m_mainMemoryPool.sizeTotal = size;
#if _DEBUG
		void* baseAddress = (void*)((uint64_t)2 * 1024 * 1024 * 1024 * 1024);
		m_mainMemoryPool.memory = VirtualAlloc(baseAddress, m_mainMemoryPool.sizeTotal, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
#else
		m_mainMemoryPool.memory = VirtualAlloc(0, m_mainMemoryPool.sizeTotal, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
#endif
		if (m_mainMemoryPool.memory != nullptr)
			return true;
	}

	return false;
}
bool Memory::CreateDisplayMemory(size_t size)
{
	if (m_displayMemoryPool.memory == nullptr)
	{
		m_displayMemoryPool.sizeTotal = size;
#if _DEBUG
		void* baseAddress = (void*)((uint64_t)6 * 1024 * 1024 * 1024 * 1024);
		m_displayMemoryPool.memory = VirtualAlloc(baseAddress, m_displayMemoryPool.sizeTotal, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
#else
		m_displayMemoryPool.memory = VirtualAlloc(0, m_displayMemoryPool.sizeTotal, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
#endif
		if (m_displayMemoryPool.memory != nullptr)
			return true;
	}

	return false;
}

void* Memory::AllocateOnPool(MemoryPool* pool, size_t size)
{
	assert(pool->sizeUsed + size <= pool->sizeTotal);
	void* block = (void*)((uint8_t*)pool->memory + pool->sizeUsed);
	pool->sizeUsed += size;
	return block;
}

void Memory::SwapBytes(uint32_t* i)
{
	uint32_t a = *i;
	uint8_t *dst = (uint8_t *)i;
	uint8_t *src = (uint8_t *)&a;

	dst[0] = src[3];
	dst[1] = src[2];
	dst[2] = src[1];
	dst[3] = src[0];
}