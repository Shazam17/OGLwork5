#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>
#include <string>

#include "Shader.h"

float width = 640;
float height = 480;

glm::mat4 proj;

Shader* tempShader;
void window_size_callback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
	glfwSetWindowSize(window, w, h);
	width = w;
	height = h;

	proj = glm::perspective(glm::radians(45.0f), width / height, 0.1f, 100.0f);
	tempShader->setUniform4m("u_proj", proj);
}

int main(void)
{
	GLFWwindow* window;
	if (!glfwInit())
		return -1;
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetWindowSizeCallback(window, window_size_callback);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	Shader basic("shaders/basic.shader");

	float vertices[] = {
		-1.5,4.0f,
		2.0f,2.0f,
		-1.0f,-2.0f,
		-1.0f,0.0f,
		-3.0f,-3.0f
	};

	unsigned indicesLines[] = {
		0,1,
		1,2,
		2,3,
		3,4,
		4,0
	};

	unsigned indicesTriangle[] = {
		0,1,3,
		0,3,4,
		3,1,2
	};

	for (unsigned i = 0; i < 10; i++) {
		vertices[i] /= 6;
	}

	unsigned vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);


	unsigned vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);


	unsigned ib;
	glGenBuffers(1, &ib);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesLines), indicesLines, GL_STATIC_DRAW);



	//Setup IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init((char *)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));

		// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	
	bool renderMode = true;
	bool changed = false;

	proj = glm::perspective(glm::radians(45.0f), width / height, 0.1f, 100.0f);
	basic.setUniform4m("u_proj", proj);
	glViewport(0, 0, width, height);
	tempShader = &basic;
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		//New Frame
		ImGui_ImplOpenGL3_NewFrame();

		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	
		
		//ImGui::InputText("string", buf, IM_ARRAYSIZE(buf));
		//ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
		if (ImGui::Button("change render"))
		{
			renderMode = !renderMode;
			changed = true;
		}

		basic.bind();
		glBindVertexArray(vao);
		if (changed) {
			if (renderMode) {
				

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesLines), indicesLines, GL_STATIC_DRAW);
			}
			else {

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesTriangle), indicesTriangle, GL_STATIC_DRAW);

			}
		}
		changed = false;

		if (renderMode) {
			glDrawElements(GL_LINES, 10, GL_UNSIGNED_INT, 0);
		}
		else {
			glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
		}

		//Render
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	//Shutdown
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();

	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}