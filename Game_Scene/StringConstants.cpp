#include "StringConstants.h"

namespace StringConstants 
{
    //---------------------Model Names------------------------------

    const std::string PlayerModelName = "Archer";
    const std::string VisorModelName = "Visor";
    const std::string TopViewScreenModelName = "TopViewMap";
    const std::string CapsuleModelName = "Capsule";
    const std::string ShieldModelName = "ShieldSphere";
    const std::string SphereColliderModelName = "SphereCollider";

    //------------------Model File Names----------------------------

    const std::string PlayerModelFileName = "Archer.dae";
    const std::string ScreenModelFileName = "Screen.ply";
    const std::string SphereModelFileName = "Sphere_1_unit_Radius_xyz_n_rgba_uv.ply";
    const std::string WallModelFileName = "Stone_Wall.ply";
    const std::string FloorModelFileName = "Flat_10x10_plane_n_rgba_uv.ply";
    const std::string CapsuleModelFileName = "Capsule.ply";
    const std::string CubeModelFileName = "Cube_1x1x1_xyz_n_rgba_uv.ply";

    //-----------------Texture File Names---------------------------

    const std::string FloorTextureFileName = "Green_Patch.bmp";
    const std::string WallTextureFileName = "Paving_Stone_1_Diffuse.bmp";

    //--------------------Texture Names-----------------------------

    const std::string HudTextureName = "HUD";
    const std::string FloorTextureName = "DungeonTexture";
    const std::string WallTextureName = "WallTexture";

    //-------------------Animation Names----------------------------

    const std::string RunAnimName = "Run";
    const std::string WalkAnimName = "Walk";
    const std::string IdleAnimName = "Idle";
    const std::string AttackAnimName = "Attack";
    const std::string LevitateAnimName = "Levitate";

    //-----------------Animation File Name--------------------------

    const std::string RunAnimFileName = "Torch_Run.dae";
    const std::string WalkAnimFileName = "Torch_Walk.dae";
    const std::string IdleAnimFileName = "Torch_Idle.dae";
    const std::string AttackAnimFileName = "Torch_Attack.dae";
    const std::string LevitateAnimFileName = "Levitating.dae";
}