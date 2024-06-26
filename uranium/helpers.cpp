#include "helpers.h"
#include <fstream>
#include <iostream>
#include <string>

namespace uranium {

	std::string loadShader(const char* path) {
		std::string temp;
		std::string buff;
		
		auto file = std::ifstream(path);
		if (!file.is_open()) {
			std::cerr << "Failed to open file: " << path << std::endl;
			return "";
		}

		// iterate through the file and copy it to the buffer
		while (std::getline(file, temp)) {
			buff += temp + "\n";
		}
		return buff;
	}

	uint16_t logCount = 0;
	template<typename T>
	void outputData(const T data) {
		std::cout << data << std::endl;
	}

	void log(const std::string& data) {
		outputData(data);
	}

	void log(const char* data) {
		outputData(data);
	}

	void log(const int data) {
		outputData(data);
	}

	void log(const float data) {
		outputData(data);
	}

	void log(const double data) {
		outputData(data);
	}

	void log(const char data) {
		outputData(data);
	}

	void log(const bool data) {
		outputData(data);
	}

	void log(const char* data, const int data2) {
		outputData(data);
	}

	void log(const char* data, const float data2) {
		outputData(data);
	}

	void log(const char* data, const double data2) {
		outputData(data);
	}

	void log(const char* data, const char data2) {
		outputData(data);
	}

	void log(const char* data, const bool data2) {
		outputData(data);
	}

	void log(tri* data) {
		std::cout << "Triangle: " << std::endl;
		for (int i = 0; i < 3; i++) {
			std::cout << "___vertex " << i << "___" << std::endl;
			std::cout << "Position: " << data->verts[i].position.x << ", " << data->verts[i].position.y << ", " << data->verts[i].position.z << std::endl;
			std::cout << "Color: " << data->verts[i].color.x << ", " << data->verts[i].color.y << ", " << data->verts[i].color.z << std::endl;
		}
	}

}