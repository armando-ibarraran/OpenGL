#define STB_IMAGE_IMPLEMENTATION
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

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

void drawFilledEllipse(float a, float b, float x, float y, int numSegments, glm::mat4 transform, Shader shader);
void drawStar(float innerRadius, float outerRadius, float x, float y, int numPoints, glm::mat4 transform, Shader shader);
void drawFerrisWheel(float x, float y, float r, glm::mat4 transform, Shader shader);
void animateSceneStart(Shader shader, GLFWwindow* window);
std::vector<float> outerTrianglesVertices(int numTriangles, int numVertices);
std::vector<float> getEllipsesVertices(float a, float b, float x, float y, int numSegments);
void bindAnimVector(unsigned int VAO, unsigned int VBO, std::vector<float> vector);

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

    colorTransShader.use();
    colorTransShader.setVec4("color", 244 / 255.0f, 177 / 255.0f, 84 / 255.0f, 1.0f);

    colorTransShader.setMat4("transform", glm::mat4(1.0f));
    // Clear the screen
    glClearColor(43 / 255.0f, 36 / 255.0f, 80 / 255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    animateSceneStart(colorTransShader, window);



    // =======================================================================
    // RENDER LOOP
    // =======================================================================
    while (!glfwWindowShouldClose(window))
    {
        /*
        // Clear the screen
        glClearColor(43 / 255.0f, 36 / 255.0f, 80 / 255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        transform = glm::mat4(1.0f);
        //transform = glm::translate(transform, glm::vec3(0.5f, 0.5f, 0.0f));
        //transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        transform = glm::scale(transform, glm::vec3(0.8f, 0.8f, 0.0f));
        colorTransShader.setMat4("transform", transform);
        animateSceneStart(colorTransShader);
        drawFerrisWheel(0.0f, 0.0f, 1.0f, transform, colorTransShader);
        */

        // Swap buffers and poll for events
        //glfwSwapBuffers(window);


       
        // input
        // -----
        processInput(window);

        glfwPollEvents();
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


// Function to create and draw a filled ellipse (of the form (x/a)^2 + (y/b)^2 = 1) centered at (x,y)
void drawFilledEllipse(float a, float b, float x, float y, int numSegments, glm::mat4 transform, Shader shader) {
    
    std::vector<float> vertices;

    // Get the vertices to draw
    vertices = getEllipsesVertices(a, b, x, y, numSegments);


    // Create and bind VAO and VBO
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Translate the ellipse to the desired coordinates without affecting other previously defined transformations
    shader.setMat4("transform", glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f))*transform);

    // Draw the ellipse as a triangle fan
    glDrawArrays(GL_TRIANGLE_FAN, 0, numSegments + 2);

    // Clean up and unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Delete the VAO and VBO after drawing since they are no longer needed
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}



// Function to create and draw a star with defined inner, and outer radius and numPoints spikes centered in (x, y)
void drawStar(float innerRadius, float outerRadius, float x, float y, int numPoints, glm::mat4 transform, Shader shader) {
    // Ensure that numPoints is at least 5 for a star shape
    if (numPoints < 5) numPoints = 5;

    // Number of vertices needed (each point of the star has an inner and an outer vertex)
    int totalVertices = numPoints * 2 + 2;  // +1 to close the loop

    // Allocate vertex data dynamically
    std::vector<float> vertices(totalVertices * 2);  // Each vertex has 2 components (x, y)

    // Center point of the star
    vertices[0] = 0.0f;  // x-coordinate of the center
    vertices[1] = 0.0f;  // y-coordinate of the center

    float angleStep = glm::pi<float>() / numPoints;  // Angle between vertices

    int index = 2;  // Start after the center vertex
    for (int i = 0; i < numPoints * 2; ++i) {
        float angle = i * angleStep;

        // Alternate between the inner and outer radius
        float radius = (i % 2 == 0) ? outerRadius : innerRadius;

        // Calculate the x and y coordinates
        float x = radius * cos(angle);
        float y = radius * sin(angle);

        // Add the vertex to the array
        vertices[index++] = x;
        vertices[index++] = y;
    }

    // Close the loop by connecting the last point back to the first outer point
    vertices[index++] = vertices[2];
    vertices[index++] = vertices[3];

    // Create and bind VAO and VBO
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Translate the star to the desired coordinates without affecting other previously defined transformations
    shader.setMat4("transform", glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f)) * transform);

    // Draw the star as a triangle fan
    glDrawArrays(GL_TRIANGLE_FAN, 0, totalVertices);

    // Clean up and unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Delete the VAO and VBO after drawing since they are no longer needed
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}



// Function to create and draw a polygon with numSides sides, external radius 'radius' centered in (x, y)
void drawPolygon(float radius, float x, float y, int numSides, glm::mat4 transform, Shader shader) {
    // Ensure that numSides is at least 3 for a valid polygon
    if (numSides < 3) numSides = 3;

    // Total vertices needed: center point + one vertex for each side + 1 to close the loop
    int totalVertices = numSides + 2;

    // Use std::vector to create the vertices array dynamically
    std::vector<float> vertices(totalVertices * 2);  // Each vertex has 2 components (x, y)

    // Center point of the polygon
    vertices[0] = 0.0f;  // x-coordinate of the center
    vertices[1] = 0.0f;  // y-coordinate of the center

    // Angle step for each side
    float angleStep = 2.0f * glm::pi<float>() / numSides;

    int index = 2;
    for (int i = 0; i <= numSides; ++i) {
        float angle = i * angleStep;

        // Calculate the x and y coordinates for each vertex
        float x = radius * cos(angle);
        float y = radius * sin(angle);

        // Add the vertex to the array
        vertices[index++] = x;
        vertices[index++] = y;
    }

    // Create and bind VAO and VBO
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    // Translate the polygon to the desired coordinates without affecting other previously defined transformations
    shader.setMat4("transform", glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f)) * transform);

    // Draw the polygon as a triangle fan
    glDrawArrays(GL_TRIANGLE_FAN, 0, totalVertices);

    // Clean up and unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Delete the VAO and VBO after drawing since they are no longer needed
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}



// Draws the ferris wheel pod with width w, height h and centered in (x,y)
void drawFerrisWheelPod(float w, float h, float x, float y, glm::mat4 transform, Shader shader) {

    float halfWidth = w / 2.0f;
    float halfHeight = h / 2.0f;
    float quarterHeight = h / 4.0f;

    float vertices[] = {
        0.0f,       0.0f,
        -halfWidth, quarterHeight,
        0.0f,       halfHeight,
        halfWidth,  quarterHeight,
        halfWidth,  -quarterHeight,
        0.0f,       -halfHeight,
        -halfWidth, -quarterHeight,
        -halfWidth, quarterHeight
    };

    // Create and bind VAO and VBO
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    // Translate the polygon to the desired coordinates without affecting other previously defined transformations
    shader.setMat4("transform", glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f)) * transform);

    // Draw the polygon as a triangle fan
    glDrawArrays(GL_TRIANGLE_FAN, 0, 8);

    // Clean up and unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Delete the VAO and VBO after drawing since they are no longer needed
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}



// Function to create and draw the ferris wheel, centered at (x,y) and with outer radius r
void drawFerrisWheel(float x, float y, float r, glm::mat4 transform, Shader shader) {
    // Draw the ferris wheel arms and center
    drawStar(r / 8.0f, r * (0.5f / 0.8f), x, y, 8, transform, shader);
    // Draw the first part of the cars
    float angleStep = 2.0f * glm::pi<float>() / 8.0f;
    float carRadius = r * 0.075f / 0.8f;
    float carPositionsRadius = r * 0.575f / 0.8f;
    glm::mat4 tempTransform = glm::mat4(1.0f);
    for (int i = 0; i < 8; ++i) {
        float angle = i * angleStep;
        // Calculate the x and y coordinates for each vertex
        float tempX = carPositionsRadius * cos(angle);
        float tempY = carPositionsRadius * sin(angle);
        // We move and rotate them arround the ferris wheel
        tempTransform = glm::translate(glm::mat4(1.0f), glm::vec3(tempX, tempY, 0.0f)); // 2nd we position them arround the ferris wheel
        tempTransform = glm::rotate(tempTransform, i*glm::pi<float>()/4.0f, glm::vec3(0.0f, 0.0f, 1.0f)); // 1st give the carts the correct orientation
        tempTransform = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f)) * transform * tempTransform; // 3rd we apply the transforms applied to the ferris wheel and 4th we move them to where the ferris wheel is centered
        drawPolygon(carRadius, 0.0f, 0.0f, 4, tempTransform, shader);
    }
    // Draw the 2nd part of the cars
    float podWidth = r * (0.15f / 0.8f);
    float podHeight = r * (0.3f / 0.8f);
    float podPositionRadius = r * (0.725f / 0.8f);
    for (int i = 0; i < 8; ++i) {
        float angle = i * angleStep;
        // Calculate the x and y coordinates for each vertex
        float tempX = podPositionRadius * cos(angle);
        float tempY = podPositionRadius * sin(angle);
        // We move and rotate them arround the ferris wheel
        tempTransform = glm::translate(glm::mat4(1.0f), glm::vec3(tempX, tempY, 0.0f)); // 2nd we position them arround the ferris wheel
        tempTransform = glm::rotate(tempTransform, i * glm::pi<float>() / 4.0f, glm::vec3(0.0f, 0.0f, 1.0f)); // 1st give the carts the correct orientation
        tempTransform = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f)) * transform * tempTransform; // 3rd we apply the transforms applied to the ferris wheel and 4th we move them to where the ferris wheel is centered
        drawFerrisWheelPod(podWidth, podHeight, 0.0f, 0.0f, tempTransform, shader);
    }
}



// Function that animates the decomposition of the tile to build the start of the story 
void animateSceneStart(Shader shader, GLFWwindow* window) {
    int numTriangles = 72; // Number of triangles that we will use to build our ellipses
    int numOutTriangles = 24; // Number of triangles in the outer circle
    int totalVertices = numTriangles * 3;
    int numCharacters = 3;
    bool animEnded = false;
    bool waitOver;
    float alpha = 1.0f;
    int ratio = totalVertices / (3.0f * numOutTriangles);
    int index;
    float ferrisX = -0.45f, ferrisY = 0.45f, ferrisR = 0.5f;
    std::vector<float> trianglesVertices;
    std::vector<float> wallAnim(48);
    std::vector<float> bottomBar(24);
    std::vector<float> topBar(24);
    std::vector<float> outerTrianglesAnim(totalVertices*2*2);
    std::vector<std::vector<float>> charactersEllipses(numCharacters);
    glm::mat4 transform;

    // Set shader up
    Shader animShader("posLinearAnim.vs", "colorLinearAnim.fs");
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
            index = i * 3 * 2 * ratio;
            for (int j = 0; j < ratio; j++) { // Repeat that triangle as many times as needed
                // First point of the triangle
                outerTrianglesAnim[index * 2 + 3 * 2 * 2 * j + 0] = trianglesVertices[index + 3 * 2 * j + 0];
                outerTrianglesAnim[index * 2 + 3 * 2 * 2 * j + 1] = trianglesVertices[index + 3 * 2 * j + 1];
                outerTrianglesAnim[index * 2 + 3 * 2 * 2 * j + 2] = charactersEllipses[character][2 * ratio * (i - character * numOutTriangles / 3) + 2 * j + 2];
                outerTrianglesAnim[index * 2 + 3 * 2 * 2 * j + 3] = charactersEllipses[character][2 * ratio * (i - character * numOutTriangles / 3) + 2 * j + 3];
                // 2nd
                outerTrianglesAnim[index * 2 + 3 * 2 * 2 * j + 4] = trianglesVertices[index + 3 * 2 * j + 2];
                outerTrianglesAnim[index * 2 + 3 * 2 * 2 * j + 5] = trianglesVertices[index + 3 * 2 * j + 3];
                outerTrianglesAnim[index * 2 + 3 * 2 * 2 * j + 6] = charactersEllipses[character][0];
                outerTrianglesAnim[index * 2 + 3 * 2 * 2 * j + 7] = charactersEllipses[character][1];
                // 3rd
                outerTrianglesAnim[index * 2 + 3 * 2 * 2 * j + 8] = trianglesVertices[index + 3 * 2 * j + 4];
                outerTrianglesAnim[index * 2 + 3 * 2 * 2 * j + 9] = trianglesVertices[index + 3 * 2 * j + 5];
                outerTrianglesAnim[index * 2 + 3 * 2 * 2 * j + 10] = charactersEllipses[character][2 * ratio * (i - character * numOutTriangles / 3) + 2 * j + 4];
                outerTrianglesAnim[index * 2 + 3 * 2 * 2 * j + 11] = charactersEllipses[character][2 * ratio * (i - character * numOutTriangles / 3) + 2 * j + 5];
            }
        }
    }
    // Define the points of corner triangles and their correspondig animation target points
    wallAnim = {
        // from         // to
        // 1st quadrant triangles
        0.8f, 0.6f,     1.0f, -0.6f,
        1.0f, 0.8f,     1.0f, -0.35f,
        0.6f, 1.0f,     0.15f, -0.6f,
        0.6f, 0.8f,     0.15f, -0.6f,
        0.8f, 1.0f,     0.15f, -0.35f,
        1.0f, 0.6f,     1.0f, -0.35f,
        // 2nd
        -0.8f, 0.6f,    -1.0f, -0.6f,
        -1.0f, 0.8f,    -1.0f, -0.35f,
        -0.6f, 1.0f,    -0.15f, -0.6f,
        -0.6f, 0.8f,    -0.15f, -0.6f,
        -0.8f, 1.0f,    -0.15f, -0.35f,
        -1.0f, 0.6f,    -1.0f, -0.35f
    };
    topBar = {
        // 3rd
        -0.8f, -0.6f,   0.45f, -0.25f,
        -1.0f, -0.8f,   0.35f, -0.25f,
        -0.6f, -1.0f,   0.35f, -0.4f,
        -0.6f, -0.8f,   0.45f, -0.4f,
        -0.8f, -1.0f,   0.35f, -0.4f,
        -1.0f, -0.6f,   0.45f, -0.25f
    };
    bottomBar = {
        // 4th
        0.8f, -0.6f,    0.35f, -0.4f,
        1.0f, -0.8f,    0.45f, -0.4f,
        0.6f, -1.0f,    0.35f, -0.7f,
        0.6f, -0.8f,    0.35f, -0.7f,
        0.8f, -1.0f,    0.45f, -0.7f,
        1.0f, -0.6f,    0.45f, -0.4f
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

    // Show the start for a couple of miliseconds
    glClearColor(43 / 255.0f, 36 / 255.0f, 80 / 255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    animShader.use();
    animShader.setFloat("alpha", 1.0f);
    glBindVertexArray(VAOs[0]);
    glDrawArrays(GL_TRIANGLES, 0, totalVertices * 2);
    glBindVertexArray(VAOs[1]);
    glDrawArrays(GL_TRIANGLES, 0, 48);
    glBindVertexArray(VAOs[2]);
    glDrawArrays(GL_TRIANGLES, 0, 24);
    glBindVertexArray(VAOs[3]);
    glDrawArrays(GL_TRIANGLES, 0, 24);
    shader.use();
    drawFerrisWheel(0, 0, 0.8, glm::mat4(1.0f), shader);
    glfwSwapBuffers(window);
    waitOver = false;
    while (!waitOver && !glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            waitOver = true;
        }
        glfwPollEvents();
    }

    // Animate
    while (!animEnded && !glfwWindowShouldClose(window)) {
        processInput(window);
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
            alpha -= 0.01;
            alpha = fmax(0, alpha);

            // Draw the ferris wheel
            shader.use();
            transform = glm::mat4(1.0f);
            transform = glm::translate(transform, glm::vec3((1 - alpha) * ferrisX, (1 - alpha) * ferrisY, 0.0f));
            transform = glm::rotate(transform, 0.25f * alpha * 2 * glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f));
            drawFerrisWheel(0.0f, 0.0f, alpha * 0.8f + (1 - alpha) * ferrisR, transform, shader);
            // Draw the corner triangles
            animShader.use();
            animShader.setFloat("alpha", alpha);
            animShader.setVec4("endColor", 1.0f, 1.0f, 1.0f, 1.0f);
            glBindVertexArray(VAOs[1]);
            glDrawArrays(GL_TRIANGLES, 0, 48);
            animShader.setVec4("endColor", 86 / 255.0f, 166 / 255.0f, 84 / 255.0f, 1.0f);
            glBindVertexArray(VAOs[2]);
            glDrawArrays(GL_TRIANGLES, 0, 24);
            animShader.setVec4("endColor", 185 / 255.0f, 57 / 255.0f, 46 / 255.0f, 1.0f);
            glBindVertexArray(VAOs[3]);
            glDrawArrays(GL_TRIANGLES, 0, 24);

            // Draw the circle triangles
            animShader.use();
            animShader.setFloat("alpha", alpha);
            animShader.setVec4("endColor", 244 / 255.0f, 177 / 255.0f, 84 / 255.0f, 1.0f);
            glBindVertexArray(VAOs[0]);
            glDrawArrays(GL_TRIANGLES, 0, totalVertices * 2);


            glfwSwapBuffers(window);
        }
        if (alpha<=0) {
            animEnded = true;
        }

    }
}


// Create vertices for the original tile outer circle triangles
std::vector<float> outerTrianglesVertices(int numTriangles, int numVertices) {
    // Make sure the triangles are buildable
    if (numVertices<(3*numTriangles) || numVertices % (3*numTriangles) != 0)
        throw std::invalid_argument("The number of vertices isn't divisible by the number of triangles");
    std::vector<float> vertices(2*numVertices); // Each vertex has 2 components (x, y)

    int ratio = numVertices / (3*numTriangles); // We are allowing for triangles to overlap
    float index = 0;
    float angleStep = 2.0f * glm::pi<float>() / numTriangles;
    float angle;

    for (int i = 0; i < numTriangles; i++) { // Build each triangle
        index = i * 3 * 2 * ratio;
        angle = i * angleStep; 
        for (int j = 0; j < ratio; j++) { // Repeat that triangle as many times as needed
            // First point of the triangle
            vertices[index + 3 * 2 * j + 0] = 1.0f * glm::cos(angle);
            vertices[index + 3 * 2 * j + 1] = 1.0f * glm::sin(angle);
            // 2nd
            vertices[index + 3 * 2 * j + 2] = 0.8f * glm::cos(angle + angleStep / 2.0f);
            vertices[index + 3 * 2 * j + 3] = 0.8f * glm::sin(angle + angleStep / 2.0f);
            // 3rd
            vertices[index + 3 * 2 * j + 4] = 1.0f * glm::cos(angle + angleStep);
            vertices[index + 3 * 2 * j + 5] = 1.0f * glm::sin(angle + angleStep);
        }
    }
    return vertices;
}


// Create the vertices needed to draw an ellipse
std::vector<float> getEllipsesVertices(float a, float b, float x, float y, int numSegments) {
    // Use std::vector to create the vertices array dynamically
    std::vector<float> vertices((numSegments + 2) * 2);  // Each vertex has 2 components (x, y)

    // Center point for the filled ellipse
    vertices[0] = x;  // x-coordinate of the center
    vertices[1] = y;  // y-coordinate of the center

    int index = 2;
    for (int i = 0; i <= numSegments; ++i) {
        float angle = (2.0f * glm::pi<float>() * i) / numSegments;  // Uniform partition of the circumference

        // Calculate the x and y coordinates for the ellipse
        float tempX = x + a * cos(angle);  // Semi-major axis * cos(angle)
        float tempY = y + b * sin(angle);  // Semi-minor axis * sin(angle)

        // Add the vertex to the array
        vertices[index++] = tempX;
        vertices[index++] = tempY;
    }
    return vertices;
}

// Bind Vector to a VBO and VAO
void bindAnimVector(unsigned int VAO, unsigned int VBO, std::vector<float> vector) {
    glBindVertexArray(VAO); // For the corner triangles
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vector.size() * sizeof(float), vector.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}