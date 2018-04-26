#include "StdAfx.h"
#include "OBJLoader.h"
#include <list>

bool OBJLoader::Load(const char* sFileName)
{
	FILE* pFile = nullptr;
	fopen_s(&pFile, sFileName, "r");

	Assert_return_result(pFile != nullptr);

	char sHeader[32];
	while (fscanf(pFile, "%s", sHeader) != EOF)
	{
		if (strcmp(sHeader, "o") == 0)
		{
			fscanf(pFile, "%s", m_sObjectName);
		}
		else if (strcmp(sHeader, "v") == 0) // face
		{
			glm::vec3 vPoint;
			fscanf(pFile, "%f %f %f", &vPoint.x, &vPoint.y, &vPoint.z);
			m_oPositions.push_back(vPoint);
		}
		else if (strcmp(sHeader, "vn") == 0) // normal
		{
			glm::vec3 vNormal;
			fscanf(pFile, "%f %f %f", &vNormal.x, &vNormal.y, &vNormal.z);
			m_oNormals.push_back(vNormal);
		}
		else if (strcmp(sHeader, "vt") == 0)
		{
			glm::vec2 vUV;
			fscanf(pFile, "%f %f", &vUV.x, &vUV.y);
			m_oUVs.push_back(vUV);
		}
		else if (strcmp(sHeader, "f") == 0)
		{
			OBJFace f;
			// f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3
			fscanf(pFile, "%d/%d/%d", &f.uPositionIndices[0], &f.uUVIndices[0], &f.uNormalIndices[0]);
			fscanf(pFile, "%d/%d/%d", &f.uPositionIndices[1], &f.uUVIndices[1], &f.uNormalIndices[1]);
			fscanf(pFile, "%d/%d/%d", &f.uPositionIndices[2], &f.uUVIndices[2], &f.uNormalIndices[2]);
			m_oFaces.push_back(f);
		}
	}

	fclose(pFile);
	return true;
}


int GetVertexIndex(const std::vector<glm::vec3>& oPositionsVec, const std::vector<glm::vec3>& oNormalsVec, const std::vector<glm::vec2>& oUVsVec,
	const glm::vec3& pPosition, const glm::vec3* pNormal, const glm::vec2* pUV)
{
	std::vector<glm::vec3>::const_iterator itPositionFound = std::find(oPositionsVec.begin(), oPositionsVec.end(), pPosition);

	if (itPositionFound == oPositionsVec.end())
		return -1;

	std::vector<uint32_t> oPositionsFound;
	while (itPositionFound != oPositionsVec.end())
	{
		oPositionsFound.push_back((uint32_t)std::distance(oPositionsVec.begin(), itPositionFound));
		itPositionFound = std::find(++itPositionFound, oPositionsVec.end(), pPosition);
	}

	std::vector<uint32_t> oNormalsFound;
	if (pNormal != nullptr)
	{
		std::vector<glm::vec3>::const_iterator itNormalFound = std::find(oNormalsVec.begin(), oNormalsVec.end(), *pNormal);
		while (itNormalFound != oNormalsVec.end())
		{
			// only add the indices that are already in the positions vector
			uint32_t iNormalIndex = (uint32_t)std::distance(oNormalsVec.begin(), itNormalFound);
			std::vector<uint32_t>::const_iterator itIndexFound = std::find(oPositionsFound.begin(), oPositionsFound.end(), iNormalIndex);

			if (itIndexFound != oPositionsFound.end())
				oNormalsFound.push_back(iNormalIndex);

			itNormalFound = std::find(++itNormalFound, oNormalsVec.end(), *pNormal);
		}

		if (oNormalsFound.empty())
			return -1;
	}

	std::vector<uint32_t> oUVsFound;
	if (pUV != nullptr)
	{
		std::vector<glm::vec2>::const_iterator itUVFound = std::find(oUVsVec.begin(), oUVsVec.end(), *pUV);
		while (itUVFound != oUVsVec.end())
		{
			// and only add the indices that are already in the normals vector (and therefore in the positions vector)
			uint32_t iUVIndex = (uint32_t)std::distance(oUVsVec.begin(), itUVFound);
			
			if (pNormal != nullptr) // no normal so, serach in positions instead
			{
				std::vector<uint32_t>::iterator itIndexFound = oNormalsFound.end();
				itIndexFound = std::find(oNormalsFound.begin(), oNormalsFound.end(), iUVIndex);

				if (itIndexFound != oNormalsFound.end())
					oUVsFound.push_back(iUVIndex);
			}
			else
			{
				std::vector<uint32_t>::iterator itIndexFound = oPositionsFound.end();
				itIndexFound = std::find(oPositionsFound.begin(), oPositionsFound.end(), iUVIndex);

				if (itIndexFound != oPositionsFound.end())
					oUVsFound.push_back(iUVIndex);
			}
			itUVFound = std::find(++itUVFound, oUVsVec.end(), *pUV);
		}


		if (oUVsFound.empty())
			return -1;
	}

	Assert(oNormalsFound.size() < 2 || oUVsFound.size() < 2);

	return (int)oUVsFound[0];
}

bool OBJLoader::OptimizeToVAO(std::vector<glm::vec3>& vOutPositions, std::vector<glm::vec3>& vOutNormals, std::vector<glm::vec2>& vOutUVs, std::vector<uint32_t>& vOutIndices)
{
	Assert_return_result(vOutPositions.size() == 0 && vOutNormals.size() == 0 && vOutUVs.size() == 0 && vOutIndices.size() == 0);
	if (m_oFaces.size() <= 0 || m_oPositions.size() <= 0)
	{
		LOG_ERROR("%d faces and %d vertices", (int)m_oFaces.size(), (int)m_oPositions.size());
		return false;
	}


	bool bHasNormals = !m_oNormals.empty();
	bool bHasUVs = !m_oUVs.empty();


	for (int iFace = 0; iFace < m_oFaces.size(); ++iFace)
	{
		OBJFace& f = m_oFaces[iFace];
		for (int iFaceVertex = 0; iFaceVertex < OBJLOADER_MAX_NB_VERTEX; ++iFaceVertex)
		{
			glm::vec3& vCurrentPosition = m_oPositions[f.uPositionIndices[iFaceVertex] - 1];

			if (!bHasNormals && !bHasUVs) // already optimized since no data beside positions, just copy the positions
			{
				vOutPositions.push_back(vCurrentPosition);
				vOutIndices.push_back((uint32_t)vOutPositions.size() -1);
			}
			else
			{

				int iVertexIndex = GetVertexIndex(vOutPositions, vOutNormals, vOutUVs,
					vCurrentPosition,
					bHasNormals ? &m_oNormals[f.uNormalIndices[iFaceVertex] - 1] : nullptr,
					bHasUVs ? &m_oUVs[f.uUVIndices[iFaceVertex] - 1] : nullptr);


				if (iVertexIndex == -1)
				{
					vOutPositions.push_back(vCurrentPosition);
					if (bHasNormals)
						vOutNormals.push_back(m_oNormals[f.uNormalIndices[iFaceVertex] - 1]);
					if (bHasUVs)
						vOutUVs.push_back(m_oUVs[f.uUVIndices[iFaceVertex] - 1]);
					vOutIndices.push_back((uint32_t)vOutPositions.size() - 1);
				}
				else
				{
					vOutIndices.push_back(iVertexIndex);
				}
			}
		}

		// compute the normal if not given
		if (!bHasNormals)
		{
			glm::vec3 vAToB = vOutPositions[iFace * 3 + 2] - vOutPositions[iFace * 3];
			glm::vec3 vAToC = vOutPositions[iFace * 3 + 1] - vOutPositions[iFace * 3];
			glm::vec3 vNormal = normalize(cross(vAToC, vAToB));

			// add three times, one for each position
			vOutNormals.push_back(vNormal);
			vOutNormals.push_back(vNormal);
			vOutNormals.push_back(vNormal);
		}
	}

	/*for (int iFace = 0; iFace < m_oFaces.size(); ++iFace)
	{
		OBJFace& f = m_oFaces[iFace];
		for (int iFaceVertex = 0; iFaceVertex < OBJLOADER_MAX_NB_VERTEX; ++iFaceVertex)
		{
			glm::vec3& vCurrentPosition = m_oPositions[f.uPositionIndices[iFaceVertex] - 1];
			glm::vec3& vCurrentNormal = m_oNormals[f.uNormalIndices[iFaceVertex] - 1];
			glm::vec2& vCurrentUV = m_oUVs[f.uUVIndices[iFaceVertex] - 1];

			vOutPositions.push_back(vCurrentPosition);
			vOutNormals.push_back(vCurrentNormal);
			vOutUVs.push_back(vCurrentUV);
			vOutIndices.push_back(vOutPositions.size() - 1);
		}
	}*/

	LOG("%d positions %d normals %d UVs %d indices", (int)vOutPositions.size(), (int)vOutNormals.size(), (int)vOutUVs.size(), (int)vOutIndices.size());
	if(bHasNormals || bHasUVs)
		Assert_return_result(vOutPositions.size() == vOutNormals.size() && vOutNormals.size() == vOutUVs.size());

	return true;
}

bool OBJLoader::ExportVAO(const char* sFileName)
{
	std::vector<glm::vec3> vOutPositions;
	std::vector<glm::vec3> vOutNormals;
	std::vector<glm::vec2> vOutUVs;
	std::vector<uint32_t> vOutIndices;

	Assert_return_result( OptimizeToVAO(vOutPositions, vOutNormals, vOutUVs, vOutIndices) );

	FILE* pFile = nullptr;
	fopen_s(&pFile, sFileName, "w");
	Assert_return_result(pFile != nullptr);

	// output first the vertex attributes
	size_t uNbVertex = vOutPositions.size();
	fwrite(&uNbVertex, sizeof(size_t), 1, pFile);

	for (int i = 0; i < uNbVertex; ++i)
	{
		fwrite(&vOutPositions[i], sizeof(float), 3, pFile);
		fwrite(&vOutNormals[i], sizeof(float), 3, pFile);
		fwrite(&vOutUVs[i], sizeof(float), 2, pFile);
	}

	// and then the indices
	size_t uNbIndices = vOutIndices.size();
	fwrite(&uNbIndices, sizeof(size_t), 1, pFile);

	for (int i = 0; i < uNbIndices; ++i)
	{
		fwrite(&vOutIndices[i], sizeof(uint32_t), 3, pFile);
	}


	fclose(pFile);
	return true;
}