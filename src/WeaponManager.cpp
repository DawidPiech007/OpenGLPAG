
#include <WeaponManager.hpp>
#include <glm\ext\matrix_transform.hpp>
#include <glad\glad.h>
#include <glm\ext\matrix_clip_space.hpp>
#include <iostream>


WeaponManager::WeaponManager(std::shared_ptr<GraphNode> firstWeapon, float weaponDelay, float bullets, std::shared_ptr<GraphNode> shootEffect, float effectDiuration)
{
    WeaponManager::shootEffect = shootEffect;
    WeaponManager::effectDiuration = effectDiuration;

    curentDelay = 0;
    shootTime = 0;

    AddWeapon(firstWeapon, weaponDelay, bullets);

    weaponSelected = 0;

    shootTime = 0;
    curentDelay = 0;
    WeaponManager::effectDiuration = 0;

    effectActive = false;
    shootReady = true;

    //weaponModels = new vector<GraphNode>();
}

void WeaponManager::AddWeapon(std::shared_ptr<GraphNode> weaponModel, float weaponDelay, float bullets)
{
    weaponDelays.push_back(weaponDelay);
    weaponModels.push_back(weaponModel);
    WeaponManager::bullets.push_back(bullets);
}

void WeaponManager::Update(float time)
{
}

void WeaponManager::InputKey(GLFWwindow* window, float deltaTime)
{
    float cameraSpeed = 2.0f * deltaTime; 
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        std::cout << "strza;" << std::endl;
    } 
}

void WeaponManager::InputScroll(float yoffset)
{
    int weaponLen = weaponModels.size();
    int newWeapon = (weaponLen * 2 + weaponSelected + (int)yoffset) % weaponLen;
    //std::cout << "newWeapon      = " << newWeapon << std::endl;
    //std::cout << "weaponSelected = " << weaponSelected << std::endl;

    if (newWeapon != weaponSelected)
    {
        HideWeapon(weaponSelected);
        weaponSelected = newWeapon;
        ShowWeapon(weaponSelected);
    }
}

void WeaponManager::ShowWeapon(int index)
{
    weaponModels[index]->SetPosition(0.1f, -0.15f, -0.5f);
}

void WeaponManager::HideWeapon(int index)
{
    weaponModels[index]->SetPosition(0.1f, -2.15f, -0.5f);
}

void WeaponManager::Shoot()
{
}
