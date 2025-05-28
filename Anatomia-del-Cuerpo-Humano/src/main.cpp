#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>
#include "stb_image.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

// Callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
std::string loadShaderSource(const char* filepath);
unsigned int compileShader(const char* vertexPath, const char* fragmentPath);

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

int main() {
    // GLFW init
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Anatomia Humana", NULL, NULL);
    if (!window) { std::cout << "Failed to create GLFW window" << std::endl; glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { std::cout << "Failed to initialize GLAD" << std::endl; return -1; }

    // ImGui setup
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Load shaders
    unsigned int bgShader = compileShader("Shaders/background.vert", "Shaders/background.frag");

    // Quad setup
    float quadVertices[] = {
        -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
         1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
         1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,  1.0f, 1.0f
    };
    unsigned int quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Load background texture
    unsigned int bgTexture;
    glGenTextures(1, &bgTexture);
    glBindTexture(GL_TEXTURE_2D, bgTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    unsigned char* data = stbi_load("Resources/fondo2.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Error al cargar la textura" << std::endl;
    }
    stbi_image_free(data);

    // Variables UI
    static std::string activeSystem = "Ninguno";
    static bool showLabels = true;
    static bool isLoading = false;
    static std::string loadingMessage = "";

    // Variables para pantalla de carga inicial
    static bool showLoadingScreen = true;
    static float loadingProgress = 0.0f;
    static auto loadingStartTime = std::chrono::high_resolution_clock::now();

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw background
        glUseProgram(bgShader);
        glBindVertexArray(quadVAO);
        glBindTexture(GL_TEXTURE_2D, bgTexture);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Pantalla de carga inicial
        if (showLoadingScreen) {
            ImVec2 window_size = ImVec2(400, 120);
            ImGui::SetNextWindowSize(window_size, ImGuiCond_Always);
            ImGui::SetNextWindowPos(ImVec2(SCR_WIDTH * 0.5f, SCR_HEIGHT * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
            ImGui::Begin("Cargando recursos...");
            ImGui::Text("Por favor espera mientras se cargan los recursos.");

            auto now = std::chrono::high_resolution_clock::now();
            std::chrono::duration<float> elapsed = now - loadingStartTime;
            loadingProgress = elapsed.count() / 3.0f; 

            if (loadingProgress > 1.0f) {
                loadingProgress = 1.0f;
                showLoadingScreen = false; // Termina pantalla de carga
            }

            ImGui::ProgressBar(loadingProgress, ImVec2(-1.0f, 20));
            ImGui::End();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(window);
            glfwPollEvents();
            continue; // saltar resto del loop hasta terminar
        }

        // UI Panel principal
        ImGui::Begin("Panel de Anatomia del Cuerpo Humano");

        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.3f, 0.7f, 1.0f, 1.0f));
        ImGui::TextWrapped("Proyecto: Anatomia del Cuerpo Humano");
        ImGui::PopStyleColor();
        ImGui::Separator();

        ImGui::TextWrapped("Bienvenido al visor anatomico.\nUsa los botones para explorar cada sistema. Puedes rotar con el mouse y ajustar el zoom.");
        ImGui::Separator();
        ImGui::Text("Sistema seleccionado: %s", activeSystem.c_str());

        if (ImGui::Button("Ver Sistema Oseo")) {
            isLoading = true;
            loadingMessage = "Cargando Sistema Oseo...";
        }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Muestra el modelo del sistema oseo humano.");

        if (ImGui::Button("Ver Sistema Muscular")) {
            isLoading = true;
            loadingMessage = "Cargando Sistema Muscular...";
        }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Muestra el modelo del sistema muscular humano.");

        if (ImGui::Button("Ver Sistema Nervioso")) {
            isLoading = true;
            loadingMessage = "Cargando Sistema Nervioso...";
        }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Muestra el modelo del sistema nervioso humano.");

        if (ImGui::Button("Ver Todos los Modelos")) {
            isLoading = true;
            loadingMessage = "Cargando los modelos...";
        }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Muestra los tres modelos anteriores del cuerpo humano en una sola ventana");

        if (isLoading) {
            ImGui::Separator();
            ImGui::Text("%s", loadingMessage.c_str());
            ImGui::End();
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(window);
            glfwPollEvents();

            std::this_thread::sleep_for(std::chrono::milliseconds(1500));

            if (loadingMessage.find("Oseo") != std::string::npos)
                activeSystem = "Sistema Oseo";
            else if (loadingMessage.find("Muscular") != std::string::npos)
                activeSystem = "Sistema Muscular";
            else if (loadingMessage.find("Nervioso") != std::string::npos)
                activeSystem = "Sistema Nervioso";

            isLoading = false;
            continue;
        }

        ImGui::Separator();
        ImGui::Checkbox("Mostrar etiquetas", &showLabels);
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Activa o desactiva las etiquetas descriptivas en el modelo.");

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

// Utility: Load shader source from file
std::string loadShaderSource(const char* filepath) {
    std::ifstream file(filepath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Utility: Compile shaders and link program
unsigned int compileShader(const char* vertexPath, const char* fragmentPath) {
    std::string vertCode = loadShaderSource(vertexPath);
    std::string fragCode = loadShaderSource(fragmentPath);
    const char* vShaderCode = vertCode.c_str();
    const char* fShaderCode = fragCode.c_str();

    unsigned int vertex, fragment, program;
    int success;
    char infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) { glGetShaderInfoLog(vertex, 512, NULL, infoLog); std::cout << "VERTEX SHADER ERROR:\n" << infoLog << std::endl; }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) { glGetShaderInfoLog(fragment, 512, NULL, infoLog); std::cout << "FRAGMENT SHADER ERROR:\n" << infoLog << std::endl; }

    program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) { glGetProgramInfoLog(program, 512, NULL, infoLog); std::cout << "SHADER PROGRAM LINK ERROR:\n" << infoLog << std::endl; }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return program;
}

// Input callback
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// Resize callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
