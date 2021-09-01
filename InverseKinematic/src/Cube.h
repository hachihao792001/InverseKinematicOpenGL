#include <array>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include <iostream>

struct Vertex {
	glm::vec3 position;
	glm::vec2 texCoord;
	float texIndex;
	glm::vec3 normal;
	float useColor = 0.0f;
	glm::vec4 color;
};

struct Cube {
	Vertex vertices[24];
	glm::vec3 size;

	void createVertices(glm::vec3 pos, glm::vec3 size, float texIndex);
	Cube(glm::vec3 pos, glm::vec3 size, float texIndex);
	Cube(glm::vec3 pos, glm::vec3 size, glm::vec4 color);

	glm::vec3 GetLookDir();
	void SetLookDir(glm::vec3 dir);

	void UpdateNormals();

	glm::vec3 GetCenter();
	void SetPos(glm::vec3 pos);
	void Translate(glm::vec3 vec);
	void Rotate(glm::vec3 euler);
	void Rotate(float radian, glm::vec3 axis);
};