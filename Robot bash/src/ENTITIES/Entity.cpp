#include "StdAfx.h"
#include "Entity.h"

#include "EntityComponent.h"

namespace Entities
{
	Entity::Entity( std::string sName )
		: m_sName( sName )
	{
	}

	Entity::~Entity()
	{
	}

	bool Entity::Update(float fDeltaTime)
	{
		for (std::vector<EntityComponent*>::iterator it(m_oComponents.begin()); it != m_oComponents.end(); ++it)
		{
			Assert_return_result((*it)->Update(fDeltaTime));
		}

		return true;
	}

	void Entity::ImGuiDraw()
	{
		if( ImGui::TreeNode( m_sName.c_str() ) )
		{

			for( std::vector<EntityComponent*>::iterator it( m_oComponents.begin() ); it != m_oComponents.end(); ++it )
			{
				(*it)->ImGuiDraw();
			}

			ImGui::TreePop();
		}
	}
	
}