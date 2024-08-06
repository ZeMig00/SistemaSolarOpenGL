#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <cstring>

using namespace std;

class Light{
    string lightType;
    vector<float> lightSettings;
    

    public:
        Light();
        Light(string, vector<float>);
        void printLight();

        string getLightType()
        {return lightType;}
        void setLightType(string a)
        {lightType = a;}

        vector<float> getLightSettings()
        {return lightSettings;}
        void setLightSettings(vector<float> a)
        {lightSettings = a;}
        void addLightSetting(float a)
        {lightSettings.push_back(a);}

};