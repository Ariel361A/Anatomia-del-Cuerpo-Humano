#include "EsqueletoPanel.h"
#include "imgui.h"
#include "Camera.h"  // Asegúrate que este include esté presente
#include <string>
#include <vector>
#include <cstring>     // Para strlen y strstr
#include <algorithm>   // Para std::transform
#include <map>

// Referencia global a la cámara (debe estar declarada en main.cpp)
extern Camera camera;



// Mapa de posiciones para cada parte del esqueleto
std::map<std::string, glm::vec3> posicionesPartes = {
    {u8"1. Mandibula", glm::vec3(0.0f, 0.4f, -10.5f)},
    {u8"2. Clavicula", glm::vec3(0.0f, 0.2f, -10.5f)},
    {u8"3. Humero", glm::vec3(0.2f, 0.1f, -10.5f)},
    {u8"4. Radio", glm::vec3(0.3f, -0.1f, -10.5f)},
    {u8"5. Rotula", glm::vec3(0.0f, -0.6f, -10.5f)},
    {u8"6. Columna vertebral", glm::vec3(0.0f, 0.0f, -10.5f)},
    {u8"7. Femur", glm::vec3(0.0f, -0.4f, -10.5f)},
    {u8"8. Tibia", glm::vec3(0.0f, -0.7f, -10.5f)},
    {u8"9. Costillas", glm::vec3(0.0f, 0.1f, -10.5f)}
};



void mostrarPanelEsqueleto() {
    static char filtro[64] = "";
    static std::string seleccion = "";

    static std::vector<std::pair<int, std::string>> partes = {
        {1, u8"1. Mandibula"},
        {2, u8"2. Clavicula"},
        {3, u8"3. Humero"},
        {4, u8"4. Radio"},
        {5, u8"5. Rotula"},
        {6, u8"6. Columna vertebral"},
        {7, u8"7. Femur"},
        {8, u8"8. Tibia"},
        {9, u8"9. Costillas"},
        
    };

    ImGuiIO& io = ImGui::GetIO();

    ImVec2 panelSize = ImVec2(300, io.DisplaySize.y - 40.0f);
    ImVec2 panelPos = ImVec2(io.DisplaySize.x - panelSize.x, 20.0f);

    ImGui::SetNextWindowPos(panelPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(panelSize, ImGuiCond_Always);

    ImGui::Begin("Partes del esqueleto humano");

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

                // Movimiento personalizado de la cámara
                if (seleccion == u8"1. Mandibula") {
                    camera.Position = glm::vec3(0.24f, 1.75f, -7.00f);
                    glm::vec3 foco = glm::vec3(0.0f, 1.75f, 0.0f);
                    camera.Front = glm::normalize( camera.Position - foco);
                }
                else if (seleccion == u8"2. Clavicula") {
                    camera.Position = glm::vec3(-0.50f, 1.60f, -7.20f);
                    glm::vec3 foco = glm::vec3(0.0f, 1.75f, 0.0f);
                    camera.Front = glm::normalize(camera.Position - foco);
                }
                else if (seleccion == u8"3. Humero") {
                    camera.Position = glm::vec3(1.0f, 1.14f, -7.30f);
                    glm::vec3 foco = glm::vec3(0.0f, 1.75f, 0.0f);
                    camera.Front = glm::normalize(camera.Position - foco);
                }
                else if (seleccion == u8"4. Radio") {
                    camera.Position = glm::vec3(-0.65f, 0.30f, -8.0f);
                    glm::vec3 foco = glm::vec3(0.0f, 1.75f, 0.0f);
                    camera.Front = glm::normalize(camera.Position - foco);
                }
                else if (seleccion == u8"5. Rotula") {
                    camera.Position = glm::vec3(0.95f, -0.89f, -7.00f);
                    glm::vec3 foco = glm::vec3(0.0f, 1.75f, 0.0f);
                    camera.Front = glm::normalize(camera.Position - foco);
                }
                else if (seleccion == u8"6. Columna vertebral") {
                    camera.Position = glm::vec3(0.62f, 0.55f, -7.65f);
                    glm::vec3 foco = glm::vec3(0.0f, 1.75f, 0.0f);
                    camera.Front = glm::normalize(camera.Position - foco);
                }
                else if (seleccion == u8"7. Femur") {
                    camera.Position = glm::vec3(-0.52f, -0.47f, -7.33f);
                    glm::vec3 foco = glm::vec3(0.0f, 1.75f, 0.0f);
                    camera.Front = glm::normalize(camera.Position - foco);
                }
                else if (seleccion == u8"8. Tibia") {
                    camera.Position = glm::vec3(-0.67f, -1.28f, -7.62f);
                    glm::vec3 foco = glm::vec3(0.0f, 1.75f, 0.0f);
                    camera.Front = glm::normalize(camera.Position - foco);
                }
                else if (seleccion == u8"9. Costillas") {
                    camera.Position = glm::vec3(-0.55f, 1.36f, -7.46f);
                    glm::vec3 foco = glm::vec3(0.0f, 1.75f, 0.0f);
                    camera.Front = glm::normalize(camera.Position - foco);
                }
                else {
                    // Fallback a la posición del mapa si no está personalizada
                    auto it = posicionesPartes.find(seleccion);
                    if (it != posicionesPartes.end()) {
                        camera.Position = it->second;
                        glm::vec3 foco = glm::vec3(0.0f, 0.0f, 0.0f);
                        camera.Front = glm::normalize(camera.Position - foco);
                    }
                }
            }
        }
    }

    // Mostrar etiqueta solo si la selección actual es "Mandíbula"
    if (seleccion == u8"1. Mandibula") {
        ImVec2 etiquetaPos = ImVec2(1050, 400);  // Ajusta según tu layout
        ImGui::SetNextWindowPos(etiquetaPos, ImGuiCond_Always);
        ImGui::SetNextWindowBgAlpha(0.85f);

        ImGui::Begin("EtiquetaMandibula", nullptr,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoSavedSettings);

        ImGui::Text("Mandíbula");
        ImGui::End();

        // Coordenadas fijas de la mandíbula en pantalla (según imagen anterior)
        ImVec2 puntoMandibula2D = ImVec2(803, 464);

        // Dibuja línea
        ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        drawList->AddLine(puntoMandibula2D, etiquetaPos, IM_COL32(0, 0, 0, 255), 2.5f);
    }
    else if (seleccion == u8"2. Clavicula") {
        ImVec2 etiquetaPos = ImVec2(700, 300);  // Ajusta según tu layout
        ImGui::SetNextWindowPos(etiquetaPos, ImGuiCond_Always);
        ImGui::SetNextWindowBgAlpha(0.85f);
        ImGui::Begin("EtiquetaClavicula", nullptr,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoSavedSettings);
        ImGui::Text("Clavícula");
        ImGui::End();
        // Coordenadas fijas de la mandíbula en pantalla (según imagen anterior)
        ImVec2 puntoMandibula2D = ImVec2(1080, 570);

        // Dibuja línea
        ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        drawList->AddLine(puntoMandibula2D, etiquetaPos, IM_COL32(0, 0, 0, 255), 2.5f);
	}
    else if (seleccion == u8"3. Humero") {
        ImVec2 etiquetaPos = ImVec2(1000, 350);  // Ajusta según tu layout
        ImGui::SetNextWindowPos(etiquetaPos, ImGuiCond_Always);
        ImGui::SetNextWindowBgAlpha(0.85f);
        ImGui::Begin("EtiquetaHumero", nullptr,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoSavedSettings);
        ImGui::Text("Húmero");
		ImGui::End();

        // Coordenadas fijas de la mandíbula en pantalla (según imagen anterior)
        ImVec2 puntoMandibula2D = ImVec2(338, 452);

        // Dibuja línea
        ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        drawList->AddLine(puntoMandibula2D, etiquetaPos, IM_COL32(0, 0, 0, 255), 2.5f);
        }
    else if (seleccion == u8"4. Radio") {
        ImVec2 etiquetaPos = ImVec2(800, 300);  // Ajusta según tu layout
        ImGui::SetNextWindowPos(etiquetaPos, ImGuiCond_Always);
        ImGui::SetNextWindowBgAlpha(0.85f);
        ImGui::Begin("EtiquetaRadio", nullptr,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoSavedSettings);
		ImGui::Text("Radio");
        ImGui::End();

        // Coordenadas fijas de la mandíbula en pantalla (según imagen anterior)
        ImVec2 puntoMandibula2D = ImVec2(1088, 315);

        // Dibuja línea
        ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        drawList->AddLine(puntoMandibula2D, etiquetaPos, IM_COL32(0, 0, 0, 255), 2.5f);
    }
    else if (seleccion == u8"5. Rotula") {
        ImVec2 etiquetaPos = ImVec2(800, 260);  // Ajusta según tu layout
        ImGui::SetNextWindowPos(etiquetaPos, ImGuiCond_Always);
        ImGui::SetNextWindowBgAlpha(0.85f);
        ImGui::Begin("EtiquetaRotula", nullptr,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoSavedSettings);
        ImGui::Text("Rótula");
        ImGui::End();

        // Coordenadas fijas de la mandíbula en pantalla (según imagen anterior)
        ImVec2 puntoMandibula2D = ImVec2(225, 268);

        // Dibuja línea
        ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        drawList->AddLine(puntoMandibula2D, etiquetaPos, IM_COL32(0, 0, 0, 255), 2.5f);
    }
    else if (seleccion == u8"6. Columna vertebral") {
        ImVec2 etiquetaPos = ImVec2(900, 300);  // Ajusta según tu layout
        ImGui::SetNextWindowPos(etiquetaPos, ImGuiCond_Always);
        ImGui::SetNextWindowBgAlpha(0.85f);
        ImGui::Begin("EtiquetaColumna", nullptr,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoSavedSettings);
        ImGui::Text("Columna vertebral");
        ImGui::End();

        // Coordenadas fijas de la mandíbula en pantalla (según imagen anterior)
        ImVec2 puntoMandibula2D = ImVec2(280, 310);

        // Dibuja línea
        ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        drawList->AddLine(puntoMandibula2D, etiquetaPos, IM_COL32(0, 0, 0, 255), 2.5f);
    }
    else if (seleccion == u8"7. Femur") {
        ImVec2 etiquetaPos = ImVec2(800, 380);  // Ajusta según tu layout
        ImGui::SetNextWindowPos(etiquetaPos, ImGuiCond_Always);
        ImGui::SetNextWindowBgAlpha(0.85f);
        ImGui::Begin("EtiquetaFemur", nullptr,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoSavedSettings);
        ImGui::Text("Fémur");
        ImGui::End();

        // Coordenadas fijas de la mandíbula en pantalla (según imagen anterior)
        ImVec2 puntoMandibula2D = ImVec2(1150, 281);

        // Dibuja línea
        ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        drawList->AddLine(puntoMandibula2D, etiquetaPos, IM_COL32(0, 0, 0, 255), 2.5f);
    }
    else if (seleccion == u8"8. Tibia") {
        ImVec2 etiquetaPos = ImVec2(800, 400);  // Ajusta según tu layout
        ImGui::SetNextWindowPos(etiquetaPos, ImGuiCond_Always);
        ImGui::SetNextWindowBgAlpha(0.85f);
        ImGui::Begin("EtiquetaTibia", nullptr,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoSavedSettings);
        ImGui::Text("Tibia");
        ImGui::End();

        // Coordenadas fijas de la mandíbula en pantalla (según imagen anterior)
        ImVec2 puntoMandibula2D = ImVec2(1400, 330);

        // Dibuja línea
        ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        drawList->AddLine(puntoMandibula2D, etiquetaPos, IM_COL32(0, 0, 0, 255), 2.5f);
    }
    else if (seleccion == u8"9. Costillas") {
        ImVec2 etiquetaPos = ImVec2(800, 680);  // Ajusta según tu layout
        ImGui::SetNextWindowPos(etiquetaPos, ImGuiCond_Always);
        ImGui::SetNextWindowBgAlpha(0.85f);
        ImGui::Begin("EtiquetaCostillas", nullptr,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoSavedSettings);
        ImGui::Text("Costillas");
        ImGui::End();

        // Coordenadas fijas de la mandíbula en pantalla (según imagen anterior)
        ImVec2 puntoMandibula2D = ImVec2(1150, 650);

        // Dibuja línea
        ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        drawList->AddLine(puntoMandibula2D, etiquetaPos, IM_COL32(0, 0, 0, 255), 2.5f);
    }
   

    ImGui::EndChild();
    ImGui::End();
}
