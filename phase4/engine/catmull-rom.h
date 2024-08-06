#pragma once
#include "tMatrix.h"


void catmull_rom_getPoint(float gt, vector<float> ctrl_pts, float* pos, float* deriv);
void catmull_rom_renderCurve(vector<float> ctrl_pts, const tMat& transfMatrix);

//void catmull_rom_getPoint_setControlPoints(float gt, vector<float> ctrl_pts, float* pos, float* deriv);