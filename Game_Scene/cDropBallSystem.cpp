#include "pch.h"
#include "cDropBallSystem.h"
#include "StringConstants.h"
#include <cControlGameEngine.h>

using namespace StringConstants;

extern void DeleteAllCriticalSections();
extern void MakeThreadCallToActivateBallPhysicsThreads(std::vector < sBallAttributes >& ballAttributesList, cControlGameEngine& gameEngine, std::vector < sBallPhysicsThreadDetails* >& ballPhysicsThreadDetailsList);
extern void DeleteAllRunningThreads(std::vector < sBallPhysicsThreadDetails* >& ballPhysicsThreadDetailsList);

cDropBallSystem::cDropBallSystem()
{}

cDropBallSystem::~cDropBallSystem()
{
	DeleteAllCriticalSections();
	DeleteAllRunningThreads(this->ballPhysicsThreadDetailsList);
}

void cDropBallSystem::CreateBalls(cControlGameEngine &gameEngine)
{
	for (int count = 0; count < this->TOTAL_BALLS_IN_STORAGE; count++)
	{
		std::string	modelName = "Ball_" + std::to_string(count);
		std::string modelFileName = SphereModelFileName;

		gameEngine.LoadModelsInto3DSpace(modelFileName, modelName, 0.f, 0.f, 0.f);
		gameEngine.ScaleModel(modelName, 5.f);
		gameEngine.SetMeshSceneId(modelName, 0);
		gameEngine.UseManualColors(modelName, true);
		gameEngine.ChangeColor(modelName, 1.f, 1.f, 1.f);
		//gameEngine.TurnWireframeModeOn(modelName);
		gameEngine.TurnVisibilityOff(modelName);
		gameEngine.AddSpherePhysicsToMesh(modelName, "Sphere", 5.f);
		gameEngine.ChangeModelPhysicalMass(modelName, 5.f);

		sBallAttributes newBallAttribute;
		newBallAttribute.modelName = modelName;
		newBallAttribute.BallId = count;

		this->BallList.push_back(newBallAttribute);
	}

	MakeThreadCallToActivateBallPhysicsThreads(this->BallList, gameEngine, this->ballPhysicsThreadDetailsList);
}

void cDropBallSystem::DropBalls(cControlGameEngine& gameEngine)
{
	if (this->TOTAL_BALLS_USED < this->TOTAL_BALLS_IN_STORAGE)
	{
		int count = 0;

		glm::vec3 camPos = gameEngine.GetCurrentCameraPosition();
		glm::vec3 camDir = gameEngine.GetCurrentCameraTarget();

		glm::vec3 dropPoint = camPos + camDir * 400.f;

		for (int index = 0; index < this->BallList.size(); index++)
		{
			if (!this->BallList[index].ballDropped)
			{
				std::string modelName = this->BallList[index].modelName;

				float yVelocity = gameEngine.getRandomFloat(-10.f, -3.f);

				gameEngine.MoveModel(this->BallList[index].modelName, dropPoint.x, 50.f, dropPoint.z);
				gameEngine.UpdatePhysicsPosition(this->BallList[index].modelName, glm::vec3(dropPoint.x, 50.f, dropPoint.z));
				gameEngine.TurnVisibilityOn(modelName);
				gameEngine.ChangeColor(modelName, 1.f, 1.f, 1.f);
				gameEngine.ChangeModelPhysicsVelocity(modelName, glm::vec3(0.0f, yVelocity, 0.0f));
				gameEngine.ChangeModelPhysicsAcceleration(modelName, glm::vec3(0.0f, -9.81f, 0.0f));

				count++;
				this->BallList[index].ballDropped = true;
				this->BallList[index].BallTimer = this->BALL_MAX_TIME;
				this->TOTAL_BALLS_USED++;
			}

			if (count >= this->BALLS_DROPPED_ON_CLICK || this->TOTAL_BALLS_USED >= this->TOTAL_BALLS_IN_STORAGE)
				break;
		}
	}
}

void cDropBallSystem::UpdateBallMotion(cControlGameEngine &gameEngine)
{
	for (sBallAttributes& currentBall : this->BallList)
	{
		if (currentBall.ballDropped)
		{
			if (currentBall.BallTimer > 0.f)
			{
				gameEngine.PhysicsEulerIntegration(currentBall.modelName);

				/*glm::vec3 ballPos = gameEngine.GetModelPosition(currentBall.modelName);
				glm::vec3 camPos = gameEngine.GetCurrentCameraPosition();

				if (gameEngine.ComparePhysicsObjectWithAABBs(currentBall.modelName))
					gameEngine.MakeCollisionHappen(currentBall.modelName);

				gameEngine.ComparePhysicalAttributesWithOtherModels();*/

				currentBall.BallTimer -= 1.f * static_cast<float>(gameEngine.deltaTime);
			}

			else if (currentBall.BallTimer <= 0.f)
			{
				gameEngine.ChangeModelPhysicsVelocity(currentBall.modelName, glm::vec3(0.0f, 0.f, 0.0f));
				gameEngine.ChangeModelPhysicsAcceleration(currentBall.modelName, glm::vec3(0.0f, 0.f, 0.0f));
				gameEngine.TurnVisibilityOff(currentBall.modelName);

				currentBall.ballDropped = false;
				this->TOTAL_BALLS_USED--;
			}
		}
	}
}