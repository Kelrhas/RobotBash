#pragma once

namespace Renderer
{
	class Camera
	{
		enum CameraMoveMode
		{
			CAMERA_MOVE_MODE_ORBIT,
			CAMERA_MOVE_MODE_FREE
		};

	public:
		Camera();
		~Camera();

		virtual bool				Init();
		virtual bool				Update( float fDeltatime );

				void				SetPerspective( float fFOV, float fNearPlane, float fFarPlane );
				void				SetOrthogonal( float fWidth, float fHeight, float fNearPlane, float fFarPlane );

				void				SetActive();
				void				SetPosition( glm::vec3 vPos );
				void				Translate( glm::vec3 vTranslate );
				void				Rotate( glm::vec3 vAxis, float fAngle );
				void				LookAt( glm::vec3 vPos, glm::vec3 vTarget, glm::vec3 vUp = glm::vec3( 0, 1, 0 ) );

				void				ZoomCallback( float fZoom );
				void				MouseMoveCallback( glm::vec2 vDeltaPos );

				const glm::vec3		GetPosition() const;
				glm::vec3			GetDirection() const;
				glm::vec3			GetRight() const;
				glm::vec3			GetUp() const;
				const glm::mat4&	GetViewMatrix() const { return m_mViewMatrix; }
				const glm::mat4&	GetProjectionMatrix() const { return m_mProjectionMatrix; }
				const bool			IsPerspective() const { return m_bPerspective; }
				const float			GetNearPlane() const { return m_bPerspective ? m_fPerspectiveNearPlane : m_fOrthoNearPlane; }
				const float			GetFarPlane() const { return m_bPerspective ? m_fPerspectiveFarPlane : m_fOrthoFarPlane; }

	protected:
		CameraMoveMode				m_eCamMoveMode;
		glm::mat4					m_mViewMatrix;
		glm::mat4					m_mProjectionMatrix;
		bool						m_bPerspective;
		float						m_fPerspectiveNearPlane;
		float						m_fPerspectiveFarPlane;
		float						m_fPerspectiveFov;
		float						m_fOrthoNearPlane;
		float						m_fOrthoFarPlane;
		float						m_fOrthoWidth;
		float						m_fOrthoHeight;
		glm::vec2					m_vMousePos;
	};
}