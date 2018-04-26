#include "StdAfx.h"
#include "GBufferTechnique.h"

namespace Renderer
{
	void GBufferTechnique::GatherLocations()
	{
		m_iPositionLocation = glGetAttribLocation(m_uID, "in_Position");
		m_iUVLocation = glGetAttribLocation(m_uID, "in_TexCoord");
		
		m_iTargetLocation = glGetUniformLocation(m_uID, "uTarget");
		m_iDiffuseMapLocation = glGetUniformLocation(m_uID, "uAlbedo");
		m_iNormalMapLocation = glGetUniformLocation(m_uID, "uNormal");
		m_iDepthMapLocation = glGetUniformLocation(m_uID, "uDepth");
		m_iFinalMapLocation = glGetUniformLocation(m_uID, "uFinalLit");

		m_iNearPlaneLocation = glGetUniformLocation(m_uID, "uNearPlane");
		m_iFarPlaneLocation = glGetUniformLocation(m_uID, "uFarPlane");

		// check we have the needed ones
		Assert(m_iTargetLocation != INVALID_UNIFORM_LOCATION);
		Assert(m_iPositionLocation != INVALID_UNIFORM_LOCATION);
		Assert(m_iUVLocation != INVALID_UNIFORM_LOCATION);
		Assert(m_iDiffuseMapLocation != INVALID_UNIFORM_LOCATION);
	}

	void GBufferTechnique::SetTarget(int iTargetMode)
	{
		glUniform1i(m_iTargetLocation, iTargetMode );
		Assert_GL();
	}

	void GBufferTechnique::SetDiffuseMap(int iTexIndex)
	{
		glUniform1i(m_iDiffuseMapLocation, iTexIndex);
		Assert_GL();
	}

	void GBufferTechnique::SetNormalMap(int iTexIndex)
	{
		if (m_iNormalMapLocation != INVALID_UNIFORM_LOCATION)
			glUniform1i(m_iNormalMapLocation, iTexIndex);
	}

	void GBufferTechnique::SetDepthMap(int iTexIndex)
	{
		if(m_iDepthMapLocation != INVALID_UNIFORM_LOCATION)
			glUniform1i(m_iDepthMapLocation, iTexIndex);
	}

	void GBufferTechnique::SetFinalLitMap(int iTexIndex)
	{
		if(m_iFinalMapLocation != INVALID_UNIFORM_LOCATION)
			glUniform1i(m_iFinalMapLocation, iTexIndex);
	}

	void GBufferTechnique::SetDepthPlanes(float fNear, float fFar)
	{
		if(m_iNearPlaneLocation != INVALID_UNIFORM_LOCATION)
			glUniform1f(m_iNearPlaneLocation, fNear);
		if(m_iFarPlaneLocation != INVALID_UNIFORM_LOCATION)
			glUniform1f(m_iFarPlaneLocation, fFar);
	}

}
