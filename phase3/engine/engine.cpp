#include "engine.hpp"
using namespace std;
using namespace tinyxml2;

float posx=0,posy=0,posz=0,angle=0,scalex=1,scaley=1,scalez=1;
float alphas = 0.0f;
float betas = 0.0f;
float rad = 9.0f;
float camX, camY, camZ;

// Mouse control variables
int startX, startY, tracking = 0;
float sensitivity = 0.1f;
float sensitivityScale = 1.0f; // Sensitivity scale factor, adjust as needed

float t_seg = 0;
//std::chrono::time_point<std::chrono::high_resolution_clock> startTime;

int startTime = 0, elapsedTime = 0;

int showCRC = 0;

XML_WORLD xml;

// VBOs ///////////////////////
vector<float> vertices;
GLuint verticeCount, buffers[1];
///////////////////////////////

float (*prev_y)[3] = nullptr;





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

void calcVerticesTransforms(vector<Ponto> points, vector<Transform> transforms, float current_time, int id){
	if(!points.empty()){
		tMat transformsM = calcTransformsMatrix(transforms, current_time, showCRC, prev_y[id]);
		
		for(Ponto p : points){
			p = applyMatrixToPoint(transformsM, p);
			vertices.push_back(p.x);
			vertices.push_back(p.y);
			vertices.push_back(p.z);
			verticeCount++;
		}
	}
}

void fillVertices(vector<Figure> models, float current_time) {
    for(Figure f : models){
		//printf("Model id: %d\n", f.getId());
        calcVerticesTransforms(f.getPontos(), f.getTransforms(), current_time, f.getId());
		if(!f.getFiguras().empty()){
			fillVertices(f.getFiguras(), current_time);
		}
    }
}

int countModels(vector<Figure> models){
	int count = 0;
	for(Figure f : models){
		if(f.getId() > -1){
			count++;
		}
		if(!f.getFiguras().empty()){
			int aux = countModels(f.getFiguras());
			count += aux;
		}
	}
	return count;
}


//isto dá set ao prev_y, sendo o número de linhas igual ao número de figuras com modelos
//serve para guardar o (y_i-1) para o align dos modelos na catmull-rom curve

//o primeiro valor (ou seja, y_0) é {0,1,0} para todos os modelos

//depois, para se ir buscar o prev_y de um modelo, basta fazer prev_y[id_do_modelo]
//sendo o id_do_modelo dado no parser do xml, e sendo > -1 caso haja modelos, e -1 caso não haja
void set_prev_y(vector<Figure> models, int modelCount){
	if(modelCount > 0){
		float (*pts)[3] = new float[modelCount][3];

		int pc = 0;
		for(int i = 0; i < modelCount; i++){
			float aux[3] = {0,1,0};
			memcpy(pts[i], aux, sizeof(aux));
		}
		delete[] prev_y;
		prev_y = pts;
		
	}
}



// VBOs ///////////////////////
void initGL(){
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnableClientState(GL_VERTEX_ARRAY);
	vertices.clear();

	int modelsCount = countModels(xml.models);
	set_prev_y(xml.models, modelsCount);

	fillVertices(xml.models, 0);
	//startTimer();
	startTime = glutGet(GLUT_ELAPSED_TIME);
	glGenBuffers(1, buffers);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticeCount * 3, vertices.data(), GL_STATIC_DRAW);
}

void drawVBO(){
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, verticeCount);
}

void updateVerticesAtRenderTime() {
    // Calculate time elapsed since the start
    float t_seg = (float)(glutGet(GLUT_ELAPSED_TIME) - startTime) / 1000.0f;

    vertices.clear();
    verticeCount = 0;

    fillVertices(xml.models, t_seg);

    // Bind the existing buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * verticeCount * 3, vertices.data());
}


///////////////////////////////

void renderScene(void) {

	// set the background color to white
    //glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	camX = rad * cosf(betas) * sinf(alphas);
	camY = rad * sinf(betas);
	camZ = rad * cosf(betas) * cosf(alphas);
	gluLookAt(camX, camY, camZ,
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
		//Reset color
		glColor3f(1.0f, 1.0f, 1.0f);
	glEnd();
    

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	updateVerticesAtRenderTime();
	drawVBO();

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
        rad -= 0.5f;
        break;

    case '-':
        rad += 0.5f;
        break;
	case 'c':
		showCRC = !showCRC;
		break;
    }

    glutPostRedisplay();
}





void processMouseButtons(int button, int state, int xx, int yy) {
	if (state == GLUT_DOWN)  {
		startX = xx;
		startY = yy;
		if (button == GLUT_LEFT_BUTTON)
			tracking = 1;
		else if (button == GLUT_RIGHT_BUTTON)
			tracking = 2;
		else
			tracking = 0;
	}
	else if (state == GLUT_UP) {
		if (tracking == 1) {
			alphas += (xx - startX);
			betas += (yy - startY);
		}
		else if (tracking == 2) {
			
			rad -= yy - startY;
			if (rad < 3)
				rad = 3.0;
		}
		tracking = 0;
	}
}

void processMouseMotion(int xx, int yy) {
	int deltaX, deltaY;
    if (!tracking)
        return;

    deltaX = xx - startX;
    deltaY = yy - startY;

    // Adjust sensitivity based on the current radius
    float effectiveSensitivity = sensitivity * sensitivityScale * (1.0f / rad);

    if (tracking == 1) {
        alphas += deltaX * effectiveSensitivity;
        betas += deltaY * effectiveSensitivity;
        if (betas > 85.0f)
            betas = 85.0f;
        else if (betas < -85.0f)
            betas = -85.0f;
    } else if (tracking == 2) {
        rad -= deltaY * effectiveSensitivity;
        if (rad < 1.0f)
            rad = 1.0f;
    }

    camX = rad * cosf(betas) * sinf(alphas);
    camY = rad * sinf(betas);
    camZ = rad * cosf(betas) * cosf(alphas);

    startX = xx;
    startY = yy;

    glutPostRedisplay();
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
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);
	
// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);

//  OpenGL settings
#ifndef __APPLE__	
// init GLEW
	glewInit();
#endif	
	
	initGL();
	
// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
