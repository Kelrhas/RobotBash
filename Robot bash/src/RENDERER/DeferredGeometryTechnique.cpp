#include "StdAfx.h"
#include "DeferredGeometryTechnique.h"

namespace Renderer
{

	void DeferredGeometryTechnique::BindLocations()
	{
		glBindFragDataLocation( m_uID, renderTarget_WorldPos, "WorldPosOut" );
		Assert_GL();
		glBindFragDataLocation( m_uID, renderTarget_Albedo, "AlbedoOut" );
		Assert_GL();
		glBindFragDataLocation( m_uID, renderTarget_Normal, "NormalOut" );
		Assert_GL();
		glBindFragDataLocation( m_uID, renderTarget_TexCoord, "TexCoordOut" );
		Assert_GL();
	}

	void DeferredGeometryTechnique::GatherLocations()
	{
		m_iPositionLocation			= glGetAttribLocation( m_uID, "in_Position" );
		m_iNormalLocation			= glGetAttribLocation( m_uID, "in_Normal" );
		m_iUVLocation				= glGetAttribLocation( m_uID, "in_TexCoord" );

		m_iModelMatrixLocation		= glGetUniformLocation( m_uID, "uModelMatrix" );
		m_iMVPLocation				= glGetUniformLocation( m_uID, "uMVPMatrix" );
		m_iNormalMatrixLocation		= glGetUniformLocation( m_uID, "uNormalMtx" );

		m_iDiffuseMapLocation		= glGetUniformLocation( m_uID, "uDiffuseMap" );
		m_iNormalMapLocation		= glGetUniformLocation( m_uID, "uNormalMap" );

		// check we have the needed ones
		Assert( m_iPositionLocation			!= INVALID_UNIFORM_LOCATION );
		Assert( m_iModelMatrixLocation		!= INVALID_UNIFORM_LOCATION );
		Assert( m_iMVPLocation				!= INVALID_UNIFORM_LOCATION );
		Assert( m_iNormalMatrixLocation		!= INVALID_UNIFORM_LOCATION );
	}

	void DeferredGeometryTechnique::SetModelMatrix( const glm::mat4& mMV )
	{
		glUniformMatrix4fv( m_iModelMatrixLocation, 1, false, &mMV[0][0] );
		Assert_GL();
	}

	void DeferredGeometryTechnique::SetMVP( const glm::mat4& mMVP )
	{
		glUniformMatrix4fv( m_iMVPLocation, 1, false, &mMVP[0][0] );
		Assert_GL();
	}

	void DeferredGeometryTechnique::SetNormalMatrix( const glm::mat4& mNormalMtx )
	{
		glUniformMatrix4fv( m_iNormalMatrixLocation, 1, false, &mNormalMtx[0][0] );
		Assert_GL();
	}

	void DeferredGeometryTechnique::SetDiffuseMap( int iTexIndex )
	{
		glUniform1i( m_iDiffuseMapLocation, iTexIndex );
		Assert_GL();
	}

	void DeferredGeometryTechnique::SetNormalMap( int iTexIndex )
	{
		if( m_iDiffuseMapLocation != INVALID_UNIFORM_LOCATION )
		{
			glUniform1i( m_iDiffuseMapLocation, iTexIndex );
			Assert_GL();
		}
	}

}