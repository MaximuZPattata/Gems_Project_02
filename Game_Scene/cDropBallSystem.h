#pragma once
#include "pch.h"
#include <cControlGameEngine.h>
#include "sBallAttributes.h"
#include "sBallPhysicsThreadDetails.h"

class cDropBallSystem
{
public:
	cDropBallSystem();
	~cDropBallSystem();

	void CreateBalls(cControlGameEngine &gameEngine);
	void UpdateBallMotion(cControlGameEngine &gameEngine);
	void DropBalls(cControlGameEngine &gameEngine);

private:
	std::vector <sBallAttributes> BallList;

	int TOTAL_BALLS_IN_STORAGE = 30;
	int BALLS_DROPPED_ON_CLICK = 1;
	int TOTAL_BALLS_USED = 0;

	float BALL_MAX_TIME = 7.5f;

	std::vector < sBallPhysicsThreadDetails* > ballPhysicsThreadDetailsList;
};

