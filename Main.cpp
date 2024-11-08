
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

// settings
const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 600;



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
    // SHADERS AND MATRIXES
    // =======================================================================
    Shader colorTransShader("transform.vs", "color.fs");
    glm::mat4 transform = glm::mat4(1.0f);



    // =======================================================================
    // RENDER JUST ONCE 
    // =======================================================================
    // since our scene doesn't change, we can render only when the window pops up or changes in size

    
    // Run the dismembering initial animation (press SPACE to start)
    dismemberTile(window);
    drawScene1(window);

    // =======================================================================
    // RENDER LOOP
    // =======================================================================
    while (!glfwWindowShouldClose(window))
    {
        

        // Swap buffers and poll for events
        //glfwSwapBuffers(window);
            // Background




       
        // input
        // -----
        processInput(window);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

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


void drawScene1(GLFWwindow* window) {

    int numSegments = 5; // Number of segments the animation is divided into
    float alpha, inclination = 0.5f, pulsatingFrec = 10.0f;
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
        // 1st animation
        ftime(&end);
        t2 = end.millitm;
        elapse = t2 - t1;
        if (elapse > 20) {
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
            tempTransform = glm::rotate(tempTransform, 0.5f * (1 - alpha) * 2 * glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f));
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