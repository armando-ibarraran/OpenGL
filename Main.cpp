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


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
int loadTexture(const char* filepath);
void drawWhiteCircle(float x, float y, float r, glm::mat4 transform);
void paintCirclesInLine(float startX, float endX, float startY, float endY, int n, float r, glm::mat4 transform);
void drawWhiteCurve(glm::mat4 transform);
void draw32th(unsigned int VAOs[], glm::mat4 transform);
void draw16th(unsigned int VAOs[], glm::mat4 transform);
void draw4th(unsigned int VAOs[], glm::mat4 transform);
void displayMainPattern(unsigned int VAOs[]);
void draw8thFlower(unsigned int VAOs[], glm::mat4 transform);
void draw4thFlower(unsigned int VAOs[], glm::mat4 transform);
void drawFlower(unsigned int VAOs[]);
void dibujaBaldosa( GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 600;

char golden1Path[] = "texturas/gold.jpg";
char white1Path[] = "texturas/whiteMetal.jpg";
char blue1Path[] = "texturas/blueStone.jpg";
char white2Path[] = "texturas/whiteStone.jpg";
char blue2Path[] = "texturas/blueStone.jpg";
char grey1Path[] = "texturas/grey.jpg";



// =======================================================================
// Set up vertices
// =======================================================================

float goldenTriangles[] = {
    // positions        //textures

    // golden "fork"
    -0.6f, -1.0f, 0.0f,      0.00f, 0.00f,
    -0.4f, -0.94f, 0.0f,     0.40f, 0.12f,
    -0.4f, -1.0f, 0.0f,      0.40f, 0.00f,
    -0.1f, -0.94f, 0.0f,     1.00f, 0.12f,
    -0.1f, -1.0f, 0.0f,      1.00f, 0.00f,
    -0.38f, -0.84f, 0.0f,    0.44f, 0.32f,
    -0.28f, -0.88f, 0.0f,    0.64f, 0.24f,
    -0.31f, -0.80f, 0.0f,    0.58f, 0.40f,

    // diamond
    0.812f, -1.0f, 0.0f,     0.2419f, 0.0f,
    1.0f, -1.0f, 0.0f,      1.0f, 0.0f,
    0.867f, -0.867f, 0.0f,  0.4637f, 0.5363f,
    0.752f, -0.896f, 0.0f,  0.0f, 0.4194f
};
unsigned int goldenIndices[] = {
    0,1,2,
    1,2,3,
    2,3,4,
    1,3,6,
    1,5,6,
    5,6,7,
    8,9,10,
    8,10,11
};
float blueTriangles[] = {
    // positions        //textures
    0.752f, -0.896f, 0.0f,  1.0f, 0.147f,
    0.479f, -1.0f, 0.0f,    0.613f, 0.0f,
    0.633f, -0.633f, 0.0f,  0.831f, 0.520f,
    0.199f, -0.451f, 0.0f,  0.217f, 0.778f,
    0.046f, -0.819f, 0.0f,  0.0f, 0.256f,
    0.046f, -0.601f, 0.0f,  0.0f, 0.565f
};
unsigned int blueIndices[] = {
    0,1,2,
    1,2,3,
    1,3,4,
    3,4,5
};
float whiteTriangles[] = {
    // positions            //textures
    0.752f, -0.896f, 0.0f,  0.704f, 0.268f,
    0.812f, -1.0f, 0.0f,    0.858f, 0.0f,
    0.479f, -1.0f, 0.0f,    0.0f, 0.0f,
    0.867f, -0.867f, 0.0f,  1.0f, 0.343f,
    0.633f, -0.633f, 0.0f,  0.397f, 0.946f
};
unsigned int whiteIndices[] = {
    0,1,2,
    0,3,4
};
float whiteLinePoints[] = {
    0.208f, -0.208f,
    0.151f, -0.205f,
    0.094f, -0.206f,
    0.037f, -0.211f,
    -0.019f, -0.219f,
    -0.075f, -0.231f,
    -0.130f, -0.246f,
    -0.184f, -0.264f,
    -0.237f, -0.286f,
    -0.288f, -0.311f,
    -0.338f, -0.339f,
    -0.386f, -0.370f,
    -0.432f, -0.404f,
    -0.475f, -0.441f,
    -0.517f, -0.480f,
    -0.555f, -0.522f,
    -0.591f, -0.566f,
    -0.625f, -0.613f,
    -0.655f, -0.661f,
    -0.682f, -0.711f,
    -0.706f, -0.763f
};
float wFVertices[]{ // wF = white Flower
    // positions            //textures
    0.533f, -1.000f, 0.0f,    0.906f, 0.000f,
   -0.241f, -0.798f, 0.0f,    0.064f, 0.220f,
   -0.300f, -1.000f, 0.0f,    0.000f, 0.000f,
    0.245f, -0.779f, 0.0f,    0.593f, 0.240f,
    0.619f, -0.768f, 0.0f,    1.000f, 0.252f,
    0.458f, -0.458f, 0.0f,    0.825f, 0.590f,
    0.371f, -0.371f, 0.0f,    0.730f, 0.684f,
    0.057f, -0.183f, 0.0f,    0.388f, 0.889f,
   -0.147f, -0.479f, 0.0f,    0.166f, 0.567f,
   -0.090f, -0.285f, 0.0f,    0.229f, 0.778f

};
unsigned int wFIndices[]{
    0,1,2,
    0,1,3,
    3,4,5,
    3,5,6,
    3,6,7,
    3,7,8,
    7,8,9
};
float bFVertices[]{ // blue Flower
    // positions            //textures
    0.458f, -0.458f, 0.0f,    0.348f, 0.886f,
    0.857f, -0.857f, 0.0f,    1.000f, 0.234f,
    0.619f, -0.768f, 0.0f,    0.611f, 0.379f,
    0.743f, -0.889f, 0.0f,    0.814f, 0.181f,
    0.533f, -1.000f, 0.0f,    0.471f, 0.000f,
    0.813f, -1.000f, 0.0f,    0.928f, 0.000f,
    0.245f, -0.779f, 0.0f,    0.000f, 0.361f
};
unsigned int bFIndices[]{
    0,1,2,
    1,2,3,
    2,3,4,
    3,4,5,
    2,4,6
};
float gFVertices[]{ // grey Flower
    // positions            //textures
    1.0f,   -1.0f,   0.0f,    1.000f, 0.000f,
    0.857f, -0.857f, 0.0f,    0.444f, 0.556f,
    0.813f, -1.0f,   0.0f,    0.272f, 0.000f,
    0.743f, -0.889f, 0.0f,    0.000f, 0.432f
};
unsigned int gFIndices[]{
    0,1,2,
    1,2,3
};



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
    // RENDER JUST ONCE 
    // =======================================================================
    // since our scene doesn't change, we can render only when the window pops up or changes in size

    dibujaBaldosa(window);



    // =======================================================================
    // RENDER LOOP
    // =======================================================================
    while (!glfwWindowShouldClose(window))
    {
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
    dibujaBaldosa(window);
}

// Function that makes it easy to load multiple textures
int loadTexture(const char* filepath) {
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load image, create texture, and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filepath, &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, (nrChannels == 4 ? GL_RGBA : GL_RGB), width, height, 0, (nrChannels == 4 ? GL_RGBA : GL_RGB), GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture from " << filepath << std::endl;
    }

    stbi_image_free(data);
    return texture;
}

// Draw a white circle with center (x,y) and radius r
void drawWhiteCircle(float x, float y, float r, glm::mat4 transform) {

    Shader circleShader("transform.vs", "color.fs");
    float vertices[] = {
        1.0f, 0.0f, 0.0f,
        0.966f, 0.259f, 0.0f,
        0.0f, 0.0f, 0.0f
    };
    float radians;

    // set up buffers
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // render
    glBindVertexArray(VAO);
    circleShader.use();
    circleShader.setVec4("color", 1.0f, 1.0f, 1.0f, 1.0f);

    // draw each triangle of the circle and transform it
    for (float angle = 0; angle < 360; angle += 15)
    {
        glm::mat4 tempTransform = transform;
        radians = angle * glm::pi<float>() / 180.0f;
        tempTransform = glm::translate(tempTransform, glm::vec3(x, y, 0.0f));
        tempTransform = glm::rotate(tempTransform, radians, glm::vec3(0.0f, 0.0f, 1.0f));
        tempTransform = glm::scale(tempTransform, glm::vec3(r, r, 1.0f));
        circleShader.setMat4("transform", tempTransform);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
}

void paintCirclesInLine(float startX, float endX, float startY, float endY, int n, float r, glm::mat4 transform) {
    float stepX = (endX - startX) / (n - 1);
    float stepY = (endY - startY) / (n - 1);

    // Draw circles evenly spaced acrross a line from startPoint to endPoint
    for (int i = 0; i < n; i++) {
        float currentX = startX + i * stepX;
        float currentY = startY + i * stepY;
        drawWhiteCircle(currentX, currentY, r, transform);
    }
}

void drawWhiteCurve(glm::mat4 transform) {
    float lineWidth = 0.05f;
    int numPoints = sizeof(whiteLinePoints) / sizeof(whiteLinePoints[0]) / 2;
    // draw the arc
    for (int i = 0; i < numPoints; i++) {
        float x = whiteLinePoints[2 * i];     // X-coordinate
        float y = whiteLinePoints[2 * i + 1]; // Y-coordinate

        // Do something with each point, for example, print them
        drawWhiteCircle(x, y, lineWidth, transform);
    }
    //draw the straight segment
    float startX = -0.727f;
    float endX = -0.998f;
    float startY = -0.816f;  
    float endY = -0.816f;    

    int n = 7;  // Number of circles you want to paint
    paintCirclesInLine(startX, endX, startY, endY, n, lineWidth, transform);
}

void draw32th(unsigned int VAOs[], glm::mat4 transform) {

    unsigned int golden = loadTexture(golden1Path);
    unsigned int blueMetal = loadTexture(blue1Path);
    unsigned int whiteMetal = loadTexture(white1Path);

    Shader textureShader("texture.vs", "texture.fs");

    // DIsplay in in the origin
    transform = glm::scale(transform, glm::vec3(0.5f, 0.5f, 1.0f));
    transform = glm::translate(transform, glm::vec3(-1.0f, 1.0f, 0.0f));

    // draw golden triangles
    glBindTexture(GL_TEXTURE_2D, golden);
    textureShader.use();
    textureShader.setMat4("transform", transform);
    glBindVertexArray(VAOs[0]);
    glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);

    // draw blue triangles
    glBindTexture(GL_TEXTURE_2D, blueMetal);
    textureShader.use();
    textureShader.setMat4("transform", transform);
    glBindVertexArray(VAOs[1]);
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

    // draw white triangles
    glBindTexture(GL_TEXTURE_2D, whiteMetal);
    textureShader.use();
    textureShader.setMat4("transform", transform);
    glBindVertexArray(VAOs[2]);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // draw the white curve
    drawWhiteCurve(transform);
}

void draw16th(unsigned int VAOs[], glm::mat4 transform) {
    draw32th(VAOs, transform);
    // reflect on y=-x
    transform = glm::rotate(transform, glm::pi<float>() / 4, glm::vec3(0.0f, 0.0f, 1.0f));
    transform = glm::scale(transform, glm::vec3(-1.0f, 1.0f, 1.0f));
    transform = glm::rotate(transform, -glm::pi<float>() / 4, glm::vec3(0.0f, 0.0f, 1.0f));
    //display reflection
    draw32th(VAOs, transform);
}

void draw4th(unsigned int VAOs[], glm::mat4 transform) {
    // Get in position
    transform = glm::translate(transform, glm::vec3(-0.5f,0.5f,0.0f));
    transform = glm::scale(transform, glm::vec3(0.5f, 0.5f, 1.0f));

    // display on the 4 cuadrants
    draw16th(VAOs, transform);
    transform = glm::rotate(transform, glm::pi<float>() / 2, glm::vec3(0.0f, 0.0f, 1.0f));
    draw16th(VAOs, transform);
    transform = glm::rotate(transform, glm::pi<float>() / 2, glm::vec3(0.0f, 0.0f, 1.0f));
    draw16th(VAOs, transform);
    transform = glm::rotate(transform, glm::pi<float>() / 2, glm::vec3(0.0f, 0.0f, 1.0f));
    draw16th(VAOs, transform);
}

void displayMainPattern(unsigned int VAOs[]) {
    // Initialize transformation
    glm::mat4 transform = glm::mat4(1.0f);

    // display on the 4 cuadrants
    draw4th(VAOs, transform);
    transform = glm::rotate(transform, glm::pi<float>() / 2, glm::vec3(0.0f, 0.0f, 1.0f));
    draw4th(VAOs, transform);
    transform = glm::rotate(transform, glm::pi<float>() / 2, glm::vec3(0.0f, 0.0f, 1.0f));
    draw4th(VAOs, transform);
    transform = glm::rotate(transform, glm::pi<float>() / 2, glm::vec3(0.0f, 0.0f, 1.0f));
    draw4th(VAOs, transform);
}

void draw8thFlower(unsigned int VAOs[], glm::mat4 transform) {
    // load textures
    unsigned int glossyWhite = loadTexture(white2Path);
    unsigned int glossyBlue = loadTexture(blue2Path);
    unsigned int glossyGrey = loadTexture(grey1Path);
    Shader textureShader("texture.vs", "texture.fs");

    // Scale and translate to center
    transform = glm::translate(transform, glm::vec3(-0.25f, 0.25f, 0.0f));
    transform = glm::scale(transform, glm::vec3(0.25f, 0.25f, 0.0f));

    // draw white flower
    glBindTexture(GL_TEXTURE_2D, glossyWhite);
    textureShader.use();
    textureShader.setMat4("transform", transform);
    glBindVertexArray(VAOs[4]);
    glDrawElements(GL_TRIANGLES, 21, GL_UNSIGNED_INT, 0);

    // draw blue flower
    glBindTexture(GL_TEXTURE_2D, glossyBlue);
    textureShader.use();
    textureShader.setMat4("transform", transform);
    glBindVertexArray(VAOs[5]);
    glDrawElements(GL_TRIANGLES, 15, GL_UNSIGNED_INT, 0);

    // draw grey flower
    glBindTexture(GL_TEXTURE_2D, glossyGrey);
    textureShader.use();
    textureShader.setMat4("transform", transform);
    glBindVertexArray(VAOs[6]);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void draw4thFlower(unsigned int VAOs[], glm::mat4 transform) {
    draw8thFlower(VAOs, transform);
    // reflect on y=-x
    transform = glm::rotate(transform, glm::pi<float>() / 4, glm::vec3(0.0f, 0.0f, 1.0f));
    transform = glm::scale(transform, glm::vec3(-1.0f, 1.0f, 1.0f));
    transform = glm::rotate(transform, -glm::pi<float>() / 4, glm::vec3(0.0f, 0.0f, 1.0f));
    //display reflection
    draw8thFlower(VAOs, transform);
}

void drawFlower(unsigned int VAOs[]) {
    // Initialize transformation matrix=x
    glm::mat4 transform = glm::mat4(1.0f);

    // display on the 4 cuadrants
    draw4thFlower(VAOs, transform);
    transform = glm::rotate(transform, glm::pi<float>() / 2, glm::vec3(0.0f, 0.0f, 1.0f));
    draw4thFlower(VAOs, transform);
    transform = glm::rotate(transform, glm::pi<float>() / 2, glm::vec3(0.0f, 0.0f, 1.0f));
    draw4thFlower(VAOs, transform);
    transform = glm::rotate(transform, glm::pi<float>() / 2, glm::vec3(0.0f, 0.0f, 1.0f));
    draw4thFlower(VAOs, transform);
}


void dibujaBaldosa(GLFWwindow* window) {
    // =======================================================================
    // Set up buffers
    // =======================================================================
    unsigned int VBOs[7], VAOs[7], EBOs[7];
    glGenVertexArrays(7, VAOs);
    glGenBuffers(7, VBOs);
    glGenBuffers(7, EBOs);

    // golden triangles setup
    // --------------------
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(goldenTriangles), goldenTriangles, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(goldenIndices), goldenIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // blue triangles setup
    // --------------------
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(blueTriangles), blueTriangles, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(blueIndices), blueIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // white triangles setup
    // --------------------
    glBindVertexArray(VAOs[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(whiteTriangles), whiteTriangles, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(whiteIndices), whiteIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // white lines setup
    // --------------------
    glBindVertexArray(VAOs[3]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(whiteLinePoints), whiteLinePoints, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // white flower setup
    // --------------------
    glBindVertexArray(VAOs[4]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[4]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(wFVertices), wFVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[4]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(wFIndices), wFIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // blue flower setup
    // --------------------
    glBindVertexArray(VAOs[5]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[5]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bFVertices), bFVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[5]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(bFIndices), bFIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // grey flower setup
    // --------------------
    glBindVertexArray(VAOs[6]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[6]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gFVertices), gFVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[6]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(gFIndices), gFIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



    // =======================================================================
    // Render
    // =======================================================================
    // background color
    glClearColor(0.345f, 0.372f, 0.412f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // render de baldosa
    displayMainPattern(VAOs);
    drawFlower(VAOs);

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
}