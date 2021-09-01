#include "TestBatchTexture.h"

namespace test {
	TestBatchTexture::TestBatchTexture() :
		m_Proj(glm::ortho(0.0f, 640.0f, 0.0f, 480.0f, -1.0f, 1.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
	{
		float positions[] = {
			 50.0f,  50.0f,	 0.0f, 0.0f, 0.0f,	//0
			 150.0f, 50.0f,  1.0f, 0.0f, 0.0f,	//1
			 150.0f, 150.0f, 1.0f, 1.0f, 0.0f,	//2
			 50.0f,  150.0f, 0.0f, 1.0f, 0.0f,	//3

			 250.0f, 250.0f, 0.0f, 0.0f, 1.0f,	//0
			 350.0f, 250.0f, 1.0f, 0.0f, 1.0f,	//1
			 350.0f, 350.0f, 1.0f, 1.0f, 1.0f,	//2
			 250.0f, 350.0f, 0.0f, 1.0f, 1.0f	//3
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0,

			4, 5, 6,
			6, 7, 4
		};

		glDisable(GL_DEPTH_TEST);

		m_VB = std::make_unique<VertexBuffer>(positions, 5 * 8 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		layout.Push<float>(1);

		m_VAO = std::make_unique<VertexArray>();
		m_VAO->AddBuffer(*m_VB, layout);

		m_IBO = std::make_unique<IndexBuffer>(indices, 12);

		m_Shader = std::make_unique<Shader>("res/shaders/BatchTexture.shader");
		m_Shader->Bind();

		int textures[2] = { 0,1 };
		m_Shader->SetUniform1iv("u_Textures", 2, textures);

		m_Texture1 = std::make_unique<Texture>("res/textures/haoRedShirt.png");
		m_Texture2 = std::make_unique<Texture>("res/textures/haoBicycle.jpg");

	}
	TestBatchTexture::~TestBatchTexture()
	{
	}
	void TestBatchTexture::OnUpdate(float deltaTime)
	{

	}
	void TestBatchTexture::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;

		m_Texture1->Bind(0);
		m_Texture2->Bind(1);

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->SetUniformMat4f("u_MVP", mvp);

			renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
		}
	}
	void TestBatchTexture::OnImGUI()
	{
		//ImGui::SliderFloat3("translationA", &m_TranslationA.x, 0.0f, 640.0f);
		//ImGui::SliderFloat3("translationB", &m_TranslationB.x, 0.0f, 640.0f);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}