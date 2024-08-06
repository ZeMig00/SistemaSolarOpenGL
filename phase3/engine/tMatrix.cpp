#include "tMatrix.h"
#include "catmull-rom.h"

using namespace std;

//float prev_y[3] = {0, 1, 0};

struct vec3 {
    float x, y, z;
};

float radians(float degrees) {
    return degrees * (M_PI / 180.0f);
}

void cross(float *a, float *b, float *res) {

	res[0] = a[1]*b[2] - a[2]*b[1];
	res[1] = a[2]*b[0] - a[0]*b[2];
	res[2] = a[0]*b[1] - a[1]*b[0];
}


void normalize(float *a) {

	float l = sqrt(a[0]*a[0] + a[1] * a[1] + a[2] * a[2]);
	a[0] = a[0]/l;
	a[1] = a[1]/l;
	a[2] = a[2]/l;
}

void multMatrixVector(const tMat& m, float *v, float *res) {

	for (int j = 0; j < 4; ++j) {
		res[j] = 0;
		for (int k = 0; k < 4; ++k) {
			res[j] += v[k] * m.m[j][k];
		}
	}

}

tMat tMat_identity() {
    tMat identity;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            identity.m[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }
    return identity;
}

tMat tMat_translate(float x, float y, float z) {
    tMat translation = tMat_identity();
    translation.m[0][3] = x;
    translation.m[1][3] = y;
    translation.m[2][3] = z;
    return translation;
}

tMat tMat_scale(float x, float y, float z) {
    tMat scaling = tMat_identity();
    scaling.m[0][0] = x;
    scaling.m[1][1] = y;
    scaling.m[2][2] = z;
    return scaling;
}

tMat tMat_rotate(float angle, float x, float y, float z) {
    angle = radians(angle);

    vec3 axis = {x, y, z};
    float length = sqrt(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);
    if (length != 1.0f) {
        axis.x /= length;
        axis.y /= length;
        axis.z /= length;
    }

    tMat rotation = tMat_identity();
    rotation.m[0][0] = axis.x * axis.x + (1 - axis.x * axis.x) * cos(angle);
    rotation.m[0][1] = axis.x * axis.y * (1 - cos(angle)) - axis.z * sin(angle);
    rotation.m[0][2] = axis.x * axis.z * (1 - cos(angle)) + axis.y * sin(angle);
    rotation.m[1][0] = axis.x * axis.y * (1 - cos(angle)) + axis.z * sin(angle);
    rotation.m[1][1] = axis.y * axis.y + (1 - axis.y * axis.y) * cos(angle);
    rotation.m[1][2] = axis.y * axis.z * (1 - cos(angle)) - axis.x * sin(angle);
    rotation.m[2][0] = axis.x * axis.z * (1 - cos(angle)) - axis.y * sin(angle);
    rotation.m[2][1] = axis.y * axis.z * (1 - cos(angle)) + axis.x * sin(angle);
    rotation.m[2][2] = axis.z * axis.z + (1 - axis.z * axis.z) * cos(angle);

    return rotation;
}

tMat tMat_multiply(const tMat& a, const tMat& b) {
    tMat result;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result.m[i][j] = a.m[i][0] * b.m[0][j] + a.m[i][1] * b.m[1][j] + a.m[i][2] * b.m[2][j] + a.m[i][3] * b.m[3][j];
        }
    }
    return result;
}

tMat calcTransformsMatrix(vector<Transform> transforms, float current_time, int showCRC, float *prev_y) {
    tMat transformMatrix = tMat_identity();
    
    if (!transforms.empty()) {
        for (const auto& t : transforms) {
            if (strcmp(t.type.c_str(), "translate") == 0) {
                transformMatrix = tMat_multiply(transformMatrix, tMat_translate(t.arguments[0], t.arguments[1], t.arguments[2]));
            }
            else if (strcmp(t.type.c_str(), "translate t") == 0) {
                if(t.time > 0){
                    float pontos[3];
                    float deriv[3];

                    int number_of_times = (int)(current_time / t.time);

                    float tmp = (float)(current_time - number_of_times * t.time) / t.time;
                    //transformMatrix = tMat_multiply(transformMatrix, tMat_translate(t.arguments[0], t.arguments[1], t.arguments[2]));
                    if(showCRC == 1){
                        catmull_rom_renderCurve(t.arguments, transformMatrix);
                    }
                    catmull_rom_getPoint(tmp, t.arguments, pontos, deriv);
                    
                    transformMatrix = tMat_multiply(transformMatrix, tMat_translate(pontos[0], pontos[1], pontos[2]));
                    if(t.align){
                        // Align the model with the Catmull-Rom curve
                        //float prev_y_a[3] = {transformMatrix.m[0][1], transformMatrix.m[1][1], transformMatrix.m[2][1]};
                        float x[3] = {deriv[0], deriv[1], deriv[2]};
                        normalize(x);

                        float z[3];
                        cross(x, prev_y, z);
                        normalize(z);

                        float y[3];
                        cross(z, x, y);
                        normalize(y);

                        // Create the rotation matrix from the aligned axes
                        tMat m = tMat_identity();
                        m.m[0][0] = x[0]; m.m[0][1] = y[0]; m.m[0][2] = z[0]; m.m[0][3] = 0;
                        m.m[1][0] = x[1]; m.m[1][1] = y[1]; m.m[1][2] = z[1]; m.m[1][3] = 0;
                        m.m[2][0] = x[2]; m.m[2][1] = y[2]; m.m[2][2] = z[2]; m.m[2][3] = 0;
                        m.m[3][0] = 0; m.m[3][1] = 0; m.m[3][2] = 0; m.m[3][3] = 1;

                        // Apply the rotation matrix to the model
                        transformMatrix = tMat_multiply(transformMatrix, m);
                        //printf("prev_y: %f %f %f\n", prev_y[0], prev_y[1], prev_y[2]);
                        memcpy(prev_y, y, 3 * sizeof(float));
                        //printf("y: %f %f %f\n", y[0], y[1], y[2]);

                        
                    }

                }
            }
            else if (strcmp(t.type.c_str(), "rotate t") == 0) {
                if(t.time > 0){
                    int number_of_times = (int)(current_time / t.time);
                    float tmp = (float)(current_time - number_of_times * t.time) / t.time;
                    
                    transformMatrix = tMat_multiply(transformMatrix, tMat_rotate(tmp * 360, t.arguments[0], t.arguments[1], t.arguments[2]));
                }
            }
            else if (strcmp(t.type.c_str(), "rotate") == 0 && t.time == 0) {
                transformMatrix = tMat_multiply(transformMatrix, tMat_rotate(t.arguments[0], t.arguments[1], t.arguments[2], t.arguments[3]));
            }
            else if (strcmp(t.type.c_str(), "scale") == 0) {
                transformMatrix = tMat_multiply(transformMatrix, tMat_scale(t.arguments[0], t.arguments[1], t.arguments[2]));
            }
        }
    }
    
    return transformMatrix;
}

Ponto applyMatrixToPoint(const tMat& matrix, const Ponto& point) {
    Ponto result;
    result.x = matrix.m[0][0] * point.x + matrix.m[0][1] * point.y + matrix.m[0][2] * point.z + matrix.m[0][3];
    result.y = matrix.m[1][0] * point.x + matrix.m[1][1] * point.y + matrix.m[1][2] * point.z + matrix.m[1][3];
    result.z = matrix.m[2][0] * point.x + matrix.m[2][1] * point.y + matrix.m[2][2] * point.z + matrix.m[2][3];
    return result;
}



