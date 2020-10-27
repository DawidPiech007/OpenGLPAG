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

GLFWwindow* window;

/* Initialize vertices of our triangle */
glm::vec3 vertices[] = { glm::vec3(0.0f,  1.0f, 0.0f),
                         glm::vec3(1.0f, -1.0f, 0.0f),
                         glm::vec3(-1.0f, -1.0f, 0.0f) };

/* Initialize Vertex Buffer Object */
GLuint VBO = NULL;

/* Load sharder code from external file */
std::string loadShader(std::string fileName)
{
    std::string filetext;
    std::string line;
    std::ifstream inFile(fileName);

    

    if (!inFile)
    {
        fprintf(stderr, "Could not open file %s", fileName.c_str());
        std::cout << "Could not open file" << std::endl;
        inFile.close();

        return NULL;
    }
    else
    {
        while (inFile.good())
        {
            getline(inFile, line);
            filetext.append(line + "\n");
        }

        inFile.close();

        return filetext;
    }
}

/* Load and compile shader from the external file (uses loadShader(std::string) function) */
void loadAndCompileShaderFromFile(GLint shaderType, std::string fileName, GLuint& programHandle)
{
    GLuint shaderObject = glCreateShader(shaderType);

    if (shaderObject == 0)
    {
        fprintf(stderr, "Error creating %s.\n", fileName.c_str());
        return;
    }

    std::string shaderCodeString = loadShader(fileName);

    if (shaderCodeString.empty())
    {
        printf("Shader code is empty! Shader name %s\n", fileName.c_str());
        return;
    }

    const char* shaderCode = shaderCodeString.c_str();
    const GLint codeSize = shaderCodeString.size();

    glShaderSource(shaderObject, 1, &shaderCode, &codeSize);
    glCompileShader(shaderObject);

    GLint result;
    glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE)
    {
        fprintf(stderr, "%s compilation failed!\n", fileName.c_str());

        GLint logLen;
        glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &logLen);

        if (logLen > 0)
        {
            char* log = (char*)malloc(logLen);

            GLsizei written;
            glGetShaderInfoLog(shaderObject, logLen, &written, log);

            fprintf(stderr, "Shader log: \n%s", log);
            free(log);
        }

        return;
    }

    glAttachShader(programHandle, shaderObject);
    glDeleteShader(shaderObject);
}

int init(int width, int height)
{
    //std::cout << "132" << std::endl;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    //std::cout << "137" << std::endl;
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Hello Triangle", NULL, NULL);
    //std::cout << "140" << std::endl;
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    //std::cout << "146" << std::endl;
    /* Make the window's context current */
    glfwMakeContextCurrent(window);


    /* Initialize GLEW */       
    /*                                                          TAK BY£O
    if (glewInit() != GLEW_OK)                                  TAK BY£O
        return -1;                                              TAK BY£O
    */  
    //std::cout << "156" << std::endl;
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))//  TAK JEST
        return -1;                                          //  TAK JEST
    //std::cout << "159" << std::endl;
    /* Set the viewport */
    glViewport(0, 0, width, height);

    /* Set clear color */                                   // Kolor
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);                   // Kolor


    /* Shader init */
    GLuint programHandle = glCreateProgram();

    if (programHandle == 0)
    {
        fprintf(stderr, "Error creating program object.\n");
    }

    /* Shader load from file and compile */
    loadAndCompileShaderFromFile(GL_VERTEX_SHADER,   "res/shaders/basic.vert", programHandle);
    loadAndCompileShaderFromFile(GL_FRAGMENT_SHADER, "res/shaders/basic.frag", programHandle);

    /* Link */
    glLinkProgram(programHandle);

    GLint status;
    glGetProgramiv(programHandle, GL_LINK_STATUS, &status);

    if (status == GL_FALSE)
    {
        fprintf(stderr, "Failed to link shader program!\n");

        GLint logLen;
        glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &logLen);

        if (logLen > 0)
        {
            char* log = (char*)malloc(logLen);
            GLsizei written;
            glGetProgramInfoLog(programHandle, logLen, &written, log);

            fprintf(stderr, "Program log: \n%s", log);
            free(log);
        }
    }
    //std::cout << "197" << std::endl;
    /* Apply shader */
    glUseProgram(programHandle);
    //std::cout << "200" << std::endl;

    return true;
}

int loadContent()
{
    /* Create new buffer to store our triangle's vertices */
    glGenBuffers(1, &VBO);

    /* Tell OpenGL to use this buffer and inform that this buffer will contain an array of vertices*/
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    /* Fill buffer with data */
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    /* Enable a generic vertex attribute array */
    glEnableVertexAttribArray(0);

    /* Tell OpenGL how to interpret the data in the buffer */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    return true;
}

void render(float tpf)
{
    /* Clear the color buffer */                  // Kolor (odœwierzenie bufora)
    glClear(GL_COLOR_BUFFER_BIT);                 // Kolor 

    //glViewport(640/4, 480/4, 640/2, 480/2); // Trójk¹t 2 razy mniejszy na œrodku ekranu

    /* Draw our triangle */
    glDrawArrays(GL_TRIANGLES, 0, 3);           // Rysuje 2 trójk¹ty 
}

void update()
{
    float oldTime = 0.0f;
    float newTime = 0.0f;
    float gameTime = 0.0f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Update game time value */
        oldTime = newTime;
        newTime = (float)glfwGetTime();
        gameTime = newTime - oldTime;

        /* Render here */
        render(gameTime);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
}



int main(void)
{
    if (!init(640, 480))
        return -1;

    if (!loadContent())
        return -1;

    update();

    glfwTerminate();
    return 0;
}

//==================================== [ TUTORIAL  STOP ] ===========================================//

/*
static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int, char**)
{
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

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

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

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

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'misc/fonts/README.txt' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
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
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwMakeContextCurrent(window);
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
*/