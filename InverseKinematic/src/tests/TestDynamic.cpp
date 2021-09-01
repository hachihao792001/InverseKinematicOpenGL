#include "TestDynamic.h"

static std::array<VertexOld, 4> MakeQuad(float x, float y, float texIndex)
{
	float size = 100.0f;

	VertexOld v0;
	v0.position = { x,  y, 0 };
	v0.texCoord = { 0.0f, 0.0f };
	v0.texIndex = texIndex;

	VertexOld v1;
	v1.position = { x + size,  y, 0 };
	v1.texCoord = { 1.0f, 0.0f };
	v1.texIndex = texIndex;

	VertexOld v2;
	v2.position = { x + size,  y + size, 0 };
	v2.texCoord = { 1.0f, 1.0f };
	v2.texIndex = texIndex;

	VertexOld v3;
	v3.position = { x, y + size, 0 };
	v3.texCoord = { 0.0f, 1.0f };
	v3.texIndex = texIndex;

	return { v0,v1,v2,v3 };
}

namespace test {
	TestDynamic::TestDynamic() :
		quad1Pos{ 50.0f, 50.0f },
		quad2Pos{ 250.0f, 250.0f },
		m_Proj(glm::ortho(0.0f, 640.0f, 0.0f, 480.0f, -1.0f, 1.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
	{
		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0,

			4, 5, 6,
			6, 7, 4
		};

		glDisable(GL_DEPTH_TEST);

		m_VB = std::make_unique<VertexBuffer>(nullptr, sizeof(VertexOld) * 100, false);
		VertexBufferLayout layout;
		layout.Push<float>(3);
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
	TestDynamic::~TestDynamic()
	{
	}
	void TestDynamic::OnUpdate(float deltaTime)
	{

	}

	void TestDynamic::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		auto quad1 = MakeQuad(quad1Pos.x, quad1Pos.y, 0.0f);
		auto quad2 = MakeQuad(quad2Pos.x, quad2Pos.y, 1.0f);

		VertexOld vertices[8];
		memcpy(vertices, quad1.data(), quad1.size() * sizeof(VertexOld));
		memcpy(vertices + quad1.size(), quad2.data(), quad2.size() * sizeof(VertexOld));

		m_Texture1->Bind(0);
		m_Texture2->Bind(1);

		m_VB->UpdateData(vertices, sizeof(vertices));

		Renderer renderer;
		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->SetUniformMat4f("u_MVP", mvp);

			renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
		}
	}
	void TestDynamic::OnImGUI()
	{
		ImGui::SliderFloat2("Quad 1 pos", &quad1Pos.x, 0.0f, 640.0f);
		ImGui::SliderFloat2("Quad 2 pos", &quad2Pos.x, 0.0f, 640.0f);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}