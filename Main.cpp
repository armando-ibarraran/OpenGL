
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
#include "tilesDismembering.h"
#include "helperFunctions.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


void drawScene1(GLFWwindow* window);
void drawScene2(GLFWwindow* window);
void drawScene3(GLFWwindow* window);
void drawScene4(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1080;
const unsigned int SCR_HEIGHT = 1080;



// =======================================================================
// MAIN
// =======================================================================
int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // =======================================================================
    // RENDER ANIMATIONS
    // =======================================================================
    
    dismemberTile(window, false);
    drawScene1(window);
    drawScene2(window);
    drawScene3(window);
    drawScene4(window);
    dismemberTile(window, true);


    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glfwPollEvents();
    }


    // =======================================================================
    // END
    // =======================================================================
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}





// =======================================================================
// FUNCTIONS
// =======================================================================

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.

    glViewport(0, 0, width, height);
}


// This function draws the scene 1, where our main character is denied access to the ferris wheel
void drawScene1(GLFWwindow* window) {

    int numSegments = 5; // Number of segments the animation is divided into
    float numTurns = 1.25f; // NUmber of turns the ferris wheel will do per scene
    float alpha, inclination = 0.5f, pulsatingFrec = 10.0f, elapseTreshold = 10.0f;
    bool animEnded = false;
    float ferris[] = {0.5f, -0.45f, 0.45f}; // radius,x,y
    float mainChar[] = { 0.1f, 0.1f, 0.15f, -0.6f }; // a,b,x,y
    float scndChar[] = { 0.1f, 0.1f, 0.35f, -0.85f }; // a,b,x,y
    float villian[] = { 0.2f, 0.2f, -0.5f, -0.45f }; // a,b,x,y
    float frequencies[] = { 10.0f, 1.0f, 4.0f, 1.0f, 1.0f }, intensities[] = { 1.0f, 1.0f, 0.5f, 1.0f, 1.0f }, 
        displacements[] = { 0.0f, 0.0f, 0.5f, 0.0f, 0.0f }, alphaDisplacements[] = { 0.0f, 0.0f, -1/8.0f, 0.0f, 0.0f};
    std::vector<float> miniAlphas(numSegments), cycleAlphas(numSegments);
    glm::vec4 ferrisWheelColor = glm::vec4(244 / 255.0f, 177 / 255.0f, 84 / 255.0f, 1.0f),
        charColor = glm::vec4(244 / 255.0f, 177 / 255.0f, 84 / 255.0f, 1.0f),
        sadColor = glm::vec4(86 / 255.0f, 123 / 255.0f, 137 / 255.0f, 1.0f),
        angryColor = glm::vec4(255 / 255.0f, 0 / 255.0f, 0 / 255.0f, 1.0f),
        topBarColor = glm::vec4(86 / 255.0f, 166 / 255.0f, 84 / 255.0f, 1.0f),
        bottomBarColor = glm::vec4(185 / 255.0f, 57 / 255.0f, 46 / 255.0f, 1.0f),
        whiteColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);


    // Set up the shaders and transforms
    glm::mat4 transform = glm::mat4(1.0f);
    glm::mat4 tempTransform = glm::mat4(1.0f);
    Shader colorTransShader("transform.vs", "colorLinearAnim.fs");
    colorTransShader.use();
    colorTransShader.setMat4("transform", glm::mat4(1.0f));

    // Set up animation time variables
    struct timeb start, end;
    unsigned short elapse = 0, t1, t2;
    ftime(&start);
    t1 = start.millitm;
    alpha = 0.0f;

    // Animate
    while (!animEnded && !glfwWindowShouldClose(window))
    {
        ftime(&end);
        t2 = end.millitm;
        elapse = t2 - t1;
        if (elapse > elapseTreshold) {
            t1 = t2;
            // Set up the segment alphas
            for (int i = 0; i < numSegments; i++) {
                miniAlphas[i] = fmin(1.0f, fmax(0.0f, numSegments * (alpha - ((float) i )/ numSegments)));
                cycleAlphas[i] = intensities[i] * glm::sin(frequencies[i] * ((miniAlphas[i] + alphaDisplacements[i]) * glm::pi<float>())) + displacements[i];
            }

            // Background
            glClearColor(43 / 255.0f, 36 / 255.0f, 80 / 255.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // Draw the wall
            colorTransShader.setVec4("startColor", whiteColor);
            colorTransShader.setVec4("endColor", whiteColor);
            colorTransShader.setMat4("transform", transform);
            drawWall(fmin(1.0f, 2*miniAlphas[3]), transform, colorTransShader); // Closes the door in half of third part

            // Ferris wheel always turning
            colorTransShader.setVec4("startColor", ferrisWheelColor);
            colorTransShader.setVec4("endColor", ferrisWheelColor);
            colorTransShader.setFloat("alpha", alpha);
            tempTransform = transform;
            tempTransform = glm::rotate(tempTransform, numTurns * (1 - alpha) * 2 * glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f));
            drawFerrisWheel(ferris[1], ferris[2], ferris[0], tempTransform, colorTransShader);

            // Characters
            colorTransShader.setVec4("startColor", charColor);
            colorTransShader.setVec4("endColor", charColor);
            colorTransShader.setMat4("transform", transform);
            // Secondary character does not change
            drawFilledEllipse(scndChar[0], scndChar[1], scndChar[2], scndChar[3], 40, transform, colorTransShader);
            // Main char looks sad last 5th of animation
            colorTransShader.setVec4("endColor", sadColor);
            colorTransShader.setFloat("alpha", cycleAlphas[4]);
            tempTransform = transform;
            tempTransform = glm::translate(tempTransform, cycleAlphas[4] * -glm::vec3(0.0f, 0.25 * mainChar[2], 0.0f));
            tempTransform = glm::scale(tempTransform, glm::vec3(1.0f, 1.0f * (1 - cycleAlphas[4]) + cycleAlphas[4] * 0.5f, 1.0f));
            drawFilledEllipse(mainChar[0], mainChar[1], mainChar[2], mainChar[3], 40, tempTransform, colorTransShader);
            // Villain takes a look at the height bar in the 2nd part, denies in 3rd and closes wall in 4th
            colorTransShader.setVec4("endColor", angryColor);
            colorTransShader.setFloat("alpha", cycleAlphas[2]);
            tempTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.3f*cycleAlphas[3], 0.0f, 0.0f));
            tempTransform = glm::translate(tempTransform, glm::vec3(-villian[0], -villian[1], 0.0f)) * shearY(cycleAlphas[1]) * glm::translate(glm::mat4(1.0f), glm::vec3(villian[0], villian[1], 0.0f));
            drawFilledEllipse(villian[0], villian[1], villian[2], villian[3], 40, tempTransform, colorTransShader);

            // Height bar
            colorTransShader.setVec4("startColor", topBarColor);
            colorTransShader.setVec4("endColor", topBarColor);
            colorTransShader.setMat4("transform", transform);
            drawBarTop(transform, colorTransShader);
            // Bar bottom pulsates on 1st part
            colorTransShader.setVec4("startColor", bottomBarColor);
            colorTransShader.setVec4("endColor", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
            colorTransShader.setFloat("alpha", cycleAlphas[0]);
            drawBarBottom(transform, colorTransShader);


            // Prepare for next cycle
            alpha += 0.00075f;
            glfwPollEvents();
            glfwSwapBuffers(window);
        }
        if (alpha >= 1)
            animEnded = true;
    }
}


// This function draws the scene 2, where our two characters work together to defeat the villain
void drawScene2(GLFWwindow* window) {
    int numSegments = 5; // Number of segments the animation is divided into
    float alpha;
    bool animEnded = false;
    float numTurns = 1.25f; // NUmber of turns the ferris wheel will do per scene
    float elapseTreshold = 10.0f, alphaInc = 0.00075f, partTotTime;
    float ferris[] = { 0.5f, -0.45f, 0.45f }; // radius,x,y
    float mainChar[] = { 0.1f, 0.1f, 0.15f, -0.6f }; // a,b,x,y
    float scndChar[] = { 0.1f, 0.1f, 0.35f, -0.85f }; // a,b,x,y
    float villian[] = { 0.2f, 0.2f, -0.5f, -0.45f }; // a,b,x,y
    float frequencies[] = { 20.0f, 1.0f, 4.0f, 1.0f, 7.0f }, intensities[] = { 0.5f, 1.0f, 0.5f, 1.0f, 0.5f }, 
        displacements[] = { 0.5f, 0.0f, 0.5f, 0.0f, 0.5f }, alphaDisplacements[] = { -1.0f / 8.0f, 0.0f, -1 / 8.0f, 0.0f, -1 / 14.0f };
    std::vector<float> miniAlphas(numSegments), cycleAlphas(numSegments), jumpCoords(3);
    glm::vec4 ferrisWheelColor = glm::vec4(244 / 255.0f, 177 / 255.0f, 84 / 255.0f, 1.0f),
        charColor = glm::vec4(244 / 255.0f, 177 / 255.0f, 84 / 255.0f, 1.0f),
        happyColor = glm::vec4(255 / 255.0f, 221 / 255.0f, 0 / 255.0f, 1.0f),
        angryColor = glm::vec4(255 / 255.0f, 0 / 255.0f, 0 / 255.0f, 1.0f),
        topBarColor = glm::vec4(86 / 255.0f, 166 / 255.0f, 84 / 255.0f, 1.0f),
        bottomBarColor = glm::vec4(185 / 255.0f, 57 / 255.0f, 46 / 255.0f, 1.0f),
        whiteColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    // Set up the shaders and transforms
    glm::mat4 transform = glm::mat4(1.0f);
    glm::mat4 tempTransform = glm::mat4(1.0f);
    Shader colorTransShader("transform.vs", "colorLinearAnim.fs");
    colorTransShader.use();
    colorTransShader.setMat4("transform", glm::mat4(1.0f));
    // Set up animation time variables
    struct timeb start, end;
    unsigned short elapse = 0, t1, t2;
    ftime(&start);
    t1 = start.millitm;
    alpha = 0.0f;
    partTotTime = elapseTreshold * ((1.0f / numSegments) / alphaInc);

    // Animate
    while (!animEnded && !glfwWindowShouldClose(window)) {
        ftime(&end);
        t2 = end.millitm;
        elapse = t2 - t1;
        if (elapse > elapseTreshold) {
            t1 = t2;

            // Set up the segment alphas
            for (int i = 0; i < numSegments; i++) {
                miniAlphas[i] = fmin(1.0f, fmax(0.0f, numSegments * (alpha - ((float)i) / numSegments)));
                cycleAlphas[i] = intensities[i] * glm::sin(frequencies[i] * ((miniAlphas[i] + alphaDisplacements[i]) * glm::pi<float>())) + displacements[i];
            }

            // Background
            glClearColor(43 / 255.0f, 36 / 255.0f, 80 / 255.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);


            // Draw the wall
            colorTransShader.setVec4("startColor", whiteColor);
            colorTransShader.setVec4("endColor", whiteColor);
            colorTransShader.setMat4("transform", transform);
            drawWall(1 - fmin(1.0f, 2 * fmax(0.0f,miniAlphas[3]-0.5f)), transform, colorTransShader); // Opens the door in secondf half of third part

            // Ferris wheel always turning
            colorTransShader.setVec4("startColor", ferrisWheelColor);
            colorTransShader.setVec4("endColor", ferrisWheelColor);
            colorTransShader.setFloat("alpha", 0.0f);
            tempTransform = transform;
            tempTransform = glm::rotate(tempTransform, numTurns * (1 - alpha) * 2 * glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f));
            drawFerrisWheel(ferris[1], ferris[2], ferris[0], tempTransform, colorTransShader);

            // Height bar
            // Top bar pulsates on last 4/ths of the 1 part of the animation
            colorTransShader.setVec4("startColor", topBarColor);
            colorTransShader.setVec4("endColor", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
            colorTransShader.setFloat("alpha", intensities[0] * glm::sin(frequencies[0] * (( fmin(1, (4.0f/3.0f)*fmax(0, miniAlphas[0] - 0.25f)) + alphaDisplacements[0]) * glm::pi<float>())) + displacements[0]); // Similar to cycleAlphas[0]
            drawBarTop(transform, colorTransShader);
            // Bar bottom
            colorTransShader.setVec4("startColor", bottomBarColor);
            colorTransShader.setVec4("endColor", bottomBarColor);
            drawBarBottom(transform, colorTransShader);

            // Characters
            colorTransShader.setVec4("startColor", charColor);
            // In the first fifth of the 1st part, the second character gets on top and looks happy in 5th part
            colorTransShader.setVec4("endColor", happyColor);
            colorTransShader.setFloat("alpha", cycleAlphas[4]);
            jumpCoords = jumpTrajectory(scndChar[2], scndChar[3], mainChar[2], mainChar[3] + mainChar[1] + scndChar[1], (partTotTime/5)/1000.0f, fmin(1.0f, 5*miniAlphas[0]));
            tempTransform = transform;
            tempTransform = glm::translate(tempTransform, glm::vec3(jumpCoords[0], jumpCoords[1], jumpCoords[2]));
            colorTransShader.setMat4("transform", tempTransform);
            drawFilledEllipse(scndChar[0], scndChar[1], 0.0f, 0.0f, 40, tempTransform, colorTransShader);
            // Main char looks happy in the last 5th of animation
            colorTransShader.setVec4("endColor", happyColor);
            colorTransShader.setFloat("alpha", cycleAlphas[4]);
            drawFilledEllipse(mainChar[0], mainChar[1], mainChar[2], mainChar[3], 40, transform, colorTransShader);
            // Villain takes a look at the height bar in the 2nd part, accepts in 3rd and opens wall in 4th
            colorTransShader.setVec4("endColor", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
            colorTransShader.setFloat("alpha", cycleAlphas[2]);
            tempTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.3f * cycleAlphas[3], 0.0f, 0.0f));
            tempTransform = glm::translate(tempTransform, glm::vec3(-villian[0], -villian[1], 0.0f)) * shearY(cycleAlphas[1]) * glm::translate(glm::mat4(1.0f), glm::vec3(villian[0], villian[1], 0.0f));
            drawFilledEllipse(villian[0], villian[1], villian[2], villian[3], 40, tempTransform, colorTransShader);







            // Prepare for next cycle
            alpha += alphaInc;
            glfwPollEvents();
            glfwSwapBuffers(window);
        }
        if (alpha >= 1)
            animEnded = true;
    }
}


// This function draws the scene 3, where our two characters ride the ferris wheel
void drawScene3(GLFWwindow* window) {
    int numSegments = 5; // Number of segments the animation is divided into
    float alpha, tempAlpha, tempScale, tempX, tempY;
    bool animEnded = false;
    float numTurns = 1.25f; // Number of turns the ferris wheel will do per scene
    float elapseTreshold = 10.0f, alphaInc = 0.00075f, partTotTime;
    float ferris[] = { 0.5f, -0.45f, 0.45f }; // radius,x,y
    float mainChar[] = { 0.1f, 0.1f, 0.15f, -0.6f }; // a,b,x,y
    float scndChar[] = { 0.1f, 0.1f, 0.15f, -0.4f }; // a,b,x,y
    float villian[] = { 0.2f, 0.2f, -0.5f, -0.45f }; // a,b,x,y
    float frequencies[] = { 20.0f, 1.0f, 4.0f, 1.0f, 7.0f }, intensities[] = { 0.5f, 1.0f, 0.5f, 1.0f, 0.5f },
        displacements[] = { 0.5f, 0.0f, 0.5f, 0.0f, 0.5f }, alphaDisplacements[] = { -1.0f / 8.0f, 0.0f, -1 / 8.0f, 0.0f, -1 / 14.0f };
    std::vector<float> miniAlphas(numSegments), cycleAlphas(numSegments), circTrans(3);
    glm::vec4 ferrisWheelColor = glm::vec4(244 / 255.0f, 177 / 255.0f, 84 / 255.0f, 1.0f),
        charColor = glm::vec4(244 / 255.0f, 177 / 255.0f, 84 / 255.0f, 1.0f),
        happyColor = glm::vec4(255 / 255.0f, 221 / 255.0f, 0 / 255.0f, 1.0f),
        angryColor = glm::vec4(255 / 255.0f, 0 / 255.0f, 0 / 255.0f, 1.0f),
        topBarColor = glm::vec4(86 / 255.0f, 166 / 255.0f, 84 / 255.0f, 1.0f),
        bottomBarColor = glm::vec4(185 / 255.0f, 57 / 255.0f, 46 / 255.0f, 1.0f),
        whiteColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    // Set up the shaders and transforms
    glm::mat4 transform = glm::mat4(1.0f);
    glm::mat4 tempTransform = glm::mat4(1.0f);
    Shader colorTransShader("transform.vs", "colorLinearAnim.fs");
    colorTransShader.use();
    colorTransShader.setMat4("transform", glm::mat4(1.0f));
    // Set up animation time variables
    struct timeb start, end;
    unsigned short elapse = 0, t1, t2;
    ftime(&start);
    t1 = start.millitm;
    alpha = 0.0f;
    partTotTime = elapseTreshold * ((1.0f / numSegments) / alphaInc);

    // Animate
    while (!animEnded && !glfwWindowShouldClose(window)) {
        ftime(&end);
        t2 = end.millitm;
        elapse = t2 - t1;
        if (elapse > elapseTreshold) {
            t1 = t2;

            // Set up the segment alphas
            for (int i = 0; i < numSegments; i++) {
                miniAlphas[i] = fmin(1.0f, fmax(0.0f, numSegments * (alpha - ((float)i) / numSegments)));
                cycleAlphas[i] = intensities[i] * glm::sin(frequencies[i] * ((miniAlphas[i] + alphaDisplacements[i]) * glm::pi<float>())) + displacements[i];
            }

            // Background
            glClearColor(43 / 255.0f, 36 / 255.0f, 80 / 255.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);


            // Draw the wall
            colorTransShader.setVec4("startColor", whiteColor);
            colorTransShader.setVec4("endColor", whiteColor);
            colorTransShader.setMat4("transform", transform);
            drawWall(0.0f, transform, colorTransShader); // Door stays open

            // Ferris wheel always turning
            colorTransShader.setVec4("startColor", ferrisWheelColor);
            colorTransShader.setVec4("endColor", ferrisWheelColor);
            colorTransShader.setFloat("alpha", 0.0f);
            tempTransform = transform;
            tempTransform = glm::rotate(tempTransform, numTurns * (1 - alpha) * 2 * glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f));
            drawFerrisWheel(ferris[1], ferris[2], ferris[0], tempTransform, colorTransShader);

            // Height bar
            colorTransShader.setVec4("startColor", topBarColor);
            colorTransShader.setVec4("endColor", topBarColor);
            drawBarTop(transform, colorTransShader);
            // Bar bottom
            colorTransShader.setVec4("startColor", bottomBarColor);
            colorTransShader.setVec4("endColor", bottomBarColor);
            drawBarBottom(transform, colorTransShader);

            // Characters
            colorTransShader.setVec4("startColor", happyColor);
            colorTransShader.setVec4("endColor", happyColor);
            // In the first half of the 1st part the caracters get on the ferris wheel, they ride and they get off the the last half of the last part
            tempAlpha = fmin(1.0f, 2 * miniAlphas[0]) - fmax(0.0f, fmin(1.0f, 2*miniAlphas[4]-1.0f));
            tempScale = (1-tempAlpha) * mainChar[0] + tempAlpha * ferris[0] * (0.15f / 0.8f) * (0.25f);
            tempX = (1 - tempAlpha) * mainChar[2] + tempAlpha * (ferris[1]);
            tempY = (1 - tempAlpha) * mainChar[3] + tempAlpha * (ferris[2] - ferris[0] + ferris[0] * (0.15f / 0.8f) * (0.25f));
            tempTransform = transform;
            tempTransform = glm::translate(tempTransform, glm::vec3(tempX, tempY, 0.0f));
            // In between those times, the characters should ride the ferris wheel
            tempAlpha = fmin(1.0f, fmax(0.0f, 1.25f * (alpha - 0.1f)));
            circTrans = getTranslateToCirc(tempX, tempY, ferris[1], ferris[2], ferris[0] - ferris[0] * (0.15f / 0.8f) * (0.25f), tempAlpha);
            if (tempAlpha > 0 && tempAlpha < 1) {
                tempX = circTrans[0];
                tempY = circTrans[1];
            }
            else {
                tempX = 0.0f;
                tempY = 0.0f;

            }
            tempTransform = glm::translate(tempTransform, glm::vec3(tempX, tempY, 0.0f));
            tempTransform = glm::scale(tempTransform, glm::vec3(tempScale, tempScale, 1.0f));
            drawFilledEllipse(1.0f, 1.0f, 0.0f, 0.0f, 40, tempTransform, colorTransShader);
            // 2nd char
            tempAlpha = fmin(1.0f, 2 * miniAlphas[0]) - fmax(0.0f, fmin(1.0f, 2 * miniAlphas[4] - 1.0f));
            tempX = (1 - tempAlpha) * scndChar[2] + tempAlpha * (ferris[1]);
            tempY = (1 - tempAlpha) * scndChar[3] + tempAlpha * (ferris[2] - ferris[0] + ferris[0] * (0.15f / 0.8f) * (0.75f));
            tempTransform = transform;
            tempTransform = glm::translate(tempTransform, glm::vec3(tempX, tempY, 0.0f));
            tempAlpha = fmin(1.0f, fmax(0.0f, 1.25f * (alpha - 0.1f)));
            circTrans = getTranslateToCirc(tempX, tempY, ferris[1], ferris[2], ferris[0] - ferris[0] * (0.15f / 0.8f) * (0.75f), tempAlpha);
            if (tempAlpha > 0 && tempAlpha < 1) {
                tempX = circTrans[0];
                tempY = circTrans[1];
            }
            else {
                tempX = 0.0f;
                tempY = 0.0f;

            }
            tempTransform = glm::translate(tempTransform, glm::vec3(tempX, tempY, 0.0f));
            tempTransform = glm::scale(tempTransform, glm::vec3(tempScale, tempScale, 1.0f));
            drawFilledEllipse(1.0f, 1.0f, 0.0f, 0.0f, 40, tempTransform, colorTransShader);
            // Villain 
            colorTransShader.setVec4("startColor", ferrisWheelColor);
            colorTransShader.setVec4("endColor", ferrisWheelColor);
            colorTransShader.setFloat("alpha", 0.0f);
            drawFilledEllipse(villian[0], villian[1], villian[2], villian[3], 40, transform, colorTransShader);


            // Prepare for next cycle
            alpha += alphaInc;
            glfwPollEvents();
            glfwSwapBuffers(window);
        }
        if (alpha >= 1)
            animEnded = true;
    }
}

// This function draws the scene 4, where out second character jumps down and the characters colors go back to the original one 
void drawScene4(GLFWwindow* window) {
    int numSegments = 5; // Number of segments the animation is divided into
    float alpha, tempAlpha = 0.0f;
    bool animEnded = false;
    float numTurns = 1.25f; // NUmber of turns the ferris wheel will do per scene
    float elapseTreshold = 10.0f, alphaInc = 0.00075f, partTotTime;
    float ferris[] = { 0.5f, -0.45f, 0.45f }; // radius,x,y
    float mainChar[] = { 0.1f, 0.1f, 0.15f, -0.6f }; // a,b,x,y
    float scndChar[] = { 0.1f, 0.1f, 0.35f, -0.85f }; // a,b,x,y
    float villian[] = { 0.2f, 0.2f, -0.5f, -0.45f }; // a,b,x,y
    float frequencies[] = { 20.0f, 1.0f, 4.0f, 1.0f, 7.0f }, intensities[] = { 0.5f, 1.0f, 0.5f, 1.0f, 0.5f },
        displacements[] = { 0.5f, 0.0f, 0.5f, 0.0f, 0.5f }, alphaDisplacements[] = { -1.0f / 8.0f, 0.0f, -1 / 8.0f, 0.0f, -1 / 14.0f };
    std::vector<float> miniAlphas(numSegments), cycleAlphas(numSegments), jumpCoords(3);
    glm::vec4 ferrisWheelColor = glm::vec4(244 / 255.0f, 177 / 255.0f, 84 / 255.0f, 1.0f),
        charColor = glm::vec4(244 / 255.0f, 177 / 255.0f, 84 / 255.0f, 1.0f),
        happyColor = glm::vec4(255 / 255.0f, 221 / 255.0f, 0 / 255.0f, 1.0f),
        angryColor = glm::vec4(255 / 255.0f, 0 / 255.0f, 0 / 255.0f, 1.0f),
        topBarColor = glm::vec4(86 / 255.0f, 166 / 255.0f, 84 / 255.0f, 1.0f),
        bottomBarColor = glm::vec4(185 / 255.0f, 57 / 255.0f, 46 / 255.0f, 1.0f),
        whiteColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    // Set up the shaders and transforms
    glm::mat4 transform = glm::mat4(1.0f);
    glm::mat4 tempTransform = glm::mat4(1.0f);
    Shader colorTransShader("transform.vs", "colorLinearAnim.fs");
    colorTransShader.use();
    colorTransShader.setMat4("transform", glm::mat4(1.0f));
    // Set up animation time variables
    struct timeb start, end;
    unsigned short elapse = 0, t1, t2;
    ftime(&start);
    t1 = start.millitm;
    alpha = 0.0f;
    partTotTime = elapseTreshold * ((1.0f / numSegments) / alphaInc);

    // Animate
    while (!animEnded && !glfwWindowShouldClose(window)) {
        ftime(&end);
        t2 = end.millitm;
        elapse = t2 - t1;
        if (elapse > elapseTreshold) {
            t1 = t2;

            // Set up the segment alphas
            for (int i = 0; i < numSegments; i++) {
                miniAlphas[i] = fmin(1.0f, fmax(0.0f, numSegments * (alpha - ((float)i) / numSegments)));
                cycleAlphas[i] = intensities[i] * glm::sin(frequencies[i] * ((miniAlphas[i] + alphaDisplacements[i]) * glm::pi<float>())) + displacements[i];
            }

            // Background
            glClearColor(43 / 255.0f, 36 / 255.0f, 80 / 255.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);


            // Draw the wall
            colorTransShader.setVec4("startColor", whiteColor);
            colorTransShader.setVec4("endColor", whiteColor);
            colorTransShader.setMat4("transform", transform);
            drawWall(0.0f, transform, colorTransShader); // Door stays open

            // Ferris wheel always turning
            colorTransShader.setVec4("startColor", ferrisWheelColor);
            colorTransShader.setVec4("endColor", ferrisWheelColor);
            tempTransform = transform;
            if (tempAlpha < 1)
                tempTransform = glm::rotate(tempTransform, numTurns * (1 - alpha) * 2 * glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f));
            drawFerrisWheel(ferris[1], ferris[2], ferris[0], tempTransform, colorTransShader);

            // Height bar
            colorTransShader.setVec4("startColor", topBarColor);
            colorTransShader.setVec4("endColor", topBarColor);
            drawBarTop(transform, colorTransShader);
            // Bar bottom
            colorTransShader.setVec4("startColor", bottomBarColor);
            colorTransShader.setVec4("endColor", bottomBarColor);
            drawBarBottom(transform, colorTransShader);

            // Characters
            colorTransShader.setVec4("startColor", happyColor);
            // In the first fifth of the 1st part, the second character gets down from the main char and both chars change color in the remaining time of the part
            tempAlpha = fmin(1.0f, fmax(0.0f, 1.25f * (miniAlphas[0] - 0.2f)));
            colorTransShader.setVec4("endColor", ferrisWheelColor);
            colorTransShader.setFloat("alpha", tempAlpha);
            jumpCoords = jumpTrajectory(mainChar[2], mainChar[3] + mainChar[1] + scndChar[1], scndChar[2], scndChar[3], (partTotTime / 5) / 1000.0f, fmin(1.0f, 5 * miniAlphas[0]));
            tempTransform = transform;
            tempTransform = glm::translate(tempTransform, glm::vec3(jumpCoords[0], jumpCoords[1], jumpCoords[2]));
            colorTransShader.setMat4("transform", tempTransform);
            drawFilledEllipse(scndChar[0], scndChar[1], 0.0f, 0.0f, 40, tempTransform, colorTransShader);
            // Main char
            drawFilledEllipse(mainChar[0], mainChar[1], mainChar[2], mainChar[3], 40, transform, colorTransShader);
            // Villain 
            colorTransShader.setVec4("startColor", ferrisWheelColor);
            colorTransShader.setVec4("endColor", ferrisWheelColor);
            drawFilledEllipse(villian[0], villian[1], villian[2], villian[3], 40, transform, colorTransShader);







            // Prepare for next cycle
            alpha += alphaInc;
            glfwPollEvents();
            glfwSwapBuffers(window);
        }
        if (alpha >= 0.25f) // early finish
            animEnded = true;
    }
}