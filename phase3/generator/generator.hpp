#pragma once

#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <tuple>
#include <string.h>

using namespace std;

int main_generator(int argc, char *argv[]);
void plane(float legth, int divisions,char* filename);
void box(float length, int divisions,char* filename);
void sphere(float r, int slices, int stacks,char* filename);
void cone(float r, float height, int slices, int stacks,char* filename);
void torus(float r, float R, int slices, int stacks, char* filename);
