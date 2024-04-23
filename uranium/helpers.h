#pragma once
#include "uranium_internal.h"
#include <string>


namespace uranium {

	std::string loadShader(const char* path);

	void log(const std::string& data);

	void log(const char* data);

	void log(const int data);

	void log(const float data);

	void log(const double data);

	void log(const char data);

	void log(const bool data);

	void log(const char* data, const int data2);	

	void log(const char* data, const float data2);	

	void log(const char* data, const double data2);

	void log(const char* data, const bool data2);

	void log(tri* data);
}