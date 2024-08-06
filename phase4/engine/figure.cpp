#include "figure.h"

using namespace std;

    Figure::Figure(){
        pontos = vector<Ponto>();
        transforms = vector<Transform>();
        modelFiles = vector<string>();
        figuras = vector<Figure>();
        color = vector<Color>();
        texture = vector<string>();
        CountPerModel = vector<int>();
        normals = vector<Normal>();
        texCoords = vector<TexCoord2D>();
        
    }

    Figure::Figure(vector<Ponto> a, vector<Transform> t, vector<string> b, vector<Figure> c){
        pontos = a;
        transforms = t;
        modelFiles = b;
        figuras = c;
        color = vector<Color>();
        texture = vector<string>();
        CountPerModel = vector<int>();
        normals = vector<Normal>();
        texCoords = vector<TexCoord2D>();
    }

    void Figure::printFigure(int tabs, int tabsout){
        stringstream ss;
        for(int i = 0; i < tabs; i++){
            ss << "\t";
        }
        stringstream sso;
        for(int i = 0; i < tabsout; i++){
            sso << "\t";
        }
        printf("%s", sso.str().c_str());
        printf("%s Figure %d: ", dynamic ? "Dynamic" : "Static",id);

        printf("Model Files: ");
        for(string s : modelFiles){
            printf("%s ", s.c_str());
        }
        printf("\n");
        
        if(id > -1){
            printf("%s", sso.str().c_str());
            printf("\tPoints:\n");
                printf("\t");
                printf("%s", ss.str().c_str());
                printf("Number of Points: %d\n", (int)pontos.size());

            printf("%s", sso.str().c_str());
            printf("\tNormals:\n");
                printf("\t");
                printf("%s", ss.str().c_str());
                printf("Number of Normals: %d\n", (int)normals.size());

            printf("%s", sso.str().c_str());
            stringstream ss;
            for(string s : texture){
                ss << s << " ";
            }
            
            printf("\tTextureCoords for texture -> %s:\n", ss.str().c_str());
                printf("\t");
                printf("%s", ss.str().c_str());
                printf("Number of TexCoords: %d\n", (int)texCoords.size());
            
            printf("%s", sso.str().c_str());
            for(Color col: color){
                printf("\tColor:\n");
                col.printColor("\t" + ss.str());
                printf("\n%s", sso.str().c_str());
            }
            
        }
        
        printf("%s", sso.str().c_str());
        printf("\tTransforms:\n");
        for(Transform s : transforms){
            printf("\t");
            printf("%s", ss.str().c_str());
            printf("%s ", s.type.c_str());
            for(float n : s.arguments){
                printf("%lf ", n);
            }
            printf("\nTime: %lf\n", s.time);
            if(s.align){
                printf("Align: true\n");
            }else{
                printf("Align: false\n");
            }
            printf("\n");
        }
        
        printf("%s", sso.str().c_str());
        tabsout++;
        tabs++;
        for(Figure f : figuras){
            printf("\t\n");
            //printf("\t");
            f.printFigure(tabs, tabsout);
        }
    }