#pragma once
#include "pch.h"
#include <cControlGameEngine.h>

// Windows call
#include <Windows.h>
#define WIN32_LEAN_AND_MEAN	

struct sLoadAABBThreadDetails
{
	sLoadAABBThreadDetails(cControlGameEngine& gameEngineProvided)
		: gameEngine(gameEngineProvided) {}

	bool bThreadRunning = false;

	int startIndex = 0;
	int endIndex = 0;

	DWORD threadId = 0;

	std::vector < std::string > FilePathList;

	cControlGameEngine& gameEngine;

	HANDLE threadHandleCreated = NULL;
};
