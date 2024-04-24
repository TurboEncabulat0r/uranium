#pragma once
#include "helpers.h"
#include "uranium_internal.h"

using function = void (*)();




namespace uranium {
	int InitializeWindow(const char* name);

	bool isFinished();

	void StartFrame();

	void EndFrame();

	void AddRenderCallback(function f);
	void AddImGUIDrawCallback(function f);
	void Initialize();

	void Loop();

	void Shutdown();

	void toggleWireframe();

	vec2 getScreenSize();

	void LoggerBegin(const char* sectionName);

	float LoggerEnd(const char* name);


}