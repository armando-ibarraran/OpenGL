
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
void drawBarBottom(glm::mat4 transform, Shader shader); glm::mat4 shearY(float k);
glm::mat4 shearY(float k);
std::vector<float> jumpTrajectory(float x0, float y0, float xf, float yf, float tf, float alpha);
std::vector<float> getTranslateToCirc(float x0, float y0, float xc, float yc, float r, float t);

// Function to create and draw a filled ellipse (of the form (x/a)^2 + (y/b)^2 = 1) centered at (x,y)
void drawFilledEllipse(float a, float b, float x, float y, int numSegments, glm::mat4 transform, Shader shader) {
    // Get the vertices to draw
    std::vector<float> vertices = getEllipsesVertices(a, b, 0, 0, numSegments);

    // Create and bind VAO and VBO
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Set up vertex attribute pointers for 3D vertices (x, y, z)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Use shader and set uniform transformation matrix
    shader.use();
    shader.setMat4("transform", glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f)) * transform);

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
    int totalVertices = numPoints * 3 + 3;  // +1 to close the loop

    // Allocate vertex data dynamically
    std::vector<float> vertices(totalVertices * 2);  // Each vertex has 2 components (x, y)

    // Center point of the star
    vertices[0] = 0.0f;  // x-coordinate of the center
    vertices[1] = 0.0f;  // y-coordinate of the center
    vertices[2] = 0.0f;  // z-coordinate of the center

    float angleStep = glm::pi<float>() / numPoints;  // Angle between vertices

    int index = 3;  // Start after the center vertex
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
        vertices[index++] = 0;
    }

    // Close the loop by connecting the last point back to the first outer point
    vertices[index++] = vertices[3];
    vertices[index++] = vertices[4];
    vertices[index++] = vertices[5];

    // Create and bind VAO and VBO
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
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
    std::vector<float> vertices(totalVertices * 3);  // Each vertex has 2 components (x, y)

    // Center point of the polygon
    vertices[0] = 0.0f;  // x-coordinate of the center
    vertices[1] = 0.0f;  // y-coordinate of the center
    vertices[2] = 0.0f;  // z-coordinate of the center

    // Angle step for each side
    float angleStep = 2.0f * glm::pi<float>() / numSides;

    int index = 3;
    for (int i = 0; i <= numSides; ++i) {
        float angle = i * angleStep;

        // Calculate the x and y coordinates for each vertex
        float x = radius * cos(angle);
        float y = radius * sin(angle);

        // Add the vertex to the array
        vertices[index++] = x;
        vertices[index++] = y;
        vertices[index++] = 0.0f;
    }

    // Create and bind VAO and VBO
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
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
        0.0f,       0.0f,           0.0f,
        -halfWidth, quarterHeight,  0.0f,
        0.0f,       halfHeight,     0.0f,
        halfWidth,  quarterHeight,  0.0f,
        halfWidth,  -quarterHeight, 0.0f,
        0.0f,       -halfHeight,    0.0f,
        -halfWidth, -quarterHeight, 0.0f,
        -halfWidth, quarterHeight,  0.0f
    };

    // Create and bind VAO and VBO
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
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
        tempTransform = glm::rotate(tempTransform, i * glm::pi<float>() / 4.0f, glm::vec3(0.0f, 0.0f, 1.0f)); // 1st give the carts the correct orientation
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

// Create the vertices needed to draw an ellipse
std::vector<float> getEllipsesVertices(float a, float b, float x, float y, int numSegments) {
    // Use std::vector to create the vertices array dynamically
    std::vector<float> vertices((numSegments + 2) * 3);  // Each vertex has 3 components (x, y, z)

    // Center point for the filled ellipse (z = 0.0 for all vertices)
    vertices[0] = x;  // x-coordinate of the center
    vertices[1] = y;  // y-coordinate of the center
    vertices[2] = 0.0f;  // z-coordinate of the center

    int index = 3;
    for (int i = 0; i <= numSegments; ++i) {
        float angle = (2.0f * glm::pi<float>() * i) / numSegments;  // Uniform partition of the circumference

        // Calculate the x and y coordinates for the ellipse, with z remaining 0
        float tempX = x + a * cos(angle);  // Semi-major axis * cos(angle)
        float tempY = y + b * sin(angle);  // Semi-minor axis * sin(angle)

        // Add the vertex to the array (z is 0.0)
        vertices[index++] = tempX;
        vertices[index++] = tempY;
        vertices[index++] = 0.0f;
    }
    return vertices;
}


// Bind Vector to a VBO and VAO
void bindAnimVector(unsigned int VAO, unsigned int VBO, std::vector<float> vector) {
    glBindVertexArray(VAO); // For the corner triangles
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vector.size() * sizeof(float), vector.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}


// Draws the wall
// If alpha = 0, the wall is open, if alpha = 1, it is closed and every step in between
void drawWall(float alpha, glm::mat4 transform, Shader shader) {

    float vertices[] = {
        1.0f, -0.6f, 0.0f,
        1.0f, -0.35f, 0.0f,
        0.15f, -0.6f, 0.0f,
        0.15f, -0.35f, 0.0f,
        // 2nd
         -1.0f, -0.6f, 0.0f,
        -1.0f, -0.35f, 0.0f,
        -0.25f * (1 - alpha) + 0.15f * alpha, -0.6f, 0.0f,
        -0.25f * (1 - alpha) + 0.15f * alpha, -0.35f, 0.0f
    };

    unsigned int indices[] = {
        0,1,2,
        2,3,1,
        4,5,6,
        6,7,5
    };

    // Create and bind VAO, VBO and EBO
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Translate the polygon to the desired coordinates without affecting other previously defined transformations
    shader.setMat4("transform", transform);

    // Draw the polygon as a triangle fan
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

    // Clean up and unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Delete the VAO and VBO after drawing since they are no longer needed
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

// Draws the height measuring bar top
void drawBarTop(glm::mat4 transform, Shader shader) {

    float vertices[] = {
        0.45f, -0.25f, 0.0f,
        0.35f, -0.25f, 0.0f,
        0.35f, -0.4f, 0.0f,
        0.45f, -0.4f, 0.0f
    };

    unsigned int indices[] = {
        0,1,2,
        3,2,0
    };

    // Create and bind VAO, VBO and EBO
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Translate the polygon to the desired coordinates without affecting other previously defined transformations
    shader.setMat4("transform", transform);

    // Draw the polygon as a triangle fan
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Clean up and unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Delete the VAO and VBO after drawing since they are no longer needed
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

// Draws the height measuring bar bottom
void drawBarBottom(glm::mat4 transform, Shader shader) {

    float vertices[] = {
       0.35f, -0.4f, 0.0f,
       0.45f, -0.4f, 0.0f,
       0.35f, -0.7f, 0.0f,
       0.45f, -0.7f, 0.0f
    };

    unsigned int indices[] = {
        0,1,2,
        2,3,1
    };

    // Create and bind VAO, VBO and EBO
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Translate the polygon to the desired coordinates without affecting other previously defined transformations
    shader.setMat4("transform", transform);

    // Draw the polygon as a triangle fan
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Clean up and unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Delete the VAO and VBO after drawing since they are no longer needed
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

// Funci�n para crear una matriz de inclinacion en el eje Y respecto al eje X
glm::mat4 shearY(float k) {
    glm::mat4 shearMatrix = glm::mat4(1.0f); // Matriz identidad
    shearMatrix[1][0] = k; // Modificar el elemento para inclinacion en Y respecto a X
    return shearMatrix;
}


// Function that, given an initial position (x0,y0), a final position(xf, yf) and the time the whole parabolic shot takes (tf)  and an alpha between zero and 1, returns the correspondind point in the trayectory at time (t=alpha*tf)
std::vector<float> jumpTrajectory(float x0, float y0, float xf, float yf, float tf, float alpha) {
    // Ensure alpha is between 0 and 1 for a valid time scale
    if (alpha < 0.0f) alpha = 0.0f;
    if (alpha > 1.0f) alpha = 1.0f;

    // Calculate the time point at which we want the position
    float t = alpha * tf;

    // Horizontal position (linear interpolation)
    float x = x0 + (xf - x0) / tf * t;

    // Initial vertical velocity needed to reach yf at tf
    float g = 9.81f; // Gravitational acceleration
    float vy0 = (yf - y0 + 0.5f * g * tf * tf) / tf;

    // Vertical position (parabolic path)
    float y = y0 + vy0 * t - 0.5f * g * t * t;

    return { x, y , 0.0f};
}

// Function that returns the translate needed to position an object in (x0, y0, 0) to a point in time t in a circunference with center (xc, yc) with radius r 
// given that it takes one second (t=1) to fully travel the circunference, the object travels clockwise starting from angle pi * 3/2 
std::vector<float> getTranslateToCirc(float x0, float y0, float xc, float yc, float r, float t) {
    float angle = glm::pi<float>() * 3 / 2.0f - t * 2 * glm::pi<float>();
    float x = r * cos(angle) + xc;
    float y = r * sin(angle) + yc;

    return {x-x0, y-y0, 0.0f};
}