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
unsigned int loadCubemap(vector<std::string> faces);

void ChangeBuffer(int index, glm::mat4 newModel, bool house);

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

GLfloat deltaTime = 0.0f; // Czas pomiêdzy obecn¹ i poprzedni¹ klatk¹  
GLfloat lastFrame = 0.0f; // Czas ostatniej ramki

MyCamera* myCamera;

unsigned int bufferRoof;
unsigned int buffer;


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
    //Shader ourShader("res/shaders/basic.vert", "res/shaders/basic.frag");
    Shader lightShader("res/shaders/source.vert", "res/shaders/source.frag");
    Shader houseShader("res/shaders/domki_Vert.vert", "res/shaders/domki_Frag.frag");
    Shader singleShader("res/shaders/domki_single_Vert.vert", "res/shaders/domki_Frag.frag");
    Shader singleColorShader("res/shaders/domki_single_Vert.vert", "res/shaders/domki_color_frag.frag");
    Shader skyboxShader("res/shaders/skyBox_vert.vert", "res/shaders/skyBox_frag.frag");
    Shader mirrorShader("res/shaders/lustro_vert.vert", "res/shaders/lustro_frag.frag");
    Shader glassShader("res/shaders/szyba_vert.vert", "res/shaders/szyba_frag.frag");

    //Shader orbitShader("res/shaders/forGeometry.vert", "res/shaders/forGeometry.frag", "res/shaders/orbit.gs");
    //Shader sphereShader("res/shaders/forGeometry.vert", "res/shaders/forGeometry.frag", "res/shaders/sphere.gs");
    const int mapLen = 900;
                     //0 1 2 3 4  5 6 7 8 9 10 1 2 3 4  5 6 7 8 9 20 1 2 3 4  5 6 7 8 9
    int map[mapLen] = {1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, // 0
                       1,0,0,0,0, 0,1,0,0,0, 0,0,0,0,0, 1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1, // 1
                       1,0,0,0,0, 0,1,0,0,0, 0,0,0,0,0, 1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1, // 2
                       1,0,0,0,0, 0,1,0,0,0, 4,0,0,0,0, 2,0,0,4,0, 0,0,4,0,0, 0,4,0,0,1, // 3
                       1,0,0,4,0, 0,1,0,0,0, 0,0,0,0,0, 1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1, // 4
                                                                                         // 
                       1,0,0,0,0, 0,1,0,0,0, 0,0,0,0,0, 1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1, // 5
                       1,0,0,0,0, 0,1,0,0,0, 0,0,0,0,0, 1,1,1,1,1, 1,1,1,1,1, 1,3,1,1,1, // 6
                       1,1,3,1,1, 1,1,0,0,0, 0,0,0,0,0, 1,0,0,0,0, 0,1,0,0,0, 0,0,0,0,1, // 7
                       1,0,0,0,0, 0,1,0,0,0, 0,0,0,0,0, 5,0,0,0,0, 0,1,0,0,0, 0,0,0,0,1, // 8
                       1,0,0,0,0, 0,2,0,0,0, 4,0,0,0,0, 1,0,0,4,0, 0,2,0,0,0, 0,0,0,0,1, // 9
                                                                                         // 
                       1,0,0,4,0, 0,1,0,0,0, 0,0,0,0,0, 1,0,0,0,0, 0,1,0,0,0, 4,0,0,0,1, // 10
                       1,0,0,0,0, 0,1,0,0,0, 0,0,0,0,0, 1,0,0,0,0, 0,1,0,0,0, 0,0,0,0,1, // 11
                       1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,0,0,4,0, 0,1,0,0,0, 0,0,0,0,1, // 12
                       1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 1,0,0,0,0, 0,1,0,0,0, 0,0,0,0,1, // 13
                       1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 1,0,0,0,0, 0,1,0,0,0, 0,0,0,0,1, // 14
                                                                                         // 
                       1,0,0,0,0, 0,0,0,0,0, 0,0,0,4,0, 1,1,1,3,1, 1,1,1,1,1, 1,1,1,1,1, // 15
                       1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 5,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1, // 16
                       1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 5,0,0,4,0, 0,0,0,0,0, 0,4,0,0,1, // 17
                       1,1,1,1,3, 1,1,1,1,1, 1,1,1,1,1, 1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1, // 18
                       1,0,0,0,0, 0,0,0,0,1, 0,0,0,0,0, 1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1, // 19
                                                                                         // 
                       1,0,0,0,0, 0,0,0,0,2, 0,0,0,0,0, 2,0,0,4,0, 0,0,0,0,0, 0,4,0,0,1, // 20
                       1,0,0,0,0, 0,0,0,0,1, 0,0,0,0,0, 1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1, // 21
                       1,0,0,0,0, 0,0,0,0,1, 0,0,0,0,0, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, // 22
                       1,0,0,4,0, 0,4,0,0,1, 0,0,0,0,0, 1,0,0,0,0, 0,0,1,0,0, 0,0,0,0,1, // 23
                       1,0,0,0,0, 0,0,0,0,1, 0,0,0,0,0, 5,0,0,0,0, 0,0,2,0,0, 0,0,0,0,1, // 24
                                                                                         // 
                       1,0,0,0,0, 0,0,0,0,1, 0,0,4,0,0, 1,0,0,0,0, 0,0,1,0,0, 0,4,0,0,1, // 25
                       1,0,0,0,0, 0,0,0,0,1, 0,0,0,0,0, 1,0,0,0,4, 0,0,1,0,0, 0,0,0,0,1, // 26
                       1,0,0,0,0, 0,0,0,0,1, 0,0,0,0,0, 2,0,0,0,0, 0,0,1,0,0, 0,0,0,0,1, // 27
                       1,0,0,0,0, 0,0,0,0,1, 0,0,0,0,0, 1,0,0,0,0, 0,0,1,0,0, 0,0,0,0,1, // 28
                       1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1};// 29
    // 0 pod³oga
    // 1 œciana
    // 2 przejœcie poziome
    // 3 przejœcie pionowe
    // 4 œwiat³o punktowe
    // 5 lustra
    int wallSize = 0;

    for (int i = 0; i < mapLen; i++)
    {
        if (map[i] == 1)
            wallSize++;
    }



    // Tworzenie grafu sceny
    // ---------------------
    SceneRoot* sceneRoot = new SceneRoot();

    myCamera = new MyCamera(glm::vec3(0.0f, -5.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), SCR_WIDTH, SCR_HEIGHT);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Ukrycie kursora myski

    Model* house = new Model("res/models/Sciana/kostka.obj");
    Model* roof = new Model("res/models/DachReady/Dach5Ready.obj");



    //unsigned int amount = 40000;
    glm::mat4* modelMatrices;
    glm::mat4* modelMatricesRoof;
    modelMatrices = new glm::mat4[wallSize];
    modelMatricesRoof = new glm::mat4[wallSize];
    float offset = 1.0f;
    unsigned int index = 0;
    for (unsigned int i = 0; i < 30; i++)
    {
        for (unsigned int j = 0; j < 30; j++)
        {
            if (map[i * 30 + j] == 1)  // œciana
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(offset * i - 10, -5.0f, offset * j - 10));

                modelMatrices[index] = model;
                modelMatricesRoof[index] = model;
                index++;
            }
            else if (map[i * 30 + j] == 2) // brama pozioma
            {
                sceneRoot->AddGlass(glm::vec3(offset * i - 10, -5.0f, offset * j - 10), glm::vec3(0.0f), glm::vec3(1.0f, 1.0f, 0.1f));
            }
            else if (map[i * 30 + j] == 3) // brama pionowa
            {
                sceneRoot->AddGlass(glm::vec3(offset * i - 10, -5.0f, offset * j - 10), glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.1f));
            }
            else if (map[i * 30 + j] == 4) // œwiat³o
            {
                sceneRoot->AddLight(glm::vec3(offset* i - 10, -4.6f, offset* j - 10), glm::vec3(0.2f), glm::vec3(1.0f, 0.8f, 0.3f));
            }
            else if (map[i * 30 + j] == 5) // lustro
            {
                sceneRoot->AddMirror(glm::vec3(offset * i - 10, -5.0f, offset * j - 10), glm::vec3(0.0f), glm::vec3(1.0f));
            }
        }
    }
    //unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    //glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STREAM_DRAW);
    glBufferData(GL_ARRAY_BUFFER, wallSize * sizeof(glm::mat4), &modelMatrices[0], GL_DYNAMIC_DRAW);
    //sceneRoot->CreateAllHouse(200, 200);

    


    // set transformation matrices as an instance vertex attribute (with divisor 1)
    // note: we're cheating a little by taking the, now publicly declared, VAO of the model's mesh(es) and adding new vertexAttribPointers
    // normally you'd want to do this in a more organized fashion, but for learning purposes this will do.
    // -----------------------------------------------------------------------------------------------------------------------------------
    unsigned int VAO = house->meshes[0].VAO;
    glBindVertexArray(VAO);
    // set attribute pointers for matrix (4 times vec4)
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);

    glBindVertexArray(0);


    // Teraz Dach 
    //unsigned int bufferRoof;
    glGenBuffers(1, &bufferRoof);
    glBindBuffer(GL_ARRAY_BUFFER, bufferRoof);
    glBufferData(GL_ARRAY_BUFFER, wallSize * sizeof(glm::mat4), &modelMatricesRoof[0], GL_DYNAMIC_DRAW);
    //sceneRoot->CreateAllRoof(200, 200);

    unsigned int VAO2 = roof->meshes[0].VAO;
    glBindVertexArray(VAO2);
    // set attribute pointers for matrix (4 times vec4)
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);

    glBindVertexArray(0);


    ///////////////////////////////////// SKYBOX
    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    // skybox VAO
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    vector<std::string> faces
    {
        //FileSystem::getPath("resources/textures/skybox/right.jpg"),
        //FileSystem::getPath("resources/textures/skybox/left.jpg"),
        //FileSystem::getPath("resources/textures/skybox/top.jpg"),
        //FileSystem::getPath("resources/textures/skybox/bottom.jpg"),
        //FileSystem::getPath("resources/textures/skybox/front.jpg"),
        //FileSystem::getPath("resources/textures/skybox/back.jpg")
        "res/textures/skyboxMap/right.jpg",
        "res/textures/skyboxMap/left.jpg",
        "res/textures/skyboxMap/top.jpg",
        "res/textures/skyboxMap/bottom.jpg",
        "res/textures/skyboxMap/front.jpg",
        "res/textures/skyboxMap/back.jpg"
    };
    unsigned int cubemapTexture = loadCubemap(faces);


    mirrorShader.setInt("skybox", 0);
    
    for (int i = 0; i < 20; i++)
    {
        string name = "pointLights[";
        name += to_string(i);
        name += "]";

        houseShader.use();
        sceneRoot->graphNodes[1]->children[i]->UniformShader_PointLight(name, houseShader);
        singleShader.use();
        sceneRoot->graphNodes[1]->children[i]->UniformShader_PointLight(name, singleShader);
        singleColorShader.use();
        sceneRoot->graphNodes[1]->children[i]->UniformShader_PointLight(name, singleColorShader);
    }
    
    //sceneRoot->graphNodes[3]->UniformShader_DirLight("dirLight", houseShader);
    //sceneRoot->graphNodes[4]->UniformShader_SpotLight("spotLight1", houseShader);
    //sceneRoot->graphNodes[5]->UniformShader_SpotLight("spotLight2", houseShader);

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
    //int resolution = 4;


    glm::vec3 pointLightColor = glm::vec3(0.7f, 0.9f, 0.2f);
    glm::vec3 dirLightColor = glm::vec3(0.4f, 0.4f, 0.6f);
    glm::vec3 spot1LightColor = glm::vec3(0.0f, 1.0f, 0.5f);
    glm::vec3 spot2LightColor = glm::vec3(1.0f, 0.0f, 0.5f);

    glm::vec3 dirLightDir = glm::vec3(0.2f, -0.6f, -0.4f);
    glm::vec3 spot1LightDir = glm::vec3(1.0f, -1.0f, 0.0f);
    glm::vec3 spot2LightDir = glm::vec3(-1.0f, -1.0f, 0.0f);

    bool pointLightEnable = true;
    bool dirLightEnable = true;
    bool spot1LightEnable = true;
    bool spot2LightEnable = true;


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
            //ImGui::Checkbox("line Visible", &lineVisible);

            //if (lineVisible == true)
            //{
            //    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            //}
            //else
            //{
            //    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            //}
            

            //ImGui::SliderInt("resolution", &resolution, 1, 5);
            //ImGui::SliderFloat("zoom", &zoom, 5, 100);                // moje
            //ImGui::SliderFloat("y rotation", &yRotation, -360.0f, 360.0f);            // Edit 1 float using a slider from -360.0f to 360.0f
            //ImGui::SliderFloat("x rotation", &xRotation, -360.0f, 360.0f);            // Edit 1 float using a slider from -360.0f to 360.0f
            //ImGui::Checkbox("pointLightEnable", &pointLightEnable);
            //ImGui::ColorEdit3("pointLightColor", (float*)&pointLightColor); // Edit 3 floats representing a color
            //
            //ImGui::Checkbox("dirLightEnable", &dirLightEnable);
            //ImGui::ColorEdit3("dirLightColor", (float*)&dirLightColor);
            //ImGui::SliderFloat3("dirLightDir", (float*)&dirLightDir, -1.0f, 1.0f);
            //
            //ImGui::Checkbox("spot1LightEnable", &spot1LightEnable);
            //ImGui::ColorEdit3("spot1LightColor", (float*)&spot1LightColor);
            //ImGui::SliderFloat3("spotLight1Dir", (float*)&spot1LightDir, -1.0f, 1.0f);
            //
            //ImGui::Checkbox("spot2LightEnable", &spot2LightEnable);
            //ImGui::ColorEdit3("spot2LightColor", (float*)&spot2LightColor);
            //ImGui::SliderFloat3("spotLight2Dir", (float*)&spot2LightDir, -1.0f, 1.0f);



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

        //////////////////////////////////////////////////////////// shader dla domków i dachów
        glBindVertexArray(house->meshes[0].VAO);
        houseShader.use();
        houseShader.setMat4("projection", projection);
        houseShader.setMat4("view", view);

        //Teraz rysujemy dachy

        // Przekazywanie danych z gui do grfu sceny
        //sceneRoot->graphNodes[1]->ConfigLight(pointLightColor, glm::vec3(1.0f), pointLightEnable);
        //sceneRoot->graphNodes[3]->ConfigLight(dirLightColor, dirLightDir, dirLightEnable);
        //sceneRoot->graphNodes[4]->ConfigLight(spot1LightColor, spot1LightDir, spot1LightEnable);
        //sceneRoot->graphNodes[5]->ConfigLight(spot2LightColor, spot2LightDir, spot2LightEnable);

        // przekazywanie danych z grafu sceny do shadera 
        //sceneRoot->graphNodes[1]->UniformShader_PointLight("pointLight", houseShader);
        //sceneRoot->graphNodes[3]->UniformShader_DirLight("dirLight", houseShader);
        //sceneRoot->graphNodes[4]->UniformShader_SpotLight("spotLight1", houseShader);
        //sceneRoot->graphNodes[5]->UniformShader_SpotLight("spotLight2", houseShader);

        houseShader.setVec3("viewPos", myCamera->GetCameraPos());
        houseShader.setVec3("material.specular", glm::vec3(0.1f));
        houseShader.setFloat("material.shininess", 32.0f);



        //glBindVertexArray(VAO2);
        //glDrawElementsInstanced(GL_TRIANGLES, roof->meshes[0].indices.size(), GL_UNSIGNED_INT, 0, wallSize);
        //glBindVertexArray(0);
        //
        //glBindVertexArray(roof->meshes[0].VAO);

        // Teraz rysujemy domki
        // Tekstura domków
        houseShader.use();
        houseShader.setInt("material.texture_diffuse1", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, house->textures_loaded[0].id);


        // Próba przesuniêcia domku z grafem sceny
        //sceneRoot->graphNodes[20107]->SetPosition(2.0f, (float)glfwGetTime() / 2, -10.0f);


        glBindVertexArray(VAO);
        glDrawElementsInstanced(GL_TRIANGLES, house->meshes[0].indices.size(), GL_UNSIGNED_INT, 0, wallSize);
        glBindVertexArray(0);

        glBindVertexArray(house->meshes[0].VAO);

        ////////////////////////////////////////////////////////////////// shader bez renderingu instancjonowanego
        singleShader.use();
        singleShader.setMat4("projection", projection);
        singleShader.setMat4("view", view);

        // Przekazywanie danych z grafu sceny do shadera
        //sceneRoot->graphNodes[1]->UniformShader_PointLight("pointLight", singleShader);
        //sceneRoot->graphNodes[3]->UniformShader_DirLight("dirLight", singleShader);
        //sceneRoot->graphNodes[4]->UniformShader_SpotLight("spotLight1", singleShader);
        //sceneRoot->graphNodes[5]->UniformShader_SpotLight("spotLight2", singleShader);

        singleShader.setVec3("viewPos", myCamera->GetCameraPos());
        singleShader.setVec3("material.specular", glm::vec3(0.05f));
        singleShader.setFloat("material.shininess", 32.0f);


        ///////////////////////////////////////////////////////////// shader dla Ÿróde³ œwiat³a
        lightShader.use();
        lightShader.setMat4("projection", projection);
        lightShader.setMat4("view", view);

        mirrorShader.use();
        mirrorShader.setMat4("projection", projection);
        mirrorShader.setMat4("view", view);
        mirrorShader.setVec3("cameraPos", myCamera->GetCameraPos());

        glassShader.use();
        glassShader.setMat4("projection", projection);
        glassShader.setMat4("view", view);
        glassShader.setVec3("cameraPos", myCamera->GetCameraPos());

        singleColorShader.use();
        singleColorShader.setMat4("projection", projection);
        singleColorShader.setMat4("view", view);
        singleColorShader.setVec3("material.specular", glm::vec3(0.05f));
        singleColorShader.setFloat("material.shininess", 32.0f);

        sceneRoot->graphNodes[5]->SetPosition(myCamera->GetCameraPos().x, myCamera->GetCameraPos().y, myCamera->GetCameraPos().z);
        sceneRoot->graphNodes[5]->SetRotation(0.0f, -90-myCamera->GetRotationY(), 0.0f);

        sceneRoot->Update((float)glfwGetTime(), buffer, bufferRoof);
        sceneRoot->Draw(singleShader, singleColorShader, lightShader, mirrorShader, glassShader, cubemapTexture);



        // draw skybox as last
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        skyboxShader.use();
        view = glm::mat4(glm::mat3(myCamera->GetView())); // remove translation from the view matrix
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);
        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default


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

unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

static void ChangeBuffer(int index, glm::mat4 newModel, bool house)
{
    if (house == true)
    {
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(glm::mat4), sizeof(glm::mat4), &newModel);
    }
    else
    {
        glBindBuffer(GL_ARRAY_BUFFER, bufferRoof);
        glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(glm::mat4), sizeof(glm::mat4), &newModel);
    }
}


