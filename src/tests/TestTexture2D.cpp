#include "TestTexture2D.h"

#include "Renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc//matrix_transform.hpp"

namespace test {

	TestTexture2D::TestTexture2D()
		:m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))), 
		m_Translation_1(200,200,0), m_Translation_2(400,200,0)
	{
		//顶点buffer, 包含纹理坐标                             
		float Vertexs[] = {
			-50.0f,-50.0f, 0.0f, 0.0f, //0
			 50.0f,-50.0f, 1.0f, 0.0f,  //1
			 50.0f,  50.0f, 1.0f, 1.0f,  //2
			-50.0f,  50.0f, 0.0f, 1.0f //3
		};
		//索引buffer 
		unsigned int Indices[] = {
			0,1,2,
			2,3,0
		};

		//做混合，说明着色器如何应对透明的像素
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		//创建顶点array
		m_VAO = std::make_unique<VertexArray>();
		//创建顶点buffer
		m_VertexBuffer= std::make_unique<VertexBuffer>(Vertexs, 4 * 4 * sizeof(float));

		//创建顶点buffer的布局
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		//在顶点array中完成对应
		m_VAO->AddBuffer(*m_VertexBuffer, layout);

		//创建索引buffer
		m_IndexBuffer = std::make_unique<IndexBuffer>(Indices, 6);

		//创建并绑定shader
		m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
		m_Shader->Bind();
		//标准化Shader中的参数
		m_Shader->SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);//改变颜色

		m_Texture=std::make_unique<Texture>("res/textures/whiteloop.png");
		m_Shader->SetUniform1i("u_Texture", 0);
	}

	TestTexture2D::~TestTexture2D()
	{

	}

	void TestTexture2D::OnUpdate(float deltaTime) {

	}

	void TestTexture2D::OnRender() {
		GLCall(glClearColor(0.0f,0.0f,0.0f,1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;

		m_Texture->Bind();

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation_1);//模型变换，可以认为是模型位移的变换矩阵
			glm::mat4 mvp = m_Proj * m_View * model;//在OpenGL中矩阵乘从右往左
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);//进行变换，确定被渲染对象的位置等

			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation_2);//模型变换，可以认为是模型位移的变换矩阵
			glm::mat4 mvp =m_Proj * m_View * model;//在OpenGL中矩阵乘从右往左
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);//进行变换，确定被渲染对象的位置等

			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}
	}

	void TestTexture2D::OnImGuiRender() {
		ImGui::SliderFloat3("Translation_1", &m_Translation_1.x, 0.0f, 960.0f);
		ImGui::SliderFloat3("Translation_2", &m_Translation_2.x, 0.0f, 960.0f);
		ImGui::Text("Current average frame rate %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

}