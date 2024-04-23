// Uranium renderer
#include <GLFW/glfw3.h>
#include <vector>
#include <chrono>
#include <iostream>
#include "uranium.h"
#include "uranium_opengl.h"
#include "imgui_impl.h"


namespace uranium {
    static uint16_t screenWidth = 1280;
    static uint16_t screenHeight = 720;

    bool debug = true;

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

    vec2 getScreenSize() {
		return vec2(screenWidth, screenHeight);
	}

    void updateScreenSize(uint16_t width, uint16_t height) {
		screenWidth = width;
		screenHeight = height;
        U_INTUpdateScreenSize(screenWidth, screenHeight);

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

    bool showStats = false;

    void toggleStats() {
        showStats = !showStats;

    }

    struct loggerResult {
        std::string sectionName;
        float time;
    };

    std::vector<loggerResult> loggerResults = {};

    struct logger {
		std::string sectionName;
        std::chrono::time_point<std::chrono::system_clock> start;
	};

    std::vector<logger> loggers = {};

    void LoggerBegin(const char* sectionName) {
        if (!debug) return;
        logger l;
        l.sectionName = sectionName;
		l.start = std::chrono::system_clock::now();
		loggers.push_back(l);
    }

    void LoggerEnd() {
        if (!debug) return;
		logger l = loggers.back();
		loggers.pop_back();
		auto end = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = end - l.start;
		std::time_t end_time = std::chrono::system_clock::to_time_t(end);
		std::cout << l.sectionName << " took " << elapsed_seconds.count() << "s\n";
        loggerResult r;
        r.sectionName = l.sectionName;
        r.time = elapsed_seconds.count();
        loggerResults.push_back(r);
	}

    void uDisplayMenu() {
        ImGui::Begin("Uranium");

        if (ImGui::Button("Toggle Wireframe")) {
			toggleWireframe();
		}

        if (ImGui::Button("Show/Hide stats")) {
            
        }

        // editor for screen size
        static int width = screenWidth;
        static int height = screenHeight;
        ImGui::LabelText("", "Screen Size");

		ImGui::InputInt("Width", &width);
        ImGui::InputInt("Height", &height);

        if (ImGui::Button("Update Screen Size")) {
            updateScreenSize(width, height);

        }

        if (showStats) {
            ImGui::Begin("Logger Stats");
            for (int i = 0; i < loggerResults.size(); i++) {
				loggerResult r = loggerResults[i];
				ImGui::Text("%s: %f", r.sectionName.c_str(), r.time);
			}
			ImGui::End();
        }

        ImGui::End();
    }

    void invokeImGUICallbacks() {
        uDisplayMenu();
        for (int i = 0; i < imguiCallbacks.size(); i++) {
            invoke(imguiCallbacks[i]);
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