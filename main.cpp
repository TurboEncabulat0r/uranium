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



void onImgui() {
	ImGui::Begin("Uranium Renderer");
	ImGui::Text("Demo window test stuff yeah");
	if (ImGui::Button("wireframe")) {
		uranium::toggleWireframe();
	}
	ImGui::End();
}

void onRender() 
{

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