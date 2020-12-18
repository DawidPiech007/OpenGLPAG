// dear imgui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan graphics context creation, etc.)

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include <math.h>
//#include "stb_image.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


// About OpenGL function loaders: modern OpenGL doesn't have a standard header file and requires individual function pointers to be loaded manually. 
// Helper libraries are often used for this purpose! Here we are supporting a few common ones: gl3w, glew, glad.
// You may use another loader/header of your choice (glext, glLoadGen, etc.), or chose to manually implement your own.
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>    // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h>    // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>  // Initialize with gladLoadGL()
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif


#include <GLFW/glfw3.h> // Include glfw3.h after our OpenGL definitions

//==================================== [ TUTORIAL START ] ===========================================//
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm\gtx\transform.hpp>

#include <shader_s.h>
//#include "model_s.hpp"
//#include "mesh_s.h"
#include <SceneRoot.hpp>
#include <MyCamera.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

GLfloat deltaTime = 0.0f; // Czas pomi�dzy obecn� i poprzedni� klatk�  
GLfloat lastFrame = 0.0f; // Czas ostatniej ramki

MyCamera* myCamera;


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

//#ifdef __APPLE__ // Przed UI
//    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif

    // Decide GL+GLSL versions
#if __APPLE__
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 4.3 + GLSL 430
    const char* glsl_version = "#version 430";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Piramida", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("res/shaders/basic.vert", "res/shaders/basic.frag");
    Shader houseShader("res/shaders/domki_Vert.vert", "res/shaders/domki_Frag.frag");
    Shader orbitShader("res/shaders/forGeometry.vert", "res/shaders/forGeometry.frag", "res/shaders/orbit.gs");
    Shader sphereShader("res/shaders/forGeometry.vert", "res/shaders/forGeometry.frag", "res/shaders/sphere.gs");

    // Tworzenie grafu sceny
    // ---------------------
    SceneRoot* sceneRoot = new SceneRoot();

    myCamera = new MyCamera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), SCR_WIDTH, SCR_HEIGHT);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Ukrycie kursora myski

    Model* house = new Model("res/models/Domek/kostka.obj");

    unsigned int VAO = house->meshes[0].VAO;
    glBindVertexArray(VAO);
    glBindVertexArray(0);





    // Initialize OpenGL loader
    #if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
        bool err = gl3wInit() != 0;
    #elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
        bool err = glewInit() != GLEW_OK;
    #elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
        bool err = !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    #endif
    if (err)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }

    // Setup Dear ImGui binding
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Setup style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    //bool show_demo_window = true;
    //bool show_another_window = false;
    bool lineVisible = false;
    ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.1f, 1.00f);
    //glm::vec4 texture_color = glm::vec4(0.6f, 0.0f, 0.0f, 1.00f);
    //float yRotation = 30;
    //float xRotation = 20;
    //float zoom = 20;
    int resolution = 4;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {


        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        //if (show_demo_window)
            //ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Setings");                          // Create a window called "Hello, world!" and append into it.

            //ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            //ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("line Visible", &lineVisible);

            if (lineVisible == true)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            else
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
            

            ImGui::SliderInt("resolution", &resolution, 1, 5);
            //ImGui::SliderFloat("zoom", &zoom, 5, 100);                // moje
            //ImGui::SliderFloat("y rotation", &yRotation, -360.0f, 360.0f);            // Edit 1 float using a slider from -360.0f to 360.0f
            //ImGui::SliderFloat("x rotation", &xRotation, -360.0f, 360.0f);            // Edit 1 float using a slider from -360.0f to 360.0f
            //ImGui::ColorEdit3("texture color", (float*)&texture_color); // Edit 3 floats representing a color

            //if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            //    counter++;
            //ImGui::SameLine();
            //ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // input
        // -----
        processInput(window);
        myCamera->InputKey(window, deltaTime);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);

        // render
        // ------
        //glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // niepotrzebne
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

        // get matrix's uniform location and set matrix
        // create transformations
        glm::mat4 view = myCamera->GetView(); 
        glm::mat4 projection = myCamera->GetProjection();
        glm::mat4 model = glm::mat4(1.0f);

        // pass transformation matrices to the shader
        ourShader.use();
        ourShader.setMat4("projection", projection); 
        ourShader.setMat4("view", view);
        ourShader.setMat4("model", model);

        orbitShader.use();
        orbitShader.setMat4("projection", projection);
        orbitShader.setMat4("view", view);
        orbitShader.setMat4("model", model);

        sphereShader.use();
        sphereShader.setMat4("projection", projection);
        sphereShader.setMat4("view", view);
        sphereShader.setMat4("model", model);

        sceneRoot->Update((float)glfwGetTime());
        sceneRoot->Draw(ourShader, orbitShader, sphereShader, resolution);
        
        houseShader.use();
        houseShader.setMat4("projection", projection);
        houseShader.setMat4("view", view);
        houseShader.setMat4("model", model);

        houseShader.setInt("texture_diffuse1", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, house->textures_loaded[0].id);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, house->meshes[0].indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glBindVertexArray(house->meshes[0].VAO);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        //glfwSwapBuffers(window);
        glfwPollEvents();

        // Rendering  // kopiowane z UI
        ImGui::Render();
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwMakeContextCurrent(window); // kopiowane z UI
        glfwSwapBuffers(window);

        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
    }
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

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
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    myCamera->InputMouse(xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    myCamera->InputScroll(yoffset);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}