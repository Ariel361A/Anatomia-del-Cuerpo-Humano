#include "OrganosPanel.h"
#include "imgui.h"
#include "Camera.h" 
#include <string>
#include <vector>
#include <cstring>   
#include <algorithm>   // PRRT
#include <map>

// Referencia global a la c�mara (debe estar declarada en main.cpp)
extern Camera camera;



// Mapa de posiciones para cada parte del esqueleto
std::map<std::string, glm::vec3> posicionesPartes2 = {
    {u8"1. Corazon", glm::vec3(0.0f, 0.4f, -10.5f)},
    {u8"2. Pulmon", glm::vec3(0.0f, 0.2f, -10.5f)},
    {u8"3. Traquea", glm::vec3(0.2f, 0.1f, -10.5f)},
    {u8"4. Vasos sanguineos", glm::vec3(0.3f, -0.1f, -10.5f)},
    {u8"5. Apendice", glm::vec3(0.0f, -0.6f, -10.5f)},
    {u8"6. Intestino grueso", glm::vec3(0.0f, 0.0f, -10.5f)},
    {u8"7. Intestino delgado", glm::vec3(0.0f, -0.4f, -10.5f)},
    {u8"8. Estomago", glm::vec3(0.0f, -0.7f, -10.5f)},
};


bool panel2Visible = true;

void mostrarPanelorgano() {
    if (!panel2Visible) return;
    static char filtro[64] = "";
    static std::string seleccion = "";

    static std::vector<std::pair<int, std::string>> partes = {
        {1, u8"1. Corazon"},
        {2, u8"2. Pulmon"},
        {3, u8"3. Traquea"},
        {4, u8"4. Vasos sanguineos"},
        {5, u8"5. Apendice"},
        {6, u8"6. Intestino grueso"},
        {7, u8"7. Intestino delgado"},
        {8, u8"8. Estomago"},

    };

    ImGuiIO& io = ImGui::GetIO();

    ImVec2 panelSize = ImVec2(300, io.DisplaySize.y - 40.0f);
    ImVec2 panelPos = ImVec2(io.DisplaySize.x - panelSize.x, 20.0f);

    ImGui::SetNextWindowPos(panelPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(panelSize, ImGuiCond_Always);

    ImGui::Begin("Partes de los Organos");

    ImGui::InputTextWithHint("##Buscar", "Buscar parte...", filtro, IM_ARRAYSIZE(filtro));
    ImGui::Separator();
    ImGui::BeginChild("ListaPartes", ImVec2(0, 0), true);

    std::string filtroStr = filtro;
    std::transform(filtroStr.begin(), filtroStr.end(), filtroStr.begin(), ::tolower);

    for (const auto& parte : partes) {
        std::string parteNombre = parte.second;
        std::string parteNombreLower = parteNombre;
        std::transform(parteNombreLower.begin(), parteNombreLower.end(), parteNombreLower.begin(), ::tolower);

        if (filtroStr.empty() || parteNombreLower.find(filtroStr) != std::string::npos) {
            bool isSelected = (parteNombre == seleccion);
            if (ImGui::Selectable(parte.second.c_str(), isSelected)) {
                seleccion = parteNombre;

                // Movimiento personalizado de la c�mara
                if (seleccion == u8"1. Corazon") {
                    camera.Position = glm::vec3(0.14f, 0.37f, -5.82f);
                    glm::vec3 foco = glm::vec3(0.0f, 1.75f, 0.0f);
                    camera.Front = glm::normalize(camera.Position - foco);
                }
                else if (seleccion == u8"2. Pulmon") {
                    camera.Position = glm::vec3(0.50f, 0.76f, -5.43f);
                    glm::vec3 foco = glm::vec3(0.0f, 1.75f, 0.0f);
                    camera.Front = glm::normalize(camera.Position - foco);
                }
                else if (seleccion == u8"3. Traquea") {
                    camera.Position = glm::vec3(0.50f, 1.14f, -7.30f);
                    glm::vec3 foco = glm::vec3(0.0f, 1.75f, 0.0f);
                    camera.Front = glm::normalize(camera.Position - foco);
                }
                else if (seleccion == u8"4. Vasos sanguineos") {
                    camera.Position = glm::vec3(-0.65f, 0.30f, -4.0f);
                    glm::vec3 foco = glm::vec3(0.0f, 1.75f, 0.0f);
                    camera.Front = glm::normalize(camera.Position - foco);
                }
                else if (seleccion == u8"5. Apendice") {
                    camera.Position = glm::vec3(-0.55f, -3.33f, -7.20f);
                    
                    glm::vec3 foco = glm::vec3(0.0f, 1.75f, 0.0f);
                    camera.Front = glm::normalize(camera.Position - foco);
                }
                else if (seleccion == u8"6. Intestino grueso") {
                    camera.Position = glm::vec3(-0.01f, -1.89f, -6.00f);
                    glm::vec3 foco = glm::vec3(0.0f, 1.75f, 0.0f);
                    camera.Front = glm::normalize(camera.Position - foco);
                }
                else if (seleccion == u8"7. Intestino delgado") {
                    camera.Position = glm::vec3(-0.01f, -1.89f, -6.00f);
                    glm::vec3 foco = glm::vec3(0.0f, 1.75f, 0.0f);
                    camera.Front = glm::normalize(camera.Position - foco);
                }
                else if (seleccion == u8"8. Estomago") {
                    camera.Position = glm::vec3(0.95f, -0.89f, -7.00f);
                    glm::vec3 foco = glm::vec3(0.0f, 1.75f, 0.0f);
                    camera.Front = glm::normalize(camera.Position - foco);
                }
                
                else {
                    // Fallback a la posici�n del mapa si no est� personalizada
                    auto it = posicionesPartes2.find(seleccion);
                    if (it != posicionesPartes2.end()) {
                        camera.Position = it->second;
                        glm::vec3 foco = glm::vec3(0.0f, 0.0f, 0.0f);
                        camera.Front = glm::normalize(camera.Position - foco);
                    }
                }
            }
        }
    }

    // Mostrar etiqueta solo si la selecci�n actual es "Mand�bula"
    if (seleccion == u8"1. Corazon") {
        ImVec2 etiquetaPos2 = ImVec2(898, 122);  // Ajusta seg�n tu layout
        ImGui::SetNextWindowPos(etiquetaPos2, ImGuiCond_Always);
        ImGui::SetNextWindowBgAlpha(0.85f);

        ImGui::Begin("EtiquetaCorazon", nullptr,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoSavedSettings);

        ImGui::Text("Corazon");
        ImGui::End();

        // Coordenadas fijas de la mand�bula en pantalla (seg�n imagen anterior)
        ImVec2 puntoCorazon2D = ImVec2(653, 434);

        // Dibuja l�nea
        ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        drawList->AddLine(puntoCorazon2D, etiquetaPos2, IM_COL32(0, 0, 0, 255), 2.5f);
    }
    else if (seleccion == u8"2. Pulmon") {
        ImVec2 etiquetaPos = ImVec2(861, 225);  // Ajusta seg�n tu layout
        ImGui::SetNextWindowPos(etiquetaPos, ImGuiCond_Always);
        ImGui::SetNextWindowBgAlpha(0.85f);
        ImGui::Begin("EtiquetaPulmon", nullptr,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoSavedSettings);
        ImGui::Text("Pulmon");
        ImGui::End();
        // Coordenadas fijas de la mand�bula en pantalla (seg�n imagen anterior)
        ImVec2 puntoCorazon2D = ImVec2(612, 495);

        // Dibuja l�nea
        ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        drawList->AddLine(puntoCorazon2D, etiquetaPos, IM_COL32(0, 0, 0, 255), 2.5f);
    }
    else if (seleccion == u8"3. Traquea") {
        ImVec2 etiquetaPos2 = ImVec2(1000, 350);  // Ajusta seg�n tu layout
        ImGui::SetNextWindowPos(etiquetaPos2, ImGuiCond_Always);
        ImGui::SetNextWindowBgAlpha(0.85f);
        ImGui::Begin("EtiquetaTraquea", nullptr,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoSavedSettings);
        ImGui::Text("Traquea");
        ImGui::End();

        // Coordenadas fijas de la mand�bula en pantalla (seg�n imagen anterior)
        ImVec2 puntoCorazon2D = ImVec2(365, 620);

        // Dibuja l�nea
        ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        drawList->AddLine(puntoCorazon2D, etiquetaPos2, IM_COL32(0, 0, 0, 255), 2.5f);
    }
    else if (seleccion == u8"4. Vasos sanguineos") {
        ImVec2 etiquetaPos2 = ImVec2(285, 347);  // Ajusta seg�n tu layout
        ImGui::SetNextWindowPos(etiquetaPos2, ImGuiCond_Always);
        ImGui::SetNextWindowBgAlpha(0.85f);
        ImGui::Begin("EtiquetaVasos sanguineos", nullptr,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoSavedSettings);
        ImGui::Text("Vasos sanguineos");
        ImGui::End();

        // Coordenadas fijas de la mand�bula en pantalla (seg�n imagen anterior)
        ImVec2 puntoCorazon2D = ImVec2(715, 295);

        // Dibuja l�nea
        ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        drawList->AddLine(puntoCorazon2D, etiquetaPos2, IM_COL32(0, 0, 0, 255), 2.5f);
    }
    else if (seleccion == u8"5. Apendice") {
        ImVec2 etiquetaPos2 = ImVec2(380, 332);  // Ajusta seg�n tu layout
        ImGui::SetNextWindowPos(etiquetaPos2, ImGuiCond_Always);
        ImGui::SetNextWindowBgAlpha(0.85f);
        ImGui::Begin("EtiquetaApendice", nullptr,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoSavedSettings);
        ImGui::Text("Apendice");
        ImGui::End();

        // Coordenadas fijas de la mand�bula en pantalla (seg�n imagen anterior)
        ImVec2 puntoCorazon2D = ImVec2(824, 89);

        // Dibuja l�nea
        ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        drawList->AddLine(puntoCorazon2D, etiquetaPos2, IM_COL32(0, 0, 0, 255), 2.5f);
    }
    else if (seleccion == u8"6. Intestino grueso") {
        ImVec2 etiquetaPos2 = ImVec2(249, 542);  // Ajusta seg�n tu layout
        ImGui::SetNextWindowPos(etiquetaPos2, ImGuiCond_Always);
        ImGui::SetNextWindowBgAlpha(0.85f);
        ImGui::Begin("EtiquetaIntestinogrueso", nullptr,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoSavedSettings);
        ImGui::Text("Intestino Grueso");
        ImGui::End();

        // Coordenadas fijas de la mand�bula en pantalla (seg�n imagen anterior)
        ImVec2 puntoCorazon2D = ImVec2(433, 249);

        // Dibuja l�nea
        ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        drawList->AddLine(puntoCorazon2D, etiquetaPos2, IM_COL32(0, 0, 0, 255), 2.5f);
    }
    else if (seleccion == u8"7. Intestino delgado") {
        ImVec2 etiquetaPos2 = ImVec2(634, 619);  // Ajusta seg�n tu layout
        ImGui::SetNextWindowPos(etiquetaPos2, ImGuiCond_Always);
        ImGui::SetNextWindowBgAlpha(0.85f);
        ImGui::Begin("EtiquetaIntestinodelgado", nullptr,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoSavedSettings);
        ImGui::Text("Intestino Delgado");
        ImGui::End();

        // Coordenadas fijas de la mand�bula en pantalla (seg�n imagen anterior)
        ImVec2 puntoCorazon2D = ImVec2(586, 285);

        // Dibuja l�nea
        ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        drawList->AddLine(puntoCorazon2D, etiquetaPos2, IM_COL32(0, 0, 0, 255), 2.5f);
    }
    else if (seleccion == u8"8. Estomago") {
        ImVec2 etiquetaPos2 = ImVec2(800, 400);  // Ajusta seg�n tu layout
        ImGui::SetNextWindowPos(etiquetaPos2, ImGuiCond_Always);
        ImGui::SetNextWindowBgAlpha(0.85f);
        ImGui::Begin("EtiquetaEstomago", nullptr,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoSavedSettings);
        ImGui::Text("Estomago");
        ImGui::End();

        // Coordenadas fijas de la mand�bula en pantalla (seg�n imagen anterior)
        ImVec2 puntoCorazon2D = ImVec2(286, 387);

        // Dibuja l�nea
        ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        drawList->AddLine(puntoCorazon2D, etiquetaPos2, IM_COL32(0, 0, 0, 255), 2.5f);
    }
   


    ImGui::EndChild();
    ImGui::End();
}
