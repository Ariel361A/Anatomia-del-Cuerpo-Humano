// main.cpp

// --- Librer�as est�ndar ---
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

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);


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
    Model ourModel(FileSystem::getPath("Resources/objects/insertion/scene.gltf"));

    // insertion: https://sketchfab.com/3d-models/muscular-insertions-5f2a69f2b0cc47c2ab9853cabdea881f
    // male full body https://sketchfab.com/3d-models/male-full-body-ecorche-ab11ebff89224f03bd75efede1164cf6

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
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetMouseButtonCallback(window, mouse_button_callback);

        // render
        // ------
        glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // don't forget to enable shader before setting uniforms
        shaderModel.use();

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 500.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shaderModel.setMat4("projection", projection);
        shaderModel.setMat4("view", view);

        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f,-10.0f)); // translate it down so it's at the center of the scene
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f));	// it's a bit too big for our scene, so scale it down
        shaderModel.setMat4("model", model);

        // Set light properties
        shaderModel.setVec3("dirLight.direction", lightDir);
        shaderModel.setVec3("dirLight.ambient", lightAmbient);
        shaderModel.setVec3("dirLight.diffuse", lightDiffuse);
        shaderModel.setVec3("dirLight.specular", lightSpecular);
        ourModel.Draw(shaderModel);




        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);



        // Esperar un tiempo para escuchar el sonido
        //std::this_thread::sleep_for(std::chrono::seconds(1));

    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
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

    camera.ProcessMouseMovement(static_cast<float>(xpos), static_cast<float>(ypos));
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}


// Mouse button callback
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        camera.SetLeftMousePressed(action == GLFW_PRESS);
    }
}