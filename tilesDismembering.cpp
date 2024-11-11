
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


void dismemberTile(GLFWwindow* window, bool reverse);
std::vector<float> outerTrianglesVertices(int numTriangles, int numVertices);

// Function that animates the decomposition of the tile to build the start of the story 
void dismemberTile(GLFWwindow* window, bool reverse) {
    int numTriangles = 72; // Number of triangles that we will use to build our ellipses
    int numOutTriangles = 24; // Number of triangles in the outer circle
    int totalVertices = numTriangles * 3;
    int numCharacters = 3;
    bool animEnded = false;
    bool waitOver = false;
    float alpha;
    int ratio = totalVertices / (3.0f * numOutTriangles);
    int index;
    float ferrisX = -0.45f, ferrisY = 0.45f, ferrisR = 0.5f;
    std::vector<float> trianglesVertices;
    std::vector<float> wallAnim(48);
    std::vector<float> bottomBar(24);
    std::vector<float> topBar(24);
    std::vector<float> outerTrianglesAnim(totalVertices * 3 * 2);
    std::vector<std::vector<float>> charactersEllipses(numCharacters);
    glm::mat4 transform;

    // Set shaders up
    Shader animShader("posLinearAnim.vs", "colorLinearAnim.fs");
    Shader transformShader("transform.vs", "color.fs");
    transformShader.use();
    transformShader.setVec4("color", 244 / 255.0f, 177 / 255.0f, 84 / 255.0f, 1.0f);
    transformShader.setMat4("transform", glm::mat4(1.0f));
    animShader.use();
    animShader.setVec4("startColor", 244 / 255.0f, 177 / 255.0f, 84 / 255.0f, 1.0f);
    animShader.setVec4("endColor", 244 / 255.0f, 177 / 255.0f, 84 / 255.0f, 1.0f);

    // Define the points of outer circle triangles and the corresponding animation target points (the ellipses)
    trianglesVertices = outerTrianglesVertices(numOutTriangles, totalVertices); // Get triangles vertices 
    charactersEllipses[0] = getEllipsesVertices(0.1f, 0.1f, 0.15f, -0.6f, numTriangles / numCharacters); // Get main character vertices
    charactersEllipses[1] = getEllipsesVertices(0.1f, 0.1f, 0.35f, -0.85f, numTriangles / numCharacters);
    charactersEllipses[2] = getEllipsesVertices(0.2f, 0.2f, -0.5f, -0.45f, numTriangles / numCharacters);
    for (int character = 0; character < numCharacters; character++) {
        for (int i = character * numOutTriangles / numCharacters; i < (character + 1) * numOutTriangles / numCharacters; i++) { // Iterate over each outer triangle
            index = i * 3 * 3 * ratio;
            for (int j = 0; j < ratio; j++) { // Repeat that triangle as many times as needed
                // First point of the triangle
                outerTrianglesAnim[index * 2 + 3 * 3 * 2 * j + 0] = trianglesVertices[index + 3 * 3 * j + 0];
                outerTrianglesAnim[index * 2 + 3 * 3 * 2 * j + 1] = trianglesVertices[index + 3 * 3 * j + 1];
                outerTrianglesAnim[index * 2 + 3 * 3 * 2 * j + 2] = trianglesVertices[index + 3 * 3 * j + 2];
                outerTrianglesAnim[index * 2 + 3 * 3 * 2 * j + 3] = charactersEllipses[character][3 * ratio * (i - character * numOutTriangles / numCharacters) + 3 * j + 3];
                outerTrianglesAnim[index * 2 + 3 * 3 * 2 * j + 4] = charactersEllipses[character][3 * ratio * (i - character * numOutTriangles / numCharacters) + 3 * j + 4];
                outerTrianglesAnim[index * 2 + 3 * 3 * 2 * j + 5] = charactersEllipses[character][3 * ratio * (i - character * numOutTriangles / numCharacters) + 3 * j + 5];
                // 2nd
                outerTrianglesAnim[index * 2 + 3 * 3 * 2 * j + 6] = trianglesVertices[index + 3 * 3 * j + 3];
                outerTrianglesAnim[index * 2 + 3 * 3 * 2 * j + 7] = trianglesVertices[index + 3 * 3 * j + 4];
                outerTrianglesAnim[index * 2 + 3 * 3 * 2 * j + 8] = trianglesVertices[index + 3 * 3 * j + 5];
                outerTrianglesAnim[index * 2 + 3 * 3 * 2 * j + 9] = charactersEllipses[character][0];
                outerTrianglesAnim[index * 2 + 3 * 3 * 2 * j + 10] = charactersEllipses[character][1];
                outerTrianglesAnim[index * 2 + 3 * 3 * 2 * j + 11] = charactersEllipses[character][2];
                // 3rd
                outerTrianglesAnim[index * 2 + 3 * 3 * 2 * j + 12] = trianglesVertices[index + 3 * 3 * j + 6];
                outerTrianglesAnim[index * 2 + 3 * 3 * 2 * j + 13] = trianglesVertices[index + 3 * 3 * j + 7];
                outerTrianglesAnim[index * 2 + 3 * 3 * 2 * j + 14] = trianglesVertices[index + 3 * 3 * j + 8];
                outerTrianglesAnim[index * 2 + 3 * 3 * 2 * j + 15] = charactersEllipses[character][3 * ratio * (i - character * numOutTriangles / numCharacters) + 3 * j + 6];
                outerTrianglesAnim[index * 2 + 3 * 3 * 2 * j + 16] = charactersEllipses[character][3 * ratio * (i - character * numOutTriangles / numCharacters) + 3 * j + 7];
                outerTrianglesAnim[index * 2 + 3 * 3 * 2 * j + 17] = charactersEllipses[character][3 * ratio * (i - character * numOutTriangles / numCharacters) + 3 * j + 8];
            }
        }
    }
    // Define the points of corner triangles and their correspondig animation target points
    wallAnim = {
        // from         // to
        // 1st quadrant triangles
        0.8f, 0.6f, 0.0f,     1.0f, -0.6f, 0.0f,
        1.0f, 0.8f, 0.0f,     1.0f, -0.35f, 0.0f,
        0.6f, 1.0f, 0.0f,     0.15f, -0.6f, 0.0f,
        0.6f, 0.8f, 0.0f,     0.15f, -0.6f, 0.0f,
        0.8f, 1.0f, 0.0f,     0.15f, -0.35f, 0.0f,
        1.0f, 0.6f, 0.0f,     1.0f, -0.35f, 0.0f,
        // 2nd
        -0.8f, 0.6f, 0.0f,    -1.0f, -0.6f, 0.0f,
        -1.0f, 0.8f, 0.0f,    -1.0f, -0.35f, 0.0f,
        -0.6f, 1.0f, 0.0f,    -0.25f, -0.6f, 0.0f,
        -0.6f, 0.8f, 0.0f,    -0.25f, -0.6f, 0.0f,
        -0.8f, 1.0f, 0.0f,    -0.25f, -0.35f, 0.0f,
        -1.0f, 0.6f, 0.0f,    -1.0f, -0.35f, 0.0f
    };
    topBar = {
        // 3rd
        -0.8f, -0.6f, 0.0f,   0.45f, -0.25f, 0.0f,
        -1.0f, -0.8f, 0.0f,   0.35f, -0.25f, 0.0f,
        -0.6f, -1.0f, 0.0f,   0.35f, -0.4f, 0.0f,
        -0.6f, -0.8f, 0.0f,   0.45f, -0.4f, 0.0f,
        -0.8f, -1.0f, 0.0f,   0.35f, -0.4f, 0.0f,
        -1.0f, -0.6f, 0.0f,   0.45f, -0.25f, 0.0f
    };
    bottomBar = {
        // 4th
        0.8f, -0.6f, 0.0f,    0.35f, -0.4f, 0.0f,
        1.0f, -0.8f, 0.0f,    0.45f, -0.4f, 0.0f,
        0.6f, -1.0f, 0.0f,    0.35f, -0.7f, 0.0f,
        0.6f, -0.8f, 0.0f,    0.35f, -0.7f, 0.0f,
        0.8f, -1.0f, 0.0f,    0.45f, -0.7f, 0.0f,
        1.0f, -0.6f, 0.0f,    0.45f, -0.4f, 0.0f
    };


    // Create and bind VAOs and VBOs
    unsigned int VAOs[4], VBOs[4];
    glGenVertexArrays(4, VAOs);
    glGenBuffers(4, VBOs);
    bindAnimVector(VAOs[0], VBOs[0], outerTrianglesAnim);
    bindAnimVector(VAOs[1], VBOs[1], wallAnim);
    bindAnimVector(VAOs[2], VBOs[2], topBar);
    bindAnimVector(VAOs[3], VBOs[3], bottomBar);

    // Set up animation time variables
    struct timeb start, end;
    unsigned short elapse = 0, t1, t2;
    ftime(&start);
    t1 = start.millitm;
    if (!reverse) {
        alpha = 0.0f;
    }
    else {
        alpha = 1.0f;
    }

    if (!reverse) {
        // Show the start for a couple of miliseconds
        glClearColor(43 / 255.0f, 36 / 255.0f, 80 / 255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        transformShader.use();
        drawFerrisWheel(0, 0, 0.8, glm::mat4(1.0f), transformShader);
        animShader.use();
        animShader.setFloat("alpha", alpha);
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, totalVertices * 3);
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 72);
        glBindVertexArray(VAOs[2]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(VAOs[3]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glfwSwapBuffers(window);
        while (!waitOver && !glfwWindowShouldClose(window)) {
            // Set up animation
            ftime(&end);
            t2 = end.millitm;
            elapse = t2 - t1;
            if (elapse > 20) {
                t1 = t2;
                alpha += 0.01;
            }
            if (alpha > 0.5f)
                waitOver = true;
            glfwPollEvents();
        }
        alpha = 0.0f;
    }

    // Animate
    while (!animEnded && !glfwWindowShouldClose(window)) {
        // Set up animation
        ftime(&end);
        t2 = end.millitm;
        elapse = t2 - t1;
        if (elapse > 20) {
            t1 = t2;
            // Background
            glClearColor(43 / 255.0f, 36 / 255.0f, 80 / 255.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // The linear increment used to animate
            if (!reverse) {
                alpha += 0.01;
                alpha = fmin(1, alpha);
            }
            else {
                alpha -= 0.01;
                alpha = fmax(0.0f, alpha);
            }

            // Draw the ferris wheel
            transformShader.use();
            transform = glm::mat4(1.0f);
            transform = glm::translate(transform, glm::vec3(alpha * ferrisX, alpha * ferrisY, 0.0f));
            transform = glm::rotate(transform, 0.25f * (1-alpha) * 2 * glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f));
            drawFerrisWheel(0.0f, 0.0f, (1-alpha) * 0.8f + alpha * ferrisR, transform, transformShader);
            // Draw the corner triangles
            animShader.use();
            animShader.setVec4("startColor", 244 / 255.0f, 177 / 255.0f, 84 / 255.0f, 1.0f);
            animShader.setFloat("alpha", alpha);
            animShader.setVec4("endColor", 1.0f, 1.0f, 1.0f, 1.0f);
            glBindVertexArray(VAOs[1]);
            glDrawArrays(GL_TRIANGLES, 0, 72);
            animShader.setVec4("endColor", 86 / 255.0f, 166 / 255.0f, 84 / 255.0f, 1.0f);
            glBindVertexArray(VAOs[2]);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            animShader.setVec4("endColor", 185 / 255.0f, 57 / 255.0f, 46 / 255.0f, 1.0f);
            glBindVertexArray(VAOs[3]);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // Draw the circle triangles
            animShader.use();
            animShader.setFloat("alpha", alpha);
            animShader.setVec4("endColor", 244 / 255.0f, 177 / 255.0f, 84 / 255.0f, 1.0f);
            glBindVertexArray(VAOs[0]);
            glDrawArrays(GL_TRIANGLES, 0, totalVertices * 3);


            glfwSwapBuffers(window);
        }
        if ((alpha >= 1 && !reverse) || (alpha <= 0 && reverse)) {
            animEnded = true;
        }

    }
}


// Create vertices for the original tile outer circle triangles
std::vector<float> outerTrianglesVertices(int numTriangles, int numVertices) {
    // Make sure the triangles are buildable
    if (numVertices < (3 * numTriangles) || numVertices % (3 * numTriangles) != 0)
        throw std::invalid_argument("The number of vertices isn't divisible by the number of triangles");
    std::vector<float> vertices(3 * numVertices); // Each vertex has 3 components (x, y, z)

    int ratio = numVertices / (3 * numTriangles); // We are allowing for triangles to overlap
    float index = 0;
    float angleStep = 2.0f * glm::pi<float>() / numTriangles;
    float angle;

    for (int i = 0; i < numTriangles; i++) { // Build each triangle
        index = i * 3 * 3 * ratio;
        angle = i * angleStep;
        for (int j = 0; j < ratio; j++) { // Repeat that triangle as many times as needed
            // First point of the triangle
            vertices[index + 3 * 3 * j + 0] = 1.0f * glm::cos(angle);
            vertices[index + 3 * 3 * j + 1] = 1.0f * glm::sin(angle);
            vertices[index + 3 * 3 * j + 2] = 0.0f;
            // 2nd
            vertices[index + 3 * 3 * j + 3] = 0.8f * glm::cos(angle + angleStep / 2.0f);
            vertices[index + 3 * 3 * j + 4] = 0.8f * glm::sin(angle + angleStep / 2.0f);
            vertices[index + 3 * 3 * j + 5] = 0.0f;
            // 3rd
            vertices[index + 3 * 3 * j + 6] = 1.0f * glm::cos(angle + angleStep);
            vertices[index + 3 * 3 * j + 7] = 1.0f * glm::sin(angle + angleStep);
            vertices[index + 3 * 3 * j + 8] = 0.0f;
        }
    }
    return vertices;
}

