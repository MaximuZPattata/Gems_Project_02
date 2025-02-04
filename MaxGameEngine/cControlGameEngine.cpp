#include "pch.h"
#include "cControlGameEngine.h"
#include "cVAOManager.h"
#include "cShaderManager.h"
#include "GLWF_CallBacks.h"

//-------------------------------------------------Private Functions-----------------------------------------------------------------------

cMesh* cControlGameEngine::g_pFindMeshByFriendlyName(std::string friendlyNameToFind)
{
    for (unsigned int index = 0; index != TotalMeshList.size(); index++)
    {
        if (TotalMeshList[index]->meshUniqueName == friendlyNameToFind)
        {
            return TotalMeshList[index];
        }
    }

    std::cout << "ERROR : COULDN'T FIND MESH - MESH NAME : " << friendlyNameToFind << std::endl;
    std::cout << "<<PLEASE CHECK THE MESH NAME IN JSON FILE OR THE MODEL FILE LOCATION FOR THE MODEL>>" << std::endl;
    std::cout << std::endl;

    return NULL;
}

sModelDrawInfo* cControlGameEngine::g_pFindModelInfoByFriendlyName(std::string modelFilePath)
{
    for (unsigned int index = 0; index != MeshDrawInfoList.size(); index++)
    {
        if (MeshDrawInfoList[index]->meshFileName == modelFilePath)
        {
            return MeshDrawInfoList[index];
        }
    }

    std::cout << "ERROR : COULDN'T FIND MODEL INFO - MODEL NAME : " << modelFilePath << std::endl;
    std::cout << "<<PLEASE CHECK THE MESH NAME IN JSON FILE OR THE MODEL FILE LOCATION FOR THE MODEL>>" << std::endl;
    std::cout << std::endl;

    return NULL;
}

sPhysicsProperties* cControlGameEngine::FindPhysicalModelByName(std::string modelName)
{
    for (unsigned int index = 0; index != PhysicsModelList.size(); index++)
    {
        if (PhysicsModelList[index]->modelName == modelName)
        {
            return PhysicsModelList[index];
        }
    }

    std::cout << "ERROR : COULDN'T FIND PHYSICS MODEL - MODEL NAME : " << modelName << std::endl;
    std::cout << "<<PLEASE CHECK THE MESH NAME IN JSON FILE OR THE MODEL FILE LOCATION FOR THE MODEL>>" << std::endl;
    std::cout << std::endl;

    return NULL;
}

int cControlGameEngine::InitializeShader()
{
    mShaderManager = new cShaderManager();

    mShaderManager->setBasePath("Assets/Shaders");

    vertexShader.fileName = "vertexShaderWithUV.glsl";

    fragmentShader.fileName = "fragmentShaderWithUV.glsl";

    if (!mShaderManager->createProgramFromFile("shader01", vertexShader, fragmentShader))
    {
        std::cout << "ERROR: UNABLE TO COMPILE/LINK SHADER" << std::endl;
        std::cout << mShaderManager->getLastError();
        return -1;
    }

    shaderProgramID = mShaderManager->getIDFromFriendlyName("shader01");

    return 0;
}

//---------------------------------------------------Public Functions-----------------------------------------------------------------------

//--------------------------------------COMMON FUNCTIONS---------------------------------------------------------------

float cControlGameEngine::getRandomFloat(float num1, float num2)
{
    float randomNum = (static_cast<float>(rand())) / (static_cast<float>(RAND_MAX));
    float difference = num2 - num1;
    float product = randomNum * difference;

    return num1 + product;
}

//--------------------------------------CAMERA CONTROLS----------------------------------------------------------------

void cControlGameEngine::LockCameraMovement()
{
    this->mCamManager->bLockCamera = true;
}

void cControlGameEngine::UnlockCameraMovement()
{
    this->mCamManager->bLockCamera = false;
}

void cControlGameEngine::MoveCameraPosition(float translate_x, float translate_y, float translate_z)
{
     this->mCamManager->mCameraEye = glm::vec3(translate_x, translate_y, translate_z);
}

void cControlGameEngine::MoveCameraTarget(float translate_x, float translate_y, float translate_z)
{
     this->mCamManager->mCameraTarget = glm::vec3(translate_x, translate_y, translate_z);
}

void cControlGameEngine::ShiftCameraView()
{
    this->mCamManager->bFreeFlowCamera = !this->mCamManager->bFreeFlowCamera;
}

void cControlGameEngine::UpdateThirdPersonCamera(const glm::vec3& playerPosition, float playerYaw, float playerPitch)
{
    if (!this->mCamManager->bFreeFlowCamera)
    {
        glm::vec3 cameraOffset(0.0f, this->mCamManager->mCameraHeight, this->mCamManager->mCameraDistance);

        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(playerYaw), glm::vec3(0.0f, 1.0f, 0.0f));
        rotationMatrix = glm::rotate(rotationMatrix, glm::radians(playerPitch), glm::vec3(1.0f, 0.0f, 0.0f));

        this->mCamManager->mCameraEye = playerPosition + glm::vec3(rotationMatrix * glm::vec4(cameraOffset, 1.0f));

        this->mCamManager->mCameraTarget = playerPosition + glm::vec3(0.f, 15.f, 0.f);
    }
}

void cControlGameEngine::SetEditorMode(bool editorModeOn)
{
    this->mCamManager->bEditorModeOn = editorModeOn;
}

void cControlGameEngine::SetLightEditorMode(bool lightEditorModeOn)
{
    this->mCamManager->bLightEditorMode = lightEditorModeOn;
}

void cControlGameEngine::SetCamYaw(float yaw)
{
    this->mCamManager->mYaw = yaw;
}

void cControlGameEngine::SetCamPitch(float pitch)
{
    this->mCamManager->mPitch = pitch;
}

void cControlGameEngine::SetCameraHeight(float camHeight)
{
    this->mCamManager->mCameraHeight = camHeight;
}

void cControlGameEngine::SetCameraDistance(float camDistance)
{
    this->mCamManager->mCameraDistance = camDistance;
}

bool cControlGameEngine::IsFreeFlowCamOn()
{
    return this->mCamManager->bFreeFlowCamera;
}

bool cControlGameEngine::IsCameraLocked()
{
    return this->mCamManager->bLockCamera;
}

glm::vec3 cControlGameEngine::GetCurrentCameraPosition()
{
    return   this->mCamManager->mCameraEye;
}

glm::vec3 cControlGameEngine::GetCurrentCameraTarget()
{
    return  this->mCamManager->mCameraTarget;
}

bool cControlGameEngine::GetEditorMode()
{
    return this->mCamManager->bEditorModeOn;
}

bool cControlGameEngine::GetLightEditorMode()
{
    return this->mCamManager->bLightEditorMode;
}

float cControlGameEngine::GetCamYaw()
{
    return this->mCamManager->mYaw;
}

float cControlGameEngine::GetCamPitch()
{
    return this->mCamManager->mPitch;
}

float cControlGameEngine::GetCamHeight()
{
    return this->mCamManager->mCameraHeight;
}

float cControlGameEngine::GetCamDistance()
{
    return this->mCamManager->mCameraDistance;
}

//--------------------------------------MESH CONTROLS-----------------------------------------------------------------

void cControlGameEngine::InitializeMeshCount()
{
    this->TotalMeshList.reserve(this->TOTAL_MESH_COUNT);
}

void cControlGameEngine::ToggleColliderDraw()
{
    this->bDrawColliders = !this->bDrawColliders;
}

void cControlGameEngine::ModifyColliderMeshData(std::string modelName, float scaleValue, bool bMeshLightsOn, bool bWireframeOn, bool bUseManualColors, glm::vec3 colors, unsigned int sceneId)
{
    for (cMesh* currentMesh : this->AABBMeshesList)
    {
        if (currentMesh->meshUniqueName == modelName)
        {
            currentMesh->setUniformDrawScale(scaleValue);
            currentMesh->bDoNotLight = bMeshLightsOn;
            currentMesh->bIsWireframe = bWireframeOn;
            currentMesh->bUseManualColours = bUseManualColors;
            currentMesh->wholeObjectManualColourRGBA = glm::vec4(colors.x, colors.y, colors.z, 1.f);
            currentMesh->sceneId = sceneId;
        }
    }
}

sModelDrawInfo* cControlGameEngine::CreateModelDrawInfo(std::string fileName, std::string fullFilePath)
{
    sModelDrawInfo* newModel = NULL;

    if (!this->IsMeshFilePathAlreadyExisting(fileName))
    {
        newModel = new sModelDrawInfo();

        std::string newUniqueModelDrawInfoName = "Model_Info_" + std::to_string(this->getRandomFloat(0.0, 9999.9)) + "_" + fileName;

        if (!this->mVAOManager->LoadModelIntoVAO(newUniqueModelDrawInfoName, fileName, *newModel, shaderProgramID))
        {
            std::cout << "ERROR : UNABLE TO LOAD MODEL INFO - FILE NAME : " << fileName << std::endl;
            return NULL;
        }

        this->MeshDrawInfoList.push_back(newModel);
        this->MeshFilePathList.push_back(fileName);
    }
    else
    {
        for (sModelDrawInfo* existingModel : this->MeshDrawInfoList)
        {
            if (fullFilePath == existingModel->meshFileName)
            {
                newModel = existingModel;

                break;
            }
        }
    }

    return newModel;
}

void cControlGameEngine::LoadAllModelAssets(std::vector<std::string> fileNameList)
{
    for (int index = 0; index < fileNameList.size(); index++)
    {
        sModelDrawInfo* newModel = new sModelDrawInfo();

        std::string newUniqueModelDrawInfoName = "Model_Info_" + std::to_string(this->getRandomFloat(0.0, 9999.9)) + "_" + fileNameList[index];

        if (!this->mVAOManager->LoadModelIntoVAO(newUniqueModelDrawInfoName, fileNameList[index], *newModel, shaderProgramID))
        {
            std::cout << "ERROR : UNABLE TO LOAD MODEL INFO - FILE NAME : " << fileNameList[index] << std::endl;
            return;
        }

        this->MeshDrawInfoList.push_back(newModel);
        this->MeshFilePathList.push_back(fileNameList[index]);
    }
}

void cControlGameEngine::LoadModelsInto3DSpace(std::string filePath, std::string modelName, float initial_x, float initial_y, float initial_z)
{
    //--------------------------Checking if the mesh file path is already loaded by the engine--------------------------------------

    std::string fullFilePath = this->mVAOManager->getBaseModelPath() + "/" + filePath;

    sModelDrawInfo* modelDrawInfo = CreateModelDrawInfo(filePath, fullFilePath);

    if (modelDrawInfo == NULL)
        return;

    //------------------------------------------Creating new mesh model-------------------------------------------------------------

    cMesh* newMesh = new cMesh();

    newMesh->meshFileName = fullFilePath;

    newMesh->meshUniqueName = modelName;

    newMesh->drawPosition = glm::vec3(initial_x, initial_y, initial_z);

    newMesh->modelDrawInfo = modelDrawInfo;

    this->TotalMeshList.push_back(newMesh);
}

void cControlGameEngine::LoadColliderModelsInto3DSpace(std::string filePath, std::string modelName, float initial_x, float initial_y, float initial_z)
{
    //--------------------------Checking if the mesh file path is already loaded by the engine--------------------------------------

    std::string fullFilePath = this->mVAOManager->getBaseModelPath() + "/" + filePath;

    sModelDrawInfo* modelDrawInfo = CreateModelDrawInfo(filePath, fullFilePath);

    if (modelDrawInfo == NULL)
        return;

    //------------------------------------------Creating new mesh model-------------------------------------------------------------

    cMesh* newMesh = new cMesh();

    newMesh->meshFileName = fullFilePath;

    newMesh->meshUniqueName = modelName;

    newMesh->drawPosition = glm::vec3(initial_x, initial_y, initial_z);

    newMesh->modelDrawInfo = modelDrawInfo;

    this->AABBMeshesList.push_back(newMesh);
}

int cControlGameEngine::GetMeshCount()
{
    return this->TotalMeshList.size();
}

void cControlGameEngine::AddLODToMesh(std::string meshName, std::vector <std::string> filePathLOD, std::vector <float> minimumDistance, std::vector <bool> isDefaultLOD)
{
    cMesh* meshFound = this->g_pFindMeshByFriendlyName(meshName);

    meshFound->bUseLOD = true;

    for (int count = 0; count < filePathLOD.size(); count++)
    {
        sLevelOfDetail newLOD;

        //--------------------------Checking if the mesh file path is already loaded by the engine--------------------------------------

        std::string fullFilePath = this->mVAOManager->getBaseModelPath() + "/" + filePathLOD[count];

        sModelDrawInfo* modelDrawInfo = CreateModelDrawInfo(filePathLOD[count], fullFilePath);

        if (modelDrawInfo == NULL)
            return;

        //-------------------------------------------Add LOD Details to Mesh------------------------------------------------------------
        
        newLOD.modelLODDrawInfo = modelDrawInfo;

        newLOD.minimumDistanceForLOD = minimumDistance[count];

        meshFound->mLODList.push_back(newLOD);
    }
}

void cControlGameEngine::DisableLOD()
{
    this->bApplyLOD = false;
}

void cControlGameEngine::EnableLOD()
{
    this->bApplyLOD = true;
}

void cControlGameEngine::ChangeColor(std::string modelName, float r, float g, float b)
{
    cMesh* meshFound = g_pFindMeshByFriendlyName(modelName);

    if (meshFound == NULL)
        return;
    
    meshFound->wholeObjectManualColourRGBA = glm::vec4(r, g, b, 1.0f);
}

void cControlGameEngine::UseManualColors(std::string modelName, bool useColor)
{
    cMesh* meshFound = g_pFindMeshByFriendlyName(modelName);

    if (meshFound == NULL)
        return;

    if (useColor)
        meshFound->bUseManualColours = true;
    else
        meshFound->bUseManualColours = false;
}

void cControlGameEngine::ScaleModel(std::string modelName, float scale_value)
{
    cMesh* meshFound = g_pFindMeshByFriendlyName(modelName);

    if (meshFound == NULL)
        return;

    meshFound->setUniformDrawScale(scale_value);
}

void cControlGameEngine::ScaleModelInIndividualAxis(std::string modelName, float x, float y, float z)
{
    cMesh* meshToBeScaled = g_pFindMeshByFriendlyName(modelName);

    meshToBeScaled->setDrawScale(x, y, z);  
}

void cControlGameEngine::MoveModel(std::string modelName, float translate_x, float translate_y, float translate_z)
{
    cMesh* meshFound = g_pFindMeshByFriendlyName(modelName);

    if (meshFound == NULL)
        return;

    const glm::vec3& position = glm::vec3(translate_x, translate_y, translate_z);

    meshFound->setDrawPosition(position);
}

void cControlGameEngine::MoveColliderModel(std::string modelName, float translate_x, float translate_y, float translate_z)
{
    for (cMesh* currentMesh : this->AABBMeshesList)
    {
        if(currentMesh->meshUniqueName == modelName)
            currentMesh->setDrawPosition(glm::vec3(translate_x, translate_y, translate_z));
    }
}

void cControlGameEngine::RotateMeshModel(std::string modelName, float angleDegrees, float rotate_x, float rotate_y, float rotate_z)
{
    cMesh* meshFound = g_pFindMeshByFriendlyName(modelName);

    if (meshFound == NULL)
        return;

    float radianVal = glm::radians(angleDegrees);

    glm::quat rotationQuaternion = glm::angleAxis(radianVal, glm::vec3(rotate_x, rotate_y, rotate_z));

    if (meshFound->drawOrientation != glm::vec3(0.f))
        rotationQuaternion *= meshFound->get_qOrientation();
        
    meshFound->setDrawOrientation(rotationQuaternion, glm::vec3(rotate_x * angleDegrees, rotate_y * angleDegrees, rotate_z * angleDegrees));

    //meshFound->setDrawOrientation(rotation);

    //cMesh* meshFound = g_pFindMeshByFriendlyName(modelName);

    //glm::vec3 eulerAngles(rotate_x, rotate_y, rotate_z);

    //// Converting Euler angles to quaternion
    //glm::quat rotationQuaternion = glm::quat(glm::radians(eulerAngles));

    //if (meshFound->drawOrientation != glm::vec3(0.f))
    //    rotationQuaternion *= meshFound->get_qOrientation();
}

void cControlGameEngine::RotateMeshModelUsingQuaternion(std::string modelName, glm::quat rotationQuaternion)
{
    cMesh* meshToBeRotated = g_pFindMeshByFriendlyName(modelName);

    if (meshToBeRotated->drawOrientation != glm::vec3(0.f))
        rotationQuaternion *= meshToBeRotated->get_qOrientation();

    meshToBeRotated->setDrawOrientationQuaternion(rotationQuaternion);
}

void cControlGameEngine::TurnVisibilityOn(std::string modelName)
{
    cMesh* meshFound = g_pFindMeshByFriendlyName(modelName);

    if (meshFound == NULL)
        return;

    if (!meshFound->bIsVisible)
        meshFound->bIsVisible = true;
}

void cControlGameEngine::TurnVisibilityOff(std::string modelName)
{
    cMesh* meshFound = g_pFindMeshByFriendlyName(modelName);

    if (meshFound == NULL)
        return;

    if (meshFound->bIsVisible)
        meshFound->bIsVisible = false;
}

void cControlGameEngine::TurnWireframeModeOn(std::string modelName)
{
    cMesh* meshFound = g_pFindMeshByFriendlyName(modelName);

    if (meshFound == NULL)
        return;

    if (meshFound->bIsWireframe == true)
        meshFound->bIsWireframe = false;
    else
        meshFound->bIsWireframe = true;
}

void cControlGameEngine::TurnMeshLightsOn(std::string modelName)
{
    cMesh* meshFound = g_pFindMeshByFriendlyName(modelName);

    if (meshFound == NULL)
        return;

    if (meshFound->bDoNotLight == true)
        meshFound->bDoNotLight = false;
    else
        meshFound->bDoNotLight = true;
}

void cControlGameEngine::DeleteMesh(std::string modelName)
{
    cMesh* meshFound = g_pFindMeshByFriendlyName(modelName);

    sPhysicsProperties* physicalModel = FindPhysicalModelByName(modelName);

    if (meshFound != NULL)
        TotalMeshList.erase(std::remove(TotalMeshList.begin(), TotalMeshList.end(), meshFound), TotalMeshList.end());

    if (physicalModel != NULL)
        PhysicsModelList.erase(std::remove(PhysicsModelList.begin(), PhysicsModelList.end(), physicalModel), PhysicsModelList.end());
}

void cControlGameEngine::ShiftToNextMeshInList()
{
    meshListIndex++;

    if (meshListIndex == TotalMeshList.size())
        meshListIndex = 0;

    cMesh* existingMeshModel = TotalMeshList[meshListIndex];

    while (existingMeshModel->bIsAABBMesh)
    {
        meshListIndex++;

        if (meshListIndex == TotalMeshList.size())
            meshListIndex = 0;

        existingMeshModel = TotalMeshList[meshListIndex];
    }
}

void cControlGameEngine::ShiftToPreviousMeshInList()
{
    meshListIndex--;

    if (meshListIndex < 0)
        meshListIndex = int(TotalMeshList.size()) - 1;

    cMesh* existingMeshModel = TotalMeshList[meshListIndex];

    while (existingMeshModel->bIsAABBMesh)
    {
        meshListIndex--;

        if (meshListIndex < 0)
            meshListIndex = int(TotalMeshList.size()) - 1;

        existingMeshModel = TotalMeshList[meshListIndex];
    }
}

void cControlGameEngine::SortMeshesBasedOnTransparency()
{
    glm::vec3 cameraPos =   this->mCamManager->mCameraEye;
    glm::vec3 cameraPerspective =  this->mCamManager->mCameraTarget;
    glm::vec3 cameraDirection = glm::normalize(cameraPerspective - cameraPos);

    float offsetVal = 5'000.0f;

    // Making position of skyBox 5'000.0f away from the view of the camera
    glm::vec3 skyBoxPos = cameraPos + offsetVal * cameraDirection;

    std::sort(TotalMeshList.begin(), TotalMeshList.end(), [cameraPos, skyBoxPos](cMesh* mesh1, cMesh* mesh2) {
       
        if (mesh1->bIsSkyBox)
            return (glm::distance(skyBoxPos, cameraPos) > glm::distance(mesh2->drawPosition, cameraPos));
        else if (mesh2->bIsSkyBox)
            return (glm::distance(mesh1->drawPosition, cameraPos) > glm::distance(skyBoxPos, cameraPos));
        else
            return (glm::distance(mesh1->drawPosition, cameraPos) > glm::distance(mesh2->drawPosition,cameraPos));
    });
}

void cControlGameEngine::MakeMeshAABB(std::string modelName, bool isAABB)
{
    cMesh* meshFound = g_pFindMeshByFriendlyName(modelName);

    if (meshFound == NULL)
        return;

    meshFound->bIsAABBMesh = isAABB;
}

void cControlGameEngine::ToggleMeshBoneWeightColor(std::string modelName)
{
    cMesh* meshFound = g_pFindMeshByFriendlyName(modelName);

    if (meshFound->bApplyBones)
        meshFound->bDisplayBoneWeightColor = !meshFound->bDisplayBoneWeightColor;
}

void cControlGameEngine::SetMeshSceneId(std::string modelName, unsigned int meshSceneId)
{
    cMesh* meshFound = this->g_pFindMeshByFriendlyName(modelName);

    meshFound->sceneId = meshSceneId;
}

bool cControlGameEngine::IsMeshFilePathAlreadyExisting(std::string filePath)
{
    for (int index = 0; index < this->MeshFilePathList.size(); index++)
    {
        if (filePath == this->MeshFilePathList[index])
            return true;
    }

    return false;
}

cMesh* cControlGameEngine::GetCurrentModelSelected()
{
    return TotalMeshList[meshListIndex];
}

glm::quat cControlGameEngine::GetModelRotationQuat(std::string modelName)
{
    cMesh* meshRotation = g_pFindMeshByFriendlyName(modelName);

    return meshRotation->get_qOrientation();
}

std::vector < cMesh* > cControlGameEngine::GetMeshList()
{
    return this->TotalMeshList;
}

glm::vec3 cControlGameEngine::GetModelPosition(std::string modelName)
{
    cMesh* meshFound = g_pFindMeshByFriendlyName(modelName);

    if (meshFound == NULL)
        return glm::vec3(0.f);

    return meshFound->getDrawPosition();
}

float cControlGameEngine::GetModelScaleValue(std::string modelName)
{
    cMesh* meshFound = g_pFindMeshByFriendlyName(modelName);

    if (meshFound == NULL)
        return 0.f;

    return meshFound->drawScale.x;
}

//--------------------------------------BONES CONTROLS----------------------------------------------------------------

void cControlGameEngine::ApplyBonesToMesh(std::string modelName, bool applyBones)
{
    cMesh* meshBones = g_pFindMeshByFriendlyName(modelName);

    meshBones->bApplyBones = applyBones;
}

//------------------------------------ANIMATION CONTROLS--------------------------------------------------------------

void cControlGameEngine::UpdateAllAnimations(float deltaTime)
{
    if (mAnimationManager != NULL)
        this->mAnimationManager->UpdateAnimations(deltaTime);
}

void cControlGameEngine::PlayAnimation(std::string modelName, std::string animationName, float frameRateIncrement, bool loopAnimation, bool releasePrevAnimationsForThisMesh, 
    bool isTwoBodyAnimation, bool isLowerBodyAnimation)
{
    sModelDrawInfo* animModel = NULL;

    cMesh* animMesh = g_pFindMeshByFriendlyName(modelName);

    if (animMesh == NULL)
        std::cout << "ERROR : MODEL NAME NOT RECOGNIZED" << std::endl;
    else
    {
        animModel = g_pFindModelInfoByFriendlyName(animMesh->meshFileName);

        if (!this->mAnimationManager->AddAnimationCommand(animMesh, animModel, animationName, frameRateIncrement, loopAnimation, releasePrevAnimationsForThisMesh, isTwoBodyAnimation, 
            isLowerBodyAnimation))
            std::cout << "ERROR : ANIMATION NAME NOT RECOGNIZED" << std::endl;
    }
}

void cControlGameEngine::LoadAnimationIntoExistingMeshModel(std::string existingMeshModelName, std::string fileName, std::string animationName)
{
    cMesh* meshFound = g_pFindMeshByFriendlyName(existingMeshModelName);

    sModelDrawInfo* modelInfo = g_pFindModelInfoByFriendlyName(meshFound->meshFileName);

    if (!this->mVAOManager->LoadAnimationIntoModel(*modelInfo, fileName, animationName))
        std::cout << "ERROR : LOADING ANIMATION INTO EXISTING MODEL - " << fileName.c_str() << std::endl;
    else
        std::cout << "SUCCESS : NEW ANIMATION LOADED INTO EXISTING MODEL - " << fileName.c_str() << std::endl;
}

void cControlGameEngine::UnloopPreviousAnimations(std::string modelName)
{
    this->mAnimationManager->UnloopAnimationCommands(modelName);
}

void cControlGameEngine::LoopExistingAnimation(std::string modelName, std::string animationName)
{
    this->mAnimationManager->LoopAnimation(modelName, animationName);
}

bool cControlGameEngine::ArePreviousMeshAnimationsCompleted(std::string modelName)
{
   return this->mAnimationManager->AreAllPreviousAnimationsCompleted(modelName);
}

//-------------------------------------TEXTURE CONTROLS-----------------------------------------------------------------

void cControlGameEngine::UseTextures(std::string modelName, bool applyTexture)
{
    cMesh* meshFound = g_pFindMeshByFriendlyName(modelName);
    
    if (meshFound == NULL)
        return;

    meshFound->bUseTextures = applyTexture;
}

void cControlGameEngine::ApplyHeightMap(std::string modelName, std::string textureName, bool applyHeightMap)
{
    cMesh* meshFound = g_pFindMeshByFriendlyName(modelName);

    if (meshFound == NULL)
        return;

    for (int index = 0; index < meshFound->mTextureDetailsList.size(); index++)
    {
        if (meshFound->mTextureDetailsList[index].textureName == textureName)
        {
            meshFound->mTextureDetailsList[index].bUseHeightMap = applyHeightMap;
            
            break;
        }
    }
}

void cControlGameEngine::LoadAllTextures(std::vector <std::string> texturePathList)
{
    for (int index = 0; index < texturePathList.size(); index++)
    {
        if (this->mTextureManager->Create2DTextureFromBMPFile(texturePathList[index], true))
        {
            std::cout << "SUCCESS : TEXTURE LOADED - [" << texturePathList[index] << "]" << std::endl;

            this->TexturePathList.push_back(texturePathList[index]);
        }

        else
        {
            std::cout << "ERROR : LOADING TEXTURE FAILED - [" << texturePathList[index] << "]" << std::endl;

            return;
        }
    }
}

void cControlGameEngine::AddTexturesToOverlap(std::string modelName, std::string texturePath, std::string textureName)
{
    cMesh* meshTexture = g_pFindMeshByFriendlyName(modelName);

    if (!this->IsTextureAlreadyLoaded(texturePath))
    {
        if (this->mTextureManager->Create2DTextureFromBMPFile(texturePath, true))
        {
            std::cout << "SUCCESS : TEXTURE LOADED - [" << texturePath << "]" << std::endl;

            this->TexturePathList.push_back(texturePath);
        }

        else
        {
            std::cout << "ERROR : LOADING TEXTURE FAILED - [" << texturePath << "]" << std::endl;

            return;
        }
    }

    sMeshTexture newTexture;

    newTexture.texturePath = texturePath;
    newTexture.textureName = textureName;
    newTexture.textureUnit = textureUnitIndex;

    meshTexture->mTextureDetailsList.push_back(newTexture);

    // Incrementing the Texture Unit 
    textureUnitIndex++;

    if (textureUnitIndex >= 30) // Maintaining 30 as the threshold for the Texture Unit count
        textureUnitIndex = 0;
}

void cControlGameEngine::AddTexturesToTheMix(std::string modelName, std::string texturePath, std::string textureName, float textureRatio)
{ 
    cMesh* meshTexture = g_pFindMeshByFriendlyName(modelName);

    if (!this->IsTextureAlreadyLoaded(texturePath))
    {
        if (this->mTextureManager->Create2DTextureFromBMPFile(texturePath, true))
        {
            std::cout << "SUCCESS : TEXTURE LOADED - [" << texturePath << "]" << std::endl;

            this->TexturePathList.push_back(texturePath);
        }

        else
        {
            std::cout << "ERROR : LOADING TEXTURE FAILED - [" << texturePath << "]" << std::endl;

            return;
        }
    }

    sMeshTexture newTexture;

    meshTexture->mMixedTextures.texturePathList.push_back(texturePath);
    meshTexture->mMixedTextures.textureNameList.push_back(textureName);
    meshTexture->mMixedTextures.textureUnit.push_back(textureUnitIndex);
    meshTexture->mMixedTextures.textureRatiosList[meshTexture->mMixedTextures.textureNameList.size() - 1] = textureRatio;

    // Incrementing the Texture Unit 
    textureUnitIndex++;

    if (textureUnitIndex >= 30) // Maintaining 30 as the threshold for the Texture Unit count
        textureUnitIndex = 0;
}

void cControlGameEngine::AddDiscardMaskTexture(std::string modelName, std::string textureName, std::string discardMaskTexturePath)
{
    cMesh* meshTexture = g_pFindMeshByFriendlyName(modelName);

    if (!this->IsTextureAlreadyLoaded(discardMaskTexturePath))
    {
        if (this->mTextureManager->Create2DTextureFromBMPFile(discardMaskTexturePath, true))
        {
            std::cout << "SUCCESS : TEXTURE LOADED - [" << discardMaskTexturePath << "]" << std::endl;

            this->TexturePathList.push_back(discardMaskTexturePath);
        }

        else
        {
            std::cout << "ERROR : LOADING TEXTURE FAILED - [" << discardMaskTexturePath << "]" << std::endl;

            return;
        }
    }

    for (int index = 0; index < meshTexture->mTextureDetailsList.size(); index++)
    {
        if (meshTexture->mTextureDetailsList[index].textureName == textureName)
        {
            meshTexture->mTextureDetailsList[index].discardMaskTexturePath = discardMaskTexturePath;
            meshTexture->mTextureDetailsList[index].discardMaskTextureUnit = textureUnitIndex;

            break;
        }
    }

    // Incrementing the Texture Unit 
    textureUnitIndex++;

    if (textureUnitIndex >= 30) // Maintaining 30 as the threshold for the Texture Unit count
        textureUnitIndex = 0;
}

void cControlGameEngine::ChangeTextureRatios(std::string modelName, std::vector <float> textureRatio)
{
    cMesh* meshFound = g_pFindMeshByFriendlyName(modelName);

    if (meshFound == NULL)
        return;

    int MIXTURE_THRESHOLD = 4;
    // Maintaining a mixture threshold of 4 for now
    if (meshFound->mMixedTextures.textureNameList.size() > 0)
    {
        for (int index = 0; index < MIXTURE_THRESHOLD; index++)
            meshFound->mMixedTextures.textureRatiosList[index] = textureRatio[index];
    }
}

void cControlGameEngine::AdjustHeightMapScale(std::string modelName, std::string textureName, float height)
{
    cMesh* meshFound = g_pFindMeshByFriendlyName(modelName);

    if (meshFound == NULL)
        return;

    for (int index = 0; index < meshFound->mTextureDetailsList.size(); index++)
    {
        if (meshFound->mTextureDetailsList[index].textureName == textureName)
        {
            meshFound->mTextureDetailsList[index].heightMapScale = height;

            break;
        }
    }
}

void cControlGameEngine::AdjustHeightMapUVPos(std::string modelName, std::string textureName, glm::vec2 UVOffsetPos)
{
    cMesh* meshFound = g_pFindMeshByFriendlyName(modelName);

    if (meshFound == NULL)
        return;

    for (int index = 0; index < meshFound->mTextureDetailsList.size(); index++)
    {
        if (meshFound->mTextureDetailsList[index].textureName == textureName)
        {
            meshFound->mTextureDetailsList[index].UVOffset.x += UVOffsetPos.x;
            meshFound->mTextureDetailsList[index].UVOffset.y += UVOffsetPos.y;

            break;
        }
    }
}

void cControlGameEngine::RemoveBlackAreasInHeightMap(std::string modelName, std::string textureName, bool removeBlackAreas)
{
    cMesh* meshFound = g_pFindMeshByFriendlyName(modelName);

    if (meshFound == NULL)
        return;

    for (int index = 0; index < meshFound->mTextureDetailsList.size(); index++)
    {
        if (meshFound->mTextureDetailsList[index].textureName == textureName)
        {
            meshFound->mTextureDetailsList[index].bDiscardBlackAreasInHeightMap = removeBlackAreas;

            break;
        }
    }
}

void cControlGameEngine::RemoveColoredAreasHeightMap(std::string modelName, std::string textureName, bool removeColoredAreas)
{
    cMesh* meshFound = g_pFindMeshByFriendlyName(modelName);

    if (meshFound == NULL)
        return;

    for (int index = 0; index < meshFound->mTextureDetailsList.size(); index++)
    {
        if (meshFound->mTextureDetailsList[index].textureName == textureName)
        {
            meshFound->mTextureDetailsList[index].bDiscardColoredAreasInHeightMap = removeColoredAreas;

            break;
        }
    }
}

void cControlGameEngine::ApplyDiscardMaskTexture(std::string modelName, std::string textureName, bool applyDiscardMask)
{
    cMesh* meshFound = g_pFindMeshByFriendlyName(modelName);

    if (meshFound == NULL)
        return;

    for (int index = 0; index < meshFound->mTextureDetailsList.size(); index++)
    {
        if (meshFound->mTextureDetailsList[index].textureName == textureName)
        {
            meshFound->mTextureDetailsList[index].bUseDiscardMaskTexture = applyDiscardMask;

            break;
        }
    }
}

void cControlGameEngine::AdjustAlphaTransparency(std::string modelName, float alphaTransparencyLevel)
{
    cMesh* meshFound = g_pFindMeshByFriendlyName(modelName);

    if (meshFound == NULL)
        return;

    meshFound->alphaTransparency = alphaTransparencyLevel;
}

void cControlGameEngine::UseDiscardMaskTexture(GLuint shaderProgramID, GLint bUseDiscardMaskTexture_UL, std::string texturePath, int textureUnit)
{
    glUniform1f(bUseDiscardMaskTexture_UL, (GLfloat)GL_TRUE);

    GLuint textureToBeMasked = mTextureManager->getTextureIDFromName(texturePath);

    glActiveTexture(GL_TEXTURE0 + textureUnit);

    glBindTexture(GL_TEXTURE_2D, textureToBeMasked);

    GLint maskSampler_UL = glGetUniformLocation(shaderProgramID, "maskSampler");
    glUniform1i(maskSampler_UL, textureUnit);
}

bool cControlGameEngine::IsTextureAlreadyLoaded(std::string texturePath)
{
    for (int index = 0; index < this->TexturePathList.size(); index++)
    {
        if (this->TexturePathList[index] == texturePath)
            return true;
    }

    return false;
}

//-------------------------------------CUBE MAP CONTROLS----------------------------------------------------------------

bool cControlGameEngine::AddCubeMap(std::string modelName, std::string skyBoxName, std::string filePathPosX, std::string filePathNegX, std::string filePathPosY,
                                    std::string filePathNegY, std::string filePathPosZ, std::string filePathNegZ)
{
    std::string errors;
    bool result;

    mTextureManager->SetBasePath("Assets/Textures/CubeMaps");
   
    result = mTextureManager->CreateCubeTextureFromBMPFiles(skyBoxName, filePathPosX, filePathNegX, filePathPosY, filePathNegY, filePathPosZ,
                                                    filePathNegZ, true, errors);

    mTextureManager->SetBasePath("Assets/Textures");

    if (result)
    {
         mCubeMapManager->AddSkyBoxName(skyBoxName);
         mCubeMapManager->AddMeshModelName(modelName);
         mCubeMapManager->AddSkyBoxTextureUnit(textureUnitIndex);
         mCubeMapManager->AddSkyBoxFilePaths(filePathPosX, filePathNegX, filePathPosY, filePathNegY, filePathPosZ, filePathNegZ);

        cMesh* meshFound = g_pFindMeshByFriendlyName(modelName);

        if (meshFound == NULL)
            return -1;

        meshFound->bIsSkyBox = true;

        // Incrementing the Texture Unit 
        textureUnitIndex++;

        if (textureUnitIndex >= 30) // Maintaining 30 as the threshold for the Texture Unit count
            textureUnitIndex = 0;

        return 0;
    }
    else
    {
        std::cout << "Error loading Cube Map : " << errors << std::endl;
        return -1;
    }
}

void cControlGameEngine::DeleteCubeMap()
{
    if (mCubeMapManager != NULL)
    {
        delete mCubeMapManager;

        for (cMesh* skyBox : TotalMeshList)
        {
            if (skyBox->bIsSkyBox)
                DeleteMesh(skyBox->meshFileName);
        }
    }
}

//--------------------------------------LIGHTS CONTROLS-----------------------------------------------------------------

void cControlGameEngine::AddLight(glm::vec4 lightPosition, glm::vec4 lightDirection, bool turnOff, float lightType, float linearAtten, float quadraticAtten, glm::vec2 angle,
    glm::vec3 lightColor)
{
    if (this->mLightManager->mLightsList.size() < 30)
    {
        cLight* newLight = new cLight();

        newLight->lightID = this->mLightManager->mLightsList.size();

        newLight->position = lightPosition;
        newLight->direction = lightDirection;

        newLight->param2.x = 1.0f;        // Turn on

        newLight->param1.x = lightType;         // 0 = point light , 1 = spot light
        newLight->param1.y = angle.x;           // inner angle
        newLight->param1.z = angle.y;           // outer angle

        newLight->atten.x = 0.0f;               // Constant attenuation
        newLight->atten.y = linearAtten;        // Linear attenuation
        newLight->atten.z = quadraticAtten;     // Quadratic attenuation

        newLight->diffuse = glm::vec4(lightColor.r, lightColor.g, lightColor.b, 1.0f);

        this->mLightManager->mLightsList.push_back(newLight);

        mLightManager->SetUniformLocations(shaderProgramID, newLight->lightID);
    }
    else
        std::cout << "ERROR: LIGHT SOURCES EXCEED LIMIT - SOURCE LIMIT : " << this->mLightManager->mLightsList.size() << std::endl;
}

void cControlGameEngine::TurnOffLight(int lightId, bool turnOff)
{
    if (turnOff)
        this->mLightManager->mLightsList[lightId]->param2.x = 0.0f;
    else
        this->mLightManager->mLightsList[lightId]->param2.x = 1.0f;
}

void cControlGameEngine::PositionLight(int lightId, float translate_x, float translate_y, float translate_z)
{
    mLightManager->mLightsList[lightId]->position.x = translate_x;

    mLightManager->mLightsList[lightId]->position.y = translate_y;

    mLightManager->mLightsList[lightId]->position.z = translate_z;
}

void cControlGameEngine::ChangeLightIntensity(int lightId, float linearAttentuation, float quadraticAttentuation)
{
    this->mLightManager->mLightsList[lightId]->atten.y = linearAttentuation;

    mLightManager->mLightsList[lightId]->atten.z = quadraticAttentuation;
}

void cControlGameEngine::ChangeLightType(int lightId, float lightType)
{
    mLightManager->mLightsList[lightId]->param1.x = lightType;
}

void cControlGameEngine::ChangeLightAngle(int lightId, float innerAngle, float outerAngle)
{
    mLightManager->mLightsList[lightId]->param1.y = innerAngle; // inner angle

    mLightManager->mLightsList[lightId]->param1.z = outerAngle; // outer angle
}

void cControlGameEngine::ChangeLightDirection(int lightId, float direction_x, float direction_y, float direction_z)
{
    mLightManager->mLightsList[lightId]->direction = glm::vec4(direction_x, direction_y, direction_z, 1.0f);

}

void cControlGameEngine::ChangeLightColour(int lightId, float color_r, float color_g, float color_b)
{
    mLightManager->mLightsList[lightId]->diffuse = glm::vec4(color_r, color_g, color_b, 1.0f);
}

glm::vec3 cControlGameEngine::GetLightPosition(int lightId)
{
    return mLightManager->mLightsList[lightId]->position;
}

glm::vec3 cControlGameEngine::GetLightDirection(int lightId)
{
    return mLightManager->mLightsList[lightId]->direction;
}

float cControlGameEngine::GetLightLinearAttenuation(int lightId)
{
    return mLightManager->mLightsList[lightId]->atten.y;
}

float cControlGameEngine::GetLightQuadraticAttenuation(int lightId)
{
    return mLightManager->mLightsList[lightId]->atten.z;
}

float cControlGameEngine::GetLightType(int lightId)
{
    return mLightManager->mLightsList[lightId]->param1.x;
}

float cControlGameEngine::GetLightInnerAngle(int lightId)
{
    return mLightManager->mLightsList[lightId]->param1.y;
}

float cControlGameEngine::GetLightOuterAngle(int lightId)
{
    return mLightManager->mLightsList[lightId]->param1.z;
}

glm::vec3 cControlGameEngine::GetLightColor(int lightId)
{
    return mLightManager->mLightsList[lightId]->diffuse;
}

float cControlGameEngine::IsLightOn(int lightId)
{
    return mLightManager->mLightsList[lightId]->param2.x;
}

void cControlGameEngine::ShiftToNextLightInList()
{
    lightListIndex++;

    if (lightListIndex >= this->mLightManager->mLightsList.size())
        lightListIndex = 0;
}

int cControlGameEngine::GetCurrentLightSelected()
{
    return lightListIndex;
}

//--------------------------------------PHYSICS CONTROLS---------------------------------------------------------------

void  cControlGameEngine::ComparePhysicalAttributesWithOtherModels()
{
    for (int physicalModelCount = 0; physicalModelCount < PhysicsModelList.size(); physicalModelCount++)
    {
        if (PhysicsModelList[physicalModelCount]->physicsMeshType == "Sphere")
        {
            sPhysicsProperties* spherePhysicalModel = FindPhysicalModelByName(PhysicsModelList[physicalModelCount]->modelName);

            if (spherePhysicalModel != NULL)
            {
                for (int anotherModelCount = 0; anotherModelCount < PhysicsModelList.size(); anotherModelCount++)
                {
                    if (anotherModelCount == physicalModelCount)
                        continue;
                    else
                    {
                     /*   if (PhysicsModelList[anotherModelCount]->physicsMeshType == "Plane" || PhysicsModelList[anotherModelCount]->physicsMeshType == "Box")
                            MakePhysicsHappen(spherePhysicalModel, PhysicsModelList[anotherModelCount]->modelName, PhysicsModelList[anotherModelCount]->physicsMeshType);*/

                        if (PhysicsModelList[anotherModelCount]->physicsMeshType == "Sphere" && PhysicsModelList[anotherModelCount]->sphereProps->velocity != glm::vec3(0.f))
                            MakePhysicsHappen(spherePhysicalModel, PhysicsModelList[anotherModelCount]->modelName, PhysicsModelList[anotherModelCount]->physicsMeshType);
                    }
                }               
            }
        }
    }
}

void cControlGameEngine::ResetPosition(sPhysicsProperties* physicsModel)
{
    physicsModel->position.x = getRandomFloat(0.0, 20.0);
    physicsModel->position.y = getRandomFloat(100.0, 150.0);
    physicsModel->position.z = getRandomFloat(0.0, 20.0);
    physicsModel->sphereProps->velocity = glm::vec3(0.0f, -getRandomFloat(1.0, 5.0), 0.0f);

    ChangeColor(physicsModel->modelName, 1.0, 1.0, 1.0); //Reseting spheres to white again
}

void cControlGameEngine::PhysicsEulerIntegration(std::string modelName)
{
    sPhysicsProperties* physicsModel = FindPhysicalModelByName(modelName);

    if (physicsModel == NULL)
        return;

    cMesh* meshFound = g_pFindMeshByFriendlyName(modelName);

    if (meshFound == NULL)
        return;

    //--------Calculate acceleration & velocity(Euler forward integration step)-------------------

    mPhysicsManager->EulerForwardIntegration(physicsModel, deltaTime);

    //------------------Set objects's position based on new velocity-------------------------------

    meshFound->setDrawPosition(physicsModel->position);
}

void cControlGameEngine::MakeCollisionHappen(std::string modelName)
{
    sPhysicsProperties* physicsModel = FindPhysicalModelByName(modelName);
    cMesh* sphereMesh = g_pFindMeshByFriendlyName(physicsModel->modelName);

    mPhysicsManager->PlaneCollisionResponse(physicsModel, static_cast<float>(this->deltaTime));
    sphereMesh->setDrawPosition(physicsModel->position);
}

bool cControlGameEngine::ComparePhysicsObjectWithAABBs(std::string modelName)
{
    bool result = false;

    sPhysicsProperties* physicsModel = FindPhysicalModelByName(modelName);

    for (cAABB* boundingBox : AABBList)
    {
        if (boundingBox->IsInsideAABB(physicsModel->position))
        {
            //---------------------------Change AABB color to green---------------------------------

            if (!boundingBox->objectInsideAABB)
            {
                for (cMesh* currentMesh : this->AABBMeshesList)
                {
                    if (currentMesh->meshUniqueName == boundingBox->meshModelName)
                        currentMesh->wholeObjectManualColourRGBA = glm::vec4(0.1, 0.85, 0.1, 1.f);
                }

                boundingBox->objectInsideAABB = true;
            }

            //------------------------Check for collision with triangles----------------------------

            result = mPhysicsManager->CheckForAABBTriangleCollision(boundingBox->trianglesInsideAABBList, physicsModel);

            return result;
        }

        else
        {
            //---------------------------Change AABB color to red------------------------------------

            if (boundingBox->objectInsideAABB)
            {
                for (cMesh* currentMesh : this->AABBMeshesList)
                {
                    if (currentMesh->meshUniqueName == boundingBox->meshModelName)
                        currentMesh->wholeObjectManualColourRGBA = glm::vec4(0.90f, 0.1f, 0.1f, 1.f);
                }

                boundingBox->objectInsideAABB = false;
            }
        }
    }

    return false;
}

void cControlGameEngine::MakePhysicsHappen(sPhysicsProperties* physicsModel, std::string model2Name, std::string collisionType)
{
    //---------------------Get Mesh models--------------------------------------------------------
    
    cMesh* sphereMesh = g_pFindMeshByFriendlyName(physicsModel->modelName);

    if (sphereMesh == NULL)
        return;

    cMesh* model2Mesh = g_pFindMeshByFriendlyName(model2Name);

    if (model2Mesh == NULL)
        return;

    //----------------------------Check for Collision---------------------------------------------

    bool result = false;

    if (collisionType == "Sphere")
    {
        //---------------------Get second sphere's physical model-----------------------------------

        sPhysicsProperties * secondSphereModel = FindPhysicalModelByName(model2Name);

        //----------------------Sphere Collision Check----------------------------------------------
        
        if (secondSphereModel != NULL)
            result = mPhysicsManager->CheckForSphereCollision(physicsModel, secondSphereModel);

        if (result)
        {
            mPhysicsManager->SphereCollisionResponse(physicsModel, secondSphereModel);

            //------------------------Change colors after collision-----------------------------

            ChangeColor(physicsModel->modelName, getRandomFloat(0.0, 0.50), getRandomFloat(0.0, 0.50), getRandomFloat(0.0, 0.50));
            ChangeColor(secondSphereModel->modelName, getRandomFloat(0.0, 0.50), getRandomFloat(0.0, 0.50), getRandomFloat(0.0, 0.50));

            //-------------------Set sphere's position after response---------------------------

            sphereMesh->setDrawPosition(physicsModel->position);
            model2Mesh->setDrawPosition(secondSphereModel->position);
        }
    }
}

void cControlGameEngine::AddSpherePhysicsToMesh(std::string modelName, std::string physicsMeshType, float objectRadius)
{
    sPhysicsProperties* newPhysicsModel = new sPhysicsProperties(physicsMeshType);

    glm::vec3 modelPosition = glm::vec3(0.f);
    
    bool foundMesh = false;

    for (cMesh* meshDetails : this->TotalMeshList)
    {
        if (meshDetails->meshUniqueName == modelName)
        {
            modelPosition = meshDetails->getDrawPosition();
            foundMesh = true;
        }
    }

    if (!foundMesh)
        return;

    newPhysicsModel->physicsMeshType = physicsMeshType;

    newPhysicsModel->modelName = modelName;

    newPhysicsModel->sphereProps->radius = objectRadius;

    newPhysicsModel->position = modelPosition;

    PhysicsModelList.push_back(newPhysicsModel);
}

void cControlGameEngine::AddPlanePhysicsToMesh(std::string modelName, std::string physicsMeshType)
{
    sPhysicsProperties* newPhysicsModel = new sPhysicsProperties(physicsMeshType);

    cMesh* meshDetails = g_pFindMeshByFriendlyName(modelName);

    if (meshDetails == NULL)
        return;

    glm::vec3 modelPosition = meshDetails->getDrawPosition();

    newPhysicsModel->physicsMeshType = physicsMeshType;

    newPhysicsModel->modelName = modelName;

    newPhysicsModel->position = modelPosition;

    PhysicsModelList.push_back(newPhysicsModel);
}

void cControlGameEngine::ChangeModelPhysicsPosition(std::string modelName, float newPositionX, float newPositionY, float newPositionZ)
{
    sPhysicsProperties* physicalModelFound = FindPhysicalModelByName(modelName);

    if (physicalModelFound == NULL)
        return;

    physicalModelFound->position.x = newPositionX;
    physicalModelFound->position.y = newPositionY;
    physicalModelFound->position.z = newPositionZ;
}

void cControlGameEngine::ChangeModelPhysicsVelocity(std::string modelName, glm::vec3 velocityChange)
{
    sPhysicsProperties* physicalModelFound = FindPhysicalModelByName(modelName);

    if (physicalModelFound == NULL)
        return;

    physicalModelFound->sphereProps->velocity = velocityChange;
}

void cControlGameEngine::ChangeModelPhysicsAcceleration(std::string modelName, glm::vec3 accelerationChange)
{
    sPhysicsProperties* physicalModelFound = FindPhysicalModelByName(modelName);

    if (physicalModelFound == NULL)
        return;

    physicalModelFound->sphereProps->acceleration = accelerationChange;
}

int cControlGameEngine::ChangeModelPhysicalMass(std::string modelName, float mass)
{
    sPhysicsProperties* physicalModelFound = FindPhysicalModelByName(modelName);

    if (physicalModelFound == NULL)
        return 1;

    if (mass > 0.0f)
    {
        physicalModelFound->sphereProps->inverse_mass = 1.0 / mass;
        return 0;
    }
    
    return 1;
}

void cControlGameEngine::UpdatePhysicsPosition(std::string modelName, glm::vec3 modelPos)
{
    sPhysicsProperties* physicsModel = FindPhysicalModelByName(modelName);

    physicsModel->position = modelPos;
}

//----------------------------------------AABB CONTROLS-----------------------------------------------------------------

void cControlGameEngine::CreateSceneAABBs(glm::vec3 startingPoint, glm::ivec3 cubesCount, float cubeSize, std::string meshFileName)
{
    for (int xPosOffset = 0; xPosOffset < cubesCount.x; xPosOffset++)
    {
        for (int yPosOffset = 0; yPosOffset < cubesCount.y; yPosOffset++)
        {
            for (int zPosOffset = 0; zPosOffset < cubesCount.z; zPosOffset++)
            {
                //-------------------------------------Create AABB Mesh to Debug-------------------------------------------

                float xPos = (xPosOffset * cubeSize) + startingPoint.x;
                float yPos = (yPosOffset * cubeSize) + startingPoint.y;
                float zPos = (zPosOffset * cubeSize) + startingPoint.z;

                std::string modelName = "Cube_" + std::to_string((int)xPos) + "_" + std::to_string((int)yPos) + "_" + std::to_string((int)zPos);

                //-------------------------------------------Load Cube Mesh------------------------------------------------

                std::string fullFilePath = this->mVAOManager->getBaseModelPath() + "/" + meshFileName;

                sModelDrawInfo* modelDrawInfo = CreateModelDrawInfo(meshFileName, fullFilePath);

                if (modelDrawInfo == NULL)
                    return;

                //------------------------------------------Creating new mesh model-------------------------------------------------------------

                cMesh* newMesh = new cMesh();

                newMesh->meshFileName = fullFilePath;

                newMesh->meshUniqueName = modelName;

                newMesh->drawPosition = glm::vec3(xPos, yPos, zPos);

                newMesh->bIsAABBMesh = true;

                newMesh->bDoNotLight = true;

                newMesh->setUniformDrawScale(cubeSize);

                newMesh->bUseManualColours = true;

                newMesh->wholeObjectManualColourRGBA = glm::vec4(0.90f, 0.1f, 0.1f, 1.f);
                
                newMesh->bIsWireframe = true;

                newMesh->modelDrawInfo = modelDrawInfo;

                this->AABBMeshesList.push_back(newMesh);

                //-------------------------Create AABB instances to store vertices and triangles---------------------------
                
                cAABB* newBoundingBox = new cAABB();

                newBoundingBox->meshModelName = modelName;

                newBoundingBox->minXYZ.x = xPos - cubeSize / 2;
                newBoundingBox->minXYZ.y = yPos - cubeSize / 2;
                newBoundingBox->minXYZ.z = zPos - cubeSize / 2;

                newBoundingBox->maxXYZ.x = xPos + cubeSize / 2;
                newBoundingBox->maxXYZ.y = yPos + cubeSize / 2;
                newBoundingBox->maxXYZ.z = zPos + cubeSize / 2;

                AABBList.push_back(newBoundingBox);
            }
        }
    }

    bAABBGenerated = true;
}

void cControlGameEngine::AddVerticesToAABB(std::string modelName)
{
    //-------------------------------Checking Vertices in each AABB-------------------------------------------------------

    cMesh* currentMesh = g_pFindMeshByFriendlyName(modelName);
    sModelDrawInfo* currentMeshModel = g_pFindModelInfoByFriendlyName(currentMesh->meshFileName);

    if (currentMesh == NULL)
        return;

    if (currentMeshModel == NULL)
        return;

    if (!currentMesh->bIsAABBMesh)
    {
        for (unsigned int index = 0; index < currentMeshModel->numberOfVertices; index++)
        {
            //---------------Calculate vertex position-----------------------------

            glm::vec3 vertex;

            vertex.x = currentMeshModel->pVertices[index].x;
            vertex.y = currentMeshModel->pVertices[index].y;
            vertex.z = currentMeshModel->pVertices[index].z;

            //----------------Convert to world space------------------------------

            glm::mat4 matModel = glm::mat4(1.0f);

            glm::mat4 matTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(currentMesh->drawPosition.x, currentMesh->drawPosition.y, currentMesh->drawPosition.z));

            glm::mat4 matRotate = glm::mat4_cast(currentMesh->get_qOrientation());

            glm::mat4 matScale = glm::scale(glm::mat4(1.0f), glm::vec3(currentMesh->drawScale.x, currentMesh->drawScale.y, currentMesh->drawScale.z));

            matModel = matModel * matTranslate;

            matModel = matModel * matRotate;

            matModel = matModel * matScale;

            glm::vec4 vertexInWorldSpace;

            vertexInWorldSpace = (matModel * glm::vec4(vertex, 1.0f));

            glm::vec3 newVertices = glm::vec3(vertexInWorldSpace.x, vertexInWorldSpace.y, vertexInWorldSpace.z);
                
            for (cAABB* aabbMesh : AABBList)
            {
                if (aabbMesh->IsInsideAABB(newVertices))
                {
                    aabbMesh->verticesInsideAABBList.push_back(newVertices);
                    break;
                }
            }
        }
    }

    std::cout << "Total vertices : " << currentMeshModel->numberOfVertices<<std::endl;
    int sumVertices = 0;

    for (int i = 0; i < AABBList.size(); i++)
    {
        std::cout << "AABB[" << i << "] has " << AABBList[i]->verticesInsideAABBList.size() << " vertices inside it." << std::endl;
        sumVertices += AABBList[i]->verticesInsideAABBList.size();
    }

    std::cout << "Vertices inside AABBs : " << sumVertices<<std::endl;
}

void cControlGameEngine::AddTrianglesToAABB(std::vector <std::string> modelFileNameList, int startIndex, int endIndex)
{
    //-------------------------------Checking Vertices in each AABB-------------------------------------------------------

    for (int index = startIndex; index < endIndex; index++)
    {
        cMesh* currentMesh = this->TotalMeshList[index];

        for (int count = 0; count < modelFileNameList.size(); count++)
        {
            if (currentMesh->modelDrawInfo->meshFileName == modelFileNameList[count])
            {
                sModelDrawInfo* currentMeshModel = currentMesh->modelDrawInfo;

                if (!currentMesh->bIsAABBMesh)
                {
                    for (unsigned int index = 0; index < currentMeshModel->numberOfIndices; index += 3)
                    {
                        glm::vec3 vertices[3];

                        unsigned int triangleIndex_0 = currentMeshModel->pIndices[index];
                        unsigned int triangleIndex_1 = currentMeshModel->pIndices[index + 1];
                        unsigned int triangleIndex_2 = currentMeshModel->pIndices[index + 2];

                        vertices[0].x = currentMeshModel->pVertices[triangleIndex_0].x;
                        vertices[0].y = currentMeshModel->pVertices[triangleIndex_0].y;
                        vertices[0].z = currentMeshModel->pVertices[triangleIndex_0].z;

                        vertices[1].x = currentMeshModel->pVertices[triangleIndex_1].x;
                        vertices[1].y = currentMeshModel->pVertices[triangleIndex_1].y;
                        vertices[1].z = currentMeshModel->pVertices[triangleIndex_1].z;

                        vertices[2].x = currentMeshModel->pVertices[triangleIndex_2].x;
                        vertices[2].y = currentMeshModel->pVertices[triangleIndex_2].y;
                        vertices[2].z = currentMeshModel->pVertices[triangleIndex_2].z;

                        //----------------Convert to world space------------------------------

                        glm::mat4 matModel = glm::mat4(1.0f);

                        glm::mat4 matTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(currentMesh->drawPosition.x, currentMesh->drawPosition.y, currentMesh->drawPosition.z));

                        glm::mat4 matRotate = glm::mat4_cast(currentMesh->get_qOrientation());

                        glm::mat4 matScale = glm::scale(glm::mat4(1.0f), glm::vec3(currentMesh->drawScale.x, currentMesh->drawScale.y, currentMesh->drawScale.z));

                        matModel = matModel * matTranslate;

                        matModel = matModel * matRotate;

                        matModel = matModel * matScale;

                        glm::vec4 vertexInWorldSpace[3];

                        vertexInWorldSpace[0] = (matModel * glm::vec4(vertices[0], 1.0f));
                        vertexInWorldSpace[1] = (matModel * glm::vec4(vertices[1], 1.0f));
                        vertexInWorldSpace[2] = (matModel * glm::vec4(vertices[2], 1.0f));

                        sAABBTriangle newTriangle;

                        newTriangle.vertex1 = glm::vec3(vertexInWorldSpace[0].x, vertexInWorldSpace[0].y, vertexInWorldSpace[0].z);
                        newTriangle.vertex2 = glm::vec3(vertexInWorldSpace[1].x, vertexInWorldSpace[1].y, vertexInWorldSpace[1].z);
                        newTriangle.vertex3 = glm::vec3(vertexInWorldSpace[2].x, vertexInWorldSpace[2].y, vertexInWorldSpace[2].z);

                        for (cAABB* aabbMesh : AABBList)
                        {
                            if (aabbMesh->IsInsideAABB(newTriangle.vertex1) || aabbMesh->IsInsideAABB(newTriangle.vertex2) || aabbMesh->IsInsideAABB(newTriangle.vertex3))
                            {
                                aabbMesh->trianglesInsideAABBList.push_back(newTriangle);
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

   /* int sumTriangles = 0;

    for (int i = 0; i < AABBList.size(); i++)
    {
        std::cout << "AABB[" << i << "] has " << AABBList[i]->trianglesInsideAABBList.size() << " traingles inside it." << std::endl;
        sumTriangles += AABBList[i]->trianglesInsideAABBList.size();
    }

    std::cout << "Triangles inside AABBs : " << sumTriangles << std::endl;*/
}

//-------------------------------------SOFT BODY CONTROLS---------------------------------------------------------------

void cControlGameEngine::CreateVerletSoftBodyWithThread(std::string modelName, glm::vec3 softbodyAcceleration, bool createNewDebugSpheres, 
    std::string debugMeshName, float scaleVal, glm::vec4 sphereRGBColor, bool useThreadOnSoftBody)
{
    cSoftBody* newSoftBody = new cSoftBody();

    sModelDrawInfo softBodyObjectDrawingInfo;

    cMesh* meshFileNameNeeded = g_pFindMeshByFriendlyName(modelName);

    if (meshFileNameNeeded == NULL)
        return;

    meshFileNameNeeded->bIsSoftBody = true;

    newSoftBody->meshName = meshFileNameNeeded->meshUniqueName;
    newSoftBody->acceleration = softbodyAcceleration;

    if (mVAOManager->FindDrawInfoByModelName(meshFileNameNeeded->meshUniqueName, softBodyObjectDrawingInfo))
    {
        //-----------------------------Apply matrix transformations-----------------------------------

        glm::mat4 matTransform = glm::translate(glm::mat4(1.0f), meshFileNameNeeded->drawPosition);
        matTransform = matTransform * glm::mat4_cast(meshFileNameNeeded->get_qOrientation());
        matTransform = glm::scale(matTransform, meshFileNameNeeded->drawScale);

        //-----------------------------------Create soft body-----------------------------------------

        if (newSoftBody->CreateSoftBody(softBodyObjectDrawingInfo, meshFileNameNeeded->drawPosition, matTransform))
        {
            std::cout << std::endl;
            std::cout << "SUCCESS : CREATED SOFT BODY FOR THE MODEL - [" << meshFileNameNeeded->meshFileName << "]" << std::endl;
        }
        else
        {
            std::cout << std::endl;
            std::cout << "ERROR : UNABLE TO CREATE SOFT BODY FOR THE MODEL - [" << meshFileNameNeeded->meshFileName << "]" << std::endl;

            return;
        }

        //------------------------------Create thread for soft body-----------------------------------

        if (useThreadOnSoftBody)
        {
            if(mSoftBodyThreadManager == NULL)
                mSoftBodyThreadManager = new cSoftBodyThreaded();

            newSoftBody->bSoftBodyHandledByThread = true;

            mSoftBodyThreadManager->CreateNewSoftBodyThread(newSoftBody, 1.0 / 60.0);
        }
    }
    else
    {
        std::cout << std::endl;
        std::cout << "ERROR : UNABLE TO FIND THE MODEL - [" << meshFileNameNeeded->meshFileName << "]" << std::endl;
    }
    
    //----------------Create debug spheres to preview vertex locations in 3d space----------------

    if (createNewDebugSpheres)
    {
        InitializeDebugRenderer(shaderProgramID);

        newSoftBody->bEnableDebugSphere = true;
        newSoftBody->debugSpheresScaleValue = scaleVal;
        newSoftBody->debugSpheresColor = sphereRGBColor;
        newSoftBody->debugModelName = debugMeshName;

        std::vector < glm::vec3 > addPositions;

        for (unsigned int index = 0; index < newSoftBody->nodesList.size(); index++)
        {
            addPositions.push_back(newSoftBody->nodesList[index]->currentPosition);
        }

        CreateDebugSpheres(debugMeshName, addPositions, scaleVal, sphereRGBColor, meshFileNameNeeded->sceneId);
    }

    SoftBodyList.push_back(newSoftBody);
}

void cControlGameEngine::CreateVerletSoftBody(std::string modelName, glm::vec3 softbodyAcceleration, bool createNewDebugSpheres,
    bool isBall, bool isChain, bool isCube, bool bApplyDamping)
{
    cSoftBody* newSoftBody = new cSoftBody();

    cMesh* meshFileNameNeeded = g_pFindMeshByFriendlyName(modelName);

    sModelDrawInfo& softBodyObjectDrawingInfo = *meshFileNameNeeded->modelDrawInfo;

    if (meshFileNameNeeded == NULL)
        return;

    meshFileNameNeeded->bIsSoftBody = false;

    newSoftBody->meshName = meshFileNameNeeded->meshUniqueName;
    newSoftBody->acceleration = softbodyAcceleration;
    newSoftBody->bApplyDampingEffect = bApplyDamping;

    if (isCube)
        newSoftBody->softBodyScaleValue = 20.f;
    else
        newSoftBody->softBodyScaleValue = meshFileNameNeeded->drawScale.x;

    if (isBall)
        newSoftBody->bBallEffect = isBall;
    else if (isChain)
        newSoftBody->bChainEffect = isChain;
    else if (isCube)
        newSoftBody->bCubeEffect = isCube;

    if (meshFileNameNeeded->modelDrawInfo != NULL)
    {
        //-----------------------------Apply matrix transformations-----------------------------------

        glm::mat4 matTransform = glm::translate(glm::mat4(1.0f), meshFileNameNeeded->drawPosition);
        matTransform = matTransform * glm::mat4_cast(meshFileNameNeeded->get_qOrientation());
        matTransform = glm::scale(matTransform, meshFileNameNeeded->drawScale);

        //-----------------------------------Create soft body-----------------------------------------

        if (newSoftBody->CreateSoftBody(softBodyObjectDrawingInfo, meshFileNameNeeded->drawPosition, matTransform))
        {
            std::cout << std::endl;
            std::cout << "SUCCESS : CREATED SOFT BODY FOR THE MODEL - [" << meshFileNameNeeded->meshFileName << "]" << std::endl;
        }
        else
        {
            std::cout << std::endl;
            std::cout << "ERROR : UNABLE TO CREATE SOFT BODY FOR THE MODEL - [" << meshFileNameNeeded->meshFileName << "]" << std::endl;

            return;
        }
    }
    else
    {
        std::cout << std::endl;
        std::cout << "ERROR : UNABLE TO FIND THE MODEL - [" << meshFileNameNeeded->meshFileName << "]" << std::endl;
    }

    //----------------Create debug spheres to preview vertex locations in 3d space----------------

    if (createNewDebugSpheres)
    {
        InitializeDebugRenderer(shaderProgramID);

        newSoftBody->bEnableDebugSphere = true;
        newSoftBody->debugSpheresScaleValue = 0.25f;
        newSoftBody->debugSpheresColor = glm::vec4(0.1, 0.1, 0.1, 1.0);
        newSoftBody->debugModelName = "DebugSphere_" + meshFileNameNeeded->meshUniqueName;

        std::vector < glm::vec3 > addPositions;

        for (unsigned int index = 0; index < newSoftBody->nodesList.size(); index++)
        {
            addPositions.push_back(newSoftBody->nodesList[index]->currentPosition);
        }

        CreateDebugSpheres(newSoftBody->debugModelName, addPositions, newSoftBody->debugSpheresScaleValue, newSoftBody->debugSpheresColor, meshFileNameNeeded->sceneId);
    }

    SoftBodyList.push_back(newSoftBody);
}

void cControlGameEngine::ApplyRandomBracingToSoftBody(std::string modelName, unsigned int bracesCount, float distanceBetweenVertices)
{
    for (int index = 0; index < SoftBodyList.size(); index++)
    {
        if (SoftBodyList[index]->meshName == modelName)
            SoftBodyList[index]->CreateRandomBracing(bracesCount, distanceBetweenVertices);
    }
}

void cControlGameEngine::ApplyWheelBracingToSoftBody(std::string modelName)
{
    for (int index = 0; index < SoftBodyList.size(); index++)
    {
        if (SoftBodyList[index]->meshName == modelName)
            SoftBodyList[index]->CreateWheelBracing();
    }
}

void cControlGameEngine::UpdateSoftBodyTransformation()
{
    for (unsigned int index = 0; index < SoftBodyList.size(); index++)
    {
        if (!SoftBodyList[index]->bSoftBodyHandledByThread)
        {
            SoftBodyList[index]->VerletUpdate(deltaTime);
            SoftBodyList[index]->ApplyCollision(deltaTime, SoftBodyList);
            SoftBodyList[index]->SatisfyConstraints();
            SoftBodyList[index]->UpdateVertexPositions();
            SoftBodyList[index]->UpdateNormals();
        }
        else
        {
            mSoftBodyThreadManager->CheckThreadActivity();
            mSoftBodyThreadManager->MakeThreadsRunnable();
        }

        mVAOManager->UpdateVertexBuffers(SoftBodyList[index]->meshName, SoftBodyList[index]->mModelVertexInfo, shaderProgramID);

        if (SoftBodyList[index]->bEnableDebugSphere)
        {
            std::vector <glm::vec3> newPositions;

            for (unsigned int vertexIndex = 0; vertexIndex < SoftBodyList[index]->nodesList.size(); vertexIndex++)
                newPositions.push_back(SoftBodyList[index]->nodesList[vertexIndex]->currentPosition);

            UpdateDebugSpherePositions(SoftBodyList[index]->debugModelName , newPositions);
        }
    }
}

void cControlGameEngine::ApplyAccelerationToSoftBody(std::string modelName, glm::vec3 acceleration)
{
    for (int index = 0; index < SoftBodyList.size(); index++)
    {
        if (SoftBodyList[index]->meshName == modelName)
            SoftBodyList[index]->ApplyDirectionAcceleration(acceleration);
    }
}

void cControlGameEngine::ConnectTwoSoftBodies(std::string topSoftBodyName, std::string bottomSoftBodyName)
{
    sNode* topSoftBodyBottomNode = NULL;
    sNode* bottomSoftBodyTopNode = NULL;

    cSoftBody* topSoftBody = NULL;
    cSoftBody* bottomSoftBody = NULL;

    cMesh* topSoftBodyMesh = g_pFindMeshByFriendlyName(topSoftBodyName);

    if (topSoftBodyMesh == NULL)
        return;

    cMesh* bottomSoftBodyMesh = g_pFindMeshByFriendlyName(bottomSoftBodyName);

    if (bottomSoftBodyMesh == NULL)
        return;

    for (int index = 0; index < SoftBodyList.size(); index++)
    {
        if (SoftBodyList[index]->meshName == topSoftBodyMesh->meshUniqueName)
            topSoftBody = SoftBodyList[index];

        if (SoftBodyList[index]->meshName == bottomSoftBodyMesh->meshUniqueName)
            bottomSoftBody = SoftBodyList[index];
    }

    topSoftBodyBottomNode = topSoftBody->GetBottomNode();
    bottomSoftBodyTopNode = bottomSoftBody->GetTopNode();

    topSoftBody->AddBottomConnectorNode(bottomSoftBodyTopNode);
    bottomSoftBody->AddTopConnectorNode(topSoftBodyBottomNode);
}

void cControlGameEngine::FixSoftBodyPosition(std::string softBodyMeshName, glm::vec3 fixedPosition)
{
    //------------------Create a new imaginary node with a fixed position---------------------------

    sNode* newNode = new sNode();

    newNode->currentPosition = fixedPosition;
    newNode->oldPosition = fixedPosition;
    newNode->bFixedNode = true;

    //----------------Add the new node to the soft body as a connector node-------------------------

    cMesh* topSoftBodyMesh = g_pFindMeshByFriendlyName(softBodyMeshName);

    if (topSoftBodyMesh == NULL)
        return;

    for (int index = 0; index < SoftBodyList.size(); index++)
    {
        if (SoftBodyList[index]->meshName == topSoftBodyMesh->meshUniqueName)
            SoftBodyList[index]->AddTopConnectorNode(newNode);
    }
}

void cControlGameEngine::ApplyImaginaryCenterNodeBracing(std::string modelName)
{
    for (int index = 0; index < SoftBodyList.size(); index++)
    {
        if (SoftBodyList[index]->meshName == modelName)
            SoftBodyList[index]->CreateImaginaryCenterNodeBracing();
    }
}

void cControlGameEngine::ToggleDebugSphereVisibility(bool turnOnVisibility)
{
    for (unsigned int index = 0; index < SoftBodyList.size(); index++)
    {
        if (turnOnVisibility)
        {
            for (unsigned int vertexIndex = 0; vertexIndex < SoftBodyList[index]->nodesList.size(); vertexIndex++)
                TurnVisibilityOn(SoftBodyList[index]->debugSpheresMeshNameList[vertexIndex]);
        }
        else
        {
            for (unsigned int vertexIndex = 0; vertexIndex < SoftBodyList[index]->nodesList.size(); vertexIndex++)
                TurnVisibilityOff(SoftBodyList[index]->debugSpheresMeshNameList[vertexIndex]);
        }

        SoftBodyList[index]->bEnableDebugSphere = turnOnVisibility;
    }
}

void cControlGameEngine::TerminateSoftBodyThreads()
{
    if(mSoftBodyThreadManager != NULL)
        mSoftBodyThreadManager->TerminateThreads();
}

void cControlGameEngine::MoveSoftBodyUsingImaginaryCenterNode(std::string modelName, glm::vec3 nodePos)
{
    for (int index = 0; index < SoftBodyList.size(); index++)
    {
        if (SoftBodyList[index]->meshName == modelName)
            SoftBodyList[index]->MoveImaginaryCenterNode(nodePos);
    }
}

glm::vec3 cControlGameEngine::GetCurrentSoftBodyPosition(std::string modelName)
{
    for (int index = 0; index < SoftBodyList.size(); index++)
    {
        if (SoftBodyList[index]->meshName == modelName)
            return SoftBodyList[index]->GetCurrentPos();
    }
}

//----------------------------------DEBUG RENDERER CONTROLS-------------------------------------------------------------

void cControlGameEngine::InitializeDebugRenderer(GLuint shaderId)
{
    if (mDebugRenderManager == NULL)
    {
        mDebugRenderManager = new cDebugRenderer();
        /*   mDebugRendererShaderManager = new cShaderManager();
           mDebugRendererMeshManager = new cVAOManager();*/

        mDebugRenderManager->debugShaderProgramID = shaderId;

        if (!mDebugRenderManager->Initialize(mDebugRendererShaderManager, mDebugRendererMeshManager, cDebugRenderer_vertex_buffer, cDebugRenderer_VAO))
            std::cout << "ERROR : Initializing Debug Renderer" << std::endl;
    }
}

void cControlGameEngine::CreateDebugSpheres(std::string debugSphereName, std::vector<glm::vec3> spherePositionsList, float scale, glm::vec4 colourRGBA, unsigned int meshSceneId)
{
    std::string debugSphereFileName = "Sphere_1_unit_Radius_xyz_n_rgba_uv.ply";

    cMesh* newDebugSphereMesh = new cMesh();
    sModelDrawInfo* debugSphereModel = new sModelDrawInfo;

    if (!mVAOManager->LoadModelIntoVAO(debugSphereName, debugSphereFileName, *debugSphereModel, shaderProgramID))
        std::cout << "ERROR : Unable to load debug sphere" << std::endl;

    newDebugSphereMesh->sceneId = meshSceneId;
    newDebugSphereMesh->meshFileName = debugSphereFileName;
    newDebugSphereMesh->meshUniqueName = debugSphereName;
    newDebugSphereMesh->bIsWireframe = true;
    newDebugSphereMesh->bDoNotLight = true;
    newDebugSphereMesh->bIsVisible = false;
    newDebugSphereMesh->setUniformDrawScale(scale);
    newDebugSphereMesh->bUseManualColours = true;
    newDebugSphereMesh->wholeObjectManualColourRGBA = colourRGBA;

    this->mDebugRenderManager->CreateNewDebugSpheres(newDebugSphereMesh, spherePositionsList);

    this->mSceneManager->AddDebugMeshToScene(meshSceneId, newDebugSphereMesh, this->mDebugRenderManager);
}

void cControlGameEngine::UpdateDebugSpherePositions(std::string modelName, std::vector<glm::vec3> spherePositionsList)
{
    mDebugRenderManager->UpdateDebugSpherePositions(modelName, spherePositionsList);
}

//------------------------------------SCENE & FBO CONTROLS--------------------------------------------------------------

void cControlGameEngine::CreateANewScene(bool isTheMainScene, std::vector <glm::vec3> camPos, std::vector <glm::vec3> camTarget)
{
    this->mSceneManager->CreateScene(isTheMainScene, camPos, camTarget);
}

void cControlGameEngine::MakeSceneIntoFBOTexture(unsigned int sceneNum, unsigned int width, unsigned int height)
{
    this->mSceneManager->ConvertSceneToFBOTexture(sceneNum, width, height);
}

void cControlGameEngine::RenderMeshWithFBOTexture(unsigned int sceneNum, std::string modelName, unsigned int frameBufferId)
{
    cMesh* meshNeedingTexture = g_pFindMeshByFriendlyName(modelName);

    this->mSceneManager->AddFBOTextureToMesh(sceneNum, meshNeedingTexture, frameBufferId);
}

void cControlGameEngine::AddSecondPassFilterToScene(std::string modelName, bool useFSQ, bool useStandardColor,
    bool useChromicAberration, bool UseScreenMeasurements, bool useNightVision)
{
    cMesh* meshTextureFilter = g_pFindMeshByFriendlyName(modelName);

    if (meshTextureFilter != NULL)
        this->mSceneManager->AddSecondPassFilter(meshTextureFilter, useFSQ, useStandardColor, useChromicAberration, UseScreenMeasurements, useNightVision);
    else
        std::cout << "ERROR : MESH NAME PROVIDED DOES NOT EXIST" << std::endl;
}

void cControlGameEngine::AnimateChromicAmount(std::string modelName, float offsetAmount)
{
    cMesh* meshTextureFilter = g_pFindMeshByFriendlyName(modelName);

    if (meshTextureFilter == NULL)
        std::cout << "ERROR : MESH NAME PROVIDED DOES NOT EXIST" << std::endl;
    else
    {
        if (meshTextureFilter->mSecondPassFilters.bUseChromicAberration)
        {
            if (!meshTextureFilter->mSecondPassFilters.reverseAnimation)
            {
                if (meshTextureFilter->mSecondPassFilters.chromicAberrationAmount < 0.7f)
                    meshTextureFilter->mSecondPassFilters.chromicAberrationAmount += offsetAmount;
                else
                    meshTextureFilter->mSecondPassFilters.reverseAnimation = true;
            }
            else
            {
                if (meshTextureFilter->mSecondPassFilters.chromicAberrationAmount > 0.f)
                    meshTextureFilter->mSecondPassFilters.chromicAberrationAmount -= offsetAmount;
                else
                    meshTextureFilter->mSecondPassFilters.reverseAnimation = false;
            }
        }

        else
            std::cout << "ERROR : MESH NAME PROVIDED DOES NOT USE CHROMIC ABERRATION FILTER" << std::endl;
    }
}

void cControlGameEngine::SwitchCurrentToMainScene(unsigned int sceneIdToSwitchTo)
{
    this->mSceneManager->SwitchMainScene(sceneIdToSwitchTo);
}

//--------------------------------------ENGINE CONTROLS-----------------------------------------------------------------

int cControlGameEngine::InitializeGameEngine()
{
    //-------------------------------------Shader Initialize----------------------------------------------------------------

    int result = InitializeShader();

    if (result != 0)
        return -1;

    //-------------------------------------VAO Initialize--------------------------------------------------------------------------
    
    if (mVAOManager == NULL)
        mVAOManager = new cVAOManager();

    mVAOManager->setBaseModelPath("Assets/Models");
    mVAOManager->setBaseAnimationPath("Assets/Animations");
    
    if (mPhysicsManager == NULL)
        mPhysicsManager = new cPhysics();

    mPhysicsManager->setVAOManager(mVAOManager);

    //------------------------------------Lights Initialize-----------------------------------------------------------------------

    if (mLightManager == NULL)
        mLightManager = new cLightManager();

    //------------------------------------Texture Initialize----------------------------------------------------------------------

    if (mTextureManager == NULL)
        mTextureManager = new cBasicTextureManager();

    mTextureManager->SetBasePath("Assets/Textures");

    //-----------------------------------Cube Map Initialize----------------------------------------------------------------------
    
    if (mCubeMapManager == NULL)
        mCubeMapManager = new cCubeMap();

    //-----------------------------------Animation Initialize---------------------------------------------------------------------

    if (mAnimationManager == NULL)
        mAnimationManager = new cAnimationSystem();

    //-------------------------------------Camera Initialize----------------------------------------------------------------------

    if (mCamManager == NULL)
        mCamManager = new sCameraAttributes();

    //---------------------------------Scene Manager Initialize-------------------------------------------------------------------
    
    if (mSceneManager == NULL)
        mSceneManager = new cSceneManager();

    mSceneManager->InitializeSceneManager(this->mVAOManager, this->mTextureManager, this->mCubeMapManager, this->mLightManager);

    //----------------------------------Initialize Mesh Count---------------------------------------------------------------------

    this->InitializeMeshCount();

    return 0;
}

void cControlGameEngine::RunGameEngine(GLFWwindow* window)
{
    this->mLightManager->UpdateUniformValues(shaderProgramID);

    this->mSceneManager->LoadScene(shaderProgramID, window, this->TotalMeshList, this->mCamManager->mCameraEye, this->mCamManager->mCameraTarget, this->mCamManager->bFreeFlowCamera, this->bApplyLOD,
        this->bDrawColliders, this->AABBMeshesList);

    //----------------------------Title Screen Values---------------------------------------------

    std::stringstream ssTitle;

    if (this->mCamManager->bEditorModeOn) // If Editor Mode is On
    {
        if (this->mCamManager->bLightEditorMode)
        {
            //---------------------Light values displayed---------------------------

            int lightId = lightListIndex;

            glm::vec3 lightPosition = GetLightPosition(lightId);
            float lightLinearAtten = GetLightLinearAttenuation(lightId);
            float lightQuadraticAtten = GetLightQuadraticAttenuation(lightId);
            glm::vec3 lightDirection = GetLightDirection(lightId);
            float lightType = GetLightType(lightId);
            float lightInnerAngle = GetLightInnerAngle(lightId);
            float lightOuterAngle = GetLightOuterAngle(lightId);

            ssTitle << "Light Id : "
                << lightId << " | Light On : "
                << IsLightOn(lightId) << " | Pos : ("
                << lightPosition.x << ", "
                << lightPosition.y << ", "
                << lightPosition.z << ") | Dir : ("
                << lightDirection.x << ", "
                << lightDirection.y << ", "
                << lightDirection.z << ") | Linear Atten : "
                << lightLinearAtten << " | Quad Atten : "
                << lightQuadraticAtten << " | Type : "
                << lightType << " | Inner Angle : "
                << lightInnerAngle << " | Outer Angle : "
                << lightOuterAngle;
        }

        else
        {
            //----------------Cam and Model values displayed-----------------------------

            cMesh* meshObj = GetCurrentModelSelected();

            ssTitle << "Camera Eye(x, y, z) : ("
                <<   this->mCamManager->mCameraEye.x << ", "
                <<   this->mCamManager->mCameraEye.y << ", "
                <<   this->mCamManager->mCameraEye.z << ") | "
                << "Camera Target(x,y,z): ("
                <<  this->mCamManager->mCameraTarget.x << ", "
                <<  this->mCamManager->mCameraTarget.y << ", "
                <<  this->mCamManager->mCameraTarget.z << ") | Yaw/Pitch : ("
                << mCamManager->mYaw << ", " << mCamManager->mPitch << ") | ModelName : "
                << meshObj->meshUniqueName << " | ModelPos : ("
                << meshObj->drawPosition.x << ", "
                << meshObj->drawPosition.y << ", "
                << meshObj->drawPosition.z << ") | ModelScaleVal : "
                << meshObj->drawScale.x << " | ModelRotateDeg : ("
                << meshObj->drawOrientation.x << ", "
                << meshObj->drawOrientation.y << ", "
                << meshObj->drawOrientation.z << ")";
        }
    }
    else
        ssTitle << "ESCAPE WORLD";

    std::string theTitle = ssTitle.str();

    glfwSwapBuffers(window);

    glfwPollEvents();

    glfwSetWindowTitle(window, theTitle.c_str());
}


