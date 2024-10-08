#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vertices.h>
#include <camera.h>
// GLOBAL DECLARATIONS
float lastFrame = 0.0f, deltaTime = 0.0f;


// FUNCTION PROTOTYPES
void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

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
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return -1;

    glEnable(GL_DEPTH_TEST);

    Shader shader("shader.vs", "shader.fs");

    float vertices[] = {CUBE};
    glm::vec3 positions[] = {
        glm::vec3(0.0f,0.0f,0.0f) // sample value
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);

    Camera camera = Camera();
    
    while (!glfwWindowShouldClose(window)){
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        // clear the frame
        glClearColor(0.18f, 0.28f, 0.32f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);        

        shader.use(); // not rlly too sure what calling the use method does;
        
        glm::mat4 view;
        view = glm::lookAt(camera.pos, camera.pos + camera.front, camera.up);

        glm::mat4 projection = glm::perspective(glm::radians(80.f), 800.0f/600.0f, 0.1f, 100.0f);
        projection = glm::translate(projection, glm::vec3(0.0f, 0.0f, -3.0f));
        view = glm::translate(view, glm)

        // aim labs ahh rhythm game focusing on 3d aim instead of 2d.

    }   

    return 0;
}