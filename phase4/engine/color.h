#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <cstring>

using namespace std;

struct RGB{
    float r;
    float g;
    float b;
};

class Color{
    RGB diffuse;
    RGB ambient;
    RGB specular;
    RGB emissive;
    float shininess;
    

    public:
        Color();
        Color(RGB, RGB, RGB, RGB, float);
        void printColor(string);

        RGB getDiffuse()
        {return diffuse;}
        void setDiffuse(RGB a)
        {diffuse = a;}

        RGB getAmbient()
        {return ambient;}
        void setAmbient(RGB a)
        {ambient = a;}

        RGB getSpecular()
        {return specular;}
        void setSpecular(RGB a)
        {specular = a;}
        
        RGB getEmissive()
        {return emissive;}
        void setEmissive(RGB a)
        {emissive = a;}

        float getShininess()
        {return shininess;}
        void setShininess(float a)
        {shininess = a;}

};