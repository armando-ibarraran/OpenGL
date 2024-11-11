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
void drawWall(float alpha, glm::mat4 transform, Shader shader);
void drawBarTop(glm::mat4 transform, Shader shader);
void drawBarBottom(glm::mat4 transform, Shader shader);
glm::mat4 shearY(float k);
std::vector<float> jumpTrajectory(float x0, float y0, float xf, float yf, float tf, float alpha);
std::vector<float> getTranslateToCirc(float x0, float y0, float xc, float yc, float r, float t);

#endif