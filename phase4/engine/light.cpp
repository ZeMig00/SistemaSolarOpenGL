#include "light.h"

using namespace std;

    Light::Light(){
        lightType = "";
        lightSettings = vector<float>();        
    }

    Light::Light(string type, vector<float> settings){
        lightType = type;
        lightSettings = settings;
    }

    void Light::printLight(){
        printf("\tLight: \n\t\ttype:%s;\n\t\tsettings: ", lightType.c_str());
        for(float s : lightSettings){
            printf("%lf ", s);
        }
        printf("\n");
    }