
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


    // =======================================================================
    // RENDER LOOP
    // =======================================================================
    while (!glfwWindowShouldClose(window))
    {
        

        // Swap buffers and poll for events
        //glfwSwapBuffers(window);
            // Background


        glClearColor(43 / 255.0f, 36 / 255.0f, 80 / 255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        drawScene1(window);


       
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

    float ferris[] = {0.5f, -0.45f, 0.45f}; // radius,x,y
    float mainChar[] = { 0.1f, 0.1f, 0.15f, -0.6f }; // a,b,x,y


    // Set up the shaders and transforms
    glm::mat4 transform = glm::mat4(1.0f);

    Shader colorTransShader("transform.vs", "color.fs");
    colorTransShader.use();
    colorTransShader.setMat4("transform", glm::mat4(1.0f));
    colorTransShader.setVec4("color", glm::vec4(244 / 255.0f, 177 / 255.0f, 84 / 255.0f, 1.0f));

    // Draw the ferris wheel
    drawFerrisWheel(ferris[1], ferris[2], ferris[0], transform, colorTransShader);
    drawFilledEllipse(mainChar[0], mainChar[1], mainChar[2], mainChar[3], 40, transform, colorTransShader);

}