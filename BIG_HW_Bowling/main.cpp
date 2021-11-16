#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

#include "dependente\glew\glew.h"

#include "dependente\glfw\glfw3.h"

#include "dependente\glm\glm.hpp"
#include "dependente\glm\gtc\matrix_transform.hpp"
#include "dependente\glm\gtc\type_ptr.hpp"

#include "Cylinder.hpp"
#include "shader.hpp"
#include "Sphere.hpp"
#include "Plane.hpp"
#include "Popice.hpp"

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
    // Initialise GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

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
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSetKeyCallback(window, key_callback);

    GLuint vbo, vao, ibo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ibo);

    glBindVertexArray(vao);

    Sphere bowlingBall(0.08f, 24, 24); // radius, sectors, stacks
    //Plane bowlingAlley(8, 4, -1.0f);
    Popice popics(glm::vec3(0.0f, 2.0f, 0.0f), 0.4f); // pos (x, y, z), space between pins
  
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    //glBufferData(GL_ARRAY_BUFFER, bowlingAlley.vertices.size() * sizeof(float) * 3, &bowlingAlley.vertices[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, bowlingAlley.indices.size() * sizeof(unsigned int), &bowlingAlley.indices[0], GL_STATIC_DRAW);

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

    glm::mat4 view = glm::lookAt(glm::vec3(-500.0f, 50.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)(windowSizes.x) / windowSizes.y, 1.0f, 100000.0f);

    // Check if the window was closed
    while (!glfwWindowShouldClose(window))
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, 1);

        glfwSwapBuffers(window);
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(bowlingBall.shaderID);

        glBufferData(GL_ARRAY_BUFFER, bowlingBall.vertices.size() * sizeof(float) * 3, &bowlingBall.vertices[0], GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, bowlingBall.indices.size() * sizeof(unsigned int), &bowlingBall.indices[0], GL_STATIC_DRAW);

        glm::mat4 mvp;

        bowlingBall.model = glm::rotate(bowlingBall.model, 2.0f, glm::vec3(1.0f, 1.0f, 0.0f));
        mvp = projection * view * bowlingBall.model;

        unsigned int transformUniformLoc = glGetUniformLocation(bowlingBall.shaderID, "transform");
        glUniformMatrix4fv(transformUniformLoc, 1, GL_FALSE, glm::value_ptr(mvp));

        glm::vec4 color(0.75f, 0.2f, 0.2f, 0.2f);
        unsigned int colorTransform = glGetUniformLocation(bowlingBall.shaderID, "color");
        glUniform4fv(colorTransform, 1, glm::value_ptr(color));

        glBindVertexArray(vao);

        glDrawElements(GL_TRIANGLES, bowlingBall.indices.size(), GL_UNSIGNED_INT, 0);

        unsigned int popicsTransformLoc = glGetUniformLocation(popics.shaderID, "transform");
        unsigned int popicsColorLoc = glGetUniformLocation(popics.shaderID, "color");
        for (auto const& popicaPos : popics.popicaMapPositions)
        {
            glBufferData(GL_ARRAY_BUFFER, popics.defaultPopic.vertices.size() * sizeof(float) * 3, &popics.defaultPopic.vertices[0], GL_STATIC_DRAW);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, popics.defaultPopic.indices.size() * sizeof(unsigned int), &popics.defaultPopic.indices[0], GL_STATIC_DRAW);

            glm::mat4 currentPopicModel = glm::translate(glm::mat4(1.0f), popicaPos);
            currentPopicModel = glm::rotate(currentPopicModel, -90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
            mvp = projection * view * currentPopicModel;
            glUniformMatrix4fv(popicsTransformLoc, 1, GL_FALSE, glm::value_ptr(mvp));
            glUniform4fv(popicsColorLoc, 1, glm::value_ptr(glm::vec4(0.9f, 0.9f, 0.9f, 1.0f)));

            glBindVertexArray(vao);
            glDrawElements(GL_TRIANGLES, popics.defaultPopic.indices.size(), GL_UNSIGNED_INT, 0);
        }
        //glDrawElements(GL_TRIANGLES, 4 * 4 * 2, GL_UNSIGNED_INT, 0);
        
    }

    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
    glDeleteVertexArrays(1, &vao);
    glDeleteProgram(bowlingBall.shaderID);

    glfwTerminate();

    return 0;
}


