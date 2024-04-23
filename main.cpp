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

//crandom number generator
float randomFloat(float min, float max) {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(min, max);
	return dis(gen);
}

void onImgui() {

}

uranium::vec3 getRandomPos() {
	return uranium::vec3(randomFloat(-1.0f, 1.0f), randomFloat(-1.0f, 1.0f), randomFloat(-1.0f, 1.0f));
}

float getScale() {
	return randomFloat(0.1f, 0.5f);
}
std::vector<uranium::triangle* > triangles;
int num = 50;
void onRender() 
{
	
	
	if (triangles.size() == 0) {
		for (int i = 0; i < num; i++) {
			uranium::triangle* tri = new uranium::triangle();
			tri->position = getRandomPos();
			tri->color = uranium::vec3(randomFloat(0.0f, 1.0f), randomFloat(0.0f, 1.0f), randomFloat(0.0f, 1.0f));
			float scale = getScale();
			tri->scale = uranium::vec3(scale, scale, 1);
			triangles.push_back(tri);
			uranium::addRenderable(tri);
		}
		uranium::log("added triangles");
	}
}

int main(void)
{
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