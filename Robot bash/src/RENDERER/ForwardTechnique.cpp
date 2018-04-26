#include "StdAfx.h"
#include "ForwardTechnique.h"

namespace Renderer
{
	void ForwardTechnique::GatherLocations()
	{
		m_iPositionLocation = glGetAttribLocation( m_uID, "in_Position" );
		m_iNormalLocation = glGetAttribLocation( m_uID, "in_Normal" );
		m_iUVLocation = glGetAttribLocation( m_uID, "in_TexCoord" );

		m_iMVPLocation = glGetUniformLocation( m_uID, "uMVP" );

		m_iDiffuseMapLocation = glGetUniformLocation( m_uID, "uDiffuseMap" );
		m_iNormalMapLocation = glGetUniformLocation( m_uID, "uNormalMap" );

		// check we have the needed ones
		Assert( m_iPositionLocation != INVALID_UNIFORM_LOCATION );
		Assert( m_iMVPLocation != INVALID_UNIFORM_LOCATION );
		Assert( m_iDiffuseMapLocation != INVALID_UNIFORM_LOCATION );
	}

	void ForwardTechnique::SetMVP( const glm::mat4& mMVP )
	{
		glUniformMatrix4fv( m_iMVPLocation, 1, false, &mMVP[0][0] );
		Assert_GL();
	}

	void ForwardTechnique::SetDiffuseMap( int iTexIndex )
	{
		if( m_iDiffuseMapLocation != INVALID_UNIFORM_LOCATION )
			glUniform1i( m_iDiffuseMapLocation, iTexIndex );
		Assert_GL();
	}

	void ForwardTechnique::SetNormalMap( int iTexIndex )
	{
		if( m_iNormalMapLocation != INVALID_UNIFORM_LOCATION )
			glUniform1i( m_iNormalMapLocation, iTexIndex );
		Assert_GL();
	}
}
