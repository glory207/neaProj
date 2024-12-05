# pragma once
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>
#include <vector>



unsigned int initShader(const char* vert, const char* frag);
unsigned int initShader(const char* vert, const char* geom, const char* frag);
unsigned int loadShader();