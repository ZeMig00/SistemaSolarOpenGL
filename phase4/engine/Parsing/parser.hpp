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
#include "tinyxml2.h"
#include "../figure.h"
#include "../color.h"
#include "../light.h"

typedef struct World {
    int windowWidth;
    int windowHeight;
    float cameraPosX;
    float cameraPosY;
    float cameraPosZ;
    float cameraLookAtX;
    float cameraLookAtY;
    float cameraLookAtZ;
    float cameraUpX;
    float cameraUpY;
    float cameraUpZ;
    float cameraFov;
    float cameraNear;
    float cameraFar;
    vector<Figure> models;
    vector<Light> sceneLights;
}XML_WORLD;

void lerXML(string ficheiro, XML_WORLD &xml, float &rad, float &betas, float &alphas);
void printWorldStructure(XML_WORLD &xml);