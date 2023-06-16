#include<GL/glew.h>
#include <GLFW/glfw3.h>
#include<iostream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"

int main(void)
{
    GLFWwindow* window;

    //初始化GLFW
    if (!glfwInit())
        return -1;

    //更改核心文件，使用VertexArray而不是VertexBuffer
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,2);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    //创建运行窗体
    window = glfwCreateWindow(960, 540, "Test Window", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    //生成OpenGL的上下文
    glfwMakeContextCurrent(window);
    //稳定渲染的帧率
    glfwSwapInterval(2);

    //初始化GLEW，要在OpenGL上下文生成以后
    if (glewInit() != GLEW_OK) {
        std::cout << "GLEW Initial Error" << std::endl;
    }

    //输出版本号，确定当前使用的OpenGL版本以及是什么硬件厂商提供的
    std::cout << glGetString(GL_VERSION) << std::endl;
    {
        //创建一个渲染器
        Renderer renderer;

        //初始化GUI架构
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);
        ImGui::StyleColorsDark();

        //使用测试类进行测试
        test::Test* currentTest = nullptr;
        test::TestMenu* testMenu=new test::TestMenu(currentTest);
        currentTest = testMenu;

        //注册所有想进行的测试
        testMenu->RegisterTest<test::TestClearColor>("Clear Color");
        testMenu->RegisterTest<test::TestTexture2D>("2D Texture");

        //主要的循环，如果窗体没被关闭就一直存在
        while (!glfwWindowShouldClose(window))
        {
            GLCall(glClearColor(0.0f,0.0f,0.0f,0.0f));
            renderer.Clear();

            //GUI架构开始工作
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            if (currentTest) 
            {
                currentTest->OnUpdate(0.0f);
                currentTest->OnRender();

                ImGui::Begin("Test");
                    if (currentTest != testMenu && ImGui::Button("<-"))
                    {
                        delete currentTest;
                        currentTest = testMenu;
                    }
                    currentTest->OnImGuiRender();
                ImGui::End();

            }

            //使用GUI架构渲染
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);

            glfwPollEvents();
        }
    }

    //GUI架构注销
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}