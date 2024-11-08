#ifndef TILEDISMEMBERING
#define TILEDISMEMBERING

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader.h>
#include <shader_m.h>
#include <shader_s.h>
#include <stb_image.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp> 
#include <vector>
#include <ctime>
#include <sys\timeb.h>
#include "helperFunctions.h"


void dismemberTile(GLFWwindow* window);
std::vector<float> outerTrianglesVertices(int numTriangles, int numVertices);

#endif