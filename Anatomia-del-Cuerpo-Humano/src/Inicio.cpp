#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

extern int iniciarAppModelo();  // Llamado al main real

bool iniciar = false;

int main() {
    if (!glfwInit())
        return -1;

    GLFWwindow* window = glfwCreateWindow(800, 600, "Bienvenida", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    fs::path current = fs::current_path();
    std::cout << "Ruta actual de ejecución: \"" << current.string() << "\"" << std::endl;

    std::string fontPath = "Resources/Fonts/times.ttf";
    if (!fs::exists(fontPath)) {
        std::cerr << "ERROR: No se encontró la fuente en la ruta: " << fontPath << std::endl;
        return -1;
    }

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    float last_display_h = -1.0f;
    ImFont* fuenteGrande = nullptr;
    ImFont* fuenteBoton = nullptr;

    while (!glfwWindowShouldClose(window) && !iniciar) {
        glfwPollEvents();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Regenerar fuentes si cambia la altura de la pantalla
        if (last_display_h != display_h) {
            io.Fonts->Clear();
            float tamanioFuenteTitulo = display_h * 0.06f;
            float tamanioFuenteBoton = display_h * 0.045f;
            fuenteGrande = io.Fonts->AddFontFromFileTTF(fontPath.c_str(), tamanioFuenteTitulo);
            fuenteBoton = io.Fonts->AddFontFromFileTTF(fontPath.c_str(), tamanioFuenteBoton);
            io.Fonts->Build();  // reconstruye la fuente
            ImGui_ImplOpenGL3_DestroyDeviceObjects();
            ImGui_ImplOpenGL3_CreateDeviceObjects();
            last_display_h = display_h;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2((float)display_w, (float)display_h));
        ImGui::Begin("Bienvenida", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);

        const char* textoBienvenida = "Bienvenido a la aplicación de la anatomía del cuerpo humano";

        if (fuenteGrande) ImGui::PushFont(fuenteGrande);
        ImVec2 textoSize = ImGui::CalcTextSize(textoBienvenida);
        float posX = (display_w - textoSize.x) * 0.5f;
        float posY = display_h * 0.2f;

        ImGui::SetCursorPos(ImVec2(posX + 2, posY + 2));  // sombra
        ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.4f, 0.6f), textoBienvenida);
        ImGui::SetCursorPos(ImVec2(posX, posY));          // texto frontal
        ImGui::TextColored(ImVec4(0.0f, 0.4f, 0.9f, 1.0f), textoBienvenida);

        if (fuenteGrande) ImGui::PopFont();

        ImGui::Dummy(ImVec2(0.0f, display_h * 0.1f)); // Espacio entre texto y botón

        if (fuenteBoton) ImGui::PushFont(fuenteBoton);
        float botonAncho = display_w * 0.2f;
        float botonAlto = display_h * 0.1f;
        ImGui::SetCursorPosX((display_w - botonAncho) * 0.5f);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.4f, 0.9f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.5f, 1.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

        if (ImGui::Button("Iniciar", ImVec2(botonAncho, botonAlto))) {
            iniciar = true;
        }

        ImGui::PopStyleColor(3);
        if (fuenteBoton) ImGui::PopFont();

        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    // Limpieza completa
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    if (!iniciar) return 0;
    return iniciarAppModelo();
}
