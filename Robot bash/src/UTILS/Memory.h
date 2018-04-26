#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include <windows.h>
#include <new>

#if _DEBUG
#define ALLOCATE_MAIN(type) new(Memory::GetInstance()->AllocateMain(sizeof(type))) type()
#else
#define ALLOCATE_MAIN(type) new type()
#endif

#if _DEBUG
#define ALLOCATE_ARRAY_MAIN(type, numElm) new(Memory::GetInstance()->AllocateMain((size_t)(sizeof(type) * numElm))) type[(int)numElm]
#else
#define ALLOCATE_ARRAY_MAIN(type, numElm) new type[(int)numElm]
#endif


#if _DEBUG
#define ALLOCATE_DISPLAY(type) new(Memory::GetInstance()->AllocateDisplay(sizeof(type))) type()
#else
#define ALLOCATE_DISPLAY(type) new type()
#endif

#if _DEBUG
#define ALLOCATE_ARRAY_DISPLAY(type, numElm) new(Memory::GetInstance()->AllocateDisplay((size_t)(sizeof(type) * numElm))) type[(int)numElm]
#else
#define ALLOCATE_ARRAY_DISPLAY(type, numElm) new type[(int)numElm]
#endif

#define KILOBYTE(size) (size * 1024)
#define MEGABYTE(size) (size * 1024 * 1024)
#define GIGABYTE(size) (size * 1024 * 1024 * 1024)


#define IS_BIG_ENDIAN (*(uint16_t *)"\0\xff" < 0x100)

typedef struct MemoryPool
{
	size_t sizeTotal = 0;
	size_t sizeUsed = 0;
	void* memory = nullptr;
}MemoryPool;


class Memory
{
public:
	bool CreateMainMemory(size_t size);
	bool CreateDisplayMemory(size_t size);
	inline void* AllocateMain(size_t size){ return AllocateOnPool(&m_mainMemoryPool, size); }
	inline void* AllocateDisplay(size_t size){ return AllocateOnPool(&m_displayMemoryPool, size); }


	static Memory* GetInstance();
	static void SwapBytes(uint32_t* i);

private:
	Memory(){}
	void* AllocateOnPool(MemoryPool* pool, size_t size);
	static Memory* ms_instance;
	static MemoryPool m_mainMemoryPool;
	static MemoryPool m_displayMemoryPool;

};

#endif