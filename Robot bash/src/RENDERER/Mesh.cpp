#include "StdAfx.h"
#include "Mesh.h"

#include "Node.h"

#include "ENTITIES/Entity.h"

#include "RENDERER/GLVAO.h"
#include "RENDERER/GLRenderer.h"

#include "IO/OBJLoader.h"

namespace Renderer
{
	Mesh::Mesh()
		: m_uNbVertex(0)
		, m_pVAO(nullptr)
	{
	}

	Mesh::~Mesh()
	{
		delete m_pVAO;
	}

	bool Mesh::Init( Entities::Entity* pOwner )
	{
		Assert_return_result( EntityComponent::Init( pOwner ) );

		return true;
	}

	bool Mesh::InitWithData(const Entities::InitData* pInitData)
	{
		Assert_return_result(Entities::EntityComponent::InitWithData(pInitData));

		MeshInitData* pData = (MeshInitData*)pInitData;

		m_pVAO = new GLVAO();

		Assert_return_result(Load(pData->sFileName.c_str()));
		if(pData->sDiffuseMap != "")
			m_xDiffuseMap = g_pRenderer->LoadTexture(pData->sDiffuseMap);

		m_bForwardRendering = pData->bForward;

		m_sName = pData->sFileName;
		glObjectLabel( GL_VERTEX_ARRAY, m_pVAO->GetID(), -1, m_sName.c_str() );

		return true;
	}

	bool Mesh::Update(float fDeltaTime)
	{
		Assert_return_result(Render());

		return true;
	}

	bool Mesh::Load(const char* sFileName)
	{
		std::vector<glm::vec3> vOutPositions;
		std::vector<glm::vec3> vOutNormals;
		std::vector<glm::vec2> vOutUVs;
		std::vector<uint32_t> vOutIndices;

		//m_xShaderProgram = g_pRenderer->LoadShaderProgram("Data/shaders/base.shader");

		OBJLoader modelLoader;
		modelLoader.Load(sFileName);
		modelLoader.OptimizeToVAO(vOutPositions, vOutNormals, vOutUVs, vOutIndices);
		
		// TODO get location from technique instead of Renderer::VAO_BUFFER_INDEX
		m_pVAO->SetData(Renderer::VAO_BUFFER_INDEX, &vOutIndices[0], (int)vOutIndices.size());
		m_pVAO->SetData(Renderer::VAO_BUFFER_POSITION, &vOutPositions[0], (int)vOutPositions.size());
		if(!vOutNormals.empty())
			m_pVAO->SetData(Renderer::VAO_BUFFER_NORMAL, &vOutNormals[0], (int)vOutNormals.size());
		if(!vOutUVs.empty())
			m_pVAO->SetData(Renderer::VAO_BUFFER_UV, &vOutUVs[0], (int)vOutUVs.size());

		m_uNbVertex = vOutIndices.size();

		return true;
	}

	bool Mesh::Render()
	{
		RenderBatch batch;
		batch.uVAO = m_pVAO->GetID();
		batch.pWorldMatrix = GetOwner()->GetNode()->GetWorldMatrixPtr();
		batch.uNbVertex = m_uNbVertex;
		if (m_bForwardRendering)
			batch.uFlags |= RENDER_FORWARD;
		batch.xDiffuseMap = m_xDiffuseMap;
		g_pRenderer->AddBatch(batch);

		return true;
	}

}