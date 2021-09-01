#pragma once
#include "Test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test {
	class TestBatch :public Test
	{
	private:
		glm::mat4 m_Proj;
		glm::mat4 m_View;

		std::unique_ptr<VertexBuffer> m_VB;
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<IndexBuffer> m_IBO;
		std::unique_ptr<Shader> m_Shader;


	public:
		TestBatch();
		~TestBatch();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGUI() override;
	};
}
