#pragma once

#include "Node.h"

namespace Entities
{
	class EntityComponent;

	class Entity
	{
	public:
		Entity(std::string sName);
		virtual	~Entity();

		virtual bool						Update(float fDeltaTime);
		virtual void						ImGuiDraw();

		template <class T>
		T*									AddComponent()
		{
			T* pComponent = new T();

			if (pComponent->Init( this ) == false)
			{
				Assert(!"Error on component init");
				delete pComponent;
				return nullptr;
			}

			m_oComponents.push_back(pComponent);

			return (T*)pComponent;
		}

				Node*						GetNode() { return &m_oNode; }
				const Node*					GetNode() const { return &m_oNode; }

	protected:
		std::vector<EntityComponent*>		m_oComponents;
		Node								m_oNode;
		std::string							m_sName;
	};
}