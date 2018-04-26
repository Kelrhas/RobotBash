#include "StdAfx.h"
#include "GLRenderer.h"

#include "UTILS/imgui_impl.h"

#include <algorithm>

#include "ENTITIES/EntityManager.h"
#include "RENDERER/Light.h"
#include "RENDERER/DirectionalLight.h"
#include "RENDERER/PointLight.h"
//#include "RENDERER/SpotLight.h"

#include "GLShaderProgram.h"
#include "Camera.h"
#include "DirectionalLight.h"

Renderer::GLRenderer* g_pRenderer = nullptr;


namespace Renderer
{
	GLRenderer::GLRenderer()
		: m_uWindowWidth( -1 )
		, m_uWindowHeight( -1 )
		, m_pCamera( nullptr )
		, m_eCurrentTarget( renderTarget_Count )
		, m_pSunLight( nullptr )
		, m_uShadowmapFBOId( -1 )
		, m_uShadowmapTexID( -1 )
	{}

	GLRenderer::~GLRenderer()
	{}

	bool GLRenderer::Init( uint32_t uWindowWidth, uint32_t uWindowHeight )
	{
		m_uWindowWidth = uWindowWidth;
		m_uWindowHeight = uWindowHeight;

		glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

		glEnable( GL_DEPTH_TEST );
		glDepthFunc( GL_LESS );

		glEnable( GL_MULTISAMPLE );

		glEnable( GL_BLEND );
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

		glEnable( GL_CULL_FACE );
		glCullFace( GL_BACK );

		Assert_return_result( m_oGBuffer.Init( uWindowWidth, uWindowHeight ) );

		m_oDeferredGeometry.Load( "Data/shaders/deferred_geometry.shader" );
		m_oDeferredLightingTech.Load( "Data/shaders/deferred_light.shader" );
		m_oForwardTechnique.Load( "Data/shaders/forward.shader" );
		m_oGBufferTechnique.Load( "Data/shaders/gbuffer.shader" );
		m_oShadowmapTechnique.Load( "Data/shaders/shadowmap.shader" );

		m_oSkybox.Load("Data/textures/cubemap/hills/");

		MeshInitData data;
		data.bForward = false;
		data.sFileName = "Data/models/cube.obj";
		m_oCubeMesh.InitWithData( &data );

		///////////////////////////////////////
		// SHADOW MAP
		///////////////////////////////////////

		glGenFramebuffers( 1, &m_uShadowmapFBOId );
		Assert_GL();

		glGenTextures( 1, &m_uShadowmapTexID );
		glBindTexture( GL_TEXTURE_2D, m_uShadowmapTexID );
		Assert_GL();
		glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, uWindowWidth, uWindowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr );
		Assert_GL();
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		//glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL );
		Assert_GL();

		glBindFramebuffer( GL_FRAMEBUFFER, m_uShadowmapFBOId );
		glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_uShadowmapTexID, 0 );
		Assert_GL();
		glObjectLabel( GL_TEXTURE, m_uShadowmapTexID, -1, "Shadow map" );
		Assert_GL();
		glObjectLabel( GL_FRAMEBUFFER, m_uShadowmapFBOId, -1, "Shadow map FBO" );
		Assert_GL();

		GLenum eStatus = glCheckFramebufferStatus( GL_FRAMEBUFFER );

		if( eStatus != GL_FRAMEBUFFER_COMPLETE )
		{
			LOG_GL( "ERROR: framebuffer not created" );
			return false;
		}

		glBindFramebuffer( GL_FRAMEBUFFER, 0 );
		Assert_GL();

		return true;
	}

	void GLRenderer::Resize( uint32_t uWindowWidth, uint32_t uWindowHeight )
	{
		glViewport( 0, 0, uWindowWidth, uWindowHeight );
		m_uWindowWidth = uWindowWidth;
		m_uWindowHeight = uWindowHeight;
	}

	void GLRenderer::AddBatch( RenderBatch& oBatch )
	{
		if( oBatch.uFlags & RENDER_FORWARD )
			m_oOpaqueForwardBatches.push_back( oBatch );
		else if( oBatch.uFlags & RENDER_TRANSPARENT )
			m_oTransparencyBatches.push_back( oBatch );
		else
			m_oOpaqueDeferredBatches.push_back( oBatch );
	}

	void GLRenderer::AddLight( Light* pLight )
	{
		Assert_return( pLight );
		switch( pLight->GetType() )
		{
		case LightType::lightType_Directional:
			Assert_return( m_pSunLight == nullptr );

			m_pSunLight = static_cast<Renderer::DirectionalLight*>(pLight);
			break;

		case LightType::lightType_Point:
			m_oPointLights.push_back( static_cast<PointLight*>(pLight) );
			break;

		case LightType::lightType_Spot:
			//m_oSpotLights.push_back( static_cast<const SpotLight*>(pLight) );
			break;

		default:
			Assert( false );
		}
	}

	TextureWeakPtr GLRenderer::LoadTexture( string sFilename )
	{
		string sUpperFilename = sFilename;
		transform( sUpperFilename.begin(), sUpperFilename.end(), sUpperFilename.begin(), ::toupper );
		map<string, TexturePtr>::iterator itTexture = m_oLoadedTextures.find( sUpperFilename );

		if( itTexture != m_oLoadedTextures.end() )
			return itTexture->second;

		TexturePtr xTexPtr = make_shared<Texture>();
		xTexPtr->Load( sFilename );
		m_oLoadedTextures[sUpperFilename] = xTexPtr;

		return xTexPtr;
	}

	bool GLRenderer::Render()
	{
		// first check if the shaders have changed
#ifdef _DEBUG
		m_oDeferredGeometry.LiveUpdate();
		m_oDeferredLightingTech.LiveUpdate();
		m_oForwardTechnique.LiveUpdate();
		m_oGBufferTechnique.LiveUpdate();
#endif

		// render the shadowmaps
		Assert( ShadowMap() );
		Assert_GL();

		// render the RT needed
		Assert( RenderOpaqueDeferred() );
		Assert_GL();
		//Assert( RenderTransparent() );
		Assert( LightPassDeferred() );
		Assert_GL();

		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
		Assert_GL();
		glClear( GL_COLOR_BUFFER_BIT );
		Assert_GL();
			
		m_oGBuffer.BindForReading();
		Assert_GL();

		m_oGBuffer.BindTexture( renderTarget_FinalLit, renderTarget_FinalLit );
			
		m_oGBuffer.SetReadBuffer(renderTarget_FinalLit);
		glBlitFramebuffer(0, 0, m_uWindowWidth, m_uWindowHeight, 0, 0, m_uWindowWidth, m_uWindowHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
		Assert_GL();

		// render the cubemap
		Assert( RenderCubemap() );

		Assert( ForwardPass() );
		Assert_GL();

		if( DEBUG::bShowGBuffer )
		{
			m_oGBuffer.BindForReading();
			Assert_GL();

			Assert( DisplayGBuffer() );
		}

		// TODO render gizmos

		{
			CREATE_DEBUG_GROUP( "Imgui" );

			if( DEBUG::bShowDebugMenu )
			{
				if( ImGui::Begin( "Debug" ) )
				{
					DEBUG::ShowMenu();
					g_pEntityMgr->ImGuiDraw();
				}
				ImGui::End();
			}

			ImGui::Render();
			ImGui_Impl_RenderDrawLists( ImGui::GetDrawData() );
		}

		Assert( g_pRenderer->Flush() );

		return true;
	}

	bool GLRenderer::ShadowMap()
	{
		CREATE_DEBUG_GROUP( "shadow map" );

#ifdef _DEBUG
		m_oShadowmapTechnique.LiveUpdate();
#endif

		glBindFramebuffer( GL_FRAMEBUFFER, m_uShadowmapFBOId );
		glClear( GL_DEPTH_BUFFER_BIT );
		Assert_GL();

		m_oShadowmapTechnique.Use();
		glDrawBuffer( GL_NONE );
		glReadBuffer( GL_NONE );
		Assert_GL();

		const glm::mat4& mView = m_pSunLight->GetViewMatrix();
		const glm::mat4& mProj = m_pSunLight->GetProjMatrix();

		for( std::vector<RenderBatch>::const_iterator itBatch( m_oOpaqueDeferredBatches.begin() );
			 itBatch != m_oOpaqueDeferredBatches.end();
			 ++itBatch )
		{
			const glm::mat4& mModel = *itBatch->pWorldMatrix;
			const glm::mat4 mMV = mView * mModel;
			const glm::mat4 mMVP = mProj * mMV;
			//glm::mat4 mNormalMtx = glm::inverse( mMV );
			//mNormalMtx = mModel;
			//mNormalMtx[3] = glm::vec4( 0, 0, 0, 1 );

			glBindVertexArray( itBatch->uVAO );
			Assert_GL();

			m_oShadowmapTechnique.SetMVP( mMVP );

			glDrawElements( GL_TRIANGLES, (GLsizei) itBatch->uNbVertex, GL_UNSIGNED_INT, 0 );
			Assert_GL();
		}

		return true;
	}

	bool GLRenderer::PreRenderDeferred()
	{


		// clear default framebuffer
		/*glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
		Assert_GL();
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		Assert_GL();*/


		return true;
	}

	bool GLRenderer::RenderOpaqueDeferred()
	{
		CREATE_DEBUG_GROUP( "render opaque" );

		m_oGBuffer.BindForWriting();

		GLenum aDrawBuffers[] = {
			aRenderTargetInfos[renderTarget_WorldPos].eAttachment,
			aRenderTargetInfos[renderTarget_Albedo].eAttachment,
			aRenderTargetInfos[renderTarget_Normal].eAttachment,
			aRenderTargetInfos[renderTarget_TexCoord].eAttachment };
		glDrawBuffers( 4, aDrawBuffers );
		Assert_GL();

		glEnable( GL_DEPTH_TEST );
		Assert_GL();
		glDepthMask( GL_TRUE );
		Assert_GL();
		glDisable( GL_BLEND );
		Assert_GL();

		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		Assert_GL();

		m_oDeferredGeometry.Use();

		const glm::mat4& mView = m_pCamera->GetViewMatrix();
		const glm::mat4& mProj = m_pCamera->GetProjectionMatrix();

		for( std::vector<RenderBatch>::const_iterator itBatch( m_oOpaqueDeferredBatches.begin() );
			 itBatch != m_oOpaqueDeferredBatches.end();
			 ++itBatch )
		{
			const glm::mat4& mModel = *itBatch->pWorldMatrix;
			const glm::mat4 mMV = mView * mModel;
			const glm::mat4 mMVP = mProj * mMV;
			glm::mat4 mNormalMtx = glm::inverse( mMV );
			mNormalMtx = mModel;
			mNormalMtx[3] = glm::vec4( 0, 0, 0, 1 );

			glBindVertexArray( itBatch->uVAO );

			m_oDeferredGeometry.SetModelMatrix( mModel );
			m_oDeferredGeometry.SetMVP( mMVP );
			m_oDeferredGeometry.SetNormalMatrix( mNormalMtx );

			int iCurrentTexture = 0;
			if( !itBatch->xDiffuseMap.expired() )
			{
				TexturePtr xPtr = itBatch->xDiffuseMap.lock();
				glActiveTexture( GL_TEXTURE0 + iCurrentTexture );
				glBindTexture( GL_TEXTURE_2D, xPtr->GetID() );
				Assert_GL();
				m_oDeferredGeometry.SetDiffuseMap( iCurrentTexture );
			}

			glDrawElements( GL_TRIANGLES, (GLsizei) itBatch->uNbVertex, GL_UNSIGNED_INT, 0 );
			Assert_GL();
		}

		glDepthMask( GL_FALSE );
		glDisable( GL_DEPTH_TEST );
		Assert_GL();

		// make sure no one touch the geometry
		glBindVertexArray( 0 );

		return true;
	}

	bool GLRenderer::RenderTransparent()
	{
		CREATE_DEBUG_GROUP( "render transparent" );

		// TODO
		return true;
	}

	bool GLRenderer::RenderCubemap()
	{
		CREATE_DEBUG_GROUP( "render cubemap" );

		glEnable( GL_DEPTH_TEST );

		m_oGBuffer.BindForReading();
		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
		glBlitFramebuffer( 0, 0, m_uWindowWidth, m_uWindowHeight, 0, 0, m_uWindowWidth, m_uWindowHeight, GL_DEPTH_BUFFER_BIT, GL_NEAREST );
		glBindFramebuffer( GL_FRAMEBUFFER, 0 );
		glDepthFunc( GL_LEQUAL );

		// use skybox technique
		// set mvp
		// draw the cube
		// unuse skybox technique
		const glm::mat4 mView = glm::mat4( glm::mat3( m_pCamera->GetViewMatrix() ) );
		const glm::mat4& mProj = m_pCamera->GetProjectionMatrix();
		glm::mat4 mVP = mProj * mView;
		m_oSkybox.Draw( mVP );
		

		glDepthFunc( GL_LESS );

		return true;
	}

	bool GLRenderer::LightPassDeferred()
	{
		CREATE_DEBUG_GROUP( "light pass" );

		m_oGBuffer.BindForReading(); // reading previous RT: position, albedo, normal ...
		m_oGBuffer.BindForWriting();
		Assert_GL();

		glDrawBuffer( aRenderTargetInfos[renderTarget_FinalLit].eAttachment );
		Assert_GL();

		glClear( GL_COLOR_BUFFER_BIT );

		m_oGBuffer.BindTextures();

		glActiveTexture( GL_TEXTURE5 );
		glBindTexture( GL_TEXTURE_2D, m_uShadowmapTexID );

		const glm::mat4& mLightView = m_pSunLight->GetViewMatrix();
		const glm::mat4& mLightProj = m_pSunLight->GetProjMatrix();

		glm::mat4 mCameraView = m_pCamera->GetViewMatrix();
		const glm::mat4& mCameraProj = m_pCamera->GetProjectionMatrix();

		m_oDeferredLightingTech.Use();
		m_oDeferredLightingTech.BindLocations();

		// shadowmap var
		m_oDeferredLightingTech.SetShadowmap( 5 );
		m_oDeferredLightingTech.SetLightMVMatrix( mLightView );
		m_oDeferredLightingTech.SetLightProjMatrix( mLightProj );
		m_oDeferredLightingTech.SetEyeWorldPos( m_pCamera->GetPosition() );

		// dir light
		m_oDeferredLightingTech.SetSunData( static_cast<DirectionalLight::DirectionalLightData*>( m_pSunLight->GetGLSLData() ) );

		// TODO separate light passes to only render the fragment "visible" by the light and apply different shadows

		// point lights
		m_oDeferredLightingTech.SetNbPointLight( m_oPointLights.size() );
		for( int i=0; i < m_oPointLights.size(); ++i )
		{
			const PointLight* pPointLight = m_oPointLights[i];

			m_oDeferredLightingTech.SetPointLightData( i, static_cast<PointLight::PointLightData*>(pPointLight->GetGLSLData() ) );
		}

		m_oGBuffer.DrawQuad();

		glDisable( GL_DEPTH_TEST );

		return true;
	}

	bool GLRenderer::ForwardPass()
	{
		CREATE_DEBUG_GROUP( "forward pass" );

		m_oGBuffer.BindForReading();
		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
		Assert_GL();

		glDepthMask( GL_TRUE );
		glEnable( GL_DEPTH_TEST );
		Assert_GL();

		// copy depth from geometry pass
		glBlitFramebuffer( 0, 0, m_uWindowWidth, m_uWindowHeight, 0, 0, m_uWindowWidth, m_uWindowHeight, GL_DEPTH_BUFFER_BIT, GL_NEAREST );
		Assert_GL();


		m_oForwardTechnique.Use();

		for( std::vector<RenderBatch>::const_iterator itBatch( m_oOpaqueForwardBatches.begin() );
			 itBatch != m_oOpaqueForwardBatches.end();
			 ++itBatch )
		{
			glm::mat4& mModel = *itBatch->pWorldMatrix;
			const glm::mat4& mView = m_pCamera->GetViewMatrix();
			const glm::mat4& mProj = m_pCamera->GetProjectionMatrix();
			glm::mat4 mMVP = mProj * mView * mModel;

			glBindVertexArray( itBatch->uVAO );
			Assert_GL();

			m_oForwardTechnique.SetMVP( mMVP );
			m_oForwardTechnique.SetDiffuseMap( 0 );

			int iCurrentTexture = 0;
			if( !itBatch->xDiffuseMap.expired() )
			{
				TexturePtr xPtr = itBatch->xDiffuseMap.lock();
				glActiveTexture( GL_TEXTURE0 );
				glBindTexture( GL_TEXTURE_2D, xPtr->GetID() );
				Assert_GL();
				m_oDeferredGeometry.SetDiffuseMap( iCurrentTexture );
			}

			glDrawElements( GL_TRIANGLES, (GLsizei) itBatch->uNbVertex, GL_UNSIGNED_INT, 0 );
			Assert_GL();
		}

		return true;
	}

	bool GLRenderer::DisplayGBuffer()
	{
		CREATE_DEBUG_GROUP( "display gbuffer" );

		m_oGBuffer.BindForReading();
		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );

		//glBlitFramebuffer(0, 0, m_uWindowWidth, m_uWindowHeight, 0, 0, m_uWindowWidth, m_uWindowHeight, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

		m_oGBufferTechnique.Use();

		if( DEBUG::eTargetToDisplay == renderTarget_Count )
		{
			m_oGBufferTechnique.SetTarget( 0 );
			m_oGBuffer.BindTexture( renderTarget_Albedo, 0 );
			m_oGBufferTechnique.SetDiffuseMap( 0 );
			m_oGBuffer.BindTexture( renderTarget_Normal, 1 );
			m_oGBufferTechnique.SetNormalMap( 1 );
			m_oGBuffer.BindTexture( renderTarget_Depth, 2 );
			m_oGBufferTechnique.SetDepthMap( 2 );
			m_oGBufferTechnique.SetDepthPlanes( m_pCamera->GetNearPlane(), m_pCamera->GetFarPlane() );
			m_oGBuffer.BindTexture( renderTarget_FinalLit, 3 );
			m_oGBufferTechnique.SetFinalLitMap( 3 );
		}
		else if( DEBUG::eTargetToDisplay == renderTarget_Depth )
		{
			m_oGBufferTechnique.SetTarget( 2 );
			m_oGBuffer.BindTexture( renderTarget_Depth, 0 );
			m_oGBufferTechnique.SetDepthMap( 0 );
			m_oGBufferTechnique.SetDepthPlanes( m_pCamera->GetNearPlane(), m_pCamera->GetFarPlane() );
		}
		else if( DEBUG::eTargetToDisplay == renderTarget_Shadowmap )
		{
			glBindFramebuffer( GL_READ_FRAMEBUFFER, m_uShadowmapFBOId );
			m_oGBufferTechnique.SetTarget( 3 );
			glActiveTexture( GL_TEXTURE0 );
			glBindTexture( GL_TEXTURE_2D, m_uShadowmapTexID );
			m_oGBufferTechnique.SetDepthMap( 0 );
			m_oGBufferTechnique.SetDepthPlanes( -1, 1 );
		}
		else
		{
			m_oGBufferTechnique.SetTarget( 1 );
			m_oGBuffer.BindTexture( DEBUG::eTargetToDisplay, 0 );
			m_oGBufferTechnique.SetDiffuseMap( 0 );
		}

		m_oGBuffer.DrawQuad();

		return true;
	}

	bool GLRenderer::Flush()
	{
		glFlush();
		m_oOpaqueDeferredBatches.clear();
		m_oOpaqueForwardBatches.clear();
		m_oTransparencyBatches.clear();

		return true;
	}

}