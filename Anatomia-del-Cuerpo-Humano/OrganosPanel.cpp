#include "OrganosPanel.h"
#include "imgui.h"
#include "Camera.h" 
#include <string>
#include <vector>
#include <cstring>    
#include <algorithm>   
#include <map>

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

                if (seleccion == u8"1. Corazon") {
                    camera.Position = glm::vec3(0.25f, 0.34f, -5.01f);
                    glm::vec3 foco = glm::vec3(0.0f, 1.75f, 0.0f);
                    camera.Front = glm::normalize(camera.Position - foco);
                }
                else if (seleccion == u8"2. Pulmon") {
                    camera.Position = glm::vec3(0.61f, 0.14f, -4.95f);
                    glm::vec3 foco = glm::vec3(0.0f, 1.75f, 0.0f);
                    camera.Front = glm::normalize(camera.Position - foco);
                }
                else if (seleccion == u8"3. Traquea") {
                    camera.Position = glm::vec3(0.30f, 1.28f, -5.59f);
                    glm::vec3 foco = glm::vec3(0.0f, 1.75f, 0.0f);
                    camera.Front = glm::normalize(camera.Position - foco);
                }
                else if (seleccion == u8"4. Vasos sanguineos") {
                    camera.Position = glm::vec3(0.03f, -0.01f, -3.86f);
                    glm::vec3 foco = glm::vec3(0.0f, 1.75f, 0.0f);
                    camera.Front = glm::normalize(camera.Position - foco);
                }
                else if (seleccion == u8"5. Apendice") {
                    camera.Position = glm::vec3(-0.85, -3.10f, -6.90);
                    glm::vec3 foco = glm::vec3(0.0f, 1.75f, 0.0f);
                    camera.Front = glm::normalize(camera.Position - foco);
                }
                else if (seleccion == u8"6. Intestino grueso") {
                    camera.Position = glm::vec3(0.25f, -1.34f, -5.01f);
                    glm::vec3 foco = glm::vec3(0.0f, 1.75f, 0.0f);
                    camera.Front = glm::normalize(camera.Position - foco);
                }
                else if (seleccion == u8"7. Intestino delgado") {
                    camera.Position = glm::vec3(0.25f, -1.34f, -5.01f);
                    glm::vec3 foco = glm::vec3(0.0f, 1.75f, 0.0f);
                    camera.Front = glm::normalize(camera.Position - foco);
                }
                else if (seleccion == u8"8. Estomago") {
                    camera.Position = glm::vec3(-0.10f, -0.19f, -4.86f);
                    glm::vec3 foco = glm::vec3(0.0f, 1.75f, 0.0f);
                    camera.Front = glm::normalize(camera.Position - foco);
                }
                
                else {
                    
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

    // Mostrar etiqueta solo si la selección actual es "Mandíbula"
    if (seleccion == u8"1. Corazon") {
        ImVec2 etiquetaPos2 = ImVec2(892, 127);  // Ajusta según tu layout
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

        // Coordenadas fijas de la mandíbula en pantalla (según imagen anterior)
        ImVec2 puntoCorazon2D = ImVec2(590, 364);

        // Dibuja línea
        ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        drawList->AddLine(puntoCorazon2D, etiquetaPos2, IM_COL32(0, 0, 0, 255), 2.5f);
    }
    else if (seleccion == u8"2. Pulmon") {
        ImVec2 etiquetaPos = ImVec2(924, 191);  // Ajusta según tu layout
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
        // Coordenadas fijas de la mandíbula en pantalla (según imagen anterior)
        ImVec2 puntoCorazon2D = ImVec2(555, 310);

        // Dibuja línea
        ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        drawList->AddLine(puntoCorazon2D, etiquetaPos, IM_COL32(0, 0, 0, 255), 2.5f);
    }
    else if (seleccion == u8"3. Traquea") {
        ImVec2 etiquetaPos2 = ImVec2(854, 367);  // Ajusta según tu layout
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

        // Coordenadas fijas de la mandíbula en pantalla (según imagen anterior)
        ImVec2 puntoCorazon2D = ImVec2(530, 502);

        // Dibuja línea
        ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        drawList->AddLine(puntoCorazon2D, etiquetaPos2, IM_COL32(0, 0, 0, 255), 2.5f);
    }
    else if (seleccion == u8"4. Vasos sanguineos") {
        ImVec2 etiquetaPos2 = ImVec2(119, 400);  // Ajusta según tu layout
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

        // Coordenadas fijas de la mandíbula en pantalla (según imagen anterior)
        ImVec2 puntoCorazon2D = ImVec2(374, 188);

        // Dibuja línea
        ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        drawList->AddLine(puntoCorazon2D, etiquetaPos2, IM_COL32(0, 0, 0, 255), 2.5f);
    }
    else if (seleccion == u8"5. Apendice") {
        ImVec2 etiquetaPos2 = ImVec2(534, 331);  // Ajusta según tu layout
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

        // Coordenadas fijas de la mandíbula en pantalla (según imagen anterior)
        ImVec2 puntoCorazon2D = ImVec2(1008, 140);

        // Dibuja línea
        ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        drawList->AddLine(puntoCorazon2D, etiquetaPos2, IM_COL32(0, 0, 0, 255), 2.5f);
    }
    else if (seleccion == u8"6. Intestino grueso") {
        ImVec2 etiquetaPos2 = ImVec2(894, 544);  // Ajusta según tu layout
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

        // Coordenadas fijas de la mandíbula en pantalla (según imagen anterior)
        ImVec2 puntoCorazon2D = ImVec2(754, 277);

        // Dibuja línea
        ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        drawList->AddLine(puntoCorazon2D, etiquetaPos2, IM_COL32(0, 0, 0, 255), 2.5f);
    }
    else if (seleccion == u8"7. Intestino delgado") {
        ImVec2 etiquetaPos2 = ImVec2(239, 443);  // Ajusta según tu layout
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

        // Coordenadas fijas de la mandíbula en pantalla (según imagen anterior)
        ImVec2 puntoCorazon2D = ImVec2(588, 268);

        // Dibuja línea
        ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        drawList->AddLine(puntoCorazon2D, etiquetaPos2, IM_COL32(0, 0, 0, 255), 2.5f);
    }
    else if (seleccion == u8"8. Estomago") {
        ImVec2 etiquetaPos2 = ImVec2(236, 347);  // Ajusta según tu layout
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

        // Coordenadas fijas de la mandíbula en pantalla (según imagen anterior)
        ImVec2 puntoCorazon2D = ImVec2(760, 390);

        // Dibuja línea
        ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        drawList->AddLine(puntoCorazon2D, etiquetaPos2, IM_COL32(0, 0, 0, 255), 2.5f);
    }
   


    ImGui::EndChild();
    ImGui::End();
}
