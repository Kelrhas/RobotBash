#include "StdAfx.h"
#include "GBuffer.h"

#include "RENDERER/GLRenderer.h"

namespace Renderer
{
	GBuffer::GBuffer()
		: m_uFBOId( -1 )
		, m_uDepthTexture( -1 )
	{
		for( int i = 0; i < renderTarget_Count; ++i )
			m_aTexturesID[i] = -1;
	}

	GBuffer::~GBuffer()
	{

	}

	bool GBuffer::Init( uint32_t uWindowWidth, uint32_t uWindowHeight )
	{
		// create everything
		glGenFramebuffers( 1, &m_uFBOId );
		glGenBuffers( 2, m_uVBOId );
		glGenVertexArrays( 1, &m_uVAO );
		glGenTextures( renderTarget_Count, m_aTexturesID );
		glGenTextures( 1, &m_uDepthTexture );

		// render targets
		glBindFramebuffer( GL_FRAMEBUFFER, m_uFBOId );
		for( unsigned int i = 0; i < renderTarget_Count; i++ )
		{
			glBindTexture( GL_TEXTURE_2D, m_aTexturesID[i] );
			glTexImage2D( GL_TEXTURE_2D, 0, aRenderTargetInfos[i].uInternalFormat, uWindowWidth, uWindowHeight, 0, aRenderTargetInfos[i].uFormat, aRenderTargetInfos[i].uType, nullptr );
			Assert_GL();
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
			Assert_GL();
			glFramebufferTexture2D( GL_FRAMEBUFFER, aRenderTargetInfos[i].eAttachment, GL_TEXTURE_2D, m_aTexturesID[i], 0 );
			Assert_GL();
			glObjectLabel( GL_TEXTURE, m_aTexturesID[i], -1, aRenderTargetInfos[i].sName );
			Assert_GL();
		}

		// depth
		glBindTexture( GL_TEXTURE_2D, m_uDepthTexture );
		glTexImage2D( GL_TEXTURE_2D, 0, aRenderTargetInfos[renderTarget_Depth].uInternalFormat, uWindowWidth, uWindowHeight, 0, aRenderTargetInfos[renderTarget_Depth].uFormat, GL_FLOAT, nullptr );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		glFramebufferTexture2D( GL_FRAMEBUFFER, aRenderTargetInfos[renderTarget_Depth].eAttachment, GL_TEXTURE_2D, m_uDepthTexture, 0 );
		glObjectLabel( GL_TEXTURE, m_uDepthTexture, -1, aRenderTargetInfos[renderTarget_Depth].sName );
		Assert_GL();
		
		GLenum eStatus = glCheckFramebufferStatus( GL_FRAMEBUFFER );

		if( eStatus != GL_FRAMEBUFFER_COMPLETE )
		{
			LOG_GL( "ERROR: framebuffer not created" );
			return false;
		}

		glBindFramebuffer( GL_FRAMEBUFFER, 0 );
		Assert_GL();

		GLfloat verts[] = {
			-1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
			-1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f
		};
		GLfloat uvs[] = {
			0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
		};

		glBindVertexArray( m_uVAO );

		glEnableVertexAttribArray( 0 );  // Vertex position
		glBindBuffer( GL_ARRAY_BUFFER, m_uVBOId[0] );
		glVertexAttribPointer( (GLuint) 0, 3, GL_FLOAT, GL_FALSE, 0, nullptr );
		glBufferData( GL_ARRAY_BUFFER, 6 * 3 * sizeof( float ), verts, GL_STATIC_DRAW );
		Assert_GL();

		glEnableVertexAttribArray( 1 ); // Texture coordinates
		glBindBuffer( GL_ARRAY_BUFFER, m_uVBOId[1] );
		glVertexAttribPointer( (GLuint) 1, 2, GL_FLOAT, GL_FALSE, 0, nullptr );
		glBufferData( GL_ARRAY_BUFFER, 6 * 2 * sizeof( float ), uvs, GL_STATIC_DRAW );
		Assert_GL();

		glBindVertexArray( 0 );

		return true;
	}

	void GBuffer::BindForWriting()
	{
		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, m_uFBOId );
		Assert_GL();
	}

	void GBuffer::BindForReading()
	{
		glBindFramebuffer( GL_READ_FRAMEBUFFER, m_uFBOId );
		Assert_GL();
	}

	void GBuffer::SetReadBuffer( RenderTarget eTextureType )
	{
		glReadBuffer( GL_COLOR_ATTACHMENT0 + eTextureType );
		Assert_GL();
	}

	void GBuffer::BindTextures()
	{
		BindTexture( renderTarget_WorldPos, renderTarget_WorldPos );
		BindTexture( renderTarget_Albedo, renderTarget_Albedo );
		BindTexture( renderTarget_Normal, renderTarget_Normal );
		BindTexture( renderTarget_TexCoord, renderTarget_TexCoord );
		BindTexture( renderTarget_Picking, renderTarget_Picking );
	}

	void GBuffer::BindTexture( RenderTarget eRenderTarget, uint8_t id )
	{
		Assert( eRenderTarget != renderTarget_Count && eRenderTarget != renderTarget_All );
		Assert( id < 32 ); // only 32 texture channels

		glActiveTexture( GL_TEXTURE0 + id );
		if( eRenderTarget == renderTarget_Depth )
		{
			glBindTexture( GL_TEXTURE_2D, m_uDepthTexture );
		}
		else
		{
			glBindTexture( GL_TEXTURE_2D, m_aTexturesID[eRenderTarget] );
		}
	}

	void GBuffer::DrawQuad()
	{
		glBindVertexArray( m_uVAO );
		
		glDrawArrays( GL_TRIANGLES, 0, 6 );
		
		glBindVertexArray( 0 );
	}

}