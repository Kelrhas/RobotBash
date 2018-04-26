#pragma once

#include "ogl.h"

namespace Renderer
{
	class Batch
	{
	public:
		Batch();
		~Batch();

	protected:
		GLuint			m_uVAO;
		glm::mat4*		m_mWorldMatrix;
	};
}