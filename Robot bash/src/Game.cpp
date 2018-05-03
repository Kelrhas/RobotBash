#include "StdAfx.h"
#include "Game.h"

#include <sstream>

#include "ENTITIES/Entity.h"
#include "ENTITIES/EntityManager.h"

#include "RENDERER/DirectionalLight.h"
#include "RENDERER/PointLight.h"
#include "RENDERER/Mesh.h"
#include "RENDERER/GLRenderer.h"

#include "IO\InputMgr.h"

Game::Game(){}

Game::~Game(){}

bool Game::Init()
{
	Renderer::MeshInitData oMeshData;

	// load every entity inside the Data/Entities folder
	WIN32_FIND_DATA data;
	HANDLE hFind;
	const wchar_t* pFolder = L"Data\\entities\\";
	wchar_t pFolderPattern[256];
	wcscpy( pFolderPattern, pFolder );
	wcscat( pFolderPattern, L"*" );

	if( (hFind = FindFirstFile( pFolderPattern, &data )) != INVALID_HANDLE_VALUE )
	{
		do
		{
			tinyxml2::XMLDocument docEntity;

			wchar_t pFilePath[512];
			wcscpy( pFilePath, pFolder );
			wcscat( pFilePath, data.cFileName );

			char pFileName[512];
			WideCharToMultiByte( CP_UTF8, 0, pFilePath, -1, pFileName, 512, "$", NULL );
			if( docEntity.LoadFile( pFileName ) != tinyxml2::XML_SUCCESS )
			{
				LOG_ERROR( "ERROR loading %ls (%s)", pFilePath, pFileName );
				continue;
			}

			Entity* pEntity = nullptr;

			tinyxml2::XMLElement* pRoot = docEntity.FirstChildElement( "Entity" );
			const char* pName = pRoot->Attribute( "name" );
			Assert( pName != nullptr && pName != "" );

			pEntity = new Entity( pName );

			if( tinyxml2::XMLElement* pPosition = pRoot->FirstChildElement( "Position" ) )
			{
				glm::vec3 vPos;
				std::stringstream ss(pPosition->GetText());
				ss >> vPos.x;
				ss.ignore();
				ss >> vPos.y;
				ss.ignore();
				ss >> vPos.z;

				pEntity->GetNode()->WorldTranslate( vPos );
			}

			if( tinyxml2::XMLElement* pRotation = pRoot->FirstChildElement( "Rotation" ) )
			{
				glm::vec3 vAxis;
				float fAngle;
				std::stringstream ss( pRotation->GetText() );
				ss >> vAxis.x;
				ss.ignore();
				ss >> vAxis.y;
				ss.ignore();
				ss >> vAxis.z;
				ss.ignore();
				ss >> fAngle; // degrees

				pEntity->GetNode()->WorldRotate( DEG_TO_RAD(fAngle), vAxis );
			}

			if( tinyxml2::XMLElement* pScale = pRoot->FirstChildElement( "Scale" ) )
			{
				glm::vec3 vScale;
				std::stringstream ss( pScale->GetText() );
				ss >> vScale.x;
				ss.ignore();
				ss >> vScale.y;
				ss.ignore();
				ss >> vScale.z;

				pEntity->GetNode()->WorldScale( vScale );
			}

			if( tinyxml2::XMLElement* pMesh = pRoot->FirstChildElement( "Mesh" ) )
			{
				tinyxml2::XMLElement* pModel = pMesh->FirstChildElement( "Model" );
				oMeshData.sFileName = pModel->GetText();

				tinyxml2::XMLElement* pDiffuse = pMesh->FirstChildElement( "DiffuseMap" );
				if( pDiffuse )
					oMeshData.sDiffuseMap = pDiffuse->GetText();
				else
					oMeshData.sDiffuseMap = "";

				Renderer::Mesh* pMeshComponent = pEntity->AddComponent<Renderer::Mesh>();
				pMeshComponent->InitWithData( &oMeshData );
			}

			g_pEntityMgr->AddEntity( pEntity );

		} while( FindNextFile( hFind, &data ) != 0 );
		FindClose( hFind );
	}
	else
		LOG( "ERROR parsing entities folder: %d", GetLastError() );

	m_pSuzanne = new Entities::Entity("Suzanne");
	Renderer::Mesh* pSuzanneMesh = m_pSuzanne->AddComponent<Renderer::Mesh>();
	oMeshData.sFileName = "Data/models/suzanne.obj";
	oMeshData.sDiffuseMap = "Data/textures/uvtemplate.bmp";
	pSuzanneMesh->InitWithData( &oMeshData );
	m_pSuzanne->GetNode()->WorldTranslate( glm::vec3( 5, 2, 1.5f ) );
	g_pEntityMgr->AddEntity( m_pSuzanne );

	m_pDodecahedron = new Entities::Entity("Dodeca");
	Renderer::Mesh* pDodecaMesh = m_pDodecahedron->AddComponent<Renderer::Mesh>();
	oMeshData.sFileName = "Data/models/dodecahedron.obj";
	oMeshData.sDiffuseMap = "Data/textures/white.bmp";
	pDodecaMesh->InitWithData( &oMeshData );
	m_pDodecahedron->GetNode()->WorldTranslate( glm::vec3( 6, 6, 6 ) );
	m_pDodecahedron->GetNode()->WorldScale( 0.2f );
	Renderer::PointLight* pLight = m_pDodecahedron->AddComponent<Renderer::PointLight>();
	pLight->SetIntensity( 1.f );
	pLight->SetColor( glm::vec3( 1.f, 1.f, 1.f ) );
	pLight->SetConstantAttenuation( 0.16f );
	pLight->SetLinearAttenuation( 0.09f );
	pLight->SetExpAttenuation( 0.03f );
	g_pRenderer->AddLight( pLight );
	g_pEntityMgr->AddEntity( m_pDodecahedron );

	Entities::Entity* pSun = new Entities::Entity("Sun");
	Renderer::DirectionalLight* pSunLight = pSun->AddComponent<Renderer::DirectionalLight>();
	pSunLight->SetIntensity( 1.f );
	pSunLight->SetColor( glm::vec3( 1.f, 1.f, 1.f ) );
	pSunLight->SetDirection( glm::normalize( glm::vec3( -0.9f, -0.308f, -0.308f ) ) );
	g_pRenderer->AddLight( pSunLight );
	g_pEntityMgr->AddEntity( pSun );

	//Entities::Entity* pTestScene = new Entities::Entity("Scene");
	//Renderer::Mesh* pTestSceneMesh = pTestScene->AddComponent<Renderer::Mesh>();
	//oMeshData.sFileName = "Data/models/test scene.obj";
	//oMeshData.sDiffuseMap = "Data/textures/palette.bmp";
	//pTestSceneMesh->InitWithData( &oMeshData );
	//g_pEntityMgr->AddEntity( pTestScene );

	Entities::Entity* p3DArrows = new Entities::Entity("3D arrows");
	Renderer::Mesh* pArrowsMesh = p3DArrows->AddComponent<Renderer::Mesh>();
	oMeshData.sFileName = "Data/models/arrows.obj";
	oMeshData.sDiffuseMap = "Data/textures/palette.bmp";
	oMeshData.bForward = true;
	pArrowsMesh->InitWithData( &oMeshData );
	g_pEntityMgr->AddEntity( p3DArrows );

	return true;
}

bool Game::Update( float fDeltaTime )
{
	//m_pSuzanne->GetNode()->WorldRotate( glm::pi<float>() * fDeltaTime * 0.25f, VEC3_UP );
	//m_pDodecahedron->GetNode()->WorldRotate( glm::pi<float>() * fDeltaTime * 0.25f, VEC3_UP );

	//if( Input::InputMgr::IsMouseLeftDown() )
	//{
	//	glm::vec2 pos = Input::InputMgr::GetCursorPos();
	//	uint32_t id = g_pRenderer->GetPickingId( (uint32_t)pos.x, (uint32_t) pos.y );
	//	Entity* pEntity = g_pEntityMgr->GetEntityByID( id );
	//	if( pEntity )
	//		pEntity->GetNode()->WorldRotate( glm::pi<float>() * fDeltaTime * 0.25f, VEC3_UP );
	//}

	return true;
}