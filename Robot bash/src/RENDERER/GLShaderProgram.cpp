#include "StdAfx.h"
#include "GLShaderProgram.h"

#include <cerrno>

namespace Renderer
{
	/************************************************************************/
	/* GLShader                                                            */
	/************************************************************************/
	GLShader::GLShader(GLenum uShaderType)
	{
		m_sFileName = "";
		m_uType = uShaderType;
		m_uID = glCreateShader(uShaderType);
		m_pCode = nullptr;
		m_bNeedReloading = false;
	}

	GLShader::~GLShader()
	{
		if (m_pCode)
			delete[] m_pCode;

		glDeleteShader(m_uID);
	}

	bool GLShader::Load(std::string sFileName)
	{
		FILE *pFile = nullptr;
		char *content = nullptr;

		size_t count = 0;

		if (m_sFileName == "") // first load
		{
			m_sFileName = sFileName;
			m_uLastTimestamp = GetTimestamp();
		}

		if (sFileName != "" && sFileName[0] != '\0')
		{
			fopen_s(&pFile, sFileName.c_str(), "rt");
			if (pFile != nullptr)
			{
				fseek(pFile, 0, SEEK_END);
				count = ftell(pFile);
				rewind(pFile);
				if (count > 0)
				{
					content = new char[count + 1];
					count = fread(content, sizeof(char), count, pFile);
					content[count] = '\0';
				}
				fclose(pFile);
			}
			else if(errno == EACCES)
			{
				LOG_ERROR("Could not open file %s, access error", sFileName.c_str());
				return false;
			}
			else
			{
				LOG_ERROR("Could not open file %s, %i");
				return false;
			}
		}
		else
		{
			LOG_ERROR("Not a valid filename %s", sFileName.c_str());
			return false;
		}

		m_pCode = content;
		return true;
	}

	bool GLShader::Compile()
	{
		if (m_pCode == nullptr)
			return false;

		const char* pCode = m_pCode;

		glShaderSource(m_uID, 1, &pCode, nullptr);
		glCompileShader(m_uID);

		// check compilation validation
		int param;
		glGetShaderiv(m_uID, GL_COMPILE_STATUS, &param);
		if (param != GL_TRUE)
		{
			char buffer[4096];
			GLsizei uSize;
			glGetShaderInfoLog(m_uID, 4096, &uSize, buffer);
			LOG_GL(buffer);
			return false;
		}

		return true;
	}


	bool GLShader::LiveUpdate()
	{
		if(m_bNeedReloading)
		{
			m_uLastTimestamp = GetTimestamp();
			Assert_return_result(Load(m_sFileName));
			Assert_return_result(Compile());
			m_bNeedReloading = false;

			return true;
		}

		if (GetTimestamp() > m_uLastTimestamp)
			m_bNeedReloading = true;

		return false;
	}

	uint64_t GLShader::GetTimestamp()
	{
		Assert(m_sFileName != "");

		struct stat fileStat;
		if (stat(m_sFileName.c_str(), &fileStat) == -1)
		{
			LOG_ERROR("IO error %s", m_sFileName.c_str());
			return 0;
		}

		return fileStat.st_mtime;
	}
}