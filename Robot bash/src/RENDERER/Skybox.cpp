#include "StdAfx.h"
#include "Skybox.h"

#include "IO/BMPLoader.h"
#include "GLVAO.h"

namespace Renderer
{
	static const uint32_t skyboxIndices[] = {
		// left
		0, 1, 2, 0, 2, 3,
		// right
		4, 6, 5, 4, 7, 6,
		//down
		8, 10, 9, 8, 11, 10,
		// up
		12, 14, 13, 12, 15, 14,
		//front
		16, 18, 17, 16, 19, 18,
		//back
		20, 22, 21, 20, 23, 22
	};

	const GLfloat fSize = 5.f;
	static const GLfloat skyboxVertices[] = {
		//-fSize, -fSize, -fSize,
		//-fSize, -fSize, fSize,
		//-fSize, fSize, fSize,
		//-fSize, fSize, -fSize,
		//fSize, -fSize, -fSize,
		//fSize, fSize, -fSize,
		//fSize, fSize, fSize,
		//fSize, -fSize, fSize,

		//-fSize,-fSize,-fSize,
		//-fSize,-fSize,fSize,
		//-fSize,fSize,fSize,
		//
		//-fSize,-fSize,-fSize,
		//-fSize,fSize,fSize,
		//-fSize,fSize,-fSize,





		-fSize,-fSize,-fSize, // 0
		-fSize,-fSize,fSize,
		-fSize,fSize,fSize,
		-fSize,fSize,-fSize,

		-fSize, -fSize, fSize,
		fSize,-fSize,fSize,
		fSize,fSize,fSize,
		-fSize,fSize,fSize,

		-fSize,-fSize,-fSize,
		-fSize,fSize,-fSize, // 5
		fSize,fSize,-fSize,
		fSize,-fSize,-fSize,

		-fSize,fSize,-fSize,
		-fSize,fSize,fSize,
		fSize,fSize,fSize, // 10
		fSize,fSize,-fSize,

		-fSize,-fSize,-fSize,
		fSize,-fSize,-fSize,
		fSize,-fSize,fSize,
		-fSize,-fSize,fSize, // 15

		fSize,-fSize,-fSize,
		fSize,fSize,-fSize,
		fSize,fSize,fSize,
		fSize,-fSize,fSize,

		//-fSize,-fSize,-fSize, // 20
		//-fSize,-fSize,fSize,
		//-fSize,fSize,fSize,
		//-fSize,fSize,-fSize
	};

	static const GLfloat skyboxUV[] = {
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
	};

	Skybox::Skybox()
		: m_iTexture( -1 )
		, m_uVAO( -1 )
	{

	}

	Skybox::~Skybox()
	{
		glDeleteTextures( 1, &m_iTexture );
	}

	void Skybox::Load(std::string folder)
	{
		std::string files[6] =
		{
			"right.bmp",
			"left.bmp",
			"up.bmp",
			"down.bmp",
			"back.bmp",
			"front.bmp",
		};

		glEnable( GL_TEXTURE_CUBE_MAP );
		Assert_GL();
		glGenTextures( 1, &m_iTexture );
		Assert_GL();
		//glBindTexture( GL_TEXTURE_CUBE_MAP, m_iTexture );
		//Assert_GL();
		glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
		Assert_GL();

		//glGenVertexArrays( 1, &m_uVAO );
		//glBindVertexArray( m_uVAO );
		//glGenBuffers( 1, &m_uVBO );
		//glBindBuffer( GL_ARRAY_BUFFER, m_uVBO );
		//Assert_GL();
		//glEnableVertexAttribArray( 0 );
		//glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, nullptr );
		//glBufferData( GL_ARRAY_BUFFER, sizeof(uint32_t) * 8, skyboxVertices, GL_STATIC_DRAW );
		//Assert_GL();
		m_pVAO = new GLVAO();
		m_pVAO->SetData( Renderer::VBO_BUFFER_INDEX, (void*) &skyboxIndices[0], 36 );
		m_pVAO->SetData( Renderer::VBO_BUFFER_POSITION, (void*)&skyboxVertices[0], 72 );
		
		BMPLoader texLoader;
		for( int i = 0; i < 6; ++i )
		{
			texLoader.Load( (folder + files[i]).c_str() );
			glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, texLoader.GetWidth(), texLoader.GetHeight(), 0, GL_BGR, GL_UNSIGNED_BYTE, texLoader.GetData() );
			Assert_GL();
		}

		m_oTechnique.Load( "Data/shaders/skybox.shader" );
		m_oTechnique.GatherLocations();

		// load vao
	}

	void Skybox::Draw( glm::mat4 mMVP )
	{
		m_oTechnique.LiveUpdate();

		m_oTechnique.Use();
		m_oTechnique.SetMVP( mMVP );

		glBindVertexArray( m_pVAO->GetID() );
		//glBindVertexArray( m_uVAO );
		glBindTexture( GL_TEXTURE_CUBE_MAP, m_iTexture );
		m_oTechnique.SetDiffuseMap( 0 );

		glDisable( GL_CULL_FACE );

		
		glDrawElements( GL_TRIANGLES, (GLsizei) 36, GL_UNSIGNED_INT, 0 );
		Assert_GL();
	}
}