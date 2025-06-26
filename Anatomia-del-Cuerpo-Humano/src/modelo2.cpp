#include <iostream>
#include <filesystem>

#include <glad/glad.h>
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

// === ImGui ===
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "OrganosPanel.h"


// Declaraciones externas
extern Camera camera;
extern float lastX, lastY;
extern float deltaTime, lastFrame;
extern bool firstMouse;

extern void framebuffer_size_callback(GLFWwindow* window, int width, int height);
extern void mouse_callback(GLFWwindow* window, double xpos, double ypos);
extern void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
extern void processInput(GLFWwindow* window);
extern void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;



float lastX2 = SCR_WIDTH / 2.0f;
float lastY2 = SCR_HEIGHT / 2.0f;
bool firstMouse2 = true;

glm::vec2 currentMousePos2;
glm::vec3 lastRayOrigin2;
glm::vec3 lastRayDirection2;

void mouse_callback_modelo2(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback_modelo2(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback_modelo2(GLFWwindow* window, int button, int action, int mods);

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

    // Callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback_modelo2);
    glfwSetScrollCallback(window, scroll_callback_modelo2);
    glfwSetMouseButtonCallback(window, mouse_button_callback_modelo2);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // === ImGui Setup ===
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    ImGui::StyleColorsDark();
    // ====================

    stbi_set_flip_vertically_on_load(true);
    glEnable(GL_DEPTH_TEST);

    ShaderModel shaderModel("src/LoadModel.vs", "src/LoadModel.fs");

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
        model = glm::scale(model, glm::vec3(0.03f));
        shaderModel.setMat4("model", model);

        shaderModel.setVec3("dirLight.direction", lightDir);
        shaderModel.setVec3("dirLight.ambient", lightAmbient);
        shaderModel.setVec3("dirLight.diffuse", lightDiffuse);
        shaderModel.setVec3("dirLight.specular", lightSpecular);

        ourModel.Draw(shaderModel);

        // === ImGui Frame ===
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        mostrarPanelorgano(); // <-- Llama al panel aquí

        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(300, 120), ImGuiCond_Always);
        ImGui::Begin("Raycast Info", nullptr,
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoSavedSettings);

        ImGui::Text("Mouse Pos (Screen): %.1f, %.1f", currentMousePos2.x, currentMousePos2.y);
        ImGui::Text("Ray Origin: (%.2f, %.2f, %.2f)", lastRayOrigin2.x, lastRayOrigin2.y, lastRayOrigin2.z);
        ImGui::Text("Ray Dir:    (%.2f, %.2f, %.2f)", lastRayDirection2.x, lastRayDirection2.y, lastRayDirection2.z);
        ImGui::End();

        // Puedes agregar tu interfaz aquí (si lo deseas)
        // ImGui::Begin("Modelo cargado"); ImGui::Text("Sistema digestivo"); ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        // ====================

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    


    // === Cleanup ImGui ===
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}
void mouse_callback_modelo2(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    currentMousePos2 = glm::vec2(xpos, ypos);

    if (firstMouse2)
    {
        lastX2 = xpos;
        lastY2 = ypos;
        firstMouse2 = false;
    }

    float xoffset = xpos - lastX2;
    float yoffset = lastY2 - ypos;

    lastX2 = xpos;
    lastY2 = ypos;

    ImGuiIO& io = ImGui::GetIO();
    if (!io.WantCaptureMouse)
    {
        camera.ProcessMouseMovement(static_cast<float>(xpos), static_cast<float>(ypos));
    }

    float ndcX = (2.0f * xpos) / SCR_WIDTH - 1.0f;
    float ndcY = 1.0f - (2.0f * ypos) / SCR_HEIGHT;

    glm::vec4 rayClip = glm::vec4(ndcX, ndcY, -1.0f, 1.0f);
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 500.0f);
    glm::mat4 view = camera.GetViewMatrix();

    glm::vec4 rayEye = glm::inverse(projection) * rayClip;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);

    glm::vec3 rayWorld = glm::vec3(glm::inverse(view) * rayEye);
    rayWorld = glm::normalize(rayWorld);

    lastRayOrigin2 = camera.Position;
    lastRayDirection2 = rayWorld;
}

void scroll_callback_modelo2(GLFWwindow* window, double xoffset, double yoffset)
{
    ImGuiIO& io = ImGui::GetIO();
    if (!io.WantCaptureMouse) {
        camera.ProcessMouseScroll(static_cast<float>(yoffset));
    }
}


void mouse_button_callback_modelo2(GLFWwindow* window, int button, int action, int mods)
{
    ImGuiIO& io = ImGui::GetIO();

    // Activar panel cuando se presiona clic izquierdo en escena
    if (!io.WantCaptureMouse && button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        panel2Visible = true;
        camera.SetLeftMousePressed(true);
    }

    // Desactivar cuando se suelta
    if (!io.WantCaptureMouse && button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        camera.SetLeftMousePressed(false);
    }
}
