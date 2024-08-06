#include "catmull-rom.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define _USE_MATH_DEFINES
#include <math.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif


int POINT_COUNT = 0;
// Points that make up the loop for catmull-rom interpolation
//float p[POINT_COUNT][3] = { {1,0,0},{sqrt(0.5),0,sqrt(0.5)},{0,0,1},{-sqrt(0.5),0,sqrt(0.5)},{-1,0,0},{-sqrt(0.5),0,-sqrt(0.5)},{0,0,-1},{sqrt(0.5),0,-sqrt(0.5)} };
float (*p)[3] = nullptr;


#define TESSELATION 100.0

void getCatmullRomPoint(float t, float* p0, float* p1, float* p2, float* p3, float* pos, float* deriv) {
	tMat m = { 
        {
            { -0.5f,  1.5f, -1.5f,  0.5f },
            { 1.0f, -2.5f,  2.0f, -0.5f },
            { -0.5f,  0.0f,  0.5f,  0.0f },
            { 0.0f,  1.0f,  0.0f,  0.0f }
        }
    };

	for (int i = 0; i < 3; i++) {	 // i = x, y, z
		float p[4] = { p0[i], p1[i], p2[i], p3[i] };
		float a[4];
		multMatrixVector(m, p, a);
		pos[i] = powf(t, 3.0) * a[0] + powf(t, 2.0) * a[1] + t * a[2] + a[3];
		deriv[i] = 3 * powf(t, 2.0) * a[0] + 2 * t * a[1] + a[2];
	}
}



void catmull_rom_getPoint(float gt, vector<float> ctrl_pts, float* pos, float* deriv) {

	// If there are control points, update the global variable p
	//(only for the purpose of rendering the curve. When rendering the curve is the objective,
	// the number of control points is 0)
	if(ctrl_pts.size() > 0){
		const int sizep = ctrl_pts.size() / 3;
		float (*pts)[3] = new float[sizep][3];

		int pc = 0;
		for(int i = 0; i < ctrl_pts.size(); i+=3){
			pts[i/3][0] = ctrl_pts[i];
			pts[i/3][1] = ctrl_pts[i+1];
			pts[i/3][2] = ctrl_pts[i+2];
			pc++;
		}
		POINT_COUNT = pc;
		delete[] p;
		p = pts;
	}
	//


	float t = gt * POINT_COUNT; 
	int index = floor(t);  
	t = t - index; 

	// indices store the points
	int indices[4];
	indices[0] = (index + POINT_COUNT - 1) % POINT_COUNT;
	indices[1] = (indices[0] + 1) % POINT_COUNT;
	indices[2] = (indices[1] + 1) % POINT_COUNT;
	indices[3] = (indices[2] + 1) % POINT_COUNT;

	getCatmullRomPoint(t, p[indices[0]], p[indices[1]], p[indices[2]], p[indices[3]], pos, deriv);
}

void catmull_rom_renderCurve(vector<float> ctrl_pts, const tMat& transfMatrix) {

	if(ctrl_pts.size() > 0){
		//apply transformation matrix to the control points 
		//so that the curve is rendered in the correct position
		//when doing animations
		for (int i = 0; i < ctrl_pts.size(); i += 3) {
			float point[4] = {ctrl_pts[i], ctrl_pts[i + 1], ctrl_pts[i + 2], 1.0f};
			float aux[4];
			multMatrixVector(transfMatrix, point, aux);
			ctrl_pts[i] = aux[0];
			ctrl_pts[i + 1] = aux[1];
			ctrl_pts[i + 2] = aux[2];
    	}

		const int sizep = ctrl_pts.size() / 3;
		float (*pts)[3] = new float[sizep][3];

		int pc = 0;
		for(int i = 0; i < ctrl_pts.size(); i+=3){
			pts[i/3][0] = ctrl_pts[i];
			pts[i/3][1] = ctrl_pts[i+1];
			pts[i/3][2] = ctrl_pts[i+2];
			pc++;
		}
		POINT_COUNT = pc;
		delete[] p;
		p = pts;
	}

	glBegin(GL_LINE_LOOP);
	float gt = 0;
	while (gt < 1) {
		float pos[3];
		float deriv[3];
		vector<float> aux;
		catmull_rom_getPoint(gt, aux, pos, deriv);
		glVertex3f(pos[0], pos[1], pos[2]);
		gt += 1.0 / TESSELATION;
	}
	glEnd();
}