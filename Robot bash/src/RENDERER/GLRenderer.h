#pragma once

#include "ogl.h"
#include "RENDERER/GBuffer.h"
#include "RENDERER/DeferredGeometryTechnique.h"
#include "RENDERER/DeferredDirectionalLightTechnique.h"
#include "RENDERER/ForwardTechnique.h"
#include "RENDERER/GBufferTechnique.h"
#include "RENDERER/ShadowmapTechnique.h"
#include "RENDERER/RendererDefinitions.h"
#include "RENDERER/Texture.h"
#include "RENDERER/Skybox.h"
#include "RENDERER/Mesh.h"

#include <map>

using namespace std;

namespace Renderer
{
	enum VertexAttribLocation
	{
		vertAttrib_Position_V3 = 0,
		vertAttrib_Normal_V3 = 1,
		vertAttrib_TexCoord_V2 = 2,

		vertAttrib_MVP_M4 = 10
	};

	enum FragDataLocation
	{
		fragData_Position = 0,
		fragData_Albedo,
		fragData_Normal,
		fragData_TexCoord,
	};

#define RENDER_FORWARD		0x01
#define RENDER_TRANSPARENT	0x08

	struct RenderBatch
	{
		GLuint				uVAO;
		size_t				uNbVertex;
		glm::mat4			mWorldMatrix; // by copy instead of pointer to avoid cache miss ? 64o / 8o
		uint8_t				uFlags;
		TextureWeakPtr		xDiffuseMap;
		uint32_t			uObjectId;

#ifdef _DEBUG
		GLVAO*				pVAO;
#endif

		RenderBatch()
			: uVAO(-1)
			, uNbVertex(0)
			, uFlags(0)
			, uObjectId(-1)
		{
			mWorldMatrix = glm::one<glm::mat4>(); // identity
		}
	};


	class GLShader;
	class Camera;
	class Light;
	class PointLight;
	class SpotLight;
	class DirectionalLight;

	typedef map<string, shared_ptr<GLShader>> ShaderMap;

	class GLRenderer
	{
		friend class Camera;
	public:
		GLRenderer();
		~GLRenderer();

		bool						Init(uint32_t uWindowWidth, uint32_t uWindowHeight);
		void						Resize(uint32_t uWindowWidth, uint32_t uWindowHeight);

		TextureWeakPtr				LoadTexture(string sFilename);

		void						AddBatch(RenderBatch& oBatch);
		void						AddLight(Light* pLight);

		bool						Render();
		uint32_t					GetPickingId( uint32_t x, uint32_t y ) const;

		Camera*						GetCamera() const { return m_pCamera; }

	protected:
		bool						ShadowMap();
		bool						PreRenderDeferred();
		bool						RenderOpaqueDeferred();
		bool						LightPassDeferred();
		bool						ForwardPass();
		bool						RenderTransparent();
		bool						RenderCubemap();
		bool						DisplayGBuffer();
		bool						Flush();

		void						SetCamera(Camera* pCamera) { m_pCamera = pCamera; }
		float						GetAspectRatio() const { return (float)m_uWindowWidth / m_uWindowHeight; }

		uint32_t					m_uWindowWidth;
		uint32_t					m_uWindowHeight;

		Camera*						m_pCamera;

		vector<RenderBatch>			m_oOpaqueDeferredBatches;
		vector<RenderBatch>			m_oOpaqueForwardBatches;
		vector<RenderBatch>			m_oTransparencyBatches;
		vector<PointLight*>			m_oPointLights;
		//vector<SpotLight*>			m_oSpotLights;
		DirectionalLight*			m_pSunLight;

		map<string, TexturePtr>		m_oLoadedTextures;

		GBuffer						m_oGBuffer;
		RenderTarget				m_eCurrentTarget;
		DeferredGeometryTechnique	m_oDeferredGeometry;
		DeferredLightingTechnique	m_oDeferredLightingTech;
		ForwardTechnique			m_oForwardTechnique;
		GBufferTechnique			m_oGBufferTechnique;
		ShadowmapTechnique			m_oShadowmapTechnique;
		Skybox						m_oSkybox;
		GLuint						m_uShadowmapFBOId;
		GLuint						m_uShadowmapTexID;
		Mesh m_oCubeMesh;


		enum DebugShape
		{
			DEBUG_SHAPE_CUBE,
			DEBUG_SHAPE_COUNT
		};
		GLVAO						m_debugVAO[DEBUG_SHAPE_COUNT];

	public:

		void DEBUG_DRAW_CUBE( glm::vec3 vPos )
		{
			RenderBatch batch;
			batch.mWorldMatrix = glm::lookAt( vPos, vPos + VEC3_FORWARD, VEC3_UP );
			batch.uNbVertex = 12;
			batch.uObjectId = 0;
			batch.uVAO = m_debugVAO[DEBUG_SHAPE_CUBE].GetID();
			AddBatch( batch );
		}
	};

}

extern Renderer::GLRenderer* g_pRenderer;
