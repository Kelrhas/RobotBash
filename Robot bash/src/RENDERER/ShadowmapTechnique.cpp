#include "StdAfx.h"
#include "ShadowmapTechnique.h"

namespace Renderer
{

	void ShadowmapTechnique::BindLocations()
	{
		glBindFragDataLocation( m_uID, renderTarget_WorldPos, "WorldPosOut" );
		Assert_GL();
	}

	void ShadowmapTechnique::GatherLocations()
	{
		m_iPositionLocation = glGetAttribLocation( m_uID, "in_Position" );

		m_iMVPLocation = glGetUniformLocation( m_uID, "uMVP" );

		Assert( m_iPositionLocation != INVALID_UNIFORM_LOCATION );
		Assert( m_iMVPLocation != INVALID_UNIFORM_LOCATION );
	}

	void ShadowmapTechnique::SetMVP( const glm::mat4& mMVP )
	{
		glUniformMatrix4fv( m_iMVPLocation, 1, false, &mMVP[0][0] );
		Assert_GL();
	}
}