// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

// Include GLEW
#include "dependente\glew\glew.h"

// Include GLFW
#include "dependente\glfw\glfw3.h"

// Include GLM
#include "dependente\glm\glm.hpp"
#include "dependente\glm\gtc\matrix_transform.hpp"
#include "dependente\glm\gtc\type_ptr.hpp"

#include "shader.hpp"

glm::uvec2 windowSizes(1024, 1024);
GLFWwindow* window;

namespace shapes
{
    enum class SHAPE_TYPE
    {
        // for convenience, the numbers they represent in the enum is the amount of indices
        None, Square, Hexagon, Octagon
    };

    SHAPE_TYPE SelectedShape = SHAPE_TYPE::Square;
    SHAPE_TYPE DrawnShape = SHAPE_TYPE::None;

    namespace square
    {
        std::vector<GLfloat> vertices =
        {
             1.0f,  1.0f,  0.0f, // top right
             1.0f, -1.0f,  0.0f, // bottom right
            -1.0f, -1.0f,  0.0f, // bottom left
            -1.0f,  1.0f,  0.0f, // top left 
        };

        std::vector<unsigned int> indices =
        {
            0, 1, 2,
            2, 3, 0
        };
    }

    namespace hexagon
    {
        std::vector<GLfloat> vertices =
        {
             0.0f,  1.0f, 0.0f,     // 0, top
            -1.0f,  0.5f, 0.0f,     // 1, left top
            -1.0f, -0.5f, 0.0f,     // 2, left bot
             0.0f, -1.0f, 0.0f,     // 3, bot
             1.0f, -0.5f, 0.0f,     // 4, right bot
             1.0f,  0.5f, 0.0f,     // 5, right top
             0.0f,  0.0f, 0.0f      // 6, center
        };

        std::vector<unsigned int> indices =
        { 
            0, 1, 6,
            1, 2, 6,
            2, 3, 6,
            3, 4, 6,
            4, 5, 6,
            5, 0, 6
        };
    }

    namespace octagon
    {
        std::vector<GLfloat> vertices =
        {
             0.75f,  0.75f, 0.0f,     // 0, right top
             0.0f,   1.0f,  0.0f,     // 1, top
            -0.75f,  0.75f, 0.0f,     // 2, left top
            -1.0f,   0.0f,  0.0f,     // 3, left
            -0.75f, -0.75f, 0.0f,     // 4, left bot
             0.0f,  -1.0f,  0.0f,     // 5, bot
             0.75f, -0.75f, 0.0f,     // 6, right bot
             1.0f,   0.0f,  0.0f      // 7, right
        };

        std::vector<unsigned int> indices =
        {
            0, 1, 2,    // top triangle
            2, 3, 4,    // left triangle
            2, 4, 0,    // first part of square
            4, 6, 0,    // second part of square
            4, 5, 6,    // bot triangle
            6, 7, 0     // right triangle
        };
    }

    std::vector<GLfloat> getVerticesForShape(SHAPE_TYPE shape)
    {
        switch (shape)
        {
        case SHAPE_TYPE::Square:
            return square::vertices;
            break;
        case SHAPE_TYPE::Hexagon:
            return hexagon::vertices;
            break;
        case SHAPE_TYPE::Octagon:
            return octagon::vertices;
            break;
        default:
            return std::vector<GLfloat>();
            break;
        }
    }

    std::vector<unsigned int> getIndicesForShape(SHAPE_TYPE shape)
    {
        switch (shape)
        {
        case SHAPE_TYPE::Square:
            return square::indices;
            break;
        case SHAPE_TYPE::Hexagon:
            return hexagon::indices;
            break;
        case SHAPE_TYPE::Octagon:
            return octagon::indices;
            break;
        default:
            return std::vector<unsigned int>();
            break;
        }
    }
}

namespace transforms
{
    const glm::vec3 INITIAL_SHAPE_SCALE(0.02f);
    glm::mat4 transformMatrix = glm::scale(glm::mat4(1.0f), INITIAL_SHAPE_SCALE);
    const glm::mat4 DEFAULT_TRANSFORM_MATRIX = transformMatrix;
    const float VELOCITY = 0.5f;
    /* not sure how to explain SCALE_FACTOR_TRANSFORM_TO_COORDS.
     * After scaling my initial shape down to 5%, my edges are now at 0.2.
     * To check if touching bounds, must multiply by this constant (used in handleTransform)
     */
    const float SCALE_FACTOR_TRANSFORM_TO_COORDS = 5;

    int direction = 1; // Controls what translate/scale does. If 1, grow/go right, if -1 shrink/go left

    enum class TRANSFORM_TYPE : int
    {
        NoTransform, Rotate, Scale, Translate
    };

    TRANSFORM_TYPE nextDesiredTransform = TRANSFORM_TYPE::NoTransform; // what transform will be applied next tick

    void setNextTransform(TRANSFORM_TYPE TransformType)
    {
        nextDesiredTransform = TransformType;
        if (TransformType != TRANSFORM_TYPE::NoTransform)
        {
            transformMatrix = DEFAULT_TRANSFORM_MATRIX;
        }
    }

    namespace bounds
    {
        float minScale = 0.0025f, maxScale = 0.195f;
        float edgeTranslate = 1.0f; // if pos.x > edgeTranslate, start moving left. if pos.x < -edgeTranslate, start moving right (with account for shape's size)
    };
};

// call this in the while loop to apply this tick's transform. returns updated transform matrix
glm::mat4 handleTransform(transforms::TRANSFORM_TYPE TransformType, glm::mat4 transformMatrix)
{
    if (TransformType != transforms::nextDesiredTransform)
    {
        transformMatrix = transforms::DEFAULT_TRANSFORM_MATRIX;
    }

    switch (TransformType)
    {
    case transforms::TRANSFORM_TYPE::Rotate:
    {
        transformMatrix = glm::rotate(transformMatrix, -(float) sin(glfwGetTime()), glm::vec3(0, 0, 1));
        break;
    }

    case transforms::TRANSFORM_TYPE::Scale:
    {
        float maxCoord = std::max(std::max(transformMatrix[0][0], transformMatrix[1][1]), transformMatrix[2][2]);
        float minCoord = std::min(std::min(transformMatrix[0][0], transformMatrix[1][1]), transformMatrix[2][2]);
        if (maxCoord > transforms::bounds::maxScale || minCoord < transforms::bounds::minScale)
            transforms::direction = -transforms::direction;
        transformMatrix = glm::scale(transformMatrix, glm::vec3(1.0f + transforms::VELOCITY * 0.05f * transforms::direction));

        break;
    }

    case transforms::TRANSFORM_TYPE::Translate:
    {
        float maxCoord = transformMatrix[3][0];
        if (maxCoord + transforms::INITIAL_SHAPE_SCALE.x * transforms::SCALE_FACTOR_TRANSFORM_TO_COORDS > transforms::bounds::edgeTranslate ||
            maxCoord - transforms::INITIAL_SHAPE_SCALE.x * transforms::SCALE_FACTOR_TRANSFORM_TO_COORDS < -transforms::bounds::edgeTranslate)
            transforms::direction = -transforms::direction;
        transformMatrix = glm::translate(transformMatrix, glm::vec3(transforms::VELOCITY * transforms::direction, 0.0f, 0.0f));
        break;
    }

    default:
    {
        break;
    }
    }

    return transformMatrix;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // if a key was pressed
    if (action == GLFW_PRESS)
    {
        transforms::TRANSFORM_TYPE TransformType = transforms::TRANSFORM_TYPE::NoTransform;
        switch (key)
        {
        // ------- TRANSFORMS
        case GLFW_KEY_R:
            std::cout << "Want Rotate\n";
            transforms::setNextTransform(transforms::TRANSFORM_TYPE::Rotate);
            break;
        case GLFW_KEY_S:
            std::cout << "Want Scale\n";
            transforms::setNextTransform(transforms::TRANSFORM_TYPE::Scale);
            break;
        case GLFW_KEY_T:
            std::cout << "Want Translate\n";
            transforms::setNextTransform(transforms::TRANSFORM_TYPE::Translate);
            break;
        case GLFW_KEY_N:
            // None
            transforms::setNextTransform(transforms::TRANSFORM_TYPE::NoTransform);
            break;

        // ------- SHAPES
        case GLFW_KEY_1:
            std::cout << "Want Square\n";
            shapes::SelectedShape = shapes::SHAPE_TYPE::Square;
            break;
        case GLFW_KEY_2:
            std::cout << "Want Hexagon\n";
            shapes::SelectedShape = shapes::SHAPE_TYPE::Hexagon;
            break;
        case GLFW_KEY_3:
            std::cout << "Want Octagon\n";
            shapes::SelectedShape = shapes::SHAPE_TYPE::Octagon;
            break;
        // ------- OTHER
        default:
            break;
        }
    }
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

    // Open a window and create its OpenGL context

    window = glfwCreateWindow(windowSizes.x, windowSizes.y, "Lab 4", NULL, NULL);
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

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSetKeyCallback(window, key_callback);

    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

    // A Vertex Array Object (VAO) is an object which contains one or more Vertex Buffer Objects and is designed to store the information for a complete rendered object. 
    GLuint vbo, vao, ibo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ibo);

    //bind VAO
    glBindVertexArray(vao);
    //bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    //bind IBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    std::vector<GLfloat> verticesForSelectedShape;
    std::vector<unsigned int> indicesForSelectedShape;

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

        if (shapes::DrawnShape != shapes::SelectedShape)
        {
            std::cout << "Selected different shape. Redrawing...\n";
            verticesForSelectedShape = shapes::getVerticesForShape(shapes::SelectedShape);
            indicesForSelectedShape = shapes::getIndicesForShape(shapes::SelectedShape);

            glBufferData(GL_ARRAY_BUFFER, verticesForSelectedShape.size() * sizeof(GLfloat), &(*verticesForSelectedShape.begin()), GL_STATIC_DRAW);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesForSelectedShape.size() * sizeof(unsigned int), &(*indicesForSelectedShape.begin()), GL_STATIC_DRAW);

            for (auto vertex : verticesForSelectedShape)
            {
                std::cout << vertex << " ";
            }
            std::cout << '\n';

            // acts as flag so that no need to set buffer data again
            shapes::DrawnShape = shapes::SelectedShape;
        }

        // Swap buffers
        glfwSwapBuffers(window);

        // Check for events
        glfwPollEvents();

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        // Use our shader
        glUseProgram(programID);

        transforms::transformMatrix = handleTransform(transforms::nextDesiredTransform, transforms::transformMatrix);

        //bind VAO
        glBindVertexArray(vao);

        // send variables to shader
        unsigned int transformUniformLoc = glGetUniformLocation(programID, "transform");
        glUniformMatrix4fv(transformUniformLoc, 1, GL_FALSE, glm::value_ptr(transforms::transformMatrix));

        unsigned int colorTransform = glGetUniformLocation(programID, "color");
        glm::vec4 color = glm::vec4(0.5f, 0, 0.5f, 1.0);
        glUniform4fv(colorTransform, 1, glm::value_ptr(color));

        glDrawElements(GL_TRIANGLES, indicesForSelectedShape.size(), GL_UNSIGNED_INT, 0);
    }

    // Cleanup
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
    glDeleteVertexArrays(1, &vao);
    glDeleteProgram(programID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}


