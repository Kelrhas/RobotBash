#pragma once

#include "ogl.h"
#define INVALID_UNIFORM_LOCATION 0xFFFFFFFF

namespace Renderer
{
	enum ShaderType
	{
		VERTEX_SHADER,
		GEOMETRY_SHADER,
		FRAGMENT_SHADER,

		SHADER_TYPE_COUNT
	};

	class GLShader;
	
	class Technique
	{
	public:
		Technique();
		~Technique();

				void		Load(std::string sFileName);
				void		Use();

				bool		LiveUpdate();

	protected:
				bool		LoadProgram(std::string sFileName);
				bool		LoadShader(std::string sFileName, GLenum uShaderType);

		virtual void		BindLocations(){}
		virtual void		GatherLocations() = 0;



		GLuint		m_uID;
		GLShader*	m_aShaders[SHADER_TYPE_COUNT];
	};

}