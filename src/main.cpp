#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <string.h>
#include <shader.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

const float pi = 3.14159275359;

int main() {
    GLFWwindow* window;
    
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);

    if (!window) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)){
        std::cerr << "Failed to initialize GLAD" << std::endl;        
        return -1;
    }

    // create shader program and link the shaders
        float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };  
    
    // vertex buffer object
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    // Copy vertices into buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Set the vertix attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // vertex array object
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    Shader shader("shader.vs", "shader.fs");

    while(!glfwWindowShouldClose(window)){
        // update here
        // the memory from the first vertice to the index of 2 is copied to p1
        // re-allocate the buffer
        // this code is likely in the incorrect place
        shader.use();
        shader.setFloat("someUniform", 1.0f);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shader.ID);

        float timeValue = glfwGetTime();
        float greenValue = sin(timeValue) / 2.0f + 0.5f;

        int vertexColorLocation = glGetUniformLocation(shader.ID, "ourColor");
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
        // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        // glEnableVertexAttribArray(0);

        // draw here
        // glClear(GL_COLOR_BUFFER_BIT);
        // use the shaderProgram
            // glUseProgram(shaderProgram);
            // glBindVertexArray(VAO);
            // glDrawArrays(GL_TRIANGLES, 0, 3);
            // glEnd();
        // swap front and back buffer
            // glfwSwapBuffers(window);
        // poll for events
            // glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}