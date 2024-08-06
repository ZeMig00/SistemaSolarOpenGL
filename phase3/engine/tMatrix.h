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
#include <cmath>
#include "figure.h"

struct tMat {
    float m[4][4];
};

tMat tMat_identity();
tMat tMat_translate(float x, float y, float z);
tMat tMat_scale(float x, float y, float z);
tMat tMat_rotate(float angle, float x, float y, float z);
tMat tMat_multiply(const tMat& a, const tMat& b);
tMat calcTransformsMatrix(vector<Transform> transforms, float current_time, int showCRC, float *prev_y);
Ponto applyMatrixToPoint(const tMat& matrix, const Ponto& point);
void multMatrixVector(const tMat& m, float *v, float *res);
