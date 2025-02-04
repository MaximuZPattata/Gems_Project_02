#pragma once

#include "pch.h"
#include "cMesh.h"
#include "cPhysics.h"
#include "cLightManager.h"
#include "cLightHelper.h"
#include "cVAOManager.h"
#include "cShaderManager.h"
#include "cCubeMap.h"
#include "cAABB.h"
#include "TextureManager/cBasicTextureManager.h"
#include "cSoftBody.h"
#include "cDebugRender.h"
#include "cSoftBodyThreaded.h"
#include "cSceneManager.h"
#include "cAnimationSystem.h"
#include "sCameraAttributes.h"

class cControlGameEngine
{
private:
    int gSelectedLight = 0;
    int meshListIndex = 0;
    int lightListIndex = 0;
    int textureUnitIndex = 0;
    int TOTAL_MESH_COUNT = 9'000;

    bool bAABBGenerated = false;
    bool bApplyLOD = true;
    bool bDrawColliders = false;

    GLuint shaderProgramID = 0;
    GLuint cDebugRenderer_vertex_buffer = 0;
    GLuint cDebugRenderer_VAO = 0;

    cAnimationSystem* mAnimationManager = NULL;
    cBasicTextureManager* mTextureManager = NULL;
    cCubeMap* mCubeMapManager = NULL;
    cDebugRenderer* mDebugRenderManager = NULL;
    cLightManager* mLightManager = NULL;
    cPhysics* mPhysicsManager = NULL;
    cSceneManager* mSceneManager = NULL;
    cShaderManager* mShaderManager = NULL;
    cShaderManager* mDebugRendererShaderManager = NULL;
    cSoftBodyThreaded* mSoftBodyThreadManager = NULL;
    cVAOManager* mDebugRendererMeshManager = NULL;
    cVAOManager* mVAOManager = NULL;
    sCameraAttributes* mCamManager = NULL;

    std::vector < sPhysicsProperties* > PhysicsModelList;
    std::vector < cMesh* > TotalMeshList;
    std::vector < sModelDrawInfo* > MeshDrawInfoList;
    std::vector < cMesh* > TotalDebugMeshList;
    std::vector < cMesh* > AABBMeshesList;
    std::vector < cAABB* > AABBList;
    std::vector < cSoftBody* > SoftBodyList;
    std::vector < std::string > TexturePathList;
    std::vector < std::string > MeshFilePathList;

    //std::map< unsigned int, cAABB* > AABBIdMap;

    cShaderManager::cShader vertexShader;
    cShaderManager::cShader fragmentShader;

    int InitializeShader();

    cMesh* g_pFindMeshByFriendlyName(std::string friendlyNameToFind);

    sModelDrawInfo* g_pFindModelInfoByFriendlyName(std::string friendlyNameToFind);

    sPhysicsProperties* FindPhysicalModelByName(std::string modelName);

public:
    double deltaTime = 0.0f;

    float DRAW_WITHIN_THIS_RANGE = 1000.f;
 
    //-------------------COMMON FUNCTIONS--------------------------------------------------

    float getRandomFloat(float num1, float num2);

    //-------------------CAMERA CONTROLS---------------------------------------------------

    void MoveCameraPosition(float translate_x, float translate_y, float translate_z);

    void MoveCameraTarget(float translate_x, float translate_y, float translate_z);
    
    void ShiftCameraView();

    void LockCameraMovement();

    void UnlockCameraMovement();

    void UpdateThirdPersonCamera(const glm::vec3& playerPosition, float playerYaw, float playerPitch);
    
    void SetEditorMode(bool editorModeOn);
    
    void SetLightEditorMode(bool lightEditorModeOn);
    
    void SetCamYaw(float yaw);
    
    void SetCamPitch(float pitch);
    
    void SetCameraHeight(float camHeight);
    
    void SetCameraDistance(float camDistance);
    
    glm::vec3 GetCurrentCameraPosition();
    
    glm::vec3 GetCurrentCameraTarget();
    
    bool IsFreeFlowCamOn();

    bool IsCameraLocked();
    
    bool GetEditorMode();
    
    bool GetLightEditorMode();
    
    float GetCamYaw();
    
    float GetCamPitch();
    
    float GetCamHeight();
    
    float GetCamDistance();

    //-------------------MESH CONTROLS-----------------------------------------------------

    void InitializeMeshCount();

    void ModifyColliderMeshData(std::string modelName, float scaleValue, bool bMeshLightsOn, bool bWireframeOn, bool bUseManualColors, glm::vec3 colors, unsigned int sceneId);

    void LoadAllModelAssets(std::vector<std::string> fileNameList);

    void LoadModelsInto3DSpace(std::string filePath, std::string modelName, float initial_x, float initial_y, float initial_z);

    void LoadColliderModelsInto3DSpace(std::string filePath, std::string modelName, float initial_x, float initial_y, float initial_z);

    sModelDrawInfo* CreateModelDrawInfo(std::string fileName, std::string fullFilePath);

    void DisableLOD();

    void EnableLOD();

    void ToggleColliderDraw();

    void AddLODToMesh(std::string meshName, std::vector <std::string> filePathLOD, std::vector <float> minimumDistance, std::vector <bool> isDefaultLOD);

    void ChangeColor(std::string modelName, float r, float g, float b);

    void UseManualColors(std::string modelName, bool useColor);

    void ScaleModel(std::string modelModel, float scale_value);

    void ScaleModelInIndividualAxis(std::string modelName, float x, float y, float z);

    void MoveModel(std::string modelModel, float translate_x, float translate_y, float translate_z);
    
    void MoveColliderModel(std::string modelModel, float translate_x, float translate_y, float translate_z);

    void RotateMeshModel(std::string modelModel, float angleDegrees, float rotate_x, float rotate_y, float rotate_z);

    void RotateMeshModelUsingQuaternion(std::string modelName, glm::quat rotationQuaternion);

    void TurnVisibilityOn(std::string modelModel);

    void TurnVisibilityOff(std::string modelModel);

    void TurnWireframeModeOn(std::string modelModel);

    void TurnMeshLightsOn(std::string modelModel);

    void DeleteMesh(std::string modelName);

    void ShiftToNextMeshInList();

    void ShiftToPreviousMeshInList();

    void SortMeshesBasedOnTransparency();

    void MakeMeshAABB(std::string modelName, bool isAABB);

    void ToggleMeshBoneWeightColor(std::string modelName);

    void SetMeshSceneId(std::string modelName, unsigned int sceneId);

    bool IsMeshFilePathAlreadyExisting(std::string filePath);

    cMesh* GetCurrentModelSelected();

    int GetMeshCount();

    glm::quat GetModelRotationQuat(std::string modelName);

    glm::vec3 GetModelPosition(std::string modelModel);

    float GetModelScaleValue(std::string modelModel);

    std::vector< cMesh* >  GetMeshList();

    //-------------------BONE CONTROLS-----------------------------------------------------

    void ApplyBonesToMesh(std::string modelName, bool applyBones);

    //-----------------ANIMATION CONTROLS--------------------------------------------------

    void UpdateAllAnimations(float deltaTime);

    void PlayAnimation(std::string modelName, std::string animationName, float frameRateIncrement, bool loopAnimation, bool releasePrevAnimationsForThisMesh, 
        bool isTwoBodyAnimation, bool isLowerBodyAnimation);

    void LoadAnimationIntoExistingMeshModel(std::string existingMeshModelName, std::string fileName, std::string animationName);

    void UnloopPreviousAnimations(std::string modelName);

    void LoopExistingAnimation(std::string modelName, std::string animationName);

    bool ArePreviousMeshAnimationsCompleted(std::string modelName);

    //------------------TEXTURE CONTROLS--------------------------------------------------

    void LoadAllTextures(std::vector <std::string> texturePathList);

    void UseTextures(std::string modelName, bool applyTexture);

    void ApplyHeightMap(std::string modelName, std::string textureName, bool applyHeightMap);

    void AddTexturesToTheMix(std::string modelName, std::string texturePath, std::string textureName, float textureRatio);

    void AddTexturesToOverlap(std::string modelName, std::string texturePath, std::string textureName);

    void AddDiscardMaskTexture(std::string modelName, std::string textureName, std::string discardMaskTexturePath);

    void ChangeTextureRatios(std::string modelName, std::vector <float> textureRatio);

    void AdjustHeightMapScale(std::string modelName, std::string textureName, float height);

    void AdjustHeightMapUVPos(std::string modelName, std::string textureName, glm::vec2 UVOffsetPos);

    void RemoveBlackAreasInHeightMap(std::string modelName, std::string textureName, bool removeBlackAreas);

    void RemoveColoredAreasHeightMap(std::string modelName, std::string textureName, bool removeColoredAreas);

    void ApplyDiscardMaskTexture(std::string modelName, std::string textureName, bool applyDiscardMask);

    void AdjustAlphaTransparency(std::string modelName, float alphaTransparencyLevel);

    void UseDiscardMaskTexture(GLuint shaderProgramID, GLint bUseDiscardMaskTexture_UL, std::string texturePath, int textureUnit);

    bool IsTextureAlreadyLoaded(std::string texturePath);

    //-----------------CUBE MAP CONTROLS--------------------------------------------------

    bool AddCubeMap(std::string modelName, std::string skyBoxName, std::string filePathPosX, std::string filePathNegX, std::string filePathPosY, std::string filePathNegY,
                    std::string filePathPosZ, std::string filePathNegZ);
    void DeleteCubeMap();

    //-------------------AABB CONTROLS----------------------------------------------------

    void CreateSceneAABBs(glm::vec3 startingPoint, glm::ivec3 cubesCount, float cubeSize, std::string meshFilePath);
    
    void AddVerticesToAABB(std::string modelName);
    
    void AddTrianglesToAABB(std::vector <std::string> modelFileNameList, int startIndex, int endIndex);

    //-------------------LIGHT CONTROLS---------------------------------------------------

    void AddLight(glm::vec4 lightPosition, glm::vec4 lightDirection, bool turnOff, float lightType, float linearAtten, float quadraticAtten, glm::vec2 angle,
        glm::vec3 lightColor);

    void TurnOffLight(int lightId, bool turnOff);

    void PositionLight(int lightId, float translate_x, float translate_y, float translate_z);

    void ChangeLightIntensity(int lightId, float linearAttentuation, float quadraticAttentuation);

    void ChangeLightType(int lightId, float lightType);

    void ChangeLightAngle(int lightId, float innerAngle, float outerAngle);

    void ChangeLightDirection(int lightId, float direction_x, float direction_y, float direction_z);

    void ChangeLightColour(int lightId, float color_r, float color_g, float color_b);

    float GetLightLinearAttenuation(int lightId);

    float GetLightQuadraticAttenuation(int lightId);

    float GetLightType(int lightId);

    float GetLightInnerAngle(int lightId);

    float GetLightOuterAngle(int lightId);

    float IsLightOn(int lightId);

    glm::vec3 GetLightPosition(int lightId);

    glm::vec3 GetLightDirection(int lightId);

    glm::vec3 GetLightColor(int lightId);

    void ShiftToNextLightInList();

    int GetCurrentLightSelected();

    //------------------PHYSICS CONTROLS---------------------------------------------------

    void ComparePhysicalAttributesWithOtherModels();

    void MakePhysicsHappen(sPhysicsProperties* physicsModel, std::string Model2, std::string collisionType);

    void ChangeModelPhysicsPosition(std::string modelName, float newPositionX, float newPositionY, float newPositionZ);

    void AddSpherePhysicsToMesh(std::string modelName, std::string physicsMeshType, float objectRadius);

    void AddPlanePhysicsToMesh(std::string modelName, std::string physicsMeshType);

    void ChangeModelPhysicsVelocity(std::string modelName, glm::vec3 velocityChange);

    void ChangeModelPhysicsAcceleration(std::string modelName, glm::vec3 accelerationChange);

    int ChangeModelPhysicalMass(std::string modelName, float mass);

    void ResetPosition(sPhysicsProperties* physicsModel);

    void PhysicsEulerIntegration(std::string modelName);

    bool ComparePhysicsObjectWithAABBs(std::string modelName);

    void UpdatePhysicsPosition(std::string modelName, glm::vec3 modelPos);

    void MakeCollisionHappen(std::string modelName);

    //------------------SOFT BODY CONTROLS-------------------------------------------------

    void CreateVerletSoftBodyWithThread(std::string modelName, glm::vec3 softbodyAcceleration, bool createDebugSpheres, std::string debugModelName,
        float scaleVal, glm::vec4 sphereRGBColor, bool useThreadOnSoftBody);

    void CreateVerletSoftBody(std::string modelName, glm::vec3 softbodyAcceleration, bool createDebugSpheres, bool isBall, bool isChain, bool isCube, bool bApplyDamping);

    void UpdateSoftBodyTransformation();

    void ToggleDebugSphereVisibility(bool turnOnVisibility);

    void ApplyRandomBracingToSoftBody(std::string modelName, unsigned int bracesCount, float distanceBetweenVertices);

    void ApplyWheelBracingToSoftBody(std::string modelName);

    void ApplyImaginaryCenterNodeBracing(std::string modelName);

    void ApplyAccelerationToSoftBody(std::string modelName, glm::vec3 acceleration);

    void ConnectTwoSoftBodies(std::string topSoftBodyName, std::string bottomSoftBodyName);

    void FixSoftBodyPosition(std::string softBodyMeshName, glm::vec3 fixedPosition);

    void TerminateSoftBodyThreads();

    void MoveSoftBodyUsingImaginaryCenterNode(std::string modelName, glm::vec3 nodePos);

    glm::vec3 GetCurrentSoftBodyPosition(std::string modelName);

    //---------------DEBUG RENDERER CONTROLS----------------------------------------------

    void InitializeDebugRenderer(GLuint shaderId);

    void CreateDebugSpheres(std::string debugSphereName, std::vector<glm::vec3> spherePositionsList, float scale, glm::vec4 colourRGBA, unsigned int meshSceneId);
   
    void UpdateDebugSpherePositions(std::string modelName, std::vector<glm::vec3> spherePositionsList);
    
    //----------------SCENE & FBO CONTROLS-------------------------------------------------

    void CreateANewScene(bool isTheMainScene, std::vector <glm::vec3> camPos, std::vector <glm::vec3> camTarget);

    void MakeSceneIntoFBOTexture(unsigned int sceneNum, unsigned int width, unsigned int height);

    void RenderMeshWithFBOTexture(unsigned int sceneNum, std::string modelName, unsigned int frameBufferId);

    void AddSecondPassFilterToScene(std::string modelName, bool useFSQ, bool useStandardColor, bool useChromicAberration, bool UseScreenMeasurements, bool useNightVision);

    void AnimateChromicAmount(std::string modelName, float offsetAmount);

    void SwitchCurrentToMainScene(unsigned int sceneIdToSwitchTo);

    //-------------------ENGINE CONTROLS---------------------------------------------------

    int InitializeGameEngine();

    void RunGameEngine(GLFWwindow* window);
};

