#include "stdafx.h"
#include "SkyboxTechnique.h"

namespace Renderer
{
	void SkyboxTechnique::GatherLocations()
	{
		glBindAttribLocation( m_uID, 0, "in_Position" );
		glBindAttribLocation( m_uID, 1, "in_Normal" );
		glBindAttribLocation( m_uID, 2, "in_TexCoord" );
		//glBindAttribLocation( m_uID, m_iMVPLocation = 4, "uMVP" );
		//glBindAttribLocation( m_uID, m_iDiffuseMapLocation = 5, "uDiffuseMap" );

		//m_iPositionLocation = glGetAttribLocation( m_uID, "in_Position" );
		//m_iUVLocation = glGetAttribLocation( m_uID, "in_TexCoord" );

		m_iMVPLocation = glGetUniformLocation( m_uID, "uMVP" );

		m_iDiffuseMapLocation = glGetUniformLocation( m_uID, "uCubeMap" );

		// check we have the needed ones
		//Assert( m_iPositionLocation != INVALID_UNIFORM_LOCATION );
		//Assert( m_iUVLocation != INVALID_UNIFORM_LOCATION );
		Assert( m_iMVPLocation != INVALID_UNIFORM_LOCATION );
		Assert( m_iDiffuseMapLocation != INVALID_UNIFORM_LOCATION );
	}

	void SkyboxTechnique::SetMVP( const glm::mat4& mMVP )
	{
		glUniformMatrix4fv( m_iMVPLocation, 1, false, &mMVP[0][0] );
		Assert_GL();
	}

	void SkyboxTechnique::SetDiffuseMap( int iTexIndex )
	{
		if( m_iDiffuseMapLocation != INVALID_UNIFORM_LOCATION )
			glUniform1i( m_iDiffuseMapLocation, iTexIndex );
		Assert_GL();
	}
}