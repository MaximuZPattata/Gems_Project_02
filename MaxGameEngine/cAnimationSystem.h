#pragma once
#include "pch.h"
#include "cMesh.h"
#include "sModelDrawInfo.h"
#include "cAnimationCommands.h"

struct sLowerBodyNodes
{


};

class cAnimationSystem
{
public:
	cAnimationSystem();
	~cAnimationSystem();

	void RunBoneAnimation(cMesh* animMesh, AnimationInfo& animationDetails, Node& nodeAnim, float deltaTime, bool isTwoBodyAnimationVariation, bool isLowerBodyAnimation);
	void UpdateAnimations(float deltaTime);
	bool AddAnimationCommand(cMesh* animMesh, sModelDrawInfo* animModel, std::string animationName, float frameRateIncrement, bool loopAnimation, bool releasePrevAnimations, 
		bool isTwoBodyAnimation, bool isLowerBodyAnimation);
	void UnloopAnimationCommands(std::string meshnName);
	void LoopAnimation(std::string meshName, std::string animationName);
	bool AreAllPreviousAnimationsCompleted(std::string meshName);

private:
	cAnimationCommands* mAnimationCommandsManager = NULL;

	std::set <std::string> LowerBodyNodeNameSet;

	glm::mat4 AnimationTransformations(NodeAnimation* nodeAnim, float dt);

	bool CheckIfNodeCanBeAnimated(Node& nodeAnim, bool isTwoBodyAnimationVariation, bool isLowerBodyAnimation);

	float FindValueAfterEasingEffect(double startKeyFrameTime, double endKeyFrameTime, EasingType easeType, float dt);
};
