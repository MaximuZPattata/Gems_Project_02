#include "pch.h"
#include "sLoadAABBThreadDetails.h"

// Windows call
#include <Windows.h>
#define WIN32_LEAN_AND_MEAN	

DWORD WINAPI LoadOtherAABBs(LPVOID lpParameter)
{
	//---------------------Initialize variables----------------------------

	sLoadAABBThreadDetails* aabbThreadDetails = (sLoadAABBThreadDetails*)lpParameter;

	//--------------------------Run Thread--------------------------------- 

	if (aabbThreadDetails->bThreadRunning)
	{
		aabbThreadDetails->gameEngine.AddTrianglesToAABB(aabbThreadDetails->FilePathList, aabbThreadDetails->startIndex, aabbThreadDetails->endIndex);
	}

	//--------------------Set Thread Run to false--------------------------

	std::cout << std::endl;
	std::cout << "LOADED ALL TRINAGLES INTO THE AABBs" << std::endl;

	aabbThreadDetails->bThreadRunning = false;

	return 0;
}

void MakeThreadCallToLoadOtherAABBs(sLoadAABBThreadDetails* loadOtherAABBs, std::vector < std::string > ModelFilePathList, int startingIndex, int endingIndex)
{
	loadOtherAABBs->FilePathList = ModelFilePathList;
	loadOtherAABBs->startIndex = startingIndex;
	loadOtherAABBs->endIndex = endingIndex;
	loadOtherAABBs->threadId = 0;

	HANDLE threadHandle_01 = CreateThread(NULL, 0, LoadOtherAABBs, loadOtherAABBs, 0, &loadOtherAABBs->threadId);

	if (threadHandle_01 == NULL)
	{
		std::cout << "ERROR : UNABLE TO CREATE THREAD" << std::endl;
	}
	else
		loadOtherAABBs->threadHandleCreated = threadHandle_01;

	loadOtherAABBs->bThreadRunning = true;
}

