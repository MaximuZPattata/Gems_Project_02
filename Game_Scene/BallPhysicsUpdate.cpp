#include "pch.h"
#include "sBallPhysicsThreadDetails.h"
#include "sBallAttributes.h"

// Windows call
#include <Windows.h>
#define WIN32_LEAN_AND_MEAN	

CRITICAL_SECTION GetModelDetailsCriticalSection;
CRITICAL_SECTION CompareSphereWithAABBCriticalSection;
CRITICAL_SECTION CompareSphereWithSphereCriticalSection;

void UpdateBallPhysics(cControlGameEngine &gameEngine, sBallAttributes &currentBall)
{
	EnterCriticalSection(&GetModelDetailsCriticalSection);

	glm::vec3 ballPos = gameEngine.GetModelPosition(currentBall.modelName);
	glm::vec3 camPos = gameEngine.GetCurrentCameraPosition();

	LeaveCriticalSection(&GetModelDetailsCriticalSection);

	EnterCriticalSection(&CompareSphereWithAABBCriticalSection);

	if (gameEngine.ComparePhysicsObjectWithAABBs(currentBall.modelName))
		gameEngine.MakeCollisionHappen(currentBall.modelName);

	LeaveCriticalSection(&CompareSphereWithAABBCriticalSection);

	EnterCriticalSection(&CompareSphereWithSphereCriticalSection);

	gameEngine.ComparePhysicalAttributesWithOtherModels();

	LeaveCriticalSection(&CompareSphereWithSphereCriticalSection);
}

DWORD WINAPI UpdateBallPhysics0_4(LPVOID lpParameter)
{
	//---------------------Initialize variables----------------------------

	sBallPhysicsThreadDetails* ballPhysicsDetails = (sBallPhysicsThreadDetails*)lpParameter;

	//--------------------------Run Thread---------------------------------

	while(ballPhysicsDetails->bThreadRunning)
	{
		for (int index = ballPhysicsDetails->startId; index < ballPhysicsDetails->endId; index++)
		{
			if (ballPhysicsDetails->ballAttributesList[index].ballDropped)
			{
				UpdateBallPhysics(ballPhysicsDetails->gameEngine, ballPhysicsDetails->ballAttributesList[index]);
			}
		}

		Sleep(0.f);
	}

	//--------------------Set Thread Run to false--------------------------

	ballPhysicsDetails->bThreadRunning = false;

	return 0;
}

DWORD WINAPI UpdateBallPhysics4_8(LPVOID lpParameter)
{
	//---------------------Initialize variables----------------------------

	sBallPhysicsThreadDetails* ballPhysicsDetails = (sBallPhysicsThreadDetails*)lpParameter;

	//--------------------------Run Thread---------------------------------

	while (ballPhysicsDetails->bThreadRunning)
	{
		for (int index = ballPhysicsDetails->startId; index < ballPhysicsDetails->endId; index++)
		{
			if (ballPhysicsDetails->ballAttributesList[index].ballDropped)
			{
				UpdateBallPhysics(ballPhysicsDetails->gameEngine, ballPhysicsDetails->ballAttributesList[index]);
			}
		}

		Sleep(0.f);
	}

	//--------------------Set Thread Run to false--------------------------

	ballPhysicsDetails->bThreadRunning = false;

	return 0;
}

DWORD WINAPI UpdateBallPhysics8_12(LPVOID lpParameter)
{
	//---------------------Initialize variables----------------------------

	sBallPhysicsThreadDetails* ballPhysicsDetails = (sBallPhysicsThreadDetails*)lpParameter;

	//--------------------------Run Thread---------------------------------

	while (ballPhysicsDetails->bThreadRunning)
	{
		for (int index = ballPhysicsDetails->startId; index < ballPhysicsDetails->endId; index++)
		{
			if (ballPhysicsDetails->ballAttributesList[index].ballDropped)
			{
				UpdateBallPhysics(ballPhysicsDetails->gameEngine, ballPhysicsDetails->ballAttributesList[index]);
			}
		}

		Sleep(0.f);
	}

	//--------------------Set Thread Run to false--------------------------

	ballPhysicsDetails->bThreadRunning = false;

	return 0;
}

DWORD WINAPI UpdateBallPhysics12_16(LPVOID lpParameter)
{
	//---------------------Initialize variables----------------------------

	sBallPhysicsThreadDetails* ballPhysicsDetails = (sBallPhysicsThreadDetails*)lpParameter;

	//--------------------------Run Thread---------------------------------

	while (ballPhysicsDetails->bThreadRunning)
	{
		for (int index = ballPhysicsDetails->startId; index < ballPhysicsDetails->endId; index++)
		{
			if (ballPhysicsDetails->ballAttributesList[index].ballDropped)
			{
				UpdateBallPhysics(ballPhysicsDetails->gameEngine, ballPhysicsDetails->ballAttributesList[index]);
			}
		}

		Sleep(0.f);
	}

	//--------------------Set Thread Run to false--------------------------

	ballPhysicsDetails->bThreadRunning = false;

	return 0;
}

DWORD WINAPI UpdateBallPhysics16_20(LPVOID lpParameter)
{
	//---------------------Initialize variables----------------------------

	sBallPhysicsThreadDetails* ballPhysicsDetails = (sBallPhysicsThreadDetails*)lpParameter;

	//--------------------------Run Thread---------------------------------

	while (ballPhysicsDetails->bThreadRunning)
	{
		for (int index = ballPhysicsDetails->startId; index < ballPhysicsDetails->endId; index++)
		{
			if (ballPhysicsDetails->ballAttributesList[index].ballDropped)
			{
				UpdateBallPhysics(ballPhysicsDetails->gameEngine, ballPhysicsDetails->ballAttributesList[index]);
			}
		}

		Sleep(0.f);
	}

	//--------------------Set Thread Run to false--------------------------

	ballPhysicsDetails->bThreadRunning = false;

	return 0;
}

DWORD WINAPI UpdateBallPhysics20_24(LPVOID lpParameter)
{
	//---------------------Initialize variables----------------------------

	sBallPhysicsThreadDetails* ballPhysicsDetails = (sBallPhysicsThreadDetails*)lpParameter;

	//--------------------------Run Thread---------------------------------

	while (ballPhysicsDetails->bThreadRunning)
	{
		for (int index = ballPhysicsDetails->startId; index < ballPhysicsDetails->endId; index++)
		{
			if (ballPhysicsDetails->ballAttributesList[index].ballDropped)
			{
				UpdateBallPhysics(ballPhysicsDetails->gameEngine, ballPhysicsDetails->ballAttributesList[index]);
			}
		}

		Sleep(0.f);
	}

	//--------------------Set Thread Run to false--------------------------

	ballPhysicsDetails->bThreadRunning = false;

	return 0;
}

DWORD WINAPI UpdateBallPhysics24_28(LPVOID lpParameter)
{
	//---------------------Initialize variables----------------------------

	sBallPhysicsThreadDetails* ballPhysicsDetails = (sBallPhysicsThreadDetails*)lpParameter;

	//--------------------------Run Thread---------------------------------

	while (ballPhysicsDetails->bThreadRunning)
	{
		for (int index = ballPhysicsDetails->startId; index < ballPhysicsDetails->endId; index++)
		{
			if (ballPhysicsDetails->ballAttributesList[index].ballDropped)
			{
				UpdateBallPhysics(ballPhysicsDetails->gameEngine, ballPhysicsDetails->ballAttributesList[index]);
			}
		}

		Sleep(0.f);
	}

	//--------------------Set Thread Run to false--------------------------

	ballPhysicsDetails->bThreadRunning = false;

	return 0;
}

DWORD WINAPI UpdateBallPhysics28_30(LPVOID lpParameter)
{
	//---------------------Initialize variables----------------------------

	sBallPhysicsThreadDetails* ballPhysicsDetails = (sBallPhysicsThreadDetails*)lpParameter;

	//--------------------------Run Thread---------------------------------

	while (ballPhysicsDetails->bThreadRunning)
	{
		for (int index = ballPhysicsDetails->startId; index < ballPhysicsDetails->endId; index++)
		{
			if (ballPhysicsDetails->ballAttributesList[index].ballDropped)
			{
				UpdateBallPhysics(ballPhysicsDetails->gameEngine, ballPhysicsDetails->ballAttributesList[index]);
			}
		}

		Sleep(0.f);
	}

	//--------------------Set Thread Run to false--------------------------

	ballPhysicsDetails->bThreadRunning = false;

	return 0;
}

void MakeThreadCallToActivateBallPhysicsThreads(std::vector < sBallAttributes > &ballAttributesList, cControlGameEngine &gameEngine, std::vector < sBallPhysicsThreadDetails* > &ballPhysicsThreadDetailsList)
{
	InitializeCriticalSection(&GetModelDetailsCriticalSection);
	InitializeCriticalSection(&CompareSphereWithAABBCriticalSection);
	InitializeCriticalSection(&CompareSphereWithSphereCriticalSection);

	//-------------------------------------------------Thread_01-------------------------------------------------------------------

	sBallPhysicsThreadDetails* ballPhysicsDetails01 = new sBallPhysicsThreadDetails(gameEngine, ballAttributesList);

	ballPhysicsDetails01->startId = 0;
	ballPhysicsDetails01->endId = 4;
	ballPhysicsDetails01->threadId = 0;

	HANDLE threadHandle_01 = CreateThread(NULL, 0, UpdateBallPhysics0_4, ballPhysicsDetails01, 0, &ballPhysicsDetails01->threadId);

	if (threadHandle_01 == NULL)
	{
		std::cout << "ERROR : UNABLE TO CREATE THREAD" << std::endl;
	}
	else
		ballPhysicsDetails01->threadHandleCreated = threadHandle_01;

	ballPhysicsDetails01->bThreadRunning = true;
	ballPhysicsThreadDetailsList.push_back(ballPhysicsDetails01);

	//-------------------------------------------------Thread_02-------------------------------------------------------------------

	sBallPhysicsThreadDetails* ballPhysicsDetails02 = new sBallPhysicsThreadDetails(gameEngine, ballAttributesList);

	ballPhysicsDetails02->startId = 4;
	ballPhysicsDetails02->endId = 8;
	ballPhysicsDetails02->threadId = 1;

	HANDLE threadHandle_02 = CreateThread(NULL, 0, UpdateBallPhysics4_8, ballPhysicsDetails02, 0, &ballPhysicsDetails02->threadId);

	if (threadHandle_02 == NULL)
	{
		std::cout << "ERROR : UNABLE TO CREATE THREAD" << std::endl;
	}
	else
		ballPhysicsDetails02->threadHandleCreated = threadHandle_02;

	ballPhysicsDetails02->bThreadRunning = true;
	ballPhysicsThreadDetailsList.push_back(ballPhysicsDetails02);

	//-------------------------------------------------Thread_03-------------------------------------------------------------------

	sBallPhysicsThreadDetails* ballPhysicsDetails03 = new sBallPhysicsThreadDetails(gameEngine, ballAttributesList);

	ballPhysicsDetails03->startId = 8;
	ballPhysicsDetails03->endId = 12;
	ballPhysicsDetails03->threadId = 2;

	HANDLE threadHandle_03 = CreateThread(NULL, 0, UpdateBallPhysics8_12, ballPhysicsDetails03, 0, &ballPhysicsDetails03->threadId);

	if (threadHandle_03 == NULL)
	{
		std::cout << "ERROR : UNABLE TO CREATE THREAD" << std::endl;
	}
	else
		ballPhysicsDetails03->threadHandleCreated = threadHandle_03;

	ballPhysicsDetails03->bThreadRunning = true;
	ballPhysicsThreadDetailsList.push_back(ballPhysicsDetails03);

	//-------------------------------------------------Thread_04-------------------------------------------------------------------

	sBallPhysicsThreadDetails* ballPhysicsDetails04 = new sBallPhysicsThreadDetails(gameEngine, ballAttributesList);

	ballPhysicsDetails04->startId = 12;
	ballPhysicsDetails04->endId = 16;
	ballPhysicsDetails04->threadId = 3;

	HANDLE threadHandle_04 = CreateThread(NULL, 0, UpdateBallPhysics12_16, ballPhysicsDetails04, 0, &ballPhysicsDetails04->threadId);

	if (threadHandle_04 == NULL)
	{
		std::cout << "ERROR : UNABLE TO CREATE THREAD" << std::endl;
	}
	else
		ballPhysicsDetails04->threadHandleCreated = threadHandle_04;

	ballPhysicsDetails04->bThreadRunning = true;
	ballPhysicsThreadDetailsList.push_back(ballPhysicsDetails04);

	//-------------------------------------------------Thread_05-------------------------------------------------------------------

	sBallPhysicsThreadDetails* ballPhysicsDetails05 = new sBallPhysicsThreadDetails(gameEngine, ballAttributesList);

	ballPhysicsDetails05->startId = 16;
	ballPhysicsDetails05->endId = 20;
	ballPhysicsDetails05->threadId = 4;

	HANDLE threadHandle_05 = CreateThread(NULL, 0, UpdateBallPhysics16_20, ballPhysicsDetails05, 0, &ballPhysicsDetails05->threadId);

	if (threadHandle_05 == NULL)
	{
		std::cout << "ERROR : UNABLE TO CREATE THREAD" << std::endl;
	}
	else
		ballPhysicsDetails05->threadHandleCreated = threadHandle_05;

	ballPhysicsDetails05->bThreadRunning = true;
	ballPhysicsThreadDetailsList.push_back(ballPhysicsDetails05);

	//-------------------------------------------------Thread_06-------------------------------------------------------------------

	sBallPhysicsThreadDetails* ballPhysicsDetails06 = new sBallPhysicsThreadDetails(gameEngine, ballAttributesList);

	ballPhysicsDetails06->startId = 20;
	ballPhysicsDetails06->endId = 24;
	ballPhysicsDetails06->threadId = 5;

	HANDLE threadHandle_06 = CreateThread(NULL, 0, UpdateBallPhysics20_24, ballPhysicsDetails06, 0, &ballPhysicsDetails06->threadId);

	if (threadHandle_06 == NULL)
	{
		std::cout << "ERROR : UNABLE TO CREATE THREAD" << std::endl;
	}
	else
		ballPhysicsDetails06->threadHandleCreated = threadHandle_06;

	ballPhysicsDetails06->bThreadRunning = true;
	ballPhysicsThreadDetailsList.push_back(ballPhysicsDetails06);

	//-------------------------------------------------Thread_07-------------------------------------------------------------------

	sBallPhysicsThreadDetails* ballPhysicsDetails07 = new sBallPhysicsThreadDetails(gameEngine, ballAttributesList);

	ballPhysicsDetails07->startId = 24;
	ballPhysicsDetails07->endId = 28;
	ballPhysicsDetails07->threadId = 6;

	HANDLE threadHandle_07 = CreateThread(NULL, 0, UpdateBallPhysics24_28, ballPhysicsDetails07, 0, &ballPhysicsDetails07->threadId);

	if (threadHandle_07 == NULL)
	{
		std::cout << "ERROR : UNABLE TO CREATE THREAD" << std::endl;
	}
	else
		ballPhysicsDetails07->threadHandleCreated = threadHandle_07;

	ballPhysicsDetails07->bThreadRunning = true;
	ballPhysicsThreadDetailsList.push_back(ballPhysicsDetails07);

	//-------------------------------------------------Thread_08-------------------------------------------------------------------

	sBallPhysicsThreadDetails* ballPhysicsDetails08 = new sBallPhysicsThreadDetails(gameEngine, ballAttributesList);

	ballPhysicsDetails08->startId = 28;
	ballPhysicsDetails08->endId = 30;
	ballPhysicsDetails08->threadId = 7;

	HANDLE threadHandle_08 = CreateThread(NULL, 0, UpdateBallPhysics28_30, ballPhysicsDetails08, 0, &ballPhysicsDetails08->threadId);

	if (threadHandle_08 == NULL)
	{
		std::cout << "ERROR : UNABLE TO CREATE THREAD" << std::endl;
	}
	else
		ballPhysicsDetails08->threadHandleCreated = threadHandle_08;

	ballPhysicsDetails08->bThreadRunning = true;
	ballPhysicsThreadDetailsList.push_back(ballPhysicsDetails08);
}

void DeleteAllCriticalSections()
{
	DeleteCriticalSection(&GetModelDetailsCriticalSection);
	DeleteCriticalSection(&CompareSphereWithAABBCriticalSection);
	DeleteCriticalSection(&CompareSphereWithSphereCriticalSection);
}

void DeleteAllRunningThreads(std::vector < sBallPhysicsThreadDetails* > &ballPhysicsThreadDetailsList)
{
	for (sBallPhysicsThreadDetails* currentBallDetails : ballPhysicsThreadDetailsList)
	{
		if (currentBallDetails->bThreadRunning)
			currentBallDetails->bThreadRunning = false;

		CloseHandle(currentBallDetails->threadHandleCreated);
	}
}