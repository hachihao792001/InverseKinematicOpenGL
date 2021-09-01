#include "Cube.h"

void Cube::createVertices(glm::vec3 pos, glm::vec3 size, float texIndex)
{
	vertices[0] = { glm::vec3(pos.x,			pos.y,			pos.z),
			glm::vec2(0.0f, 0.0f), texIndex, glm::vec3(0.0f) };
	vertices[1] = { glm::vec3(pos.x + size.x,	pos.y,			pos.z),
		glm::vec2(1.0f, 0.0f), texIndex, glm::vec3(0.0f) };
	vertices[2] = { glm::vec3(pos.x + size.x,	pos.y,			pos.z + size.z),
		glm::vec2(1.0f, 1.0f), texIndex, glm::vec3(0.0f) };
	vertices[3] = { glm::vec3(pos.x,			pos.y,			pos.z + size.z),
		glm::vec2(0.0f, 1.0f), texIndex, glm::vec3(0.0f) };

	vertices[4] = { glm::vec3(pos.x,			pos.y + size.y, pos.z),
		glm::vec2(0.0f, 0.0f), texIndex, glm::vec3(0.0f) };
	vertices[5] = { glm::vec3(pos.x + size.x,	pos.y + size.y, pos.z),
		glm::vec2(1.0f, 0.0f), texIndex, glm::vec3(0.0f) };
	vertices[6] = { glm::vec3(pos.x + size.x,	pos.y + size.y, pos.z + size.z),
		glm::vec2(1.0f, 1.0f), texIndex, glm::vec3(0.0f) };
	vertices[7] = { glm::vec3(pos.x ,			pos.y + size.y, pos.z + size.z),
		glm::vec2(0.0f, 1.0f), texIndex, glm::vec3(0.0f) };

	vertices[8] = { glm::vec3(pos.x,			pos.y,			pos.z),
		glm::vec2(0.0f, 0.0f), texIndex, };
	vertices[9] = { glm::vec3(pos.x,			pos.y,			pos.z + size.z),
		glm::vec2(1.0f, 0.0f), texIndex, glm::vec3(0.0f) };
	vertices[10] = { glm::vec3(pos.x,			pos.y + size.y, pos.z + size.z),
		glm::vec2(1.0f, 1.0f), texIndex, glm::vec3(0.0f) };
	vertices[11] = { glm::vec3(pos.x,			pos.y + size.y, pos.z),
		glm::vec2(0.0f, 1.0f), texIndex, glm::vec3(0.0f) };

	vertices[12] = { glm::vec3(pos.x + size.x,	pos.y,			pos.z),
		glm::vec2(0.0f, 0.0f), texIndex, glm::vec3(0.0f) };
	vertices[13] = { glm::vec3(pos.x + size.x,	pos.y,			pos.z + size.z),
		glm::vec2(1.0f, 0.0f), texIndex, glm::vec3(0.0f) };
	vertices[14] = { glm::vec3(pos.x + size.x,	pos.y + size.y, pos.z + size.z),
		glm::vec2(1.0f, 1.0f), texIndex, glm::vec3(0.0f) };
	vertices[15] = { glm::vec3(pos.x + size.x,	pos.y + size.y, pos.z),
		glm::vec2(0.0f, 1.0f), texIndex, glm::vec3(0.0f) };

	vertices[16] = { glm::vec3(pos.x,			pos.y,			pos.z),
		glm::vec2(0.0f, 0.0f), texIndex, glm::vec3(0.0f) };
	vertices[17] = { glm::vec3(pos.x + size.x,	pos.y,			pos.z),
		glm::vec2(1.0f, 0.0f), texIndex, glm::vec3(0.0f) };
	vertices[18] = { glm::vec3(pos.x + size.x,	pos.y + size.y, pos.z),
		glm::vec2(1.0f, 1.0f), texIndex, glm::vec3(0.0f) };
	vertices[19] = { glm::vec3(pos.x,			pos.y + size.y, pos.z),
		glm::vec2(0.0f, 1.0f), texIndex, glm::vec3(0.0f) };

	vertices[20] = { glm::vec3(pos.x,			pos.y,			pos.z + size.z),
		glm::vec2(0.0f, 0.0f), texIndex, glm::vec3(0.0f) };
	vertices[21] = { glm::vec3(pos.x + size.x,	pos.y,			pos.z + size.z),
		glm::vec2(1.0f, 0.0f), texIndex, glm::vec3(0.0f) };
	vertices[22] = { glm::vec3(pos.x + size.x,	pos.y + size.y, pos.z + size.z),
		glm::vec2(1.0f, 1.0f), texIndex, glm::vec3(0.0f) };
	vertices[23] = { glm::vec3(pos.x,			pos.y + size.y, pos.z + size.z),
		glm::vec2(0.0f, 1.0f), texIndex, glm::vec3(0.0f) };

	UpdateNormals();
}

Cube::Cube(glm::vec3 pos, glm::vec3 size, float texIndex)
{
	this->size = size;
	createVertices(pos, size, texIndex);
	for (int i = 0; i < 24; i++)
		vertices[i].useColor = 0.0f;
}

Cube::Cube(glm::vec3 pos, glm::vec3 size, glm::vec4 color)
{
	this->size = size;
	createVertices(pos, size, 0.0f);
	for (int i = 0; i < 24; i++) {
		vertices[i].useColor = 1.0f;
		vertices[i].color = color;
	}
}

void Cube::SetLookDir(glm::vec3 dir)
{
	dir = glm::normalize(dir);
	glm::vec3 oldLookDir = GetLookDir();

	glm::vec3 axis;
	if (glm::angle(oldLookDir, dir) < 0.001f || (
		glm::angle(oldLookDir, dir) >= glm::pi<float>() - 0.001f &&
		glm::angle(oldLookDir, dir) <= glm::pi<float>() + 0.001f))
		axis = glm::vec3(0.0f);	//2 vecto cùng phương thì tích có hướng = 0
	else
		axis = glm::normalize(glm::cross(oldLookDir, dir));

	Rotate(glm::acos(glm::dot(dir, oldLookDir)), axis);
}

void Cube::UpdateNormals()
{
	glm::vec3 normal = glm::cross(
		vertices[1].position - vertices[0].position,
		vertices[2].position - vertices[1].position);
	for (int i = 0; i < 4; i++)
		vertices[i].normal = normal;

	normal = glm::cross(
		vertices[4].position - vertices[5].position,
		vertices[6].position - vertices[5].position);
	for (int i = 4; i < 8; i++)
		vertices[i].normal = normal;

	normal = glm::cross(
		vertices[9].position - vertices[8].position,
		vertices[10].position - vertices[9].position);
	for (int i = 8; i < 12; i++)
		vertices[i].normal = normal;

	normal = glm::cross(
		vertices[12].position - vertices[13].position,
		vertices[14].position - vertices[13].position);
	for (int i = 12; i < 16; i++)
		vertices[i].normal = normal;

	normal = glm::cross(
		vertices[16].position - vertices[17].position,
		vertices[18].position - vertices[17].position);
	for (int i = 16; i < 20; i++)
		vertices[i].normal = normal;

	normal = glm::cross(
		vertices[21].position - vertices[20].position,
		vertices[22].position - vertices[21].position);
	for (int i = 20; i < 24; i++)
		vertices[i].normal = normal;
}

glm::vec3 Cube::GetLookDir()
{
	glm::vec3 forwardQuadCenter = (vertices[8].position + vertices[9].position + vertices[10].position + vertices[11].position) / 4.0f;
	glm::vec3 result = forwardQuadCenter - GetCenter();
	return glm::normalize(result);
}

glm::vec3 Cube::GetCenter()
{
	Vertex trueVertices[8];
	for (int i = 0; i < 8; i++)
		trueVertices[i] = vertices[i];

	glm::vec3 center = glm::vec3(0.0f);

	for (int i = 0; i < 8; i++)
		center += trueVertices[i].position;
	center /= 8.0f;

	return center;
}

void Cube::SetPos(glm::vec3 pos)
{
	Translate(pos - vertices[0].position);
	glm::vec3 forwardQuadCenter = (vertices[8].position + vertices[9].position + vertices[10].position + vertices[11].position) / 4.0f;
	Translate(pos - forwardQuadCenter);
}

void Cube::Translate(glm::vec3 vec)
{
	for (int i = 0; i < 24; i++)
		vertices[i].position += vec;
}

void Cube::Rotate(glm::vec3 euler)
{
	glm::vec3 center = GetCenter();

	for (int i = 0; i < 24; i++) {
		vertices[i].position = center + glm::rotate(
			vertices[i].position - center, glm::radians(euler.x), glm::vec3(1.0f, 0.0f, 0.0f));
		vertices[i].position = center + glm::rotate(
			vertices[i].position - center, glm::radians(euler.y), glm::vec3(0.0f, 1.0f, 0.0f));
		vertices[i].position = center + glm::rotate(
			vertices[i].position - center, glm::radians(euler.z), glm::vec3(0.0f, 0.0f, 1.0f));
	}
}

void Cube::Rotate(float radian, glm::vec3 axis)
{
	if (axis == glm::vec3(0.0f)) return;
	glm::vec3 center = GetCenter();

	for (int i = 0; i < 24; i++)
		vertices[i].position = center + glm::rotate(vertices[i].position - center, radian, axis);
}
