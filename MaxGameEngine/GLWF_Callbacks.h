#pragma once
#include "OpenGLCommon.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

void mouse_callback(GLFWwindow* window, double xPos, double yPos);

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);