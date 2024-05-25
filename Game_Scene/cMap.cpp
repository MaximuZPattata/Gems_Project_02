#include "pch.h"
#include "cMap.h"

CRITICAL_SECTION LoadModelsCriticalSection;
CRITICAL_SECTION EnterFunctionCriticalSection;
CRITICAL_SECTION SetFlagCriticalSection;

using namespace StringConstants;

cMap::cMap()
{
	InitializeCriticalSection(&LoadModelsCriticalSection);
	InitializeCriticalSection(&SetFlagCriticalSection);
	InitializeCriticalSection(&EnterFunctionCriticalSection);
}

cMap::~cMap()
{
	DeleteCriticalSection(&LoadModelsCriticalSection);
	DeleteCriticalSection(&SetFlagCriticalSection);
	DeleteCriticalSection(&EnterFunctionCriticalSection);
}

unsigned int RandomNumberGenerator(unsigned int maxSize)
{
	// Seeding with a random device
	std::random_device rd;
	std::mt19937 gen(rd());

	// Fixing the random index range
	std::uniform_int_distribution<> dist(0, maxSize);

	return dist(gen);
}


void EnteringCriticalSectionDataToLoadModels(cControlGameEngine& gameEngine, std::string modelName, std::string modelFileName, glm::vec3(modePos), std::vector <std::string> modelFilePathList, 
	std::vector <bool> defaultModelBooleanList, std::vector <float> minimumDistanceList, float scaleValue, unsigned int sceneID, bool useTexture, std::string textureFileName = "", std::string modelTextureName = "")
{
	EnterCriticalSection(&LoadModelsCriticalSection);

	gameEngine.LoadModelsInto3DSpace(modelFileName, modelName, modePos.x, modePos.y, modePos.z);
	gameEngine.ScaleModel(modelName, scaleValue);
	gameEngine.SetMeshSceneId(modelName, sceneID);
	gameEngine.AddLODToMesh(modelName, modelFilePathList, minimumDistanceList, defaultModelBooleanList);
	//gameEngine.TurnMeshLightsOn(modelName);

	if (useTexture)
	{
		gameEngine.UseTextures(modelName, true);
		gameEngine.AddTexturesToOverlap(modelName, textureFileName, modelTextureName);
	}

	LeaveCriticalSection(&LoadModelsCriticalSection);
}

DWORD WINAPI LoadAllWalls(LPVOID lpParameter)
{
	//---------------------Initialize variables----------------------------

	sMapLoadThreadDetails* mapThreadDetails = (sMapLoadThreadDetails*)lpParameter;

	if (mapThreadDetails->bThreadRunning)
	{
		int wallIndex = 0;

		for (int index = 0; index < mapThreadDetails->indexList.size(); index++)
		{
			//--------------------------Initialize all Wall Data------------------------------------

			wallIndex = mapThreadDetails->indexList[index];

			float yPosOffset = mapThreadDetails->yOffset;
			float zPosOffset = mapThreadDetails->zOffset;

			std::vector <std::string> modelFilePathList;
			std::vector <float> minimumDistanceList;
			std::vector <bool> defaultModelBooleanList;

			std::string	modelName = "Wall_" + std::to_string(wallIndex);
			std::string modelFileName = mapThreadDetails->modelFileName;

			modelFilePathList.push_back(modelFileName);
			minimumDistanceList.push_back(mapThreadDetails->drawRange);
			defaultModelBooleanList.push_back(true);

			glm::vec3 modelPos = glm::vec3(mapThreadDetails->modelPositions[wallIndex].x, mapThreadDetails->modelPositions[wallIndex].y + yPosOffset, mapThreadDetails->modelPositions[wallIndex].z + zPosOffset);

			//------------------Enter Critical Section to Load Wall Data----------------------------

			EnterCriticalSection(&EnterFunctionCriticalSection);

			EnteringCriticalSectionDataToLoadModels(mapThreadDetails->gameEngine, modelName, modelFileName, modelPos, modelFilePathList, defaultModelBooleanList, 
				minimumDistanceList, mapThreadDetails->scaleValue, mapThreadDetails->sceneID, true, mapThreadDetails->textureFileName, mapThreadDetails->textureName);

			LeaveCriticalSection(&EnterFunctionCriticalSection);

			Sleep(0);
		}

		std::cout << std::endl;
		std::cout << "LOADED ALL WALLS IN MAZE" << std::endl;
	}

	EnterCriticalSection(&SetFlagCriticalSection);
	mapThreadDetails->bThreadRunning = false;
	LeaveCriticalSection(&SetFlagCriticalSection);

	return 0;
}

DWORD WINAPI LoadAllFloors(LPVOID lpParameter)
{
	//---------------------Initialize variables----------------------------

	sMapLoadThreadDetails* mapThreadDetails = (sMapLoadThreadDetails*)lpParameter;

	if (mapThreadDetails->bThreadRunning)
	{
		int floorIndex = 0;

		for (int index = 0; index < mapThreadDetails->indexList.size(); index++)
		{
			//--------------------------Initialize all Floor Data------------------------------------

			floorIndex = mapThreadDetails->indexList[index];

			float yPosOffset = mapThreadDetails->yOffset;
			float zPosOffset = mapThreadDetails->zOffset;

			std::vector <std::string> modelFilePathList;
			std::vector <float> minimumDistanceList;
			std::vector <bool> defaultModelBooleanList;

			std::string	modelName = "Floor_" + std::to_string(floorIndex);
			std::string modelFileName = mapThreadDetails->modelFileName;
			std::string modelTextureFileName = mapThreadDetails->textureFileName;
			std::string modelTextureName = mapThreadDetails->textureName;

			modelFilePathList.push_back(modelFileName);
			minimumDistanceList.push_back(mapThreadDetails->drawRange);
			defaultModelBooleanList.push_back(true);

			glm::vec3 modelPos = glm::vec3(mapThreadDetails->modelPositions[floorIndex].x, mapThreadDetails->modelPositions[floorIndex].y + yPosOffset, mapThreadDetails->modelPositions[floorIndex].z + zPosOffset);

			//------------------Enter Critical Section to Load Floor Data----------------------------

			EnterCriticalSection(&EnterFunctionCriticalSection);

			EnteringCriticalSectionDataToLoadModels(mapThreadDetails->gameEngine, modelName, modelFileName, modelPos, modelFilePathList, defaultModelBooleanList, 
				minimumDistanceList, mapThreadDetails->scaleValue, mapThreadDetails->sceneID, true, mapThreadDetails->textureFileName, mapThreadDetails->textureName);

			LeaveCriticalSection(&EnterFunctionCriticalSection);

			Sleep(0);
		}

		std::cout << std::endl;
		std::cout << "LOADED ALL FLOORS IN MAZE" << std::endl;
	}

	EnterCriticalSection(&SetFlagCriticalSection);
	mapThreadDetails->bThreadRunning = false;
	LeaveCriticalSection(&SetFlagCriticalSection);

	return 0;
}

bool cMap::InitializeMap(cControlGameEngine& gameEngine)
{
	this->drawWithinThisRange = gameEngine.DRAW_WITHIN_THIS_RANGE;

	if (!LoadMapFromFile("DungeonFile/MyMaze.txt"))
		return false;

	this->Load3DMaze(gameEngine);
	//this->LoadTreasuresRandomly(250);

	return true;
}

bool cMap::LoadMapFromFile(std::string fileName) // Loading the maze from the text file
{
	std::ifstream mazeFile(fileName);

	if (!mazeFile.is_open())
	{
		std::cout << std::endl;
		std::cout << "ERROR : UNABLE TO OPEN MAZE FILE" << std::endl;

		return false;
	}

	char singleCharacter;
	int characterCount = 0;

	while (mazeFile.get(singleCharacter))
	{
		if (singleCharacter == 'X')
			mazeVector.push_back(1);

		else if (singleCharacter == '.')
			mazeVector.push_back(0);

		characterCount++;
	}

	std::cout << std::endl;
	std::cout << "SUCCESS : MAZE FILE READ" << std::endl;

	return true;
}

// Load Wall Model Details
void cMap::LoadWallModel(cControlGameEngine& gameEngine, glm::vec3 modelPos, float index)
{
	float yPosOffset = 18.f;
	float zPosOffset = 18.f;

	std::vector <std::string> modelFilePathList;
	std::vector <float> minimumDistanceList;
	std::vector <bool> defaultModelBooleanList;

	std::string	modelName = "Wall_" + std::to_string(index);
	std::string modelFileName = WallModelFileName;
	std::string modelTextureFileName = WallTextureFileName;
	std::string modelTextureName = WallTextureName;

	modelFilePathList.push_back(modelFileName);
	minimumDistanceList.push_back(this->drawWithinThisRange);
	defaultModelBooleanList.push_back(true);

	gameEngine.LoadModelsInto3DSpace(modelFileName, modelName, modelPos.x, modelPos.y + yPosOffset, modelPos.z + zPosOffset);
	gameEngine.ScaleModel(modelName, 4.5f);
	gameEngine.SetMeshSceneId(modelName, 0);
	gameEngine.AddLODToMesh(modelName, modelFilePathList, minimumDistanceList, defaultModelBooleanList);
	gameEngine.UseTextures(modelName, true);
	gameEngine.AddTexturesToOverlap(modelName, modelTextureFileName, modelTextureName);
	//gameEngine.UseManualColors(modelName, true);
	//gameEngine.ChangeColor(modelName, 0.68f, 0.85f, 0.90f);
	//gameEngine.TurnMeshLightsOn(modelName);
}

// Load Floor Model details
void cMap::LoadFloorModel(cControlGameEngine& gameEngine, glm::vec3 modelPos, float index)
{
	float yPosOffset = 1.f;
	float zPosOffset = 18.f;

	std::vector <std::string> modelFilePathList;
	std::vector <float> minimumDistanceList;
	std::vector <bool> defaultModelBooleanList;

	std::string	modelName = "Floor_" + std::to_string(index);
	std::string modelFileName = FloorModelFileName;
	std::string modelTextureFileName = FloorTextureFileName;
	std::string modelTextureName = FloorTextureName;

	modelFilePathList.push_back(modelFileName);
	minimumDistanceList.push_back(this->drawWithinThisRange);
	defaultModelBooleanList.push_back(true);

	gameEngine.LoadModelsInto3DSpace(modelFileName, modelName, modelPos.x, modelPos.y + yPosOffset, modelPos.z + zPosOffset);
	gameEngine.ScaleModel(modelName, 3.5f);
	gameEngine.SetMeshSceneId(modelName, 0);
	gameEngine.UseTextures(modelName, true);
	gameEngine.AddTexturesToOverlap(modelName, modelTextureFileName, modelTextureName);
	gameEngine.AddLODToMesh(modelName, modelFilePathList, minimumDistanceList, defaultModelBooleanList);
}

void cMap::Load3DMaze(cControlGameEngine& gameEngine)
{
	float xPos = 0.f;
	float yPos = 0.f;
	float zPos = 0.f;
	float distanceBetweenBlocks = 35.f; // 35 units

	int endOfColumnCounter = 0;

	//-------------------------Scan through the maze vector for walls and free spaces and load them------------------------------

	for (int index = 0; index < mazeVector.size(); index++)
	{
		endOfColumnCounter++;

		maze3DPosList.push_back(glm::vec3(xPos, yPos, zPos));

		if (mazeVector[index] == 1)
		{
			if (index < this->PRELOAD_THESE_NUMBER_OF_MESHES)
				LoadWallModel(gameEngine, maze3DPosList[index], index); // PreLoad Meshes
			else
				this->mWallIndexList.push_back(index);
			
			xPos += distanceBetweenBlocks;
		}
		else if (mazeVector[index] == 0)
		{
			xPos += distanceBetweenBlocks;
			
			if (index < this->PRELOAD_THESE_NUMBER_OF_MESHES)
				LoadFloorModel(gameEngine, maze3DPosList[index], index); // PreLoad Meshes
			else
				this->mFloorIndexList.push_back(index);
		}

		if (endOfColumnCounter == NUM_OF_COLUMNS_IN_MAZE)
		{
			xPos = 0;
			zPos += distanceBetweenBlocks;
			endOfColumnCounter = 0;
		}
	}

	this->MakingThreadedMapLoad(gameEngine);
}

void cMap::MakingThreadedMapLoad(cControlGameEngine &gameEngine)
{
	//-----------------------------Creating Thread to Load Walls---------------------------------------

	sMapLoadThreadDetails* loadMapWalls = new sMapLoadThreadDetails(this->maze3DPosList, gameEngine);

	loadMapWalls->drawRange = this->drawWithinThisRange;
	loadMapWalls->scaleValue = 4.5f;
	loadMapWalls->yOffset = 18.f;
	loadMapWalls->zOffset = 18.f;
	loadMapWalls->sceneID = 0;
	loadMapWalls->modelFileName = WallModelFileName;
	loadMapWalls->textureFileName = WallTextureFileName;
	loadMapWalls->textureName = WallTextureName;
	loadMapWalls->indexList = this->mWallIndexList;
	loadMapWalls->threadId = 0;

	this->mMapLoadthreadDetailsList.push_back(loadMapWalls);

	HANDLE threadHandle_01 = CreateThread(NULL, 0, LoadAllWalls, loadMapWalls, 0, &loadMapWalls->threadId);

	if (threadHandle_01 == NULL)
	{
		std::cout << "ERROR : UNABLE TO CREATE THREAD" << std::endl;
	}
	else
		this->mMapThreadHandleList.push_back(threadHandle_01);

	loadMapWalls->bThreadRunning = true;

	//-----------------------------Creating Thread to Load Floors--------------------------------------

	sMapLoadThreadDetails* loadMapFloors = new sMapLoadThreadDetails(this->maze3DPosList, gameEngine);

	loadMapFloors->drawRange = this->drawWithinThisRange;
	loadMapFloors->scaleValue = 3.5f;
	loadMapFloors->yOffset = 1.f;
	loadMapFloors->zOffset = 18.f;
	loadMapFloors->sceneID = 0;
	loadMapFloors->modelFileName = FloorModelFileName;
	loadMapFloors->textureFileName = FloorTextureFileName;
	loadMapFloors->textureName = FloorTextureName;
	loadMapFloors->indexList = this->mFloorIndexList;
	loadMapFloors->threadId = 1;

	this->mMapLoadthreadDetailsList.push_back(loadMapFloors);

	HANDLE threadHandle_02 = CreateThread(NULL, 0, LoadAllFloors, loadMapFloors, 0, &loadMapFloors->threadId);

	if (threadHandle_02 == NULL)
	{
		std::cout << "ERROR : UNABLE TO CREATE THREAD" << std::endl;
	}
	else
		this->mMapThreadHandleList.push_back(threadHandle_02);

	loadMapFloors->bThreadRunning = true;
}

//
//void LoadTreasuresRandomly(int totalTreasures)
//{
//	std::string modelName;
//
//	bool newRandomNumGenerated = false;
//
//	unsigned int randomNumIndex = 0;
//
//	int LeftOfIndex = 0;
//	int RightOfIndex = 0;
//	int TopOfIndex = 0;
//	int BottomOfIndex = 0;
//
//	std::cout << std::endl;
//
//	for (int treasureCount = 1; treasureCount <= totalTreasures; treasureCount++)
//	{
//		randomNumIndex = RandomNumberGenerator(mazeVector.size() - 1);
//		newRandomNumGenerated = false;
//
//		while (!newRandomNumGenerated)
//		{
//			newRandomNumGenerated = true;
//
//			//-----------------To check if there are any treasures already in that location-------------------------
//
//			for (int treasureIndex = 0; treasureIndex < treasurePosIndexList.size(); treasureIndex++)
//			{
//				if (randomNumIndex == treasurePosIndexList[treasureIndex])
//					newRandomNumGenerated = false;
//			}
//
//			//-----------------------To check if there are any walls in that location-------------------------------
//
//			if (mazeVector[randomNumIndex] != 0)
//				newRandomNumGenerated = false;
//
//			//-------------------To check if there are any treasures adjacent to this one--------------------------
//
//			if (newRandomNumGenerated)
//			{
//				if (randomNumIndex - 1 > 0)
//				{
//					if (maze3DPosList[randomNumIndex - 1].z == maze3DPosList[randomNumIndex].z)
//						LeftOfIndex = randomNumIndex - 1;
//					else
//						LeftOfIndex = 0;
//				}
//
//				if (randomNumIndex + 1 < NUM_OF_ROWS_IN_MAZE * NUM_OF_COLUMNS_IN_MAZE)
//				{
//					if (maze3DPosList[randomNumIndex + 1].z == maze3DPosList[randomNumIndex].z)
//						RightOfIndex = randomNumIndex + 1;
//					else
//						RightOfIndex = 0;
//				}
//
//				if (randomNumIndex - NUM_OF_COLUMNS_IN_MAZE > NUM_OF_COLUMNS_IN_MAZE)
//					TopOfIndex = randomNumIndex - NUM_OF_COLUMNS_IN_MAZE;
//				else
//					TopOfIndex = 0;
//
//				if (randomNumIndex + NUM_OF_COLUMNS_IN_MAZE < NUM_OF_ROWS_IN_MAZE * NUM_OF_COLUMNS_IN_MAZE)
//					BottomOfIndex = randomNumIndex + NUM_OF_COLUMNS_IN_MAZE;
//				else
//					BottomOfIndex = 0;
//
//				for (int treasureIndex = 0; treasureIndex < treasurePosIndexList.size(); treasureIndex++)
//				{
//					if (treasurePosIndexList[treasureIndex] == LeftOfIndex || treasurePosIndexList[treasureIndex] == RightOfIndex
//						|| treasurePosIndexList[treasureIndex] == TopOfIndex || treasurePosIndexList[treasureIndex] == BottomOfIndex)
//						newRandomNumGenerated = false;
//				}
//			}
//
//			//----------------------------Generating another random number again-----------------------------------
//
//			if (!newRandomNumGenerated)
//				randomNumIndex = RandomNumberGenerator(mazeVector.size() - 1);
//		}
//
//		modelName = "Treasure_" + std::to_string(randomNumIndex);
//
//		gameEngine.LoadModelsInto3DSpace("Sphere_1_unit_Radius_uv.ply", modelName, maze3DPosList[randomNumIndex].x, 0.5f, maze3DPosList[randomNumIndex].z);
//		gameEngine.ScaleModel(modelName, 0.4f);
//		gameEngine.UseManualColors(modelName, true);
//		gameEngine.ChangeColor(modelName, 0.9, 0.9, 0.0);
//
//		std::cout << "Treasure - " << treasureCount << " | Name - " << modelName
//			<< " | Pos - (" << maze3DPosList[randomNumIndex].x << "," << maze3DPosList[randomNumIndex].y << "," << maze3DPosList[randomNumIndex].z << ")" << std::endl;
//
//		treasurePosIndexList.push_back(randomNumIndex);
//	}
//
//	std::cout << std::endl;
//	std::cout << "LOADED ALL TREASURES" << std::endl;
//}