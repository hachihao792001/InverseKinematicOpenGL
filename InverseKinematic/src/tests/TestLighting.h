#pragma once
#include "Test.h"
#include "TestDynamic.h"
#include "Camera.h"

namespace test {
	class TestLighting : public Test
	{
	private:
		glm::mat4 m_Proj;
		glm::mat4 m_View;

		std::unique_ptr<VertexBuffer> m_VB;
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<IndexBuffer> m_IBO;
		std::unique_ptr<Shader> m_Shader;

		std::unique_ptr<Texture> m_Texture1;
		std::unique_ptr<Texture> m_Texture2;

		Camera camera;
		bool cameraEnabled;

	public:
		TestLighting();
		~TestLighting();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGUI() override;
	};
}
