// Uranium renderer
#include <GLFW/glfw3.h>
#include <vector>
#include <chrono>
#include <iostream>
#include "uranium.h"
#include "uranium_opengl.h"
#include "imgui_impl.h"
#include "cmath"

namespace uranium {
    static uint16_t screenWidth = 1280;
    static uint16_t screenHeight = 720;

    static float deltaTime = 0;

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

    float deltaToMS(float dt) {
		return dt * 1000;
	}

    float deltaToFPS(float dt) {
        return 1 / dt;
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

    struct logger {
		std::string sectionName;
        std::chrono::time_point<std::chrono::system_clock> start;
        int logCount = 0;
        float history[10];
	};

    std::vector<logger* > loggers = {};

    void addLogger(const char* name, std::chrono::time_point<std::chrono::system_clock> s) {
        // if logger already exists, update it
        for (int i = 0; i < loggers.size(); i++) {
            if (loggers[i]->sectionName == name) {
				loggers[i]->start = s;
				return;
			}
		}
        logger* l = new logger();
        l->sectionName = name;
        l->start = s;
        loggers.push_back(l);
    }

    float getAverageLoggerTime(logger* l) {
		float total = 0;
        for (int i = 0; i < 10; i++) {
			total += l->history[i];
		}
        // round to 4
        return roundf((total / 10) * 10000) / 10000;
	}

    void LoggerBegin(const char* sectionName) {
        if (!debug) return;
        auto start = std::chrono::system_clock::now();
        addLogger(sectionName, start);
    }

    float LoggerEnd() {
        if (!debug) return 0;
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<float> elapsed_seconds = end - loggers[loggers.size() - 1]->start;

        logger* current = loggers[loggers.size() - 1];
        current->history[current->logCount] = elapsed_seconds.count();
        current->logCount++;

        if (current->logCount == 10) {
			current->logCount = 0;
		}

        return elapsed_seconds.count();
	}
    bool showAvg = false;
    void uDisplayMenu() {
        ImGui::Begin("Uranium");

        if (ImGui::Button("Toggle Wireframe")) {
			toggleWireframe();
		}

        if (ImGui::Button("Show/Hide stats")) {
            toggleStats();
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
            ImGui::Text(("FPS: " + std::to_string(deltaToFPS(deltaTime))).c_str());
            if (ImGui::Button("Show Average")) {
				showAvg = !showAvg;
			}
            if (showAvg) {
                for (int i = 0; i < loggers.size(); i++) {
					ImGui::Text("%s: %f", loggers[i]->sectionName.c_str(), getAverageLoggerTime(loggers[i]));
				}
			}
            else {
                for (int i = 0; i < loggers.size(); i++) {
					ImGui::Text("%s: %f", loggers[i]->sectionName.c_str(), loggers[i]->history[loggers[i]->logCount]);
				}
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

    std::chrono::time_point<std::chrono::system_clock> urnStart;
    
    void Loop() {
        while (!isFinished()) {
            urnStart = std::chrono::system_clock::now();
            LoggerBegin("Uranium");
            StartFrame();
            EndFrame();
            LoggerEnd();
            std::chrono::duration<float> elapsed_seconds = std::chrono::system_clock::now() - urnStart;
            float sec = elapsed_seconds.count();
            deltaTime = sec;
		}
	}

    void toggleWireframe() {
		U_ToggleWireframe();
	}

    void Shutdown() {
        U_ShutdownInternal();
	}
}