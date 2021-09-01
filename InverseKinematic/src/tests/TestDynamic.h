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

#include <array>

struct Vec2 {
	float x, y;
};

struct Vec3 {
	float x, y, z;
};

struct VertexOld {
	Vec3 position;
	Vec2 texCoord;
	float texIndex;

	VertexOld(){}

	VertexOld(Vec3 pos, Vec2 coord, float index) {
		position = pos;
		texCoord = coord;
		texIndex = index;
	}
};

namespace test {
	class TestDynamic :public Test
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

		Vec2 quad1Pos;
		Vec2 quad2Pos;

	public:
		TestDynamic();
		~TestDynamic();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGUI() override;
	};
}

static std::array<VertexOld, 4> MakeQuad(float x, float y, float texIndex);