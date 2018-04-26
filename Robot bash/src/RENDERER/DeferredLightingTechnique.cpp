#include "StdAfx.h"
#include "DeferredLightingTechnique.h"

#include "PointLight.h"

#include "GBuffer.h"

namespace Renderer
{

	void DeferredLightingTechnique::BindLocations()
	{
		glBindFragDataLocation(m_uID, 0, "out_FragColor");
		Assert_GL();
	}

	void DeferredLightingTechnique::GatherLocations()
	{
		m_iPositionLocation			= glGetAttribLocation( m_uID, "in_vPosition" );
		m_iUVLocation				= glGetAttribLocation( m_uID, "in_TexCoord" );

		m_iWorldPosMapLocation		= glGetUniformLocation( m_uID, "uWorldPos" );
		m_iAlbedoMapLocation		= glGetUniformLocation( m_uID, "uAlbedo" );
		m_iNormalMapLocation		= glGetUniformLocation( m_uID, "uNormal" );
		m_iUVMapLocation			= glGetUniformLocation( m_uID, "uUVs" );
		m_iEyeWorldPosLocation		= glGetUniformLocation( m_uID, "uEyePos" );
		m_iShadowmapLocation		= glGetUniformLocation( m_uID, "uShadowmap" );
		m_iLightMVLocation			= glGetUniformLocation( m_uID, "uLightWorldMatrix" );
		m_iLightViewToProjLocation	= glGetUniformLocation( m_uID, "uLightViewToProjectionMatrix" );
		m_iPointLightNumberLocation = glGetUniformLocation( m_uID, "uNbPointLights" );

		m_sunLightLocation.Color	= glGetUniformLocation( m_uID, "uSun.oBase.vColor" );
		Assert( m_sunLightLocation.Color != INVALID_UNIFORM_LOCATION );
		m_sunLightLocation.Direction = glGetUniformLocation( m_uID, "uSun.vDirection" );
		Assert( m_sunLightLocation.Direction != INVALID_UNIFORM_LOCATION );
		m_sunLightLocation.AmbientIntensity = glGetUniformLocation( m_uID, "uSun.oBase.fAmbientIntensity" );
		Assert( m_sunLightLocation.AmbientIntensity != INVALID_UNIFORM_LOCATION );
		m_sunLightLocation.DiffuseIntensity = glGetUniformLocation( m_uID, "uSun.oBase.fDiffuseIntensity" );
		Assert( m_sunLightLocation.AmbientIntensity != INVALID_UNIFORM_LOCATION );

		char Name[128];
		memset( Name, 0, 128 );
		for( unsigned int i = 0; i < MAX_POINT_LIGHTS; i++ )
		{
			sprintf( Name, "uPointLights[%d].oBase.vColor", i );
			m_pointLightsLocation[i].Color = glGetUniformLocation( m_uID, Name );
			Assert( m_pointLightsLocation[i].Color != INVALID_UNIFORM_LOCATION );

			sprintf( Name, "uPointLights[%d].oBase.fAmbientIntensity", i );
			m_pointLightsLocation[i].AmbientIntensity = glGetUniformLocation( m_uID, Name );
			Assert( m_pointLightsLocation[i].AmbientIntensity != INVALID_UNIFORM_LOCATION );

			sprintf( Name, "uPointLights[%d].oBase.fDiffuseIntensity", i );
			m_pointLightsLocation[i].DiffuseIntensity = glGetUniformLocation( m_uID, Name );
			Assert( m_pointLightsLocation[i].DiffuseIntensity != INVALID_UNIFORM_LOCATION );

			sprintf( Name, "uPointLights[%d].vPosition", i );
			m_pointLightsLocation[i].Position = glGetUniformLocation( m_uID, Name );
			Assert( m_pointLightsLocation[i].Position != INVALID_UNIFORM_LOCATION );

			sprintf( Name, "uPointLights[%d].fCstAttenuation", i );
			m_pointLightsLocation[i].CstAttenuation = glGetUniformLocation( m_uID, Name );
			Assert( m_pointLightsLocation[i].CstAttenuation != INVALID_UNIFORM_LOCATION );

			sprintf( Name, "uPointLights[%d].fLinAttenuation", i );
			m_pointLightsLocation[i].LinearAttenuation = glGetUniformLocation( m_uID, Name );
			Assert( m_pointLightsLocation[i].LinearAttenuation != INVALID_UNIFORM_LOCATION );

			sprintf( Name, "uPointLights[%d].fExpAttenuation", i );
			m_pointLightsLocation[i].ExpAttenuation = glGetUniformLocation( m_uID, Name );
			Assert( m_pointLightsLocation[i].ExpAttenuation != INVALID_UNIFORM_LOCATION );
		}
		
		// check we have the needed ones
		Assert( m_iWorldPosMapLocation		!= INVALID_UNIFORM_LOCATION );
		Assert( m_iAlbedoMapLocation		!= INVALID_UNIFORM_LOCATION );
		Assert( m_iNormalMapLocation		!= INVALID_UNIFORM_LOCATION );
		Assert( m_iWorldPosMapLocation		!= INVALID_UNIFORM_LOCATION );
		Assert( m_iWorldPosMapLocation		!= INVALID_UNIFORM_LOCATION );
		Assert( m_iWorldPosMapLocation		!= INVALID_UNIFORM_LOCATION );
		Assert( m_iWorldPosMapLocation		!= INVALID_UNIFORM_LOCATION );

		Use();

		glUniform1i(m_iWorldPosMapLocation, Renderer::renderTarget_WorldPos);
		glUniform1i(m_iAlbedoMapLocation, Renderer::renderTarget_Albedo);
		glUniform1i(m_iNormalMapLocation, Renderer::renderTarget_Normal);
		if(m_iUVMapLocation != INVALID_UNIFORM_LOCATION)
			glUniform1i(m_iUVMapLocation, Renderer::renderTarget_TexCoord);
	}
	
	void DeferredLightingTechnique::SetEyeWorldPos(glm::vec3 vEyePos)
	{
		glUniform3fv(m_iEyeWorldPosLocation, 1, &vEyePos.x);
	}

	void DeferredLightingTechnique::SetLightMVMatrix( glm::mat4 mWVP )
	{
		glUniformMatrix4fv( m_iLightMVLocation, 1, false, &mWVP[0][0] );
	}

	void DeferredLightingTechnique::SetLightProjMatrix( glm::mat4 mWVP )
	{
		glUniformMatrix4fv( m_iLightViewToProjLocation, 1, false, &mWVP[0][0] );
	}

	void DeferredLightingTechnique::SetShadowmap( GLint iTex )
	{
		glUniform1i( m_iShadowmapLocation, iTex );
		Assert_GL();
	}

	void DeferredLightingTechnique::SetSunData( const DirectionalLight::DirectionalLightData* pLightData )
	{
		glUniform3fv( m_sunLightLocation.Color, 1, &pLightData->m_vColor.r );
		glUniform3fv( m_sunLightLocation.Direction, 1, &pLightData->m_vDirection.x );
		glUniform1f( m_sunLightLocation.AmbientIntensity, 0.2f );
		glUniform1f( m_sunLightLocation.DiffuseIntensity, pLightData->m_fDiffuseIntensity );
		Assert_GL();
	}

	void DeferredLightingTechnique::SetPointLightData( int iIndex, const PointLight::PointLightData* pLightData )
	{
		glUniform3fv( m_pointLightsLocation[iIndex].Color, 1, &pLightData->m_vColor.r );
		glUniform3fv( m_pointLightsLocation[iIndex].Position, 1, &pLightData->m_vPosition.x );
		glUniform1f( m_pointLightsLocation[iIndex].AmbientIntensity, 0.02f );
		glUniform1f( m_pointLightsLocation[iIndex].DiffuseIntensity, pLightData->m_fDiffuseIntensity );
		glUniform1f( m_pointLightsLocation[iIndex].CstAttenuation, pLightData->m_fConstantAttenuation );
		glUniform1f( m_pointLightsLocation[iIndex].LinearAttenuation, pLightData->m_fLinearAttenuation );
		glUniform1f( m_pointLightsLocation[iIndex].ExpAttenuation, pLightData->m_fExpAttenuation );
		Assert_GL();
	}

	void DeferredLightingTechnique::SetNbPointLight( int iNbLight )
	{
		glUniform1i( m_iPointLightNumberLocation, iNbLight );
		Assert_GL();
	}


}

