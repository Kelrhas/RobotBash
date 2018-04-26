#include "StdAfx.h"
#include "DeferredDirectionalLightTechnique.h"

namespace Renderer
{
	void DeferredDirectionalLightTechnique::GatherLocations()
	{
		DeferredLightingTechnique::GatherLocations();

		m_iDirectionLocation		= glGetUniformLocation( m_uID, "uSun.vDirection" );
		m_iColorLocation			= glGetUniformLocation( m_uID, "uSun.oBase.vColor" );
		m_iAmbientIntensityLocation = glGetUniformLocation( m_uID, "uSun.oBase.fAmbientIntensity" );
		m_iDiffuseIntensityLocation = glGetUniformLocation( m_uID, "uSun.oBase.fDiffuseIntensity" );
		m_iShadowmapLocation		= glGetUniformLocation( m_uID, "uShadowmap" );

		Assert( m_iDirectionLocation		!= INVALID_UNIFORM_LOCATION );
		Assert( m_iColorLocation			!= INVALID_UNIFORM_LOCATION );
		Assert( m_iAmbientIntensityLocation != INVALID_UNIFORM_LOCATION );
		Assert( m_iDiffuseIntensityLocation != INVALID_UNIFORM_LOCATION );
		Assert( m_iShadowmapLocation		!= INVALID_UNIFORM_LOCATION );
	}

	void DeferredDirectionalLightTechnique::SetLightDirection( const glm::vec3& vDirection )
	{
		Assert( !glm::any( glm::isnan( vDirection ) ) );
		glUniform3fv( m_iDirectionLocation, 1, &vDirection.x );
		Assert_GL();
	}

	void DeferredDirectionalLightTechnique::SetLightColor( const glm::vec3& vColor )
	{
		glUniform3fv( m_iColorLocation, 1, &vColor.x );
		Assert_GL();
	}
	void DeferredDirectionalLightTechnique::SetAmbientIntensity( float fIntensity )
	{
		glUniform1f( m_iAmbientIntensityLocation, fIntensity );
		Assert_GL();
	}

	void DeferredDirectionalLightTechnique::SetDiffuseIntensity( float fIntensity )
	{
		glUniform1f( m_iDiffuseIntensityLocation, fIntensity );
		Assert_GL();
	}

}

