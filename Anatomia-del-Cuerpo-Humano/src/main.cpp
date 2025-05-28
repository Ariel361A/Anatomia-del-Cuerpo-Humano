// main.cpp

// --- Librerías estándar ---
#include <iostream>
#include <filesystem>

// --- GLAD: carga de punteros a funciones OpenGL ---
#include <glad/glad.h>

// --- GLFW: ventana y entrada ---
#include <GLFW/glfw3.h>

// --- GLM: matemáticas (vectores, matrices) ---
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// --- nlohmann::json: parsing JSON (por ej. para tu Model) ---
#include <JSON/json.hpp>
using json = nlohmann::json;

// --- Assimp: importación de modelos 3D ---
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// --- Tus clases propias ---
#include "shaderClass.h"  // clase Shader
#include "Camara.h"       // clase Camera (o Camara.h si usas castellano)
#include "Model.h"        // clase Model

// --- Prototipos de funciones ---
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// --- Parámetros de la ventana ---
static constexpr unsigned int SCR_WIDTH = 800;
static constexpr unsigned int SCR_HEIGHT = 800;

int main()
{
    // 1) Inicializar GLFW
    if (!glfwInit()) {
        std::cerr << "Error: Falló glfwInit()\n";
        return -1;
    }
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
     //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // macOS

    // 2) Crear ventana
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT,
        "Render GLTF con Assimp",
        nullptr, nullptr);
    if (!window) {
        std::cerr << "Error: No se pudo crear la ventana GLFW\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 3) Inicializar GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Error: Falló gladLoadGLLoader()\n";
        return -1;
    }

    // 4) Configurar viewport y depth test
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glEnable(GL_DEPTH_TEST);

    // 5) Cargar y compilar shaders
    Shader shader("default.vert", "default.frag");
    shader.Activate();
    // Configurar luz (puedes modificar estos valores)
    glUniform4f(glGetUniformLocation(shader.ID, "lightColor"),
        1.0f, 1.0f, 1.0f, 1.0f);
    glUniform3f(glGetUniformLocation(shader.ID, "lightPos"),
        0.5f, 0.5f, 0.5f);

    // 6) Inicializar cámara
    Camera camera(SCR_WIDTH, SCR_HEIGHT,
        glm::vec3(0.0f, 5.0f, 10.0f));

    // 7) Definir ruta al modelo GLTF (aquí “Mesa”)
    std::filesystem::path scenePath =
        std::filesystem::current_path() /
        "Resources" / "models" / "Mesa" / "scene.gltf";

    // 8) Verificar con Assimp que el archivo existe y es válido
    {
        Assimp::Importer testImporter;
        const aiScene* testScene = testImporter.ReadFile(
            scenePath.string(),
            aiProcess_Triangulate | aiProcess_FlipUVs
        );
        if (!testScene) {
            std::cerr << "Assimp Error: "
                << testImporter.GetErrorString() << "\n";
            return -1;
        }
        std::cout << "Assimp cargó "
            << testScene->mNumMeshes
            << " malla(s)\n";
    }

    // 9) Crear instancia de tu Model
    Model mesaModel(scenePath.string());

    // 10) Bucle principal de render
    while (!glfwWindowShouldClose(window)) {
        // 10.1) Comprobar entrada (ESC)
        processInput(window);

        // 10.2) Actualizar cámara (WASD, ratón, …)
        camera.Inputs(window);
        camera.updateMatrix(45.0f, 0.1f, 100.0f);

        // 10.3) Limpiar buffers
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 10.4) Dibujar modelo
        mesaModel.Draw(shader, camera);

        // 10.5) Swap de buffers y poll de eventos
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 11) Liberar recursos
    shader.Delete();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

// Ajusta el viewport al cambiar tamaño de ventana
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// Cierra la ventana al pulsar ESC
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
