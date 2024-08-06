#pragma once

#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <array>
#include <cstring>
#include <chrono>
#include "Parsing/tinyxml2.h"
#include "figure.h"
#include "Parsing/parser.hpp"
#include "tMatrix.h"

int main_engine(int argc, char **argv);