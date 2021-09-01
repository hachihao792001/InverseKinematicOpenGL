#include "Common.h"

#include <iostream>
#include <fstream>
#include <string>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Camera.h"
#include "Cube.h"

GLFWwindow* window;
int width = 640, height = 480;

std::vector<glm::vec3> DoFabrik(glm::vec3 start, glm::vec3 end, std::vector<glm::vec3> points, std::vector<float> lengths) {
	int pointCount = points.size();

	float sumLength = 0;
	for (int i = 0; i < lengths.size(); i++)
		sumLength += lengths[i];
	if (sumLength < glm::distance(start, end)) {
		float currentPointPos = 0;
		for (int i = 0; i < pointCount; i++) {
			points[i] = glm::normalize(end - start) * currentPointPos;
			if (i < pointCount - 1)
				currentPointPos += lengths[i];
		}
		return points;
	}

	int loopCount = 0;
	while (glm::distance(points[0], start) > 0.1f ||
		glm::distance(points[pointCount - 1], end) > 0.1f) {

		points[pointCount - 1] = end;
		for (int i = pointCount - 2; i >= 0; i--)
			points[i] = points[i + 1] + glm::normalize(points[i] - points[i + 1]) * lengths[i];

		points[0] = start;
		for (int i = 1; i < pointCount; i++)
			points[i] = points[i - 1] + glm::normalize(points[i] - points[i - 1]) * lengths[i - 1];

		if (loopCount++ > 20)
			break;
	}

	return points;
}

int main(void)
{
	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(width, height, "Inverse Kinematic OpenGL", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
		std::cout << "Error glew" << std::endl;

	glfwSwapInterval(1);

	std::cout << glGetString(GL_VERSION) << std::endl;
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));
		ImGui::StyleColorsDark();

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GLCall(glEnable(GL_DEPTH_TEST));

		Renderer renderer;

		unsigned int indices[6 * 6 * 7];	//mỗi quad cần 6 vertices, mỗi cube cần 6 quad
		for (int i = 0; i < 6 * 7; i++) {
			int firstVal = i * 4;
			indices[i * 6] = firstVal;
			indices[i * 6 + 1] = firstVal + 1;
			indices[i * 6 + 2] = firstVal + 2;
			indices[i * 6 + 3] = firstVal;
			indices[i * 6 + 4] = firstVal + 2;
			indices[i * 6 + 5] = firstVal + 3;
		}


		VertexBuffer vbo(nullptr, sizeof(Vertex) * 200, false);
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(2);
		layout.Push<float>(1);
		layout.Push<float>(3);
		layout.Push<float>(1);
		layout.Push<float>(4);

		VertexArray vao;
		vao.AddBuffer(vbo, layout);

		IndexBuffer ibo(indices, sizeof(indices) / sizeof(unsigned int));

		Shader shader("res/shaders/Diffuse.shader");
		shader.Bind();

		int textures[6] = { 0,1,2,3,4,5 };
		shader.SetUniform1iv("u_Textures", 6, textures);
		shader.SetUniform4f("u_LightColor", 1.0f, 1.0f, 1.0f, 1.0f);
		shader.SetUniform1f("u_Intensity", 1.5f);

		Texture grassTex("res/textures/grass.png");
		Texture haoTex1("res/textures/haoBicycle.jpg");
		Texture haoTex2("res/textures/haoRedShirt.png");
		Texture whiteTex("res/textures/white.png");
		Texture metalTex("res/textures/metal.jpg");
		Texture woodTex("res/textures/wood.jpg");

		Camera camera(640, 480, glm::vec3(0.0f, 0.0f, 2.0f));
		bool cameraEnabled = true;

		Cube cube1(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(2.0f, 0.2f, 0.2f), 4.0f);
		Cube cube2(glm::vec3(2.0f, 0.0f, 0.0f), glm::vec3(2.0f, 0.2f, 0.2f), 4.0f);
		Cube cube3(glm::vec3(4.0f, 0.0f, 0.0f), glm::vec3(2.0f, 0.2f, 0.2f), 4.0f);
		Cube lightSrc(glm::vec3(1.0f, 3.0f, 1.0f), glm::vec3(0.3f), glm::vec4(1.0f));
		Cube base(glm::vec3(0.0f), glm::vec3(0.2f), 1.0f);
		Cube target(glm::vec3(5.0f, 2.0f, 0.0f), glm::vec3(0.2f), 2.0f);
		Cube ground(glm::vec3(-5.0f, -1.0f, -5.0f), glm::vec3(10.0f, 0.1f, 10.0f), 5.0f);

		shader.SetUniform3f("u_LightPos",
			lightSrc.vertices[0].position.x,
			lightSrc.vertices[0].position.y,
			lightSrc.vertices[0].position.z);

		std::vector<glm::vec3> points;
		std::vector<float> lengths{ 2,2,2 };

		float currentPointPos = 0;
		for (int i = 0; i <= lengths.size(); i++) {
			points.push_back(glm::normalize(target.GetCenter() - glm::vec3(0.0f) * currentPointPos));
			if (i < points.size() - 1)
				currentPointPos += lengths[i];
		}

		float speed = 0.05f;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
			GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			grassTex.Bind(0);
			haoTex1.Bind(1);
			haoTex2.Bind(2);
			whiteTex.Bind(3);
			metalTex.Bind(4);
			woodTex.Bind(5);

			if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
				target.Translate(glm::vec3(0.0f, 0.0f, speed));
			if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
				target.Translate(glm::vec3(0.0f, 0.0f, -speed));
			if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
				target.Translate(glm::vec3(-speed, 0.0f, 0.0f));
			if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
				target.Translate(glm::vec3(speed, 0.0f, 0.0f));
			if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
				target.Translate(glm::vec3(0.0f, speed, 0.0f));
			if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
				target.Translate(glm::vec3(0.0f, -speed, 0.0f));

			points = DoFabrik(glm::vec3(0.0f), target.GetCenter(), points, lengths);
			cube1.SetLookDir(points[1] - points[0]);
			cube1.SetPos(points[1]);
			cube2.SetLookDir(points[2] - points[1]);
			cube2.SetPos(points[2]);
			cube3.SetLookDir(points[3] - points[2]);
			cube3.SetPos(points[3]);

			cube1.UpdateNormals();
			cube2.UpdateNormals();
			cube3.UpdateNormals();
			base.UpdateNormals();
			target.UpdateNormals();

			Vertex* allVertices = new Vertex[24 * 7];
			memcpy(&allVertices[0], cube1.vertices, sizeof(cube1.vertices));
			memcpy(&allVertices[24], cube2.vertices, sizeof(cube2.vertices));
			memcpy(&allVertices[24 * 2], cube3.vertices, sizeof(cube3.vertices));
			memcpy(&allVertices[24 * 3], lightSrc.vertices, sizeof(lightSrc.vertices));
			memcpy(&allVertices[24 * 4], base.vertices, sizeof(base.vertices));
			memcpy(&allVertices[24 * 5], target.vertices, sizeof(target.vertices));
			memcpy(&allVertices[24 * 6], ground.vertices, sizeof(ground.vertices));
			vbo.UpdateData(allVertices, 24 * 7 * sizeof(Vertex));
			delete[] allVertices;

			if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
				cameraEnabled = false;
			if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
				cameraEnabled = true;

			if (cameraEnabled)
				camera.Inputs(window);
			camera.Matrix(45.0f, 0.1f, 100.0f, shader, "u_CamMatrix");
			shader.SetUniformMat4f("u_ModelMatrix", glm::mat4(1.0f));
			renderer.Draw(vao, ibo, shader);


			ImGui::Begin("FPS");
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(window);
			glfwPollEvents();

		}
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}