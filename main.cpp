// Uranium C++ Renderer
// By Turbo
#include <windows.h>
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "opengl32.lib")

#include <GL/glew.h>
#include "uranium/uranium.h"
#include "imgui.h"
#include <iostream>
#include <random>
#include "uranium/uranium_culled.h"

//crandom number generator
float randomFloat(float min, float max) {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(min, max);
	return dis(gen);
}
int num = 10000;
void onImgui() {
	// adds a slider to change the number of triangles
	ImGui::SliderInt("Number of Triangles", &num, 10, 100000);
}

glm::vec3 getRandomPos() {
	return glm::vec3(randomFloat(-1.0f, 1.0f), randomFloat(-1.0f, 1.0f), randomFloat(-1.0f, 1.0f));
}

float getScale() {
	return randomFloat(0.1f, 0.5f);
}
uranium::rendering::triangle tri;

void onRender() 
{
	uranium::rendering::NewFrame();
	uranium::rendering::AddTriangle(tri);
}


int main(void)
{
	tri.verticies[0] = { getRandomPos(), {1, 0, 0}, {0, 0} };
	tri.verticies[1] = { getRandomPos(), {0, 1, 0}, {1, 0} };
	tri.verticies[2] = { getRandomPos(), {0, 0, 1}, {1, 1} };
	tri.modelTransform = glm::mat4(1.0f);
	// initialize uranium
	uranium::Initialize();
	

	// add uranium callbacks
	uranium::AddRenderCallback(onRender);
	uranium::AddImGUIDrawCallback(onImgui);

	uranium::Loop();

	// when loop stops, shutdown uranium
	uranium::Shutdown();
	return 0;
}