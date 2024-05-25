#pragma once
#include "StringConstants.h"

#include <cControlGameEngine.h>

// Windows call
#include <Windows.h>
#define WIN32_LEAN_AND_MEAN	

struct sMapLoadThreadDetails
{
	sMapLoadThreadDetails(std::vector <glm::vec3>& meshPositions, cControlGameEngine& gameEngineProvided)
		: modelPositions(meshPositions),
		 gameEngine(gameEngineProvided) {}

	bool bThreadRunning = false;

	unsigned int sceneID = 0;

	float drawRange = 0.f;
	float yOffset = 0.f;
	float zOffset = 0.f;
	float scaleValue = 0.f;

	DWORD threadId = 0;

	std::string modelFileName = "";
	std::string textureFileName = "";
	std::string textureName = "";

	std::vector <int> indexList;
	std::vector <glm::vec3>& modelPositions;

	cControlGameEngine& gameEngine;
};

class cMap
{
public:
	cMap();
	~cMap();

	int NUM_OF_ROWS_IN_MAZE = 69;
	int NUM_OF_COLUMNS_IN_MAZE = 115;
	int PRELOAD_THESE_NUMBER_OF_MESHES = 500;

	float drawWithinThisRange = 0.f;

	std::vector<int> mazeVector;
	std::vector<int> treasurePosIndexList;
	std::vector<glm::vec3> maze3DPosList;

	std::vector <HANDLE> mMapThreadHandleList;
	std::vector <sMapLoadThreadDetails*> mMapLoadthreadDetailsList;

	bool InitializeMap(cControlGameEngine& gameEngine);
	void Load3DMaze(cControlGameEngine& gameEngine);
	void LoadWallModel(cControlGameEngine& gameEngine, glm::vec3 modelPos, float index);
	void LoadFloorModel(cControlGameEngine& gameEngine, glm::vec3 modelPos, float index);
	//void LoadTreasuresRandomly(int totalTreasures);
	bool LoadMapFromFile(std::string fileName);

private:
	std::vector <int> mWallIndexList;
	std::vector <int> mFloorIndexList;

	void MakingThreadedMapLoad(cControlGameEngine& gameEngine);
};

