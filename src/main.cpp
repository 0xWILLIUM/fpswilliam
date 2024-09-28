#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vertices.h>
#include <camera.h>
// GLOBAL DECLARATIONS
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float lastFrame = 0.0f, deltaTime = 0.0f;

float lastX = SCR_WIDTH / 2.0f, lastY = SCR_HEIGHT / 2.0f;
float pitch = 0.0f, yaw = -90.0f;

bool firstMouse = true;

Camera camera = Camera();

// FUNCTION PROTOTYPES
void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);

int main() {
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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return -1;

    glEnable(GL_DEPTH_TEST);

    Shader shader("shader.vs", "shader.fs");

    float vertices[] = {CUBE};
    glm::vec3 positions[] = {
        glm::vec3(0.0f,0.0f,0.0f), // sample value
        glm::vec3(3.0f,0.0f,0.0f),
        glm::vec3(-3.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3(0.0f, 0.0f, -3.0f)
    };

    // creating the buffers
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    
    while (!glfwWindowShouldClose(window)){
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // processInput(window);

        // clear the frame
        glClearColor(0.18f, 0.28f, 0.32f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);        

        shader.use(); // not rlly too sure what calling the use method does;
        
        glm::mat4 view;
        view = glm::lookAt(camera.pos, camera.pos + camera.front, camera.up);
        glm::mat4 projection = glm::mat4(1.0f); // might be a pointless declaration o_O
        // projection = glm::perspective(glm::radians(80.f), (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 100.0f);
        projection = glm::perspective(glm::radians(60.f), (float) SCR_HEIGHT / (float) SCR_WIDTH, 0.1f, 100.0f);
        projection = glm::translate(projection, glm::vec3(0.0f, 0.0f, -3.0f));
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        
        for (int i = 0; i < 5; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, positions[i]);
            shader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
        // aim labs ahh rhythm game focusing on 3d aim instead of 2d.
    }   

    glDeleteVertexArrays(0, &VAO);
    glDeleteBuffers(0, &VBO);
    return 0;
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos) {
    // mouse input
    if (firstMouse) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    
    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos;
    
    lastY = yPos;
    lastX = xPos;

    const float sens = 0.1f;
    xOffset *= sens;
    yOffset *= sens;

    yaw += xOffset;
    pitch += yOffset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    
    camera.front = glm::normalize(direction);
}

void scroll_callback() { 

}