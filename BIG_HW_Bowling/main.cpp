#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

#include "dependente\glew\glew.h"

#include "dependente\glfw\glfw3.h"

#include "dependente\glm\glm.hpp"
#include "dependente\glm\gtc\matrix_transform.hpp"
#include "dependente\glm\gtc\type_ptr.hpp"

#include "shader.hpp"
#include "Sphere.hpp"

glm::uvec2 windowSizes(800, 800);
GLFWwindow* window;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    
}

void window_callback(GLFWwindow* window, int new_width, int new_height)
{
    glViewport(0, 0, new_width, new_height);
}

int main(void)
{
    Sphere bowlingBall(0.1f, 36, 36);
    Plane bowlingAlley({ -0.5f, 0.5f }, { 0.0f, 18.0f }, -1.0f);

    // Initialise GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    // Open a window and create its OpenGL context

    window = glfwCreateWindow(windowSizes.x, windowSizes.y, "Bowling Alley", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to open GLFW window.");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        glfwTerminate();
        return -1;
    }

    //specify the size of the rendering window
    glViewport(0, 0, windowSizes.x, windowSizes.y);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSetKeyCallback(window, key_callback);

    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

    GLuint vbo, vao, ibo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ibo);

    glBindVertexArray(vao);
  
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, bowlingBall.vertices.size() * sizeof(float) * 3, &bowlingBall.vertices[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, bowlingBall.indices.size() * sizeof(unsigned int), &bowlingBall.indices[0], GL_STATIC_DRAW);

    //set attribute pointers
    glVertexAttribPointer(
        0,                  // attribute 0, must match the layout in the shader.
        3,                  // size of each attribute
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        3 * sizeof(float),  // stride
        (void*)0            // array buffer offset
    );
    glEnableVertexAttribArray(0);

    glfwSetFramebufferSizeCallback(window, window_callback);

    // Check if the window was closed
    while (!glfwWindowShouldClose(window))
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, 1);

        glfwSwapBuffers(window);
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(programID);

        glm::mat4 view = glm::lookAt(glm::vec3(30.0f, 50.0f, 75.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float) (windowSizes.x) / windowSizes.y, 1.0f, 10000.0f);

        glBindVertexArray(vao);

        glm::mat4 model(1.0f);
        model = glm::rotate(model, (float) glfwGetTime() * 100, glm::vec3(0.2f, 1.0f, 0.0f));

        glm::mat4 mvp = projection * view * model;

        unsigned int transformUniformLoc = glGetUniformLocation(programID, "transform");
        glUniformMatrix4fv(transformUniformLoc, 1, GL_FALSE, glm::value_ptr(mvp));

        glm::vec4 color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
        unsigned int colorTransform = glGetUniformLocation(programID, "color");
        glUniform4fv(colorTransform, 1, glm::value_ptr(color));

        glDrawElements(GL_TRIANGLES, bowlingBall.indices.size(), GL_UNSIGNED_INT, 0);
    }

    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
    glDeleteVertexArrays(1, &vao);
    glDeleteProgram(programID);

    glfwTerminate();

    return 0;
}


