#include "parser.hpp"

using namespace std;
using namespace tinyxml2;

int idTracker = 0;


int containsAnimation(vector<Transform> transforms){
    for(Transform t : transforms){
        if(t.type == "translate t" || t.type == "rotate t"){
            return 1;
            break;
        }
    }
    return 0;

}

void addFatherTransforms(Figure &figura, vector<Transform> transforms){
    if(transforms.empty()){
        transforms = figura.getTransforms();
    }
    else{
        for(Transform t : figura.getTransforms()){
            transforms.push_back(t);
        }
        figura.setTransforms(transforms);
    }

    if(containsAnimation(transforms)){
        figura.setDynamic(1);
    }

    vector<Figure> vFigs;
    for(Figure f : figura.getFiguras()){
        addFatherTransforms(f, transforms);
        vFigs.push_back(f);
    }
    figura.setFiguras(vFigs);
}

void fillFigurePointsNormalsTexCoords(Figure &f){
	if(!f.getModelFiles().empty()){
        vector<Ponto> points;
        vector<Normal> normals;
        vector<TexCoord2D> texcoords;
        int countPM = 0;
        for(string s : f.getModelFiles()){
            ifstream file(s);
            //printf("DEBUG : file read\n");
            if(file.is_open()){
                
                string line;
                while(getline(file, line)){
                    Ponto p;
                    Normal n;
                    TexCoord2D t;
                    
                    int nr = sscanf(line.c_str(), "%f %f %f %f %f %f %f %f", &p.x, &p.y, &p.z, &n.x, &n.y, &n.z, &t.s, &t.t);
                    
                    switch(nr){
                        case 3:
                            points.push_back(p);
                            countPM++;
                            break;
                        
                        case 6:
                            points.push_back(p);
                            normals.push_back(n);
                            countPM++;
                            break;
                        
                        case 8:
                            points.push_back(p);
                            normals.push_back(n);
                            texcoords.push_back(t);
                            countPM++;
                            break;
                        
                        default:
                            cout << "Erro ao ler o ficheiro" << endl;
                            break;
                    }
                    
                }
                file.close();
            }
            else{
                cout << "Erro ao abrir o ficheiro" << endl;
            }
            f.addCountPerModel(countPM);
            countPM = 0;
        }
        f.setPontos(points);
        f.setNormals(normals);
        f.setTexCoords(texcoords);
        
    }
    if(!f.getFiguras().empty()){
        vector<Figure> vFigs;
        for(Figure fig : f.getFiguras()){
            fillFigurePointsNormalsTexCoords(fig);
            vFigs.push_back(fig);
        }
        f.setFiguras(vFigs);
    }
}



void lerModelTextureColor(XMLElement* modelElem, Figure &figura){
    string ficheiro = modelElem->Attribute("file");
    figura.addModelFile("../3d_files/" + ficheiro);
    XMLElement* textureElem = modelElem->FirstChildElement("texture");
    if(textureElem != nullptr){
        string texture = textureElem->Attribute("file");
        figura.addTexture("../textures/" + texture);
    }
    else{
        figura.addTexture("");
    }

    

    RGB d, a, s, e;
    float sh;
    
    XMLElement* colorElem = modelElem->FirstChildElement("color");
    if(colorElem != nullptr){
        XMLElement* diffuseElem = colorElem->FirstChildElement("diffuse");
        if(diffuseElem != nullptr){
            float r, g, b;
            diffuseElem->QueryFloatAttribute("R", &r);
            diffuseElem->QueryFloatAttribute("G", &g);
            diffuseElem->QueryFloatAttribute("B", &b);
            d = {(float)r/255, (float)g/255, (float)b/255};
        }
        XMLElement* ambientElem = colorElem->FirstChildElement("ambient");
        if(ambientElem != nullptr){
            float r, g, b;
            ambientElem->QueryFloatAttribute("R", &r);
            ambientElem->QueryFloatAttribute("G", &g);
            ambientElem->QueryFloatAttribute("B", &b);
            a = {(float)r/255, (float)g/255, (float)b/255};
        }
        XMLElement* specularElem = colorElem->FirstChildElement("specular");
        if(specularElem != nullptr){
            float r, g, b;
            specularElem->QueryFloatAttribute("R", &r);
            specularElem->QueryFloatAttribute("G", &g);
            specularElem->QueryFloatAttribute("B", &b);
            s = {(float)r/255, (float)g/255, (float)b/255};
        }
        XMLElement* emissiveElem = colorElem->FirstChildElement("emissive");
        if(emissiveElem != nullptr){
            float r, g, b;
            emissiveElem->QueryFloatAttribute("R", &r);
            emissiveElem->QueryFloatAttribute("G", &g);
            emissiveElem->QueryFloatAttribute("B", &b);
            e = {(float)r/255, (float)g/255, (float)b/255};
        }
        XMLElement* shininessElem = colorElem->FirstChildElement("shininess");
        if(shininessElem != nullptr){
            shininessElem->QueryFloatAttribute("value", &sh);
        }
        
        figura.addColor(Color(d, a, s, e, sh));

        colorElem = colorElem->NextSiblingElement("color");
    }
    else{
        figura.addColor(Color());
    }

}

void lerGrupoXML(XMLElement* grupoElem, Figure &figura) {
    //printf("igroup\n");
    XMLElement* transformElem = grupoElem->FirstChildElement("transform");
    if(transformElem != nullptr){
        XMLElement* tElem = transformElem->FirstChildElement();
        while(tElem != nullptr){
            //printf("itransforms\n");
            string transform = tElem->Name();
            if(strcmp(transform.c_str(), "translate") == 0){
                //printf("itransforms->translate\n");
                vector<float> argss;
                float x, y, z;
                float time = 0;
                bool align = false;

                if(tElem->QueryFloatAttribute("time", &time) == XML_SUCCESS){
                    // translate catmull-rom ///////////////////////////////////////
                    transform.append(" t");
                    
                    const char *alignRes = tElem->Attribute("align");
                    string alignStr(alignRes);
                    for (char& c : alignStr) {
                        if (c >= 'A' && c <= 'Z') {
                            c += ('a' - 'A');
                        }
                    }
                    if(alignStr == "true"){
                        align = true;
                    }

                    XMLElement* pointElem = tElem->FirstChildElement("point");
                    while(pointElem != nullptr){
                        pointElem->QueryFloatAttribute("x", &x); argss.push_back(x);
                        pointElem->QueryFloatAttribute("y", &y); argss.push_back(y);
                        pointElem->QueryFloatAttribute("z", &z); argss.push_back(z);
                        pointElem = pointElem->NextSiblingElement("point");
                    }
                }
                else{
                    // translate normal ///////////////////////////////////////
                    tElem->QueryFloatAttribute("x", &x); argss.push_back(x);
                    tElem->QueryFloatAttribute("y", &y); argss.push_back(y);
                    tElem->QueryFloatAttribute("z", &z); argss.push_back(z);
                }
                Transform tAux = {transform, argss, time, align};
                figura.addTransform(tAux);
            }
            else if(strcmp(transform.c_str(), "rotate") == 0){
                //printf("itransforms->rotate\n");
                vector<float> argss;
                float angle,x,y,z;
                float time = 0;
                bool align = false;

                if(tElem->QueryFloatAttribute("time", &time) == XML_SUCCESS){
                    // rotate catmull-rom //////////////////////////////////
                    transform.append(" t");
                }
                else{
                    // rotate normal ///////////////////////////////////////
                    tElem->QueryFloatAttribute("angle", &angle); argss.push_back(angle);
                }
                tElem->QueryFloatAttribute("x", &x); argss.push_back(x);
                tElem->QueryFloatAttribute("y", &y); argss.push_back(y);
                tElem->QueryFloatAttribute("z", &z); argss.push_back(z);
                Transform tAux = {transform, argss, time, align};
                figura.addTransform(tAux);
            }
            else if(strcmp(transform.c_str(), "scale") == 0){
                //printf("itransforms->scale\n");
                vector<float> argss;
                float x,y,z;
                float time = 0;
                bool align = false;
                tElem->QueryFloatAttribute("x", &x); argss.push_back(x);
                tElem->QueryFloatAttribute("y", &y); argss.push_back(y);
                tElem->QueryFloatAttribute("z", &z); argss.push_back(z);
                Transform tAux = {transform, argss, time, align};
                figura.addTransform(tAux);
            }
            tElem = tElem->NextSiblingElement();
        }
    }

	XMLElement* modelsElem = grupoElem->FirstChildElement("models");
    //cout << "DEBUG : firstchildelement(models)" << endl;
    while(modelsElem != nullptr){
        //printf("imodels\n");
        figura.setId(idTracker);
        idTracker++;
        XMLElement* modelElem = modelsElem->FirstChildElement("model");
        //cout << "DEBUG : firstchildelement(model)" << endl;
        while(modelElem != nullptr){
            lerModelTextureColor(modelElem, figura);
            
            modelElem = modelElem->NextSiblingElement("model");
        }
        modelsElem = modelsElem->NextSiblingElement("models");
    }
    XMLElement* childGrupoElem = grupoElem->FirstChildElement("group");
    while(childGrupoElem != nullptr){
            //printf("iChildGroup\n");
            Figure fAux;
		    lerGrupoXML(childGrupoElem, fAux);
            figura.addFigura(fAux);
            childGrupoElem = childGrupoElem->NextSiblingElement("group");
    }
}


void lerXML(string ficheiro, XML_WORLD &xml, float &rad, float &betas, float &alphas)
{
    XMLDocument doc;
	if (!(doc.LoadFile(ficheiro.c_str()))){  //condicao que carrega o ficheiro e testa se é válido
		//cout << "DEBUG : Ficheiro lido com sucesso" << endl;

		XMLElement* root = doc.RootElement();
        //cout << "DEBUG : root lida" << endl;
        XMLElement* windowElem = root->FirstChildElement("window");
        if (windowElem) {
            windowElem->QueryIntAttribute("width", &xml.windowWidth);
            windowElem->QueryIntAttribute("height", &xml.windowHeight);
        }
        //cout << "DEBUG : window lida" << endl;
        XMLElement* cameraElem = root->FirstChildElement("camera");
        if (cameraElem) {
            XMLElement* posElem = cameraElem->FirstChildElement("position");
            if (posElem){
                posElem->QueryFloatAttribute("x", &xml.cameraPosX);
                posElem->QueryFloatAttribute("y", &xml.cameraPosY);
                posElem->QueryFloatAttribute("z", &xml.cameraPosZ);
		        float distanceFromOrigin = sqrt(pow(xml.cameraPosX - 0, 2) + pow(xml.cameraPosY - 0, 2) + pow(xml.cameraPosZ - 0, 2));
                rad = distanceFromOrigin;
                betas = asinf(xml.cameraPosY / distanceFromOrigin);
                alphas = atan2(xml.cameraPosX / (distanceFromOrigin * cos(betas)), xml.cameraPosZ / (distanceFromOrigin * cos(betas)));

            }
        
            XMLElement* lookAtElem = cameraElem->FirstChildElement("lookAt");
            if (lookAtElem) {
                lookAtElem->QueryFloatAttribute("x", &xml.cameraLookAtX);
                lookAtElem->QueryFloatAttribute("y", &xml.cameraLookAtY);
                lookAtElem->QueryFloatAttribute("z", &xml.cameraLookAtZ);
            }

            XMLElement* upElem = cameraElem->FirstChildElement("up");
            if (upElem) {
                upElem->QueryFloatAttribute("x", &xml.cameraUpX);
                upElem->QueryFloatAttribute("y", &xml.cameraUpY);
                upElem->QueryFloatAttribute("z", &xml.cameraUpZ);
            }

            XMLElement* projElem = cameraElem->FirstChildElement("projection");
            if (projElem) {
                projElem->QueryFloatAttribute("fov", &xml.cameraFov);
                projElem->QueryFloatAttribute("near", &xml.cameraNear);
                projElem->QueryFloatAttribute("far", &xml.cameraFar);
            }
        }

        XMLElement* lightsElem = root->FirstChildElement("lights");
        while(lightsElem != nullptr){
            XMLElement* lightElem = lightsElem->FirstChildElement("light");
            while(lightElem != nullptr){
                string type = lightElem->Attribute("type");
                vector<float> settings;

                if(!type.compare("point")){
                    settings.push_back(lightElem->FloatAttribute("posX"));
                    settings.push_back(lightElem->FloatAttribute("posY"));
                    settings.push_back(lightElem->FloatAttribute("posZ"));
                }
                else if(!type.compare("directional")){
                    settings.push_back(lightElem->FloatAttribute("dirX"));
                    settings.push_back(lightElem->FloatAttribute("dirY"));
                    settings.push_back(lightElem->FloatAttribute("dirZ"));
                }
                else if(!type.compare("spotlight") || !type.compare("spot")){
                    settings.push_back(lightElem->FloatAttribute("posX"));
                    settings.push_back(lightElem->FloatAttribute("posY"));
                    settings.push_back(lightElem->FloatAttribute("posZ"));

                    settings.push_back(lightElem->FloatAttribute("dirX"));
                    settings.push_back(lightElem->FloatAttribute("dirY"));
                    settings.push_back(lightElem->FloatAttribute("dirZ"));

                    settings.push_back(lightElem->FloatAttribute("cutoff"));
                }

                xml.sceneLights.push_back(Light(type, settings));
                lightElem = lightElem->NextSiblingElement("light");
            }
            lightsElem = lightsElem->NextSiblingElement("lights");
        }

		//cout << "DEBUG : camera lida" << endl;
		XMLElement* grupoElem = root->FirstChildElement("group");
        Figure f;
        while(grupoElem != nullptr){
		    lerGrupoXML(grupoElem, f);
            vector<Transform> transforms;
            addFatherTransforms(f, transforms);
            fillFigurePointsNormalsTexCoords(f);
            xml.models.push_back(f);
            grupoElem = grupoElem->NextSiblingElement("group");
        }
        //cout << "DEBUG : firstchildelement(group)" << endl;
        //cout << "DEBUG : grupo lido" << endl;


	}
	else {
        cout << "Erro ao ler o xml" << ": " << ficheiro << endl;
	}
}

void printWorldStructure(XML_WORLD &xml){
    printf("Window : %d %d\n", xml.windowWidth, xml.windowHeight);
    printf("Camera : \n\tPos: %f %f %f \n\tLookAt: %f %f %f \n\tUp: %f %f %f \n\tFOV:%f \n\tNear:%f \n\tFar:%f\n", xml.cameraPosX, xml.cameraPosY, xml.cameraPosZ, xml.cameraLookAtX, xml.cameraLookAtY, xml.cameraLookAtZ, xml.cameraUpX, xml.cameraUpY, xml.cameraUpZ, xml.cameraFov, xml.cameraNear, xml.cameraFar);
    printf("Lights:\n");
    for(Light l : xml.sceneLights){
        l.printLight();
        printf("\n");
    }
    for(Figure f : xml.models){
        f.printFigure(1,0);
        printf("\n");
    }
}
