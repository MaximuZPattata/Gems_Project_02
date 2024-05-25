#pragma once
#include "pch.h"
#include <cControlGameEngine.h>
#include "sBallAttributes.h"

// Windows call
#include <Windows.h>
#define WIN32_LEAN_AND_MEAN	

struct sBallPhysicsThreadDetails
{
	sBallPhysicsThreadDetails(cControlGameEngine& gameEngineProvided, std::vector < sBallAttributes > &ballAttributesListProvided)
		: gameEngine(gameEngineProvided) ,
		ballAttributesList(ballAttributesListProvided) {}

	int startId = 0;
	int endId = 0;

	bool bThreadRunning = false;

	DWORD threadId = 0;

	std::vector < sBallAttributes > &ballAttributesList;

	cControlGameEngine& gameEngine;

	HANDLE threadHandleCreated = NULL;
};
