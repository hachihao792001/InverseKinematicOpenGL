#include "TestCamera.h"
#include "Common.h"

namespace test {
	TestCamera::TestCamera() :
		camera(640, 480, glm::vec3(0.0f, 0.0f, 2.0f)),
		m_Proj(glm::mat4(1.0f)),
		m_View(glm::mat4(1.0f))
	{
		float positions[] = {
			 -0.5f, 0.5f, -0.5f, 	0.0f, 0.0f,  0.0f,
			  0.5f, 0.5f, -0.5f, 	1.0f, 0.0f,  0.0f,
			  0.5f, 0.5f,  0.5f, 	1.0f, 1.0f,  0.0f,
			 -0.5f, 0.5f,  0.5f, 	0.0f, 1.0f,  0.0f,

			 -0.5f, -0.5f, -0.5f, 	0.0f, 0.0f,  0.0f,
			  0.5f, -0.5f, -0.5f, 	1.0f, 0.0f,  0.0f,
			  0.5f, -0.5f,  0.5f, 	1.0f, 1.0f,  0.0f,
			 -0.5f, -0.5f,  0.5f, 	0.0f, 1.0f,  0.0f,

			 -0.5f, -0.5f, 0.5f,	0.0f, 0.0f,  1.0f,
			  0.5f, -0.5f, 0.5f,	1.0f, 0.0f,  1.0f,
			  0.5f,  0.5f, 0.5f,	1.0f, 1.0f,  1.0f,
			 -0.5f,  0.5f, 0.5f,	0.0f, 1.0f,  1.0f,

			 -0.5f, -0.5f, -0.5f,	0.0f, 0.0f,  1.0f,
			  0.5f, -0.5f, -0.5f,	1.0f, 0.0f,  1.0f,
			  0.5f,  0.5f, -0.5f,	1.0f, 1.0f,  1.0f,
			 -0.5f,  0.5f, -0.5f,	0.0f, 1.0f,  1.0f,

			 0.5f, -0.5f, -0.5f,	0.0f, 0.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,	1.0f, 0.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,	1.0f, 1.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,	0.0f, 1.0f,  0.0f,

			 -0.5f, -0.5f, -0.5f,	0.0f, 0.0f,  0.0f,
			 -0.5f,  0.5f, -0.5f,	1.0f, 0.0f,  0.0f,
			 -0.5f,  0.5f,  0.5f,	1.0f, 1.0f,  0.0f,
			 -0.5f, -0.5f,  0.5f,	0.0f, 1.0f,  0.0f

		};

		unsigned int indices[] = {
			0, 1, 2, 0, 2, 3,
			4, 5, 6, 4, 6, 7,
			8, 9, 10, 8, 10, 11,
			12, 13, 14, 12, 14 ,15,
			16, 17, 18, 16, 18, 19,
			20, 21, 22, 20, 22, 23,
		};

		glEnable(GL_DEPTH_TEST);

		m_View = glm::translate(m_View, glm::vec3(0.0f, -0.5f, -2.0f));
		m_Proj = glm::perspective(glm::radians(80.0f), (float)(640 / 480), 0.1f, 100.0f);

		m_VB = std::make_unique<VertexBuffer>(positions, sizeof(positions));
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(2);
		layout.Push<float>(1);

		m_VAO = std::make_unique<VertexArray>();
		m_VAO->AddBuffer(*m_VB, layout);

		m_IBO = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(unsigned int));

		m_Shader = std::make_unique<Shader>("res/shaders/TextureCube.shader");
		m_Shader->Bind();

		int textures[2] = { 0,1 };
		m_Shader->SetUniform1iv("u_Textures", 2, textures);

		m_Texture1 = std::make_unique<Texture>("res/textures/haoRedShirt.png");
		m_Texture2 = std::make_unique<Texture>("res/textures/haoBicycle.jpg");

	}
	TestCamera::~TestCamera()
	{
	}
	void TestCamera::OnUpdate(float deltaTime)
	{
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			cameraEnabled = !cameraEnabled;
	}
	void TestCamera::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		m_Texture1->Bind(0);
		m_Texture2->Bind(1);

		Renderer renderer;
		{
			if (cameraEnabled)
				camera.Inputs(window);
			camera.Matrix(45.0f, 0.1f, 100.0f, *m_Shader, "u_MVP");
			//glm::mat4 model = glm::mat4(1.0f);
			//glm::mat4 mvp = m_Proj * m_View * model;
			//m_Shader->SetUniformMat4f("u_MVP", mvp);

			renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
		}
	}
	void TestCamera::OnImGUI()
	{
		//ImGui::SliderFloat2("Quad 1 pos", &quad1Pos.x, 0.0f, 640.0f);
		//ImGui::SliderFloat2("Quad 2 pos", &quad2Pos.x, 0.0f, 640.0f);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}