#include "engine.hpp"
using namespace std;
using namespace tinyxml2;

float posx=0,posy=0,posz=0,angle=0,scalex=1,scaley=1,scalez=1;
float alphas = 0.0f;
float betas = 0.0f;
float rad = 9.0f;

XML_WORLD xml;





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
    //printf("%f %f %f\n", xml.cameraFov, xml.cameraNear, xml.cameraFar);
    //gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

void drawFigure(vector<Ponto> points, vector<Transform> transforms){
	if(!points.empty()){
		if(transforms.empty()){
			glBegin(GL_TRIANGLES);
				glColor3f(1,1,1);
				for(Ponto p : points){
					glVertex3f(p.x, p.y, p.z);
				}
			glEnd();
		}
		else{
			glPushMatrix();
			for(Transform t : transforms){
				if(strcmp(t.type.c_str(), "translate") == 0){
					glTranslatef(t.arguments[0], t.arguments[1], t.arguments[2]);
				}
				else if(strcmp(t.type.c_str(), "rotate") == 0){
					glRotatef(t.arguments[0], t.arguments[1], t.arguments[2], t.arguments[3]);
				}
				else if(strcmp(t.type.c_str(), "scale") == 0){
					glScalef(t.arguments[0], t.arguments[1], t.arguments[2]);
				}
			}

			glBegin(GL_TRIANGLES);
				glColor3f(1,1,1);
				for(Ponto p : points){
					glVertex3f(p.x, p.y, p.z);
				}
			glEnd();

			glPopMatrix();
		}
	}
}

void drawFigures(vector<Figure> models) {
    for(Figure f : models){
        drawFigure(f.getPontos(), f.getTransforms());
		if(!f.getFiguras().empty()){
			drawFigures(f.getFiguras());
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
	drawFigures(xml.models);

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
        lerXML("../xml_files/" + ficheiro_cpp, xml, rad, betas, alphas);
		printWorldStructure(xml);
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
