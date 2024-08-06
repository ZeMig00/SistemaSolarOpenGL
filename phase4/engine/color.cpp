#include "color.h"

using namespace std;

    Color::Color(){
        diffuse = {(float)200/255, (float)200/255, (float)200/255};
        ambient = {(float)50/255, (float)50/255, (float)50/255};
        specular = {0, 0, 0};
        emissive = {0, 0, 0};
        shininess = 0;
        
    }

    Color::Color(RGB d, RGB a, RGB s, RGB e, float sh){
        diffuse = d;
        ambient = a;
        specular = s;
        emissive = e;
        shininess = sh;
    }

    void Color::printColor(string tabs){
        printf("%sDiffuse: %f %f %f\n", tabs.c_str(), diffuse.r, diffuse.g, diffuse.b);
        printf("%sAmbient: %f %f %f\n", tabs.c_str(), ambient.r, ambient.g, ambient.b);
        printf("%sSpecular: %f %f %f\n", tabs.c_str(), specular.r, specular.g, specular.b);
        printf("%sEmissive: %f %f %f\n", tabs.c_str(), emissive.r, emissive.g, emissive.b);
        printf("%sShininess: %f\n", tabs.c_str(), shininess);
    }