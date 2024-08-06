#pragma once
#include <string>
#include <vector>

struct Ponto{
    float x;
    float y;
    float z;
};

using namespace std;

class Figure{
    vector<Ponto> pontos;
    //Color color;
    vector<string> modelFiles; //Isto tem todos os modelos so precisa de ter um 
    vector<Figure> figuras;

    public:
        Figure();
        Figure(vector<Ponto>, vector<string>,vector<Figure>);

        vector<Ponto> getPontos()
        {return pontos;}
        void setPontos(vector<Ponto> a)
        {pontos = a;}
        void addPonto(Ponto a)
        {pontos.push_back(a);}

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

};