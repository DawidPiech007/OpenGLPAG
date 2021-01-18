#ifndef WEAPON_MANAGER
#define WEAPON_MANAGER

#include <glm/mat4x4.hpp>
//#include "GraphNode.hpp"
#include <GLFW/glfw3.h>
#include "GraphNode.hpp"
#include <vector>


class WeaponManager
{
public:
	WeaponManager(std::shared_ptr<GraphNode> firstWeapon, float weaponDelay, float bullets, std::shared_ptr<GraphNode> shootEffect, float effectDiuration);

	void AddWeapon(std::shared_ptr<GraphNode> weaponModel, float weaponDelay, float bullets);

	void Update(float time);

	void InputKey(GLFWwindow* window, float deltaTime, float time);
	void InputScroll(float yoffset);

private:
	void ShowWeapon(int index);
	void HideWeapon(int index);
	void ShowEffect();
	void HideEffect();
	void Shoot();

	std::vector<std::shared_ptr<GraphNode>> weaponModels;
	std::shared_ptr<GraphNode> shootEffect;

	std::vector<float> weaponDelays;
	std::vector<int> bullets;

	int weaponSelected;
	float shootTime;
	float curentDelay;
	float effectDiuration;

	bool effectActive;
	bool shootReady;
};

#endif