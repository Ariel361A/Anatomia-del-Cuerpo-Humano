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
    {u8"9. Costillas", glm::vec3(0.0f, 0.1f, -10.5f)},
    {u8"10. Metatarso", glm::vec3(0.0f, -0.9f, -10.5f)},
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
        {10, u8"10. Metatarso"},
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

                // Acción al seleccionar: mover cámara a la parte correspondiente
                auto it = posicionesPartes.find(seleccion);
                if (it != posicionesPartes.end()) {
                    camera.Position = it->second;

                    // Que mire al centro del modelo (ajustable según tu escena)
                    glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);
                    camera.Front = glm::normalize(target - camera.Position);
                }
            }
        }
    }

    ImGui::EndChild();
    ImGui::End();
}
