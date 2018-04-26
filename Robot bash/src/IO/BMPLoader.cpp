#include "StdAfx.h"
#include "BMPLoader.h"

BMPLoader::BMPLoader()
	: m_uImageWidth(0)
	, m_uImageHeight(0)
	, m_uImageSize(0)
	, m_pData(nullptr)
{
}

BMPLoader::~BMPLoader()
{
	delete[] m_pData;
}

bool BMPLoader::Load(const char* sFileName)
{
	FILE* pFile = nullptr;
	fopen_s(&pFile, sFileName, "rb");

	Assert_return_result(pFile != nullptr);

	unsigned char pHeader[54];
	size_t iSizeRead = fread(pHeader, 1, 54, pFile);
	Assert(iSizeRead == 54);

	m_uImageWidth = *(int*)&(pHeader[0x12]);
	m_uImageHeight = *(int*)&(pHeader[0x16]);
	m_uImageSize = *(int*)&(pHeader[0x22]);
	uint32_t dataPos = *(int*)&(pHeader[0x0A]);

	if (m_uImageSize == 0)
		m_uImageSize = m_uImageWidth*m_uImageHeight * 3; // RGB 24bits
	if (dataPos == 0)
		dataPos = 54; // default position

	m_pData = new uint8_t[m_uImageSize];

	iSizeRead = fread(m_pData, sizeof(uint8_t), m_uImageSize, pFile);

	if (iSizeRead != m_uImageSize)
		LOG_ERROR("IO error: %d", ferror(pFile));

	fclose(pFile);

	return true;
}
