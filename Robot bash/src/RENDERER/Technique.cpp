#include "StdAfx.h"
#include "Technique.h"

#include "GLShaderProgram.h"

namespace Renderer
{

	Technique::Technique()
	{
		m_uID = -1;
		for (int i = 0; i < SHADER_TYPE_COUNT; ++i)
		{
			m_aShaders[i] = nullptr;
		}
	}

	Technique::~Technique()
	{

	}

	void Technique::Load(std::string sFileName)
	{
		LoadProgram(sFileName);
		GatherLocations();
	}

	void Technique::Use()
	{
		glUseProgram(m_uID);
	}

	bool Technique::LiveUpdate()
	{
		bool bHasChanged = false;
		for (int i = 0; i < SHADER_TYPE_COUNT; ++i)
		{
			if (m_aShaders[i] != nullptr)
			{
				bHasChanged |= m_aShaders[i]->LiveUpdate();
			}
		}
		if (bHasChanged)
		{
			BindLocations(); // need to be called before the link
			glLinkProgram(m_uID);

			int param;
			glGetProgramiv(m_uID, GL_LINK_STATUS, &param);
			Assert_return_result(param == GL_TRUE);

			GatherLocations();
			Assert_GL();
		}

		return bHasChanged;
	}

	bool Technique::LoadProgram(std::string sFileName)
	{
		FILE* pFile = nullptr;
		fopen_s(&pFile, sFileName.c_str(), "r");
		Assert(pFile != nullptr);

		char sVertexShaderName[128];
		char sGeometryShaderName[128];
		char sFragmentShaderName[128];
		fscanf(pFile, "%s", sVertexShaderName);
		//fscanf(pFile, "%s", sGeometryShaderName);
		fscanf(pFile, "%s", sFragmentShaderName);

		m_uID = glCreateProgram();
		Assert_GL();

		Assert(LoadShader(sVertexShaderName, GL_VERTEX_SHADER));
		//if (sGeometryShaderName != "none")
		//	ASSERT(LoadShader(sGeometryShaderName, GL_GEOMETRY_SHADER));
		Assert(LoadShader(sFragmentShaderName, GL_FRAGMENT_SHADER));

		BindLocations(); // need to be called before the link
		Assert_GL();

		glLinkProgram(m_uID);
		Assert_GL();

		glObjectLabel(GL_PROGRAM, m_uID, -1, sFileName.c_str());
		Assert_GL();

		int param;
		glGetProgramiv(m_uID, GL_LINK_STATUS, &param);
		return (param == GL_TRUE);
	}

	bool Technique::LoadShader(std::string sFileName, GLenum uShaderType)
	{
		int index = -1;
		switch (uShaderType)
		{
		case GL_VERTEX_SHADER:		index = ShaderType::VERTEX_SHADER;		break;
		case GL_GEOMETRY_SHADER:	index = ShaderType::GEOMETRY_SHADER;	break;
		case GL_FRAGMENT_SHADER:	index = ShaderType::FRAGMENT_SHADER;	break;
		default: Assert(false);
		}

		Assert(m_aShaders[index] == nullptr);

		m_aShaders[index] = new GLShader(uShaderType);
		Assert_return_result(m_aShaders[index]->Load(sFileName));
		Assert_return_result(m_aShaders[index]->Compile());

		glAttachShader(m_uID, m_aShaders[index]->GetID());
		Assert_GL();

		glObjectLabel(GL_SHADER, m_aShaders[index]->GetID(), -1, sFileName.c_str());

		return true;
	}
}