#include "pch.h"
#include "cAnimationSystem.h"

cAnimationSystem::cAnimationSystem()
{
	if (this->mAnimationCommandsManager == NULL)
		this->mAnimationCommandsManager = new cAnimationCommands();

	this->LowerBodyNodeNameSet.insert("mixamorig_LeftLeg");
	this->LowerBodyNodeNameSet.insert("mixamorig_LeftFoot");
	this->LowerBodyNodeNameSet.insert("mixamorig_LeftUpLeg");
	this->LowerBodyNodeNameSet.insert("mixamorig_LeftToeBase");
	this->LowerBodyNodeNameSet.insert("mixamorig_RightUpLeg");
	this->LowerBodyNodeNameSet.insert("mixamorig_RightLeg");
	this->LowerBodyNodeNameSet.insert("mixamorig_RightFoot");
	this->LowerBodyNodeNameSet.insert("mixamorig_RightToeBase"); 
	this->LowerBodyNodeNameSet.insert("mixamorig_Hips"); 
}

cAnimationSystem::~cAnimationSystem()
{
	delete this->mAnimationCommandsManager;
}

float cAnimationSystem::FindValueAfterEasingEffect(double startKeyFrameTime, double endKeyFrameTime, EasingType easeType, float dt)
{
	float percent = 0.f;

	percent = (dt - startKeyFrameTime) / (endKeyFrameTime - startKeyFrameTime);

	float result = 0.f;

	switch (easeType)
	{
	case EasingType::Linear:
		result = percent;
		break;

	case EasingType::EaseIn:
		result = glm::sineEaseIn(percent);
		break;

	case EasingType::EaseOut:
		result = glm::sineEaseOut(percent);
		break;

	case EasingType::EaseInOut:
		result = glm::sineEaseInOut(percent);
		break;
	}

	return result;
}

glm::mat4 cAnimationSystem::AnimationTransformations(NodeAnimation* nodeAnim, float deltaTime)
{
	glm::vec3 modelPosition = glm::vec3(0.f);
	glm::quat modelRotation = glm::quat(0.f, 0.f, 0.f, 1.f);
	glm::vec3 modelScale = glm::vec3(0.f);

	//------------------------------------------------------------------------------------
	//----------------------HANDLING POSITION KEYFRAMES-----------------------------------
	//------------------------------------------------------------------------------------

	if (nodeAnim->PositionKeys.size() == 1)
	{
		modelPosition = nodeAnim->PositionKeys[0].mPosition;
	}

	else
	{
		int positionKeyFrameEndIndex = 1;

		for (; positionKeyFrameEndIndex < nodeAnim->PositionKeys.size(); positionKeyFrameEndIndex++)
		{
			if (nodeAnim->PositionKeys[positionKeyFrameEndIndex].mTime > deltaTime)
				break;
		}


		if (positionKeyFrameEndIndex >= nodeAnim->PositionKeys.size())
		{
			modelPosition = nodeAnim->PositionKeys[positionKeyFrameEndIndex - 1].mPosition;
		}
		else
		{
			int positionKeyFrameStartIndex = positionKeyFrameEndIndex - 1;

			sPositionKeyFrame positionStartKeyFrame = nodeAnim->PositionKeys[positionKeyFrameStartIndex];
			sPositionKeyFrame positionEndKeyFrame = nodeAnim->PositionKeys[positionKeyFrameEndIndex];

			float result = FindValueAfterEasingEffect(positionStartKeyFrame.mTime, positionEndKeyFrame.mTime, positionEndKeyFrame.mEaseType, deltaTime);

			glm::vec3 delta = positionEndKeyFrame.mPosition - positionStartKeyFrame.mPosition;

			modelPosition = positionStartKeyFrame.mPosition + delta * result;
		}
	}

	//------------------------------------------------------------------------------------
	//----------------------HANDLING ROTATION KEYFRAMES-----------------------------------
	//------------------------------------------------------------------------------------

	if (nodeAnim->RotationKeys.size() == 1)
		modelRotation = nodeAnim->RotationKeys[0].mRotation;
	else
	{
		int rotationKeyFrameEndIndex = 1;

		for (; rotationKeyFrameEndIndex < nodeAnim->RotationKeys.size(); rotationKeyFrameEndIndex++)
		{
			if (nodeAnim->RotationKeys[rotationKeyFrameEndIndex].mTime > deltaTime)
				break;
		}

		if (rotationKeyFrameEndIndex >= nodeAnim->RotationKeys.size())
			modelRotation = nodeAnim->RotationKeys[rotationKeyFrameEndIndex - 1].mRotation;
		else
		{
			int rotationKeyFrameStartIndex = rotationKeyFrameEndIndex - 1;

			sRotationKeyFrame rotationStartKeyFrame = nodeAnim->RotationKeys[rotationKeyFrameStartIndex];
			sRotationKeyFrame rotationEndKeyFrame = nodeAnim->RotationKeys[rotationKeyFrameEndIndex];

			float result = FindValueAfterEasingEffect(rotationStartKeyFrame.mTime, rotationEndKeyFrame.mTime, rotationEndKeyFrame.mEaseType, deltaTime);

			glm::quat offset = glm::slerp(rotationStartKeyFrame.mRotation, rotationEndKeyFrame.mRotation, result);

			modelRotation = glm::quat(offset);
		}
	}
	//------------------------------------------------------------------------------------
	//----------------------HANDLING SCALE KEYFRAMES--------------------------------------
	//------------------------------------------------------------------------------------

	if (nodeAnim->ScalingKeys.size() == 1)
		modelScale = nodeAnim->ScalingKeys[0].mScale;
	else
	{
		int scaleKeyFrameEndIndex = 1;

		for (; scaleKeyFrameEndIndex < nodeAnim->ScalingKeys.size(); scaleKeyFrameEndIndex++)
		{
			if (nodeAnim->ScalingKeys[scaleKeyFrameEndIndex].mTime > deltaTime)
				break;
		}

		if (scaleKeyFrameEndIndex >= nodeAnim->ScalingKeys.size())
			modelScale = nodeAnim->ScalingKeys[scaleKeyFrameEndIndex - 1].mScale;
		else
		{
			int scaleKeyFrameStartIndex = scaleKeyFrameEndIndex - 1;

			sScaleKeyFrame scaleStartKeyFrame = nodeAnim->ScalingKeys[scaleKeyFrameStartIndex];
			sScaleKeyFrame scaleEndKeyFrame = nodeAnim->ScalingKeys[scaleKeyFrameEndIndex];

			float result = FindValueAfterEasingEffect(scaleStartKeyFrame.mTime, scaleEndKeyFrame.mTime, scaleEndKeyFrame.mEaseType, deltaTime);

			glm::vec3 delta = scaleEndKeyFrame.mScale - scaleStartKeyFrame.mScale;

			modelScale = scaleStartKeyFrame.mScale + delta * result;
		}
	}
	//------------------------------------------------------------------------------------
	//-------------------------MATRIX TRANSFORMATION--------------------------------------
	//------------------------------------------------------------------------------------

	glm::mat4 matModel = glm::mat4(1.0f);

	glm::mat4 translate = glm::translate(glm::mat4(1.0f), modelPosition);
	glm::mat4 rotate = glm::mat4(modelRotation);
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), modelScale);

	matModel = matModel * translate;
	matModel = matModel * rotate;
	matModel = matModel * scale;

	return matModel;
}

bool cAnimationSystem::CheckIfNodeCanBeAnimated(Node& nodeAnim, bool isTwoBodyAnimationVariation, bool isLowerBodyAnimation)
{
	// ---------------Checking if the Animation requested is different for Upper and Lower body------------------------

	if (isTwoBodyAnimationVariation)
	{
		if (isLowerBodyAnimation)
		{
			if (LowerBodyNodeNameSet.find(nodeAnim.Name) == LowerBodyNodeNameSet.end())
				return false;
		}
		else
		{
			if (LowerBodyNodeNameSet.find(nodeAnim.Name) != LowerBodyNodeNameSet.end())
				return false;
		}
	}

	return true;
}

void cAnimationSystem::RunBoneAnimation(cMesh* animMesh, AnimationInfo& animationDetails, Node& nodeAnim, float deltaTime, bool isTwoBodyAnimationVariation, bool isLowerBodyAnimation)
{
	//---------------------Finding the Node Name to Process the Animation for that Node--------------------------------

	if (this->CheckIfNodeCanBeAnimated(nodeAnim, isTwoBodyAnimationVariation, isLowerBodyAnimation))
	{
		std::map<std::string, NodeAnimation*>::iterator animIt = animationDetails.NodeAnimations.find(nodeAnim.Name);

		if (animIt != animationDetails.NodeAnimations.end())
		{
			NodeAnimation* nodeAnimInfo = animIt->second;

			glm::mat4 boneTransform = AnimationTransformations(nodeAnimInfo, deltaTime);

			animMesh->mBoneTransformationsMap[nodeAnim.Name] = boneTransform;
		}
	}

	for (Node* child : nodeAnim.Children)
		RunBoneAnimation(animMesh, animationDetails, *child, deltaTime, isTwoBodyAnimationVariation, isLowerBodyAnimation);
}

bool cAnimationSystem::AddAnimationCommand(cMesh* animMesh, sModelDrawInfo* animModel, std::string animationName, float frameRateIncrement, bool loopAnimation, 
	bool releasePrevAnimations, bool isTwoBodyAnimation, bool isLowerBodyAnimation)
{
	//------------------Releasing Previous Animation Commands under this Mesh-------------------------------

	if (releasePrevAnimations)
		this->mAnimationCommandsManager->ReleasePreviousAnimation(animMesh->meshUniqueName);

	//------------------------Adding new Animation Commands to this Mesh------------------------------------

	for (AnimationInfo animationInfo : animModel->AnimationInfoList)
	{
		if (animationInfo.AnimationName == animationName)
		{
			if (this->mAnimationCommandsManager == NULL)
				this->mAnimationCommandsManager = new cAnimationCommands();

			this->mAnimationCommandsManager->AddAnimation(animMesh->meshUniqueName, animationName, frameRateIncrement, animationInfo.Duration, 
				animationInfo.TicksPerSecond, animMesh, animModel, animationInfo, loopAnimation, isTwoBodyAnimation, isLowerBodyAnimation);

			return true;
		}
	}

	//----------------------Returning False if Animation Name Doesnt Match----------------------------------

	return false;
}

void cAnimationSystem::UpdateAnimations(float deltaTime)
{
	if (this->mAnimationCommandsManager != NULL)
	{
		//----------------------------Checking Command Manager for the Mesh Animations List--------------------------------------------

		for (sMeshWithAnimations* meshAnimationsList : this->mAnimationCommandsManager->mMeshWithAnimationsList)
		{
			std::vector < sAnimationCommandInfoInQueue* > animationsCompletedList;

			//--------------------------Checking each Mesh Animations List for the Animation Commands-------------------------------------

			for (sAnimationCommandInfoInQueue* animationCommands : meshAnimationsList->mAnimationInfoInQueueList)
			{
				//--------------------------Updating Animation Commands if Not Completed-------------------------------------

				if (!animationCommands->bAnimationCompleted)
				{
					animationCommands->mAnimationTime += animationCommands->mIncrementFrameRate + animationCommands->mTicksPerSecond * deltaTime;

					this->RunBoneAnimation(animationCommands->mMeshModel, animationCommands->mAnimationToBePlayed, *animationCommands->mModelInfo->RootNode,
						animationCommands->mAnimationTime, animationCommands->bTwoBodyAnimation, animationCommands->bLowerBodyAnimation);

					if (animationCommands->mAnimationTime >= animationCommands->mTotalAnimationDuration)
						animationCommands->bAnimationCompleted = true;
				}

				//------------------Once Completed Animation Commands are Pushed to a Completion List------------------------

				else
					animationsCompletedList.push_back(animationCommands);
			}

			//------------------Completed Animation Commands are either Deleted or Looped to Play Again------------------------

			for (sAnimationCommandInfoInQueue* animationCommand : animationsCompletedList)
			{
				if (animationCommand->bAnimationLooped)
				{
					animationCommand->mAnimationTime = 0.f;
					animationCommand->bAnimationCompleted = false;
				}
				else
					this->mAnimationCommandsManager->DeleteAnimation(meshAnimationsList->mMeshName, animationCommand);
			}
		}
	}
}

bool cAnimationSystem::AreAllPreviousAnimationsCompleted(std::string meshName)
{
	for (sMeshWithAnimations* meshAnimationsList : this->mAnimationCommandsManager->mMeshWithAnimationsList)
	{
		if (meshAnimationsList->mMeshName == meshName)
		{
			return meshAnimationsList->mAnimationInfoInQueueList.empty();
		}
	}

	return false;
}

void cAnimationSystem::UnloopAnimationCommands(std::string meshnName)
{
	this->mAnimationCommandsManager->UnloopAllAnimationCommands(meshnName);
}

void cAnimationSystem::LoopAnimation(std::string meshName, std::string animationName)
{
	this->mAnimationCommandsManager->LoopAnimationCommand(meshName, animationName);
}
