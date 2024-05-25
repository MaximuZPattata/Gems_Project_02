#pragma once
#include "pch.h"

struct sBallAttributes
{
	int BallId;

	float BallTimer = 0.f;

	bool ballDropped = false;

	std::string modelName = "";
};