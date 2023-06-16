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
		//����buffer, ������������                             
		float Vertexs[] = {
			-50.0f,-50.0f, 0.0f, 0.0f, //0
			 50.0f,-50.0f, 1.0f, 0.0f,  //1
			 50.0f,  50.0f, 1.0f, 1.0f,  //2
			-50.0f,  50.0f, 0.0f, 1.0f //3
		};
		//����buffer 
		unsigned int Indices[] = {
			0,1,2,
			2,3,0
		};

		//����ϣ�˵����ɫ�����Ӧ��͸��������
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		//��������array
		m_VAO = std::make_unique<VertexArray>();
		//��������buffer
		m_VertexBuffer= std::make_unique<VertexBuffer>(Vertexs, 4 * 4 * sizeof(float));

		//��������buffer�Ĳ���
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		//�ڶ���array����ɶ�Ӧ
		m_VAO->AddBuffer(*m_VertexBuffer, layout);

		//��������buffer
		m_IndexBuffer = std::make_unique<IndexBuffer>(Indices, 6);

		//��������shader
		m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
		m_Shader->Bind();
		//��׼��Shader�еĲ���
		m_Shader->SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);//�ı���ɫ

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
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation_1);//ģ�ͱ任��������Ϊ��ģ��λ�Ƶı任����
			glm::mat4 mvp = m_Proj * m_View * model;//��OpenGL�о���˴�������
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);//���б任��ȷ������Ⱦ�����λ�õ�

			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation_2);//ģ�ͱ任��������Ϊ��ģ��λ�Ƶı任����
			glm::mat4 mvp =m_Proj * m_View * model;//��OpenGL�о���˴�������
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);//���б任��ȷ������Ⱦ�����λ�õ�

			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}
	}

	void TestTexture2D::OnImGuiRender() {
		ImGui::SliderFloat3("Translation_1", &m_Translation_1.x, 0.0f, 960.0f);
		ImGui::SliderFloat3("Translation_2", &m_Translation_2.x, 0.0f, 960.0f);
		ImGui::Text("Current average frame rate %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

}