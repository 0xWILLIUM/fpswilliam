#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vertices.h>
#include <camera.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <stb_image.h>
#include <format>
// GLOBAL DECLARATIONS
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float lastFrame = 0.0f, deltaTime = 0.0f;

float lastX = SCR_WIDTH / 2.0f, lastY = SCR_HEIGHT / 2.0f;
float pitch = 0.0f, yaw = -90.0f;
unsigned int loadCubeMap(std::vector<std::string> faces);
bool firstMouse = true;

Camera camera = Camera();

glm::vec3 positions[] = {
    // glm::vec3(0.0f,0.0f,0.0f), // sample value
    glm::vec3(3.0f,0.0f, -10.0f),
    glm::vec3(-3.0f, 0.0f, -10.0f),
    glm::vec3(0.0f, 0.0f, -10.0f),
    // glm::vec3(0.0f, 0.0f, -3.0f)
};

volatile bool mouseDown = false;

#define _DEBUG

#ifdef _DEBUG
bool _dDown = false;
#endif

// structs
struct SphereData {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::vector<float> normals;
};


// FUNCTION PROTOTYPES
void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void drawCrosshair();
SphereData generateSphereVertices(float radius, unsigned int rings, unsigned int sectors);




int main() {
    std::srand(std::time(0));
    if (!glfwInit()){
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window;
    window = glfwCreateWindow(800, 600, "williamfps", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Shader shader("shaders/shader.vs", "shaders/shader.fs");
    Shader skyboxShader("shaders/skybox.vs", "shaders/skybox.fs");
    float skyboxvertices[] = {CUBE};
    
    
    // creating the buffers
    unsigned int skyboxVBO, skyboxVAO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxvertices), skyboxvertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    SphereData sphereData = generateSphereVertices(0.6f, 100, 100);
    
    unsigned int VBO, VAO, EBO, NBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &NBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sphereData.vertices.size() * sizeof(float), &sphereData.vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, NBO);
    glBufferData(GL_ARRAY_BUFFER, sphereData.normals.size() * sizeof(float), &sphereData.normals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereData.indices.size() * sizeof(unsigned int), &sphereData.indices[0], GL_STATIC_DRAW);

    glBindVertexArray(0); // unbinds the vertex array

    std::vector<std::string> skybox = {
        "skybox/skybox1RT.png",
        "skybox/skybox1lf.png",
        "skybox/skybox1UP.png",
        "skybox/skybox1DN.png",
        "skybox/skybox1FT.png",
        "skybox/skybox1BK.png"
    };
    
    unsigned int skyboxTexture = loadCubeMap(skybox);

    while (!glfwWindowShouldClose(window)){
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        // clear the frame
        glClearColor(0.18f, 0.28f, 0.32f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);        

        glDepthMask(GL_FALSE);
        skyboxShader.use();
        skyboxShader.setMat4("view", glm::mat4(glm::mat3(glm::lookAt(camera.pos, camera.pos + camera.front, camera.up))));
        skyboxShader.setMat4("projection", glm::perspective(glm::radians(60.f), (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 100.0f));
        glm::mat4 view = glm::lookAt(camera.pos, camera.pos + camera.front, camera.up);
        glm::mat4 projection = glm::mat4(1.0f); // might be a pointless declaration o_O
        projection = glm::perspective(glm::radians(60.f), (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 100.0f);
        
        // projection = glm::translate(projection, glm::vec3(0.0f, 0.0f, 0.0f));
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.0f)); // this was translated by -0.3f on the z axis
        
        glBindVertexArray(skyboxVAO);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthMask(GL_TRUE);
        
        shader.use(); // not rlly too sure what calling the use method does;
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        
        // light
        shader.setVec3("lightPos", glm::vec3(0.0f, 0.0f, 0.0f));
        shader.setVec3("viewPos", camera.pos);
        shader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
        shader.setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));

        glBindVertexArray(VAO);
        for (int i = 0; i < 3; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, positions[i]);
            shader.setMat4("model", model);
            glDrawElements(GL_TRIANGLES, sphereData.indices.size(), GL_UNSIGNED_INT, 0);
        }
        glBindVertexArray(0);
        drawCrosshair();

        glfwSwapBuffers(window);
        glfwPollEvents();
        // aim labs ahh rhythm game focusing on 3d aim instead of 2d.
    }   

    glDeleteVertexArrays(0, &VAO);
    glDeleteBuffers(0, &VBO);
    glfwTerminate();
    return 0;
}

void drawCrosshair() {
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushMatrix();

    glm::mat4 projection = glm::ortho(0.0f, (float) SCR_WIDTH, 0.0f, (float) SCR_HEIGHT);
    glm::mat4 model = glm::mat4(1.0f);
    
    Shader crosshairShader("shaders/crosshair.vs", "shaders/crosshair.fs");
    crosshairShader.use();
    crosshairShader.setMat4("projection", projection);
    crosshairShader.setMat4("model", model);

    glDisable(GL_DEPTH_TEST);

    float xhairSize = 10.0f;
    float centerX = SCR_WIDTH / 2.0f;
    float centerY = SCR_HEIGHT / 2.0f;

    float crosshair[] = {
        centerX - xhairSize, centerY,
        centerX + xhairSize, centerY,
        centerX, centerY - xhairSize,
        centerX, centerY + xhairSize
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(crosshair), crosshair, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_LINES, 0, 4);
    glBindVertexArray(0); // what is the purpose of this?
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);

    glPopMatrix();
    glPopAttrib();
    glEnable(GL_DEPTH_TEST);
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

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE))
        glfwSetWindowShouldClose(window, true);
    #ifdef _DEBUG
    if (glfwGetKey(window, GLFW_KEY_D) & !_dDown) {
        for (int i = 0; i < 3; i++){
            std::cout << "target " << i <<" hit, respawning at x:" << positions[i].x << " y: "<< positions[i].y << " z: "<< positions[i].z <<"\n" << std::endl;

        }
        _dDown = true;
    } else if (!glfwGetKey(window, GLFW_KEY_D)) {
        _dDown = false;
    }
    #endif
    if (mouseDown
    && !glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1))
        mouseDown = false;
    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) && !mouseDown) {
        mouseDown = true;
        for (glm::vec3 & vec : positions) {
           glm::vec3 lookingAtTargDist = camera.front * length(vec);
            if (lookingAtTargDist.x > vec.x - 0.5f && lookingAtTargDist.x < vec.x + 0.5f // copilot ahhh generated code
            && lookingAtTargDist.y > vec.y - 0.5f && lookingAtTargDist.y < vec.y + 0.5f
            && lookingAtTargDist.z > vec.z - 0.5f && lookingAtTargDist.z < vec.z + 0.5f) {
                glm::vec3 newTarg;
                newTarg.x = std::rand() % 10 - 5; // i like this
                newTarg.y = std::rand() % 5 - 2.5f;
                newTarg.z = -sqrt(100.0f - pow(newTarg.x, 2) - pow(newTarg.y, 2));
                // check if the new target collides with the other targets
                bool collides = false;
                for (glm::vec3 targ : positions) {
                    if (targ == vec) continue;
                    if (targ.x == newTarg.x && targ.y == newTarg.y && targ.z == newTarg.z) {
                        collides = true;
                        break;
                    }
                }
                vec = newTarg;
            } 
        }
    }
}

unsigned int loadCubeMap(std::vector<std::string> faces) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;

    for (unsigned int i = 0; i < faces.size(); i++) {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        } else {
            std::cout << "Failed to load texture at: "<< i << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    return textureID;
}

SphereData generateSphereVertices(float radius, unsigned int rings, unsigned int sectors) {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::vector<float> normals;
    const float PI = 3.14159265359f;

    for (unsigned int r = 0; r <= rings; ++r) {
        for (unsigned int s = 0; s <= sectors; ++s) {
            float y = sin(-PI / 2.0f + PI * r / rings);
            float x = cos(2.0f * PI * s / sectors) * sin(PI * r / rings);
            float z = sin(2.0f * PI * s / sectors) * sin(PI * r / rings);

            vertices.push_back(x * radius);
            vertices.push_back(y * radius);
            vertices.push_back(z * radius);

            normals.push_back(x);
            normals.push_back(y);
            normals.push_back(z);
        }
    }

    for (unsigned int r = 0; r < rings; ++r) {
        for (unsigned int s = 0; s < sectors; ++s) {
            unsigned int curRow = r * (sectors + 1);
            unsigned int nextRow = (r + 1) * (sectors + 1);

            indices.push_back(curRow + s);
            indices.push_back(nextRow + s);
            indices.push_back(nextRow + s + 1);

            indices.push_back(curRow + s);
            indices.push_back(nextRow + s + 1);
            indices.push_back(curRow + s + 1);
        }
    }

    return {vertices, indices, normals};
}

// std::vector<float> generateSphereIndexes()