#include "stdafx.h"
#include "EntityManager.h"

#include "Entity.h"

Entities::EntityManager* g_pEntityMgr = nullptr;

namespace Entities
{
	EntityManager::EntityManager()
	{}

	EntityManager::~EntityManager()
	{}

	bool EntityManager::Init()
	{
		return true;
	}

	bool EntityManager::Update( float fDeltaTime )
	{
		for( auto it = m_oEntities.begin(); it != m_oEntities.end(); ++it )
		{
			(*it)->Update( fDeltaTime );
		}

		return true;
	}

	void EntityManager::ImGuiDraw()
	{
		for( auto it = m_oEntities.begin(); it != m_oEntities.end(); ++it )
		{
			(*it)->ImGuiDraw();
		}
	}

	void EntityManager::Reset()
	{}

	void EntityManager::AddEntity( Entity * pEntity )
	{
		m_oEntities.push_back( pEntity );
	}

	void EntityManager::RemoveEntity( Entity * pEntity )
	{
		for( auto it = m_oEntities.begin(); it != m_oEntities.end(); ++it )
		{
			if( *it == pEntity )
			{
				m_oEntities.erase( it );
			}
		}
	}
}