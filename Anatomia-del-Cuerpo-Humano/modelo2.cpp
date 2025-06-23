#include <iostream>
#include <filesystem>

// --- GLAD: carga de punteros a funciones OpenGL ---
#include <glad/glad.h>

// --- GLFW: ventana y entrada ---
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <FileSystem.h>
#include <ShaderModel.h>
#include <Shader.h>
#include <Camera.h>
#include <Model.h>

#include "AudioManager.h"
#include <thread>
#include <chrono>

// ✅ Declaraciones externas para evitar duplicación
extern Camera camera;
extern float lastX, lastY;
extern float deltaTime, lastFrame;
extern bool firstMouse;

// ✅ Declaración de callbacks ya definidos en otro archivo
extern void framebuffer_size_callback(GLFWwindow* window, int width, int height);
extern void mouse_callback(GLFWwindow* window, double xpos, double ypos);
extern void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
extern void processInput(GLFWwindow* window);
extern void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

// ✅ Resolución de pantalla
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int iniciarAppModelo2()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Sistema Digestivo", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    stbi_set_flip_vertically_on_load(true);
    glEnable(GL_DEPTH_TEST);

    ShaderModel shaderModel("src/LoadModel.vs", "src/LoadModel.fs");

    // ✅ Cambia aquí el modelo a cargar:
    Model ourModel(FileSystem::getPath("Resources/objects/sistemas_circulatorio_respiratorio_y_digestivo/scene.gltf"));

    glm::vec3 lightDir(-0.2f, -1.0f, -0.3f);
    glm::vec3 lightAmbient(0.25f, 0.25f, 0.25f);
    glm::vec3 lightDiffuse(0.7f, 0.7f, 0.7f);
    glm::vec3 lightSpecular(1.0f, 1.0f, 1.0f);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderModel.use();

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
            (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 500.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shaderModel.setMat4("projection", projection);
        shaderModel.setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -10.0f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        shaderModel.setMat4("model", model);

        shaderModel.setVec3("dirLight.direction", lightDir);
        shaderModel.setVec3("dirLight.ambient", lightAmbient);
        shaderModel.setVec3("dirLight.diffuse", lightDiffuse);
        shaderModel.setVec3("dirLight.specular", lightSpecular);

        ourModel.Draw(shaderModel);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
