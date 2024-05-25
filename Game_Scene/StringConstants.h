#ifndef STRING_CONSTANTS_H
#define STRING_CONSTANTS_H

#include <string>

namespace StringConstants 
{
    //---------------------Model Names------------------------------

    extern const std::string PlayerModelName;
    extern const std::string VisorModelName;
    extern const std::string TopViewScreenModelName;
    extern const std::string CapsuleModelName;
    extern const std::string ShieldModelName;
    extern const std::string SphereColliderModelName;

    //------------------Model File Names----------------------------

    extern const std::string PlayerModelFileName;
    extern const std::string ScreenModelFileName;
    extern const std::string SphereModelFileName;
    extern const std::string WallModelFileName;
    extern const std::string FloorModelFileName;
    extern const std::string CapsuleModelFileName;
    extern const std::string CubeModelFileName;

    //-----------------Texture File Names---------------------------

    extern const std::string FloorTextureFileName;
    extern const std::string WallTextureFileName;
    
    //--------------------Texture Names-----------------------------
    
    extern const std::string HudTextureName;
    extern const std::string FloorTextureName;
    extern const std::string WallTextureName;
                
   //-------------------Animation Names----------------------------
                 
    extern const std::string RunAnimName;
    extern const std::string WalkAnimName;
    extern const std::string IdleAnimName;
    extern const std::string AttackAnimName;
    extern const std::string LevitateAnimName;

    
    //-----------------Animation File Name--------------------------
    
    extern const std::string RunAnimFileName;
    extern const std::string WalkAnimFileName;
    extern const std::string IdleAnimFileName;
    extern const std::string AttackAnimFileName;
    extern const std::string LevitateAnimFileName;
}

#endif