#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <cstring>
#include "color.h"

using namespace std;

struct Ponto{
    float x;
    float y;
    float z;
};

struct TexCoord2D{
    float s;
    float t;
};

struct Normal{
    float x;
    float y;
    float z;
};

struct Transform{
	string type;
	vector<float> arguments;
    float time;
    bool align;
};

class Figure{
    vector<Ponto> pontos;
    //Color color;
    vector<Transform> transforms;
    vector<string> modelFiles;
    int id = -1;
    vector<Figure> figuras;
    int dynamic = 0;
    vector<Color> color;
    vector<string> texture;
    vector<int> CountPerModel;
    vector<Normal> normals;
    vector<TexCoord2D> texCoords;
    

    public:
        Figure();
        Figure(vector<Ponto>, vector<Transform>, vector<string>, vector<Figure>);
        void printFigure(int, int);

        vector<Ponto> getPontos()
        {return pontos;}
        void setPontos(vector<Ponto> a)
        {pontos = a;}
        void addPonto(Ponto a)
        {pontos.push_back(a);}

        vector<Transform> getTransforms()
        {return transforms;}
        void setTransforms(vector<Transform> a)
        {transforms = a;}
        void addTransform(Transform a)
        {transforms.push_back(a);}

        vector<string> getModelFiles()
        {return modelFiles;}
        void setModelFiles(vector<string> a)
        {modelFiles = a;}
        void addModelFile(string a)
        {modelFiles.push_back(a);}


        vector<Figure> getFiguras()
        {return figuras;}
        void setFiguras(vector<Figure> a)
        {figuras = a;}
        void addFigura(Figure a)
        {figuras.push_back(a);}

        int getId()
        {return id;}
        void setId(int a)
        {id = a;}

        int isDynamic()
        {return dynamic;}
        void setDynamic(int a)
        {dynamic = a;}

        vector<Color> getColor()
        {return color;}
        void setColor(vector<Color> a)
        {color = a;}
        void addColor(Color a)
        {color.push_back(a);}

        vector<string> getTexture()
        {return texture;}
        void setTexture(vector<string> a)
        {texture = a;}
        void addTexture(string a)
        {texture.push_back(a);}

        vector<int> getCountPerModel()
        {return CountPerModel;}
        void setCountPerModel(vector<int> a)
        {CountPerModel = a;}
        void addCountPerModel(int a)
        {CountPerModel.push_back(a);}

        vector<Normal> getNormals()
        {return normals;}
        void setNormals(vector<Normal> a)
        {normals = a;}
        void addNormal(Normal a)
        {normals.push_back(a);}

        vector<TexCoord2D> getTexCoords()
        {return texCoords;}
        void setTexCoords(vector<TexCoord2D> a)
        {texCoords = a;}
        void addTexCoord(TexCoord2D a)
        {texCoords.push_back(a);}

};