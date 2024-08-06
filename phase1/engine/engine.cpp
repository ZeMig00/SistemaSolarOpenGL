#include "engine.hpp"
using namespace std;
using namespace tinyxml2;

float posx=0,posy=0,posz=0,angle=0,scalex=1,scaley=1,scalez=1;
float alphas = 0.0f;
float betas = 0.0f;
float rad = 9.0f;

struct World {
    int windowWidth;
    int windowHeight;
    float cameraPosX;
    float cameraPosY;
    float cameraPosZ;
    float cameraLookAtX;
    float cameraLookAtY;
    float cameraLookAtZ;
    float cameraUpX;
    float cameraUpY;
    float cameraUpZ;
    float cameraFov;
    float cameraNear;
    float cameraFar;
    vector<Figure> models;
}xml;

void lerXML(string ficheiro) {
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
		//cout << "DEBUG : camera lida" << endl;
		XMLElement* grupoElem = root->FirstChildElement("group");
        //cout << "DEBUG : firstchildelement(group)" << endl;
		while(grupoElem != nullptr){
			XMLElement* modelsElem = grupoElem->FirstChildElement("models");
            //cout << "DEBUG : firstchildelement(models)" << endl;
            while(modelsElem != nullptr){
                XMLElement* modelElem = modelsElem->FirstChildElement("model");
                //cout << "DEBUG : firstchildelement(model)" << endl;
                while(modelElem != nullptr){
                    string ficheiro = modelElem->Attribute("file");
                    //cout << "DEBUG : attribute(file)" << endl;
                    Figure faux;
                    faux.addModelFile("../3d_files/" + ficheiro);
                    xml.models.push_back(faux);
                    modelElem = modelElem->NextSiblingElement("model");
                }
                modelsElem = modelsElem->NextSiblingElement("models");
            }
            grupoElem = grupoElem->NextSiblingElement("group");
		}
        //cout << "DEBUG : grupo lido" << endl;
	}
	else {
        cout << "Erro ao ler o xml" << ": " << ficheiro << endl;
	}
}

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(/*(GLdouble)*/ xml.cameraFov ,ratio, /*(GLdouble)*/ xml.cameraNear ,/*(GLdouble)*/ xml.cameraFar);
    printf("%f %f %f\n", xml.cameraFov, xml.cameraNear, xml.cameraFar);
    //gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}


void drawFigures() {
    for(Figure f : xml.models){
        for(string s : f.getModelFiles()){
            vector<Ponto> points;
            ifstream file(s);
            //printf("DEBUG : file read\n");
            if(file.is_open()){
                
                string line;
                while(getline(file, line)){
                    Ponto p;
                    sscanf(line.c_str(), "%f %f %f", &p.x, &p.y, &p.z);
                    //printf("DEBUG : %f %f %f\n", p.x, p.y, p.z);
                    points.push_back(p);
                    
                }
                
                file.close();
            }
            else{
                cout << "Erro ao abrir o ficheiro" << endl;
            }

            glBegin(GL_TRIANGLES);
                glColor3f(1,1,1);
                for(Ponto p : points){
                    glVertex3f(p.x, p.y, p.z);
                }
            glEnd();
        }
    }
}    


void renderScene(void) {

	// set the background color to white
    //glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(rad * cosf(betas) * sinf(alphas), rad * sinf(betas), rad * cosf(betas) * cosf(alphas),
              xml.cameraLookAtX, xml.cameraLookAtY, xml.cameraLookAtZ,
              xml.cameraUpX, xml.cameraUpY, xml.cameraUpZ);
    //gluLookAt(5,5,5,0,0,0,0,1,0);

    
    //eixos x y z
    glBegin(GL_LINES);
		// X axis in red
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-100.0f, 0.0f, 0.0f);
		glVertex3f( 100.0f, 0.0f, 0.0f);
		// Y Axis in Green
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.0f, -100.0f, 0.0f);
		glVertex3f(0.0f, 100.0f, 0.0f);
		// Z Axis in Blue
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, -100.0f);
		glVertex3f(0.0f, 0.0f, 100.0f);
	glEnd();
    

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	drawFigures();

	// End of frame
	glutSwapBuffers();
}

void processKeys(unsigned char c, int xx, int yy) {
    switch (c) {
        // Move camera up
    case 'w': case 'W':
        betas += 0.1f;
        break;
        // Move camera down
    case 's': case 'S':
        betas -= 0.1f;
        break;

    case 'a': case 'A':
        alphas += 0.1f;
        break;

    case 'd': case 'D':
        alphas -= 0.1f;
        break;
    
    case '+':
        rad -= 0.1f;
        break;

    case '-':
        rad += 0.1f;
        break;
    }

    glutPostRedisplay();
}


void processKeysX(unsigned char c, int xx, int yy) {
    switch (c)
	{
	case 'a':
		posx -= 0.1;
		break;
	case 'd':
		posx += 0.1;
		break;
	case 's':
		posz += 0.1;
		break;
	case 'w':
		posz -= 0.1;
		break;
	case 'q':
		angle -= 15;
		break;
	case 'e':
		angle += 15;
		break;
	case 'i':
		scalez += 0.1;
		break;
	case 'k':
		scalez -= 0.1;
		break;
	case 'j':
		scalex -= 0.1;
		break;
	case 'l':
		scalex += 0.1;
		break;
	case 'u':
		scaley -= 0.1;
		break;
	case 'o':
		scaley += 0.1;
		break;
	case 't':
		posy += 0.1;
		break;
	case 'g':
		posy -= 0.1;
		break;
	}
	glutPostRedisplay();
}



void processSpecialKeys(int key, int xx, int yy) {

// put code to process special keys in here

}


int main_engine(int argc, char *argv[]) {

    if(argc > 2){
        string ficheiro_cpp(argv[2]);
        lerXML("../xml_files/" + ficheiro_cpp);
    }
    else{
        cout << "Falta o ficheiro xml" << endl;
        return 0;
    
    }

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(xml.windowWidth,xml.windowHeight);
    //glutInitWindowSize(800,800);
	glutCreateWindow("CG@DI-UM");
		
// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	
// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);

//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
