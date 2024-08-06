#include "figure.h"

using namespace std;

    Figure::Figure(){
        pontos = vector<Ponto>();
        //color = Color::Color();
        figuras = vector<Figure>();
        modelFiles = vector<string>();
    }

    Figure::Figure(vector<Ponto> a, vector<string> b, vector<Figure> c){
        pontos = a;
        modelFiles = b;
        figuras = c;
    }