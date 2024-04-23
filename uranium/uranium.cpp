// Uranium renderer
#include <GLFW/glfw3.h>
#include <vector>
#include <chrono>
#include <iostream>
#include "uranium.h"
#include "uranium_opengl.h"
#include "imgui_impl.h"


namespace uranium {
    std::vector<function> renderCallbacks = {};
    std::vector<function> imguiCallbacks = {};

    void U_StartFrameInternal();

    void U_EndFrameInternal();

    void U_ShutdownInternal();

    void AddRenderCallback(function f) {
        renderCallbacks.push_back(f);
    }

    void invoke(function f) {
        f();
    }

    void invokeRenderCallbacks() {
        for (int i = 0; i < renderCallbacks.size(); i++) {
            invoke(renderCallbacks[i]);
        }
    }

    void invokeImGUICallbacks() {
        for (int i = 0; i < imguiCallbacks.size(); i++) {
            invoke(imguiCallbacks[i]);
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







    enum serialType {
        FLOAT,
        INT,
        STRING,
        BOOL,
        VEC3
	};

    class Serializer;

    void addSerializer(Serializer* s);

    class Serializer {

        std::string additionalInfo = "";
        float* f;
        int* i;
        char* s;
        bool* b;
        uranium::vec3* v;
    public:
        serialType type = (serialType)0;
        std::string name;
        std::uintptr_t address;
        Serializer(std::uintptr_t addr, serialType t, std::string name) {
    		this->address = addr;
            this->type = t;
			this->name = name;
		}

        void AddAdditionalInfo(std::string info) {
			additionalInfo += info;
        }

		void Serialize() {
			switch (type) {
			case FLOAT:
                f = (float*)address;
				ImGui::InputFloat(name.c_str(), f);
				break;
			case INT:
				i = (int*)address;
				ImGui::InputInt(name.c_str(), i);
				break;
			case STRING:
				s = (char*)address;
				ImGui::InputText(name.c_str(), s, 256);
				break;
			case BOOL:
				b = (bool*)address;
				ImGui::Checkbox(name.c_str(), b);
				break;
            case VEC3:
                v = (vec3*)address;
                ImGui::InputFloat3(name.c_str(), &v->x);
                break;
			}

		}
	};;

    void addSerializer(Serializer* s) {
        //uranium::AddImGUIDrawCallback(& s.Serialize)
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

    void toggleWireframe() {
		U_ToggleWireframe();
	}

    void Shutdown() {
        U_ShutdownInternal();
	}
}