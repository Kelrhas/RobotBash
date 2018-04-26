#pragma once

class Node
{
public:
	void						WorldTranslate( glm::vec3 vTranslation );
	void						WorldRotate( float fAngle, glm::vec3 vAxis );
	void						WorldScale( float fScale );
	void						WorldScale( glm::vec3 vScale );

	void						UpdateMatrix();

	glm::vec3					GetWorldPosition() const;

	glm::mat4&					GetWorldMatrix() { return m_pWorldMatrix; }
	const glm::mat4&			GetWorldMatrix() const { return m_pWorldMatrix; }
	glm::mat4*					GetWorldMatrixPtr() { return &m_pWorldMatrix; }
	const glm::mat4*			GetWorldMatrixPtr() const { return &m_pWorldMatrix; }

private:
	glm::mat4		m_pLocalMatrix;		// parent wise
	glm::mat4		m_pWorldMatrix;		// world
};