#pragma once

#include <vector>

#ifdef OBJLOADER_ALLOW_QUADS
#define OBJLOADER_MAX_NB_VERTEX 4
#else
#define OBJLOADER_MAX_NB_VERTEX 3
#endif

class OBJLoader
{
public:
	struct OBJFace
	{
		OBJFace()
		{
#ifdef OBJLOADER_ALLOW_QUADS
			uNbVertex = 0;
#endif
			for (int i = 0; i < OBJLOADER_MAX_NB_VERTEX; ++i)
			{
				uPositionIndices[i] = (uint32_t)-1;
				uNormalIndices[i] = (uint32_t)-1;
				uUVIndices[i] = (uint32_t)-1;
			}
		}

#ifdef OBJLOADER_ALLOW_QUADS
		uint8_t uNbVertex;
#endif
		uint32_t uPositionIndices[OBJLOADER_MAX_NB_VERTEX];
		uint32_t uNormalIndices[OBJLOADER_MAX_NB_VERTEX];
		uint32_t uUVIndices[OBJLOADER_MAX_NB_VERTEX];
	};

	bool Load(const char* sFileName);
	bool OptimizeToVAO(std::vector<glm::vec3>& vOutPositions, std::vector<glm::vec3>& vOutNormals, std::vector<glm::vec2>& vOutUVs, std::vector<uint32_t>& vOutIndices);
	bool ExportVAO(const char* sFileName); // export vertices to memory-loading optimized file

protected:
	char m_sObjectName[32];
	std::vector<glm::vec3> m_oPositions;
	std::vector<glm::vec3> m_oNormals;
	std::vector<glm::vec2> m_oUVs;
	std::vector<OBJFace> m_oFaces;
#ifdef OBJLOADER_ALLOW_QUADS
	uint32_t uTotalNbVertex;
#endif
};
