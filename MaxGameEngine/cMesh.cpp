#include "pch.h"
#include "cMesh.h"

unsigned int cMesh::m_nextUniqueID = cMesh::FIRST_UNIQUE_ID;

cMesh::cMesh()
{
	this->drawPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	this->drawOrientation = glm::vec3(0.0f, 0.0f, 0.0f);
	this->wholeObjectManualColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	this->drawScale = glm::vec3(1.0f);
	this->alphaTransparency = 1.0f;

	this->bIsWireframe = false;
	this->bDoNotLight = false;
	this->bIsVisible = true;
	this->bUseManualColours = false;
	this->bUseTextures = false;
	this->bIsAABBMesh = false;
	this->bIsSkyBox = false;

	// Having just 4 mixtures for now 
	mMixedTextures.textureRatiosList[0] = 1.f;
	mMixedTextures.textureRatiosList[1] = 0.f;
	mMixedTextures.textureRatiosList[2] = 0.f;
	mMixedTextures.textureRatiosList[3] = 0.f;

	this->m_UniqueID = cMesh::m_nextUniqueID;

	cMesh::m_nextUniqueID++;
}

cMesh::~cMesh()
{

}

unsigned int cMesh::getUniqueID(void)
{
	return this->m_UniqueID;
}

void cMesh::setUniformDrawScale(float scale)
{
	this->drawScale.x = this->drawScale.y = this->drawScale.z = scale;
	return;
}

glm::vec3 cMesh::getDrawPosition(void)
{
	return this->drawPosition;
}

void cMesh::setDrawPosition(const glm::vec3& newPosition)
{
	this->drawPosition = newPosition;
	return;
}

glm::vec3 cMesh::getDrawOrientation(void)
{
	return glm::eulerAngles(this->get_qOrientation());
}

void cMesh::setDrawOrientation(const glm::quat& newOrientation, glm::vec3 newAngles)
{
	this->m_qOrientation = newOrientation;

	//glm::vec3 newAngles = glm::degrees(glm::eulerAngles(newOrientation));

	/*this->drawOrientation.x = std::fmod(this->drawOrientation.x + newAngles.x, 360.0f);
	this->drawOrientation.y = std::fmod(this->drawOrientation.y + newAngles.y, 360.0f);
	this->drawOrientation.z = std::fmod(this->drawOrientation.z + newAngles.z, 360.0f);*/

	this->drawOrientation.x = newAngles.x;
	this->drawOrientation.y = newAngles.y;
	this->drawOrientation.z = newAngles.z;

	return;
}

void cMesh::setDrawOrientationQuaternion(const glm::quat& newOrientation)
{
	this->m_qOrientation = newOrientation;

	return;
}

void cMesh::setDrawScale(float x, float y, float z)
{
	this->drawScale.x = x;
	this->drawScale.y = y;
	this->drawScale.z = z;
}

bool cMesh::CheckForLOD(glm::vec3 cameraPosition)
{
	float distanceBetweenMeshAndCamEye = glm::distance(cameraPosition, this->drawPosition);
	float minimumDistance = FLT_MAX;

	bool isWithinDrawRange = false;

	for (sLevelOfDetail& meshLOD : this->mLODList) // If it's within the LOD range it will draw high/low res models
	{
		if (meshLOD.minimumDistanceForLOD <= minimumDistance)
		{
			if (distanceBetweenMeshAndCamEye < meshLOD.minimumDistanceForLOD)
			{
				this->modelDrawInfo = meshLOD.modelLODDrawInfo;

				minimumDistance = meshLOD.minimumDistanceForLOD;

				isWithinDrawRange = true;
			}
		}
	}

	return isWithinDrawRange;
}


