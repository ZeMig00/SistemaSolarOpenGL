#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <cstring>

using namespace std;

struct Ponto{
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

};