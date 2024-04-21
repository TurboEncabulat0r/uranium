// Uranium renderer
#include <GLFW/glfw3.h>
#include <vector>
#include "uranium.h"
#include "uranium_opengl.h"
#include <iostream>


namespace uranium {
    std::vector<function> renderCallbacks = {};
    std::vector<function> imguiCallbacks = {};
    void U_StartFrameInternal();

    void U_EndFrameInternal();

    void U_ShutdownInternal();



    void AddRenderCallback(function f) {
        renderCallbacks.push_back(f);
    }

    void invokeRenderCallbacks() {
        for (int i = 0; i < renderCallbacks.size(); i++) {
			renderCallbacks[i]();
        }
    }

    void invokeImGUICallbacks() {
        for (int i = 0; i < imguiCallbacks.size(); i++) {
            imguiCallbacks[i]();
        }
    }

    void AddImGUIDrawCallback(function f) {
        imguiCallbacks.push_back(f);
	}

    void Initialize() {
        if (InitializeWindow("Uranium")) {
            log("Window initialized");
        }
        else {
			std::cerr << "Failed to initialize window" << std::endl;
		}
	}

    void StartFrame() {
		U_StartFrameInternal();
        invokeRenderCallbacks();
        invokeImGUICallbacks();
    }

    void EndFrame() {
        U_EndFrameInternal();
    }

    void Loop() {
        while (!isFinished()) {
            StartFrame();
            EndFrame();
		}
	}

    void Shutdown() {
        U_ShutdownInternal();
	}
}