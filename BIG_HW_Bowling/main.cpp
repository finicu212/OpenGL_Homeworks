#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

#include "dependente\glew\glew.h"

#include "dependente\glfw\glfw3.h"

#include "dependente\glm\glm.hpp"
#include "dependente\glm\gtc\matrix_transform.hpp"
#include "dependente\glm\gtc\type_ptr.hpp"

#include "Camera.hpp"
#include "Cylinder.hpp"
#include "shader.hpp"
#include "Sphere.hpp"
#include "Plane.hpp"
#include "Popice.hpp"

Camera cam(glm::vec3(-5.0f, 5.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
glm::uvec2 windowSizes(1280, 720);
GLFWwindow* window;
Sphere bowlingBall(0.08f, 24, 24); // radius, sectors, stacks
Popice popics(glm::vec3(10.0f, 0.0f, 0.0f), 0.4f); // pos (x, y, z), space between pins

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

double mousePosX, mousePosY, mouseDeltaX, mouseDeltaY;

bool ballIsGoing = false;
bool strikeAnimDone = false;
float strikeAnimAngle = 0.0f;
void camera_input()
{
    float speedFast, speedSlow = deltaTime * 0.7, speedRotate = deltaTime * 0.5f;

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        speedSlow = deltaTime * 4.0f;
    else
        speedSlow = deltaTime * 0.7f;

    speedFast = speedSlow * 10.0f;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        cam.translateFront(speedFast);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        cam.translateBack(speedFast);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        cam.translateLeft(speedFast);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        cam.translateRight(speedFast);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        cam.translateUp(speedFast);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        cam.translateDown(speedFast);
    }
    cam.rotateOy(mouseDeltaX * speedRotate);
    cam.rotateOx(-mouseDeltaY * speedRotate);

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
    {
        ballIsGoing = true;
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
    {
        ballIsGoing = false;
        strikeAnimDone = false;
        strikeAnimAngle = 0.0f;
        bowlingBall.model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));
    }
}

glm::mat4 doStrikeAnimation(glm::mat4 popiceModel)
{
    static const float speed = 0.2f;
    if (strikeAnimAngle < 90.0f)
        strikeAnimAngle += speed;
    else
        strikeAnimDone = true;
    return glm::rotate(popiceModel, strikeAnimAngle, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 doBallAnimation(glm::mat4 sphereModel)
{
    static const float speed = deltaTime * 0.1f;
    if (ballIsGoing)
    {
        sphereModel = glm::translate(sphereModel, glm::vec3(speed, 0.0, 0.0));
        sphereModel = glm::rotate(sphereModel, speed * 10, glm::vec3(1.0f, 0.0f, 0.0f));
    }

    return sphereModel;
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

    glViewport(0, 0, windowSizes.x, windowSizes.y);

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    bowlingBall.loadShader();
    popics.loadShader();

    GLuint vbo, vao, ibo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ibo);

    glBindVertexArray(vao);
  
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

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

    glm::mat4 view;
    glm::mat4 projection;

    // Check if the window was closed
    while (!glfwWindowShouldClose(window))
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, 1);

        double oldMousePosX = mousePosX, oldMousePosY = mousePosY;
        glfwGetCursorPos(window, &mousePosX, &mousePosY);
        mouseDeltaX = oldMousePosX - mousePosX;
        mouseDeltaY = oldMousePosY - mousePosY;

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwSwapBuffers(window);
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(bowlingBall.shaderID);

        glBufferData(GL_ARRAY_BUFFER, bowlingBall.vertices.size() * sizeof(float) * 3, &bowlingBall.vertices[0], GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, bowlingBall.indices.size() * sizeof(unsigned int), &bowlingBall.indices[0], GL_STATIC_DRAW);

        camera_input();
        view = glm::lookAt(cam.position, cam.position + cam.viewDirection, cam.up);
        projection = glm::perspective(cam.FOV, 16.0f / 9.0f, 0.01f, 100000.0f);

        glm::mat4 mvp;
        
        bowlingBall.model = doBallAnimation(bowlingBall.model);
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

            if (!strikeAnimDone)
            {
                if (bowlingBall.model[3][0] > 9.5f) // if in progress
                    currentPopicModel = doStrikeAnimation(currentPopicModel);
            }
            else
            {
                currentPopicModel = glm::rotate(currentPopicModel, 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
            }
            mvp = projection * view * currentPopicModel;
            glUniformMatrix4fv(popicsTransformLoc, 1, GL_FALSE, glm::value_ptr(mvp));
            glUniform4fv(popicsColorLoc, 1, glm::value_ptr(glm::vec4(0.9f, 0.9f, 0.9f, 1.0f)));

            glBindVertexArray(vao);
            glDrawElements(GL_TRIANGLES, popics.defaultPopic.indices.size(), GL_UNSIGNED_INT, 0);
        }
    }

    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
    glDeleteVertexArrays(1, &vao);
    glDeleteProgram(bowlingBall.shaderID);

    glfwTerminate();

    return 0;
}


