#pragma once
#include"pch.h"
#include "OpenGLCommon.h"

#include<rapidjson/filereadstream.h>
#include<rapidjson/stringbuffer.h>
#include<rapidjson/document.h>

// This struct is created to imitate the scene variables in the json file
struct sSceneDetailsFromFile
{
	unsigned int sceneId;
	unsigned int windowWidth;
	unsigned int windowHeight;

	bool bIsFBOTexture;
	bool bIsMainScene;

	std::vector < glm::vec3 > sceneCameraPositionList;
	std::vector < glm::vec3 > sceneCameraTargetList;
};

// This struct is created to imitate the model variables in the json file
struct sModelDetailsFromFile
{
	unsigned int meshSceneId;

	float modelScaleValue;

	bool meshLightsOn;
	bool wireframeModeOn;
	bool manualColors;
	bool useTextures;
	bool applyHeightMap;
	bool isSkyBox;
	bool applyBones;
	bool useLOD;
	bool isCollider;

	std::string modelName;
	std::string modelFilePath;
	std::string physicsMeshType;

	std::vector <std::string> textureFilePathList;
	std::vector <std::string> discardMaskTextureFilePath;
	std::vector <std::string> textureNameList;
	std::vector <std::string> fileNameLOD;

	std::vector <float> textureRatiosList;
	std::vector <float> minimumDistanceLOD;

	std::vector <bool> blendTexture;
	std::vector <bool> useDiscardMaskTexture;
	std::vector <bool> isDefaultLOD;

	glm::vec3 modelPosition;
	glm::vec3 modelOrientation;
	glm::vec3 modelColorRGB;
};

// This struct is created to imitate the physics variables in the json file
struct sCubeMapDetailsFromFile
{
	std::string cubeMapName;
	std::string filePathPosX;
	std::string filePathNegX;
	std::string filePathPosY;
	std::string filePathNegY;
	std::string filePathPosZ;
	std::string filePathNegZ;
};

// This struct is created to imitate the physics variables in the json file
struct sPhysicsDetailsFromFile
{
	float modelRadius;
	float modelMass;
	bool randomVelocity;

	std::string modelName;

	glm::vec3 modelVelocity;
	glm::vec3 modelAcceleration;
};

// This struct is created to imitate the light variables in the json file
struct sLightDetailsFromFile
{
	float linearAttenuation;
	float quadraticAttenuation;
	float lightType;
	float innerAngle;
	float outerAngle;
	bool lightOff;

	glm::vec3 lightPosition;
	glm::vec3 lightDirection;
	glm::vec3 lightColorRGB;
};

// This class is created to read from a Json file
class cJsonReader
{
public:

	bool ReadScene(const std::string& filePath, std::vector <sSceneDetailsFromFile>& differentSceneDetails, std::vector <sModelDetailsFromFile>& differentModelDetails, sCubeMapDetailsFromFile& cubeMapDetails,
		std::vector<sPhysicsDetailsFromFile>& differentPhysicsDetails, std::vector<sLightDetailsFromFile>& differentLightDetails);

private:

	rapidjson::Document ReadFile(std::string fileName);
};

