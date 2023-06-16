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

    //��ʼ��GLFW
    if (!glfwInit())
        return -1;

    //���ĺ����ļ���ʹ��VertexArray������VertexBuffer
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,2);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    //�������д���
    window = glfwCreateWindow(960, 540, "Test Window", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    //����OpenGL��������
    glfwMakeContextCurrent(window);
    //�ȶ���Ⱦ��֡��
    glfwSwapInterval(2);

    //��ʼ��GLEW��Ҫ��OpenGL�����������Ժ�
    if (glewInit() != GLEW_OK) {
        std::cout << "GLEW Initial Error" << std::endl;
    }

    //����汾�ţ�ȷ����ǰʹ�õ�OpenGL�汾�Լ���ʲôӲ�������ṩ��
    std::cout << glGetString(GL_VERSION) << std::endl;
    {
        //����һ����Ⱦ��
        Renderer renderer;

        //��ʼ��GUI�ܹ�
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);
        ImGui::StyleColorsDark();

        //ʹ�ò�������в���
        test::Test* currentTest = nullptr;
        test::TestMenu* testMenu=new test::TestMenu(currentTest);
        currentTest = testMenu;

        //ע����������еĲ���
        testMenu->RegisterTest<test::TestClearColor>("Clear Color");
        testMenu->RegisterTest<test::TestTexture2D>("2D Texture");

        //��Ҫ��ѭ�����������û���رվ�һֱ����
        while (!glfwWindowShouldClose(window))
        {
            GLCall(glClearColor(0.0f,0.0f,0.0f,0.0f));
            renderer.Clear();

            //GUI�ܹ���ʼ����
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

            //ʹ��GUI�ܹ���Ⱦ
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);

            glfwPollEvents();
        }
    }

    //GUI�ܹ�ע��
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}