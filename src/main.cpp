#include <stdio.h>
#include <GLFW/glfw3.h>

#include "globals.h"
#include "config_parser.h"
#include "style.h"

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define GL_SILENCE_DEPRECATION



int main(int, char**)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    //Config parser
    config_parser = new ConfigParser("../assets/config.json");

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(config_parser->window_width, config_parser->window_height, "GCS", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows



    ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None; //ImGuiDockNodeFlags_NoUndocking;

    if (!config_parser->resizable)
        dockspace_flags |= ImGuiDockNodeFlags_NoResize;
    

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    io.Fonts->AddFontFromFileTTF("../assets/fonts/CubicCoreMono.ttf", 15.0f);

    // Our state
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


    //video testing
    cv::VideoCapture cap(config_parser->video_file.c_str()); // Open the video file
    if (!cap.isOpened())
        std::cout << "Error opening video stream \n";
    cv::Mat frame;
    GLuint texture;

    while (!glfwWindowShouldClose(window))
    {
   // Get the current time at the start of the loop
    auto start_time = std::chrono::high_resolution_clock::now();
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), dockspace_flags);


            auto generate_tex_timer = std::chrono::high_resolution_clock::now();
        if (cap.read(frame)) 
        {

                // Convert the frame to RGB
                cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);

                // Generate a texture from the frame
                glGenTextures(1, &texture);
                glBindTexture(GL_TEXTURE_2D, texture);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame.cols, frame.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, frame.data);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glBindTexture(GL_TEXTURE_2D, 0);
        }
        else
        {
                std::cerr << "Error reading frame" << std::endl;
        }


        ImGui::Begin("VIDEO");
        // In your ImGui window:
        if (texture) {
            ImGui::Image((void*)(intptr_t)texture, ImVec2(frame.cols, frame.rows));
        }
        ImGui::End();

        // Rendering
        ImGui::Render();


        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);

    


        glClear(GL_COLOR_BUFFER_BIT);
                                        auto end_time_TEX = std::chrono::high_resolution_clock::now();

            // Calculate the time it took for this iteration of the loop
                auto ITERN = std::chrono::duration_cast<std::chrono::milliseconds>(end_time_TEX - generate_tex_timer).count();

                std::cout << "Time for TEXTURE: " << ITERN << " ms" << std::endl;
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


 
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }


        glfwSwapBuffers(window);

           // Get the current time at the end of the loop
    auto end_time = std::chrono::high_resolution_clock::now();

    // Calculate the time it took for this iteration of the loop
    auto iteration_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

    std::cout << "Time for this iteration: " << iteration_time << " ms" << std::endl;

    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
