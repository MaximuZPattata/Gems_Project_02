#include "pch.h"
#include "cMesh.h"
#include "cJsonReader.h"
#include "cJsonWriter.h"
#include "cHiResTimer.h"
#include "sMouseAttributes.h"
#include "cAnimationSystem.h"
#include "cMap.h"
#include "StringConstants.h"
#include "sLoadAABBThreadDetails.h"
#include "cDropBallSystem.h"

#include <cControlGameEngine.h>

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

//---------------------------Global Objects-----------------------------------------------

GLFWwindow* window;
cControlGameEngine gameEngine;
cJsonReader jsonReader;
cJsonWriter jsonWriter;
sCubeMapDetailsFromFile cubeMapDetails;
sMouseAttributes mouseAttributes;
cMap mazeCreator;
cDropBallSystem dropBallSystemControl;
sLoadAABBThreadDetails* loadAABBThread = new sLoadAABBThreadDetails(gameEngine);
cHiResTimer* timer = new cHiResTimer(60);

std::vector<sSceneDetailsFromFile> sceneDetailsList; 
std::vector<sModelDetailsFromFile> modelDetailsList;
std::vector<sLightDetailsFromFile> lightDetailsList;
std::vector<sPhysicsDetailsFromFile> physicsDetailsList;

void UpdatePerFrame();
void DeleteThreadsAndRelevantData();
bool InitializeGameAttributes();

extern void MakeThreadCallToLoadOtherAABBs(sLoadAABBThreadDetails* loadOtherAABBs, std::vector < std::string > ModelFilePathList, int startingIndex, int endingIndex);

using namespace StringConstants;

int main()
{
    //-----------------------------------Initialize Window--------------------------------------

    int result = 0;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(1200, 800, "Game World", NULL, NULL);
    //window = glfwCreateWindow(1200, 800, "Game World", glfwGetPrimaryMonitor(), NULL);

    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    //------------------------------Input key and Cursor initialize-----------------------------

    glfwSetKeyCallback(window, key_callback);

    glfwSetCursorPosCallback(window, mouse_callback);

    glfwSetScrollCallback(window, scroll_callback);

    glfwSetMouseButtonCallback(window, mouse_button_callback);

    glfwMakeContextCurrent(window);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);

    //--------------------------------Initialize Game Engine----------------------------------------

    result = gameEngine.InitializeGameEngine();

    if (result != 0)
        return -1;

    //----------------------------------------------------Loading All Model Assets------------------------------------------------------------------

    std::vector <std::string> ModelFileNameList;

    ModelFileNameList.push_back(SphereModelFileName);
    ModelFileNameList.push_back(WallModelFileName);
    ModelFileNameList.push_back(FloorModelFileName);
    ModelFileNameList.push_back(CubeModelFileName);

    gameEngine.LoadAllModelAssets(ModelFileNameList);

    //---------------------------------------------------Loading All Model Textures-----------------------------------------------------------------

    std::vector <std::string> TextureFileNameList;

    TextureFileNameList.push_back(FloorTextureFileName);

    gameEngine.LoadAllTextures(TextureFileNameList);

    //--------------------------------Loading Models, Lights and initial camera position from Json file---------------------------------------------

    bool jsonresult = jsonReader.ReadScene("SceneDescription.json", sceneDetailsList, modelDetailsList, cubeMapDetails, physicsDetailsList, lightDetailsList);

    if (jsonresult)
    {
        std::cout << "File read successfully !" << std::endl;

        std::string modelName;
        float angleDegrees;

        //-----------------------------------Adding Scenes--------------------------------------------------------------------------------

        for (int index = 0; index < sceneDetailsList.size(); index++)
        {
            if (sceneDetailsList[index].bIsMainScene)
            {
                glm::vec3 camPos = sceneDetailsList[index].sceneCameraPositionList[0];
                glm::vec3 camTarget = sceneDetailsList[index].sceneCameraTargetList[0];

                gameEngine.MoveCameraPosition(camPos.x, camPos.y, camPos.z);                                        
            }

            gameEngine.CreateANewScene(sceneDetailsList[index].bIsMainScene, sceneDetailsList[index].sceneCameraPositionList, sceneDetailsList[index].sceneCameraTargetList);

            if (sceneDetailsList[index].bIsFBOTexture)
                gameEngine.MakeSceneIntoFBOTexture(sceneDetailsList[index].sceneId, sceneDetailsList[index].windowWidth, sceneDetailsList[index].windowHeight);
        }

        //-----------------------------------Adding Models--------------------------------------------------------------------------------

        for (int index = 0; index < modelDetailsList.size(); index++)
        {
            modelName = modelDetailsList[index].modelName;

            //---------------------------Load Models with position----------------------------------------------------------------------

            if (modelDetailsList[index].isCollider)
            {
                gameEngine.LoadColliderModelsInto3DSpace(modelDetailsList[index].modelFilePath, modelName, modelDetailsList[index].modelPosition.x,
                    modelDetailsList[index].modelPosition.y, modelDetailsList[index].modelPosition.z);
            }
            else
            {
                gameEngine.LoadModelsInto3DSpace(modelDetailsList[index].modelFilePath, modelName, modelDetailsList[index].modelPosition.x,
                    modelDetailsList[index].modelPosition.y, modelDetailsList[index].modelPosition.z);
            }

            //--------------------------------Adding LOD Info---------------------------------------------------------------------------
              
            if (modelDetailsList[index].useLOD)
                gameEngine.AddLODToMesh(modelName, modelDetailsList[index].fileNameLOD, modelDetailsList[index].minimumDistanceLOD, modelDetailsList[index].isDefaultLOD);

            //---------------------------------Rotate Models----------------------------------------------------------------------------

            if (modelDetailsList[index].modelOrientation.x != 0.f)
            {
                angleDegrees = modelDetailsList[index].modelOrientation.x;
                gameEngine.RotateMeshModel(modelName, angleDegrees, 1.f, 0.f, 0.f);
            }
            if (modelDetailsList[index].modelOrientation.y != 0.f)
            {
                angleDegrees = modelDetailsList[index].modelOrientation.y;
                gameEngine.RotateMeshModel(modelName, angleDegrees, 0.f, 1.f, 0.f);
            }
            if (modelDetailsList[index].modelOrientation.z != 0.f)
            {
                angleDegrees = modelDetailsList[index].modelOrientation.z;
                gameEngine.RotateMeshModel(modelName, angleDegrees, 0.f, 0.f, 1.f);
            }

            //------------------------------------Scale Models-----------------------------------------------------------------------------

            gameEngine.ScaleModel(modelName, modelDetailsList[index].modelScaleValue);

            //-----------------------------------Model Wireframe---------------------------------------------------------------------------

            if (modelDetailsList[index].wireframeModeOn)
                gameEngine.TurnWireframeModeOn(modelName);

            //----------------------------------Model Mesh Light---------------------------------------------------------------------------

            if (modelDetailsList[index].meshLightsOn)
                gameEngine.TurnMeshLightsOn(modelName);

            //-------------------------------------Model Bones-----------------------------------------------------------------------------

            if (modelDetailsList[index].applyBones)
                gameEngine.ApplyBonesToMesh(modelName, true);

            //----------------------------------Cube Map Details-----------------------------------------------------------------------------

            if (modelDetailsList[index].isSkyBox)
            {
                if (!cubeMapDetails.cubeMapName.empty())
                {
                    result = gameEngine.AddCubeMap(modelName, cubeMapDetails.cubeMapName, cubeMapDetails.filePathPosX, cubeMapDetails.filePathNegX,
                        cubeMapDetails.filePathPosY, cubeMapDetails.filePathNegY, cubeMapDetails.filePathPosZ,
                        cubeMapDetails.filePathNegZ);
                    if (result != 0)
                        return -1;
                }
                else
                {
                    std::cout << "Cube Properties wasnt updated or added in the json file" << std::endl;

                    return -1;
                }
            }
            else
            {
                //------------------------------------Color Models-----------------------------------------------------------------------------

                if (modelDetailsList[index].manualColors)
                {
                    gameEngine.UseManualColors(modelName, true);
                    gameEngine.ChangeColor(modelName, modelDetailsList[index].modelColorRGB.x, modelDetailsList[index].modelColorRGB.y, modelDetailsList[index].modelColorRGB.z);
                }

                //------------------------------------Add Textures------------------------------------------------------------------------------

                if (modelDetailsList[index].useTextures)
                {
                    gameEngine.UseTextures(modelName, true);

                    for (int textureIndex = 0; textureIndex < modelDetailsList[index].textureNameList.size(); textureIndex++)
                    {
                        if (modelDetailsList[index].blendTexture[textureIndex])
                            gameEngine.AddTexturesToTheMix(modelName, modelDetailsList[index].textureFilePathList[textureIndex],
                                modelDetailsList[index].textureNameList[textureIndex], modelDetailsList[index].textureRatiosList[textureIndex]);
                        else
                            gameEngine.AddTexturesToOverlap(modelName, modelDetailsList[index].textureFilePathList[textureIndex],
                                modelDetailsList[index].textureNameList[textureIndex]);

                        if (modelDetailsList[index].useDiscardMaskTexture[textureIndex])
                            gameEngine.AddDiscardMaskTexture(modelName, modelDetailsList[index].textureNameList[textureIndex],
                                modelDetailsList[index].discardMaskTextureFilePath[textureIndex]);
                    }
                }
            }

            //--------------------------------Add Model into Scene--------------------------------------------------------------------------

            gameEngine.SetMeshSceneId(modelDetailsList[index].modelName, modelDetailsList[index].meshSceneId);

            //-----------------------------------Adding Physics------------------------------------------------------------------------------

            if (modelDetailsList[index].physicsMeshType == "Sphere") // Sphere Physics
            {
                for (int physicsIndex = 0; physicsIndex < physicsDetailsList.size(); physicsIndex++)
                {
                    if (physicsDetailsList[physicsIndex].modelName == modelName)
                    {
                        gameEngine.AddSpherePhysicsToMesh(modelName, modelDetailsList[index].physicsMeshType, physicsDetailsList[physicsIndex].modelRadius);

                        if (physicsDetailsList[physicsIndex].randomVelocity)
                        {
                            float randomVelocity = gameEngine.getRandomFloat(0.7, 5.0);

                            gameEngine.ChangeModelPhysicsVelocity(modelName, glm::vec3(0.0f, -5.0f, 0.0f));
                        }
                        else
                            gameEngine.ChangeModelPhysicsVelocity(modelName, glm::vec3(physicsDetailsList[physicsIndex].modelVelocity.x, physicsDetailsList[physicsIndex].modelVelocity.y,
                                physicsDetailsList[physicsIndex].modelVelocity.z));

                        gameEngine.ChangeModelPhysicsAcceleration(modelName, glm::vec3(physicsDetailsList[physicsIndex].modelAcceleration.x, physicsDetailsList[physicsIndex].modelAcceleration.y,
                            physicsDetailsList[physicsIndex].modelAcceleration.z));

                        result = gameEngine.ChangeModelPhysicalMass(modelName, physicsDetailsList[physicsIndex].modelMass);

                        if (result != 0)
                            std::cout << "Mass provided for the sphere - '" << modelName << "' is not above 0. Hence default mass 1.0/10.0 is used for this sphere model." << std::endl;
                    }
                }
            }
            else if (modelDetailsList[index].physicsMeshType == "Plane" || modelDetailsList[index].physicsMeshType == "Box") // Plane Physics
            {
                gameEngine.AddPlanePhysicsToMesh(modelName, modelDetailsList[index].physicsMeshType);
            }
        }

        //-----------------------------------Adding Lights--------------------------------------------------------------------------------

        for (int index = 0; index < lightDetailsList.size(); index++)
        {
            glm::vec4 lightPos = glm::vec4(lightDetailsList[index].lightPosition.x, lightDetailsList[index].lightPosition.y, lightDetailsList[index].lightPosition.z, 1.f);
            glm::vec4 lightDir = glm::vec4(lightDetailsList[index].lightDirection.x, lightDetailsList[index].lightDirection.y, lightDetailsList[index].lightDirection.z, 1.f);
            glm::vec2 lightAngle = glm::vec2(lightDetailsList[index].innerAngle, lightDetailsList[index].outerAngle);

            gameEngine.AddLight(lightPos, lightDir, lightDetailsList[index].lightOff, lightDetailsList[index].lightType,
                lightDetailsList[index].linearAttenuation, lightDetailsList[index].quadraticAttenuation, lightAngle, lightDetailsList[index].lightColorRGB);
        }
    }
    else
        return -1;

    //-------------------------------Initialize Game Attributes-----------------------------------------

    if (!InitializeGameAttributes())
        return -1;

    //---------------------------------------Frame Loop-------------------------------------------------

    bool bModelLoadThreadHandlesClosed = false;
    bool bAABBLoadThreadHandleClosed = false;

    while (!glfwWindowShouldClose(window))
    {
        //------------------Handle Completed threads-------------------------------------------

        if (!bModelLoadThreadHandlesClosed)
        {
            if (!mazeCreator.mMapLoadthreadDetailsList[0]->bThreadRunning && !mazeCreator.mMapLoadthreadDetailsList[1]->bThreadRunning)
            {
                DeleteThreadsAndRelevantData();
                bModelLoadThreadHandlesClosed = true;

                std::vector < std::string > ModelFilePathList;

                ModelFilePathList.push_back("Assets/Models/" + WallModelFileName);
                ModelFilePathList.push_back("Assets/Models/" + FloorModelFileName);

                MakeThreadCallToLoadOtherAABBs(loadAABBThread, ModelFilePathList, 300, gameEngine.GetMeshCount() - 1);
            }
        }

        if (!bAABBLoadThreadHandleClosed)
        {
            if (!loadAABBThread->bThreadRunning)
            {
                CloseHandle(loadAABBThread->threadHandleCreated);
                bAABBLoadThreadHandleClosed = true;

                printf("AABB Thread closed\n");
            }
        }

        //----------------------Frame Rate Count-----------------------------------------------

        gameEngine.deltaTime = timer->getFrameTime();

        //------------------Update animations-------------------------------------------------

        UpdatePerFrame();

        //--------------------Run Engine------------------------------------------------------

        gameEngine.RunGameEngine(window);
    }

    if(!bModelLoadThreadHandlesClosed)
        DeleteThreadsAndRelevantData();

    if(!bAABBLoadThreadHandleClosed)
        CloseHandle(loadAABBThread->threadHandleCreated);

    glfwDestroyWindow(window);

    glfwTerminate();

    exit(EXIT_SUCCESS);

    std::cout << "Window closed !" << std::endl;

    return 0;
}

void UpdatePerFrame()
{
    // Update Sphere Physics per frame
    //gameEngine.ComparePhysicalAttributesWithOtherModels();
   /* gameEngine.PhysicsEulerIntegration("Sphere_Ball");
    if (gameEngine.ComparePhysicsObjectWithAABBs("Sphere_Ball"))
        gameEngine.MakeCollisionHappen("Sphere_Ball");*/

    dropBallSystemControl.UpdateBallMotion(gameEngine);
}

bool InitializeGameAttributes()
{
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //-------------------------------Setting Camera------------------------------------------

    gameEngine.MoveCameraPosition(-58.0f, 250.0f, 503.f);
    gameEngine.MoveCameraTarget(0.55f, -0.42f, -0.7f);
    gameEngine.SetCamYaw(-52.4f);
    gameEngine.SetCamPitch(-25.f);    

    //--------------------------------Loading Maze-------------------------------------------
    
    if (!mazeCreator.InitializeMap(gameEngine))
    {
        std::cout << "ERROR: UNABLE TO INITIALIZE MAP" << std::endl;

        return -1;
    }

    //------------------------------Initializing AABBs---------------------------------------

    std::string fullWallModelFilePath = "Assets/Models/" + WallModelFileName;
    std::string fullFloorModelFilePath = "Assets/Models/" + FloorModelFileName;

    glm::vec3 startingPoint = glm::vec3(1.f, 1.f, 5.f);

    std::vector < std::string > modelFilePathList;

    modelFilePathList.push_back(fullWallModelFilePath);
    modelFilePathList.push_back(fullFloorModelFilePath);

    gameEngine.CreateSceneAABBs(startingPoint, glm::ivec3(27, 1, 17), 150.0f, CubeModelFileName);
    gameEngine.AddTrianglesToAABB(modelFilePathList, 0, 300);

    //---------------------------Initializing Drop Balls------------------------------------

    dropBallSystemControl.CreateBalls(gameEngine);
    
    return 1;
}

void DeleteThreadsAndRelevantData()
{
    //--------------------------Close Completed Threads----------------------------------------

    CloseHandle(mazeCreator.mMapThreadHandleList[0]);
    CloseHandle(mazeCreator.mMapThreadHandleList[1]);

    printf("Threads deleted !\n");
    printf("Total Meshes : %d\n", gameEngine.GetMeshCount());

    //--------Delete structs instances related to Threads to prevent memory leak---------------

    sMapLoadThreadDetails* wallMapThread = mazeCreator.mMapLoadthreadDetailsList[0];
    sMapLoadThreadDetails* floorMapThread = mazeCreator.mMapLoadthreadDetailsList[1];

    mazeCreator.mMapLoadthreadDetailsList.erase(std::remove(mazeCreator.mMapLoadthreadDetailsList.begin(), mazeCreator.mMapLoadthreadDetailsList.end(), wallMapThread), mazeCreator.mMapLoadthreadDetailsList.end());
    mazeCreator.mMapLoadthreadDetailsList.erase(std::remove(mazeCreator.mMapLoadthreadDetailsList.begin(), mazeCreator.mMapLoadthreadDetailsList.end(), floorMapThread), mazeCreator.mMapLoadthreadDetailsList.end());

    mazeCreator.mMapLoadthreadDetailsList.clear();
}