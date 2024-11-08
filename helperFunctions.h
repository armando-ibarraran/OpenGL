#ifndef HELPERFUNCTIONS
#define HELPERFUNCTIONS

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

void drawFilledEllipse(float a, float b, float x, float y, int numSegments, glm::mat4 transform, Shader shader);
void drawStar(float innerRadius, float outerRadius, float x, float y, int numPoints, glm::mat4 transform, Shader shader);
void drawPolygon(float radius, float x, float y, int numSides, glm::mat4 transform, Shader shader);
void drawFerrisWheelPod(float w, float h, float x, float y, glm::mat4 transform, Shader shader);
void drawFerrisWheel(float x, float y, float r, glm::mat4 transform, Shader shader);
std::vector<float> getEllipsesVertices(float a, float b, float x, float y, int numSegments);
void bindAnimVector(unsigned int VAO, unsigned int VBO, std::vector<float> vector);

#endif