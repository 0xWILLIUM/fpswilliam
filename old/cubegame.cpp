#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader.h>
#include <iostream>
// function prototypes:
void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void renderCubes(Shader shader, glm::vec3 cubePositions[]);

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float userPos = 0;

glm::vec3 targColor = glm::vec3(1.0f, 0.5f, 0.3f);
int targIndex = 0;
float speed = 2.0f;
int score = 0;

int main() {
    srand(time(NULL));
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window;
    window = glfwCreateWindow(800, 600, "williamfps", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Shader ourshader("shader.vs", "shader.fs");

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f
        };
    glm::vec3 cubePositions[] = {
        glm::vec3(-2.0f,  0.0f, 0.0f),
        glm::vec3( 0.0f,  0.0f, 0.0f),
        glm::vec3( 2.0f,  0.0f, 0.0f)
    };

//  buffers
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // layout
//  camera
    // debug :
    
    float distance = 0;

    // main loop!
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // update cubes position
        distance += (1 + log(speed)) * deltaTime * 1.5; // calculate the log component with the speed outside of the loop for performance
        
        cubePositions[0].z = distance; // could for loop these but it's not necessary
        cubePositions[1].z = distance;
        cubePositions[2].z = distance;
        
        if (distance > 8.0f) {
            // check win conditions
            // std::cout << "targIndex: " << targIndex << ",\tuserPos: " << userPos << ",\tcond: " << (userPos == targIndex) << std::endl;
            if (targIndex-1 != -userPos) {
                // game over
                glfwSetWindowShouldClose(window, true);
                std::cout << "Game over!" << std::endl;
                std::cout << "Score: " << score << std::endl;
                break;
            }
            score ++ ;
            speed ++ ;
            // else
            targColor = glm::vec3(
                rand() % 100 / 100.0f,
                rand() % 100 / 100.0f,
                rand() % 100 / 100.0f
            );

            targIndex = rand() % 3;
            
            distance = 0;
        }
        
        processInput(window); // handling user input
        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        ourshader.use();

        glm::mat4 view;
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        
        // does this assignment matter?
        glm::mat4 projection = glm::perspective(glm::radians(80.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        projection = glm::translate(projection, glm::vec3(userPos, 0.0f, -3.0f));
        view = glm::translate(view, glm::vec3(userPos, 0.0f, -3.0f));
        
        ourshader.setMat4("projection", projection);
        ourshader.setMat4("view", view);
        
        glBindVertexArray(VAO);

        renderCubes(ourshader, cubePositions);

        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

void renderCubes(Shader shader, glm::vec3 cubePositions[]) {
    for (unsigned int i =0; i < 3; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            shader.setMat4("model", model);
            if (i == targIndex) {
                shader.setVec3("color", targColor);
            } else {
                shader.setVec3("color", glm::vec3(targColor.r / 1.5, targColor.g / 1.5, targColor.b / 1.5)); 
            }
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        userPos = 1.0f;
    if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        userPos = 0.0f;
    if(glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        userPos = -1.0f;
}

/*
// vertex shader
#version 330 core
layout (location = 0) in vec3 aPos;

out vec4 vertexColor;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 color;
void main()
{
    // gl_Position = vec4(aPos, 1.0f);
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    // ourColor = aColor;
    vertexColor = vec4(color, 1.0f);
}

// fragment shader
#version 330 core
out vec4 FragColor;

in vec4 vertexColor;

void main()
{
    // FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
    FragColor = vertexColor;
}
*/