#include "stdafx.h"
#include "Node.h"



void Node::WorldTranslate( glm::vec3 vTranslation )
{
	m_pWorldMatrix = glm::translate( m_pWorldMatrix, vTranslation );
	UpdateMatrix();
}

void Node::WorldRotate( float fAngle, glm::vec3 vAxis )
{
	m_pWorldMatrix = glm::rotate( m_pWorldMatrix, fAngle, vAxis );
	UpdateMatrix();
}

void Node::WorldScale( float fScale )
{
	m_pWorldMatrix = glm::scale( m_pWorldMatrix, glm::one<glm::vec3>() * fScale );
	UpdateMatrix();
}

void Node::WorldScale( glm::vec3 vScale )
{
	m_pWorldMatrix = glm::scale( m_pWorldMatrix, vScale );
	UpdateMatrix();
}

void Node::UpdateMatrix()
{
	// update the world matrix for the children
}

glm::vec3 Node::GetWorldPosition() const
{
	return m_pWorldMatrix[3];
}