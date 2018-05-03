#include "StdAfx.h"
#include "Camera.h"

#include "GLRenderer.h"
#include "IO/InputMgr.h"

namespace Renderer
{

	Camera::Camera()
		: m_eCamMoveMode( CAMERA_MOVE_MODE_FREE )
		, m_fPerspectiveNearPlane( 0.f )
		, m_fPerspectiveFarPlane( 0.f )
		, m_fPerspectiveFov( 0.f )
		, m_fOrthoNearPlane( 0.f )
		, m_fOrthoFarPlane( 0.f )
		, m_fOrthoWidth( 0.f )
		, m_fOrthoHeight( 0.f )
	{

	}

	Camera::~Camera()
	{

	}

	bool Camera::Init()
	{
		Input::InputMgr::RegisterMouseWheelButton( [this] ( float fZoom ){this->ZoomCallback( fZoom ); } );
		Input::InputMgr::RegisterMouseMove( [this] ( glm::vec2 vMousePos ){this->MouseMoveCallback( vMousePos ); } );

		m_vOrbitCenter = VEC3_0;

		return true;
	}

	bool Camera::Update(float fDeltatime)
	{
		if( m_eCamMoveMode == CAMERA_MOVE_MODE_ORBIT )
		{
			glm::vec3 vTranslate = VEC3_0;
			if( Input::InputMgr::IsKeyDown( Input::KEY_Left ) )
				vTranslate += GetRight() * fDeltatime;
			if( Input::InputMgr::IsKeyDown( Input::KEY_Right ) )
				vTranslate -= GetRight() * fDeltatime;
			if( Input::InputMgr::IsKeyDown( Input::KEY_Up ) )
				vTranslate += GetUp() * fDeltatime;
			if( Input::InputMgr::IsKeyDown( Input::KEY_Down ) )
				vTranslate -= GetUp() * fDeltatime;

			Translate( vTranslate * DEBUG::fCameraMoveSpeed );
			m_vOrbitCenter += vTranslate;

			//g_pRenderer->DEBUG_DRAW_CUBE( m_vOrbitCenter );
		}
		else
		{

		}

		//m_mViewMatrix = glm::lookAt( m_vPosition, m_vTarget, glm::vec3( 0, 1, 0 ) );
		return true;
	}

	void Camera::ImGuiDraw()
	{
		if( ImGui::TreeNode( "Camera" ) )
		{
			{
				bool bDirtyProj = false;
				bDirtyProj |= ImGui::Checkbox( "Perspective", &m_bPerspective );
				if( m_bPerspective )
				{
					bDirtyProj |= ImGui::DragFloat( "Near plane", &m_fPerspectiveNearPlane, 0.01f, 0.f, 10.f );
					bDirtyProj |= ImGui::DragFloat( "Far plane", &m_fPerspectiveFarPlane, 0.1f, 0.1f, 1000.f );
					bDirtyProj |= ImGui::DragFloat( "Fov", &m_fPerspectiveFov, 0.01f, 0.f, 1.f );
				}
				else
				{
					bDirtyProj |= ImGui::DragFloat( "Near plane", &m_fOrthoNearPlane, 0.01f, 0.f, 10.f );
					bDirtyProj |= ImGui::DragFloat( "Far plane", &m_fOrthoFarPlane, 0.1f, 0.1f, 1000.f );
					bDirtyProj |= ImGui::DragFloat( "Width", &m_fOrthoWidth, 0.1f, 0.f, 10000.f );
					bDirtyProj |= ImGui::DragFloat( "Height", &m_fOrthoHeight, 0.1f, 0.f, 10000.f );
				}

				if( bDirtyProj )
					UpdateProjectionMatrix();
			}
			{
				bool bDirtyView = false;
				bDirtyView |= ImGui::Combo( "Mode", &(int)m_eCamMoveMode, "Orbit\0Free\0\0" );

				if( bDirtyView )
					UpdateViewMatrix();
			}


			ImGui::TreePop();
		}
	}

	void Camera::SetPerspective(float fFOV, float fNearPlane, float fFarPlane)
	{
		m_mProjectionMatrix = glm::perspective(fFOV, g_pRenderer->GetAspectRatio(), fNearPlane, fFarPlane);
		m_bPerspective = true;
		m_fPerspectiveFov = fFOV;
		m_fPerspectiveNearPlane = fNearPlane;
		m_fPerspectiveFarPlane = fFarPlane;
	}

	void Camera::SetOrthogonal(float fWidth, float fHeight, float fNearPlane, float fFarPlane)
	{
		m_mProjectionMatrix = glm::ortho(0.f, fWidth, 0.f, fHeight, fNearPlane, fFarPlane);
		m_bPerspective = false;
		m_fOrthoWidth = fWidth;
		m_fOrthoHeight = fHeight;
		m_fOrthoNearPlane = fNearPlane;
		m_fOrthoFarPlane = fFarPlane;
	}

	void Camera::UpdateProjectionMatrix()
	{
		if( m_bPerspective )
		{
			m_mProjectionMatrix = glm::perspective( m_fPerspectiveFov, g_pRenderer->GetAspectRatio(), m_fPerspectiveNearPlane, m_fPerspectiveFarPlane );
		}
		else
		{
			m_mProjectionMatrix = glm::ortho( 0.f, m_fOrthoWidth, 0.f, m_fOrthoHeight, m_fOrthoNearPlane, m_fOrthoFarPlane );
		}
	}

	void Camera::UpdateViewMatrix()
	{
		if( m_eCamMoveMode == CAMERA_MOVE_MODE_ORBIT )
		{
			TODO;
		}
		else
		{
			//TODO;
		}
	}

	void Camera::SetActive()
	{
		g_pRenderer->SetCamera(this);
	}

	void Camera::SetPosition(glm::vec3 vPos)
	{
		glm::vec4 v;
		v[0] = vPos[0];
		v[1] = vPos[1];
		v[2] = vPos[2];
		v[3] = m_mViewMatrix[3][3];
		m_mViewMatrix[3] = v;

		return;
	}

	void Camera::Translate(glm::vec3 vTranslate)
	{
		m_mViewMatrix = glm::translate( m_mViewMatrix, vTranslate );
	}

	void Camera::Rotate(glm::vec3 vAxis, float fAngle)
	{
		m_mViewMatrix = glm::rotate(m_mViewMatrix, fAngle, vAxis);
	}

	void Camera::LookAt( glm::vec3 vPos, glm::vec3 vTarget, glm::vec3 vUp /* = glm::vec3(0, 1, 0) */)
	{
		m_mViewMatrix = glm::lookAt(vPos, vTarget, vUp);
	}

	void Camera::ZoomCallback( float fZoom )
	{
		Translate( GetDirection() * fZoom * DEBUG::fCameraZoomSpeed );
	}

	void Camera::MouseMoveCallback( glm::vec2 vDeltaPos )
	{
		if( Input::InputMgr::IsMouseLeftDown() && !ImGui::IsWindowHovered( ImGuiHoveredFlags_AnyWindow ) && !ImGui::IsAnyItemActive() )
		{
			if( m_eCamMoveMode == CAMERA_MOVE_MODE_ORBIT )
			{
				glm::vec3 t = GetPosition() - m_vOrbitCenter;
				m_mViewMatrix = glm::translate( glm::rotate( glm::translate( m_mViewMatrix, -t ), vDeltaPos.x, VEC3_UP ), t );
			}
			else
			{
				Rotate( VEC3_UP, vDeltaPos.x );
				Rotate( GetRight(), vDeltaPos.y );
			}
		}
	}
		
	const glm::vec3 Camera::GetPosition() const
	{
		return -m_mViewMatrix[3] * m_mViewMatrix;
	}

	glm::vec3 Camera::GetDirection() const
	{
		glm::mat4 mT = glm::transpose( m_mViewMatrix );
		return mT[2];
	}

	glm::vec3 Camera::GetRight() const
	{
		glm::mat4 mT = glm::transpose( m_mViewMatrix );
		return mT[0];
	}

	glm::vec3 Camera::GetUp() const
	{
		glm::mat4 mT = glm::transpose( m_mViewMatrix );
		return mT[1];
	}

}