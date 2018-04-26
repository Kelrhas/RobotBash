#pragma once

#include <vector>

class BMPLoader
{
public:
	BMPLoader();
	~BMPLoader();

	bool Load(const char* sFileName);

	uint32_t	GetWidth() const { return m_uImageWidth; }
	uint32_t	GetHeight() const { return m_uImageHeight; }
	uint8_t*	GetData() const { return m_pData; }

protected:
	uint32_t	m_uImageWidth;
	uint32_t	m_uImageHeight;
	uint32_t	m_uImageSize;
	uint8_t*	m_pData;
};
