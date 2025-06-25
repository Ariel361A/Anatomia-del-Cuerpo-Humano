#include <iostream>
#include <filesystem>

// --- GLAD: carga de punteros a funciones OpenGL ---
#include <glad/glad.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


#include "EsqueletoPanel.h"

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
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "AudioManager.h"
#include <thread>      // for std::this_thread
#include <chrono>      // for std::chrono::seconds



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
// Mouse button callback
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
extern std::map<std::string, glm::vec3> posicionesPartes;
// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// Raycasting variables
glm::vec3 lastRayOrigin;
glm::vec3 lastRayDirection;
glm::vec2 currentMousePos; // en pantalla



// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// -- example for create audio --


//AudioManager flower;
//flower.loadWAV("sunflower", FileSystem::getPath("Resources/sounds/sunflower.wav"));
//flower.setLooping("sunflower", false);
//flower.play("sunflower");
//flower.stop("sunflower");
//AudioManager audio;
//audio.loadWAV("pixelJump", FileSystem::getPath("Resources/sounds/pixel-jump.wav"));
//audio.setLooping("pixelJump", false);
//audio.play("pixelJump");



int iniciarAppModelo()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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

    // Inicializar Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); // ya se usa abajo, no hace falta (void)io

    // Cargar fuente personalizada con soporte UTF-8
    io.Fonts->AddFontFromFileTTF("Resources/Fonts/times.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesDefault());

    // Estilo visual
    ImGui::StyleColorsDark(); // o ImGui::StyleColorsClassic()

    // Inicializar backend ImGui para GLFW y OpenGL
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    ShaderModel shaderModel("src/LoadModel.vs", "src/LoadModel.fs");

    // load models
    // -----------
    Model ourModel(FileSystem::getPath("Resources/objects/Esqueleto/scene.gltf"));

    // Light setup
    glm::vec3 lightDir(-0.2f, -1.0f, -0.3f);
    glm::vec3 lightAmbient(0.25f, 0.25f, 0.25f);
    glm::vec3 lightDiffuse(0.7f, 0.7f, 0.7f);
    glm::vec3 lightSpecular(1.0f, 1.0f, 1.0f);

    // draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // --- Hacer el contexto actual antes de cualquier acción ---
        glfwMakeContextCurrent(window);

        // --- Captura de eventos del sistema (teclado, mouse, etc.) ---
        glfwPollEvents();

        // --- Lógica de tiempo por frame ---
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // --- Entrada del usuario ---
        processInput(window);

        // --- Limpieza de pantalla ---
        glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // --- Inicia los nuevos frames de ImGui (¡orden correcto!) ---
        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        // --- Dibujar el panel lateral (tu panel personalizado) ---
        mostrarPanelEsqueleto();

        // --- Mostrar info del raycast ---
        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(300, 120), ImGuiCond_Always);
        ImGui::Begin("Raycast Info", nullptr,
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoSavedSettings);

        ImGui::Text("Mouse Pos (Screen): %.1f, %.1f", currentMousePos.x, currentMousePos.y);
        ImGui::Text("Ray Origin: (%.2f, %.2f, %.2f)", lastRayOrigin.x, lastRayOrigin.y, lastRayOrigin.z);
        ImGui::Text("Ray Dir:    (%.2f, %.2f, %.2f)", lastRayDirection.x, lastRayDirection.y, lastRayDirection.z);
        ImGui::End();

        // --- Renderizar modelo 3D ---
        shaderModel.use();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 500.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -10.0f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));

        shaderModel.setMat4("projection", projection);
        shaderModel.setMat4("view", view);
        shaderModel.setMat4("model", model);
        shaderModel.setVec3("dirLight.direction", lightDir);
        shaderModel.setVec3("dirLight.ambient", lightAmbient);
        shaderModel.setVec3("dirLight.diffuse", lightDiffuse);
        shaderModel.setVec3("dirLight.specular", lightSpecular);

        ourModel.Draw(shaderModel);

        // --- Renderizar ImGui encima (sin profundidad) ---
        glDisable(GL_DEPTH_TEST);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glEnable(GL_DEPTH_TEST);

        // --- Mostrar en pantalla ---
        glfwSwapBuffers(window);
    }


    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    ImGuiIO& io = ImGui::GetIO();
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (!io.WantCaptureKeyboard)
    {

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            camera.ProcessKeyboard(FORWARD, deltaTime);
            panelVisible = false;


            //// Esperar un tiempo para escuchar el sonido
            //std::this_thread::sleep_for(std::chrono::seconds(1));

        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            camera.ProcessKeyboard(BACKWARD, deltaTime);
            panelVisible = false;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            camera.ProcessKeyboard(LEFT, deltaTime);
            panelVisible = false;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
            camera.ProcessKeyboard(RIGHT, deltaTime);
        panelVisible = false;
    }
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
            camera.Position = glm::vec3(0.24f, 1.75f, -7.00f);
            glm::vec3 foco = glm::vec3(0.0f, 1.75f, 0.0f);
            camera.Front = glm::normalize( camera.Position - foco);
            panelVisible = false;
        }

        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
            camera.Position = glm::vec3(-0.50f, 1.60f, -7.20f);
            glm::vec3 foco = glm::vec3(0.0f, 1.75f, 0.0f);
            camera.Front = glm::normalize(camera.Position - foco);
            panelVisible = false;
        }

        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
            camera.Position = glm::vec3(1.0f, 1.14f, -7.30f);
            glm::vec3 foco = glm::vec3(0.0f, 1.75f, 0.0f);
            camera.Front = glm::normalize(camera.Position - foco);
            panelVisible = false;
        }

        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
            camera.Position = glm::vec3(-0.65f, 0.30f, -8.0f);
            glm::vec3 foco = glm::vec3(0.0f, 1.75f, 0.0f);
            camera.Front = glm::normalize(camera.Position - foco);
            panelVisible = false;
        }

        if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
            camera.Position = glm::vec3(0.95f, -0.89f, -7.00f);
            glm::vec3 foco = glm::vec3(0.0f, 1.75f, 0.0f);
            camera.Front = glm::normalize(camera.Position - foco);
            panelVisible = false;
        }

        if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
            camera.Position = glm::vec3(0.62f, 0.55f, -7.65f);
            glm::vec3 foco = glm::vec3(0.0f, 1.75f, 0.0f);
            camera.Front = glm::normalize(camera.Position - foco);
            panelVisible = false;
        }

        if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) {
            camera.Position = glm::vec3(-0.52f, -0.47f, -7.33f);
            glm::vec3 foco = glm::vec3(0.0f, 1.75f, 0.0f);
            camera.Front = glm::normalize(camera.Position - foco);
            panelVisible = false;
        }

        if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS) {
            camera.Position = glm::vec3(-0.67f, -1.28f, -7.62f);
            glm::vec3 foco = glm::vec3(0.0f, 1.75f, 0.0f);
            camera.Front = glm::normalize(camera.Position - foco);
            panelVisible = false;
        }

        if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) {
            camera.Position = glm::vec3(-0.55f, 1.36f, -7.46f);
            glm::vec3 foco = glm::vec3(0.0f, 1.75f, 0.0f);
            camera.Front = glm::normalize(camera.Position - foco);
        }

    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
   
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    currentMousePos = glm::vec2(xpos, ypos);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;



    ImGuiIO& io = ImGui::GetIO();
    if (!io.WantCaptureMouse) {
        camera.ProcessMouseMovement(static_cast<float>(xpos), static_cast<float>(ypos));
    }


    float ndcX = (2.0f * xpos) / SCR_WIDTH - 1.0f;
    float ndcY = 1.0f - (2.0f * ypos) / SCR_HEIGHT; // invertido para OpenGL

    glm::vec4 rayClip = glm::vec4(ndcX, ndcY, -1.0f, 1.0f);

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 500.0f);
    glm::mat4 view = camera.GetViewMatrix();

    glm::vec4 rayEye = glm::inverse(projection) * rayClip;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);

    glm::vec3 rayWorld = glm::vec3(glm::inverse(view) * rayEye);
    rayWorld = glm::normalize(rayWorld);

    lastRayOrigin = camera.Position;
    lastRayDirection = rayWorld;
}



// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    ImGuiIO& io = ImGui::GetIO();
    if (!io.WantCaptureMouse) {
        camera.ProcessMouseScroll(static_cast<float>(yoffset));
    }

}


// Mouse button callback
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    panelVisible = true;
    ImGuiIO& io = ImGui::GetIO();
    if (!io.WantCaptureMouse) {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            camera.SetLeftMousePressed(action == GLFW_PRESS);
        }
    }
}

