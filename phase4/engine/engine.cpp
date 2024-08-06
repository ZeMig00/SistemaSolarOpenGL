#include "engine.hpp"
#include <IL/il.h>
#include <unordered_map>

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
vector<float> vertices, normalsV, textures;
GLuint verticeCount, buffers[3]; // buffers[0] = vertices, buffers[1] = normals, buffers[2] = texture coordinates
///////////////////////////////

std::unordered_map<std::string, unsigned int> textures_ids = { {"", 0} };
float (*prev_y)[3] = nullptr;
vector<vector<float>> transformsMatVector;

int current_vertex = 0; // para desenhar o VBO modelo a modelo
						// utilizado no fillVerticesAndUpdateTransformsMatrices para substituir os vértices de apenas os modelos dinâmicos (final current_vertex = verticeCount * 3)
						// utilizado no drawVBOs para desenhar os modelos um a um (final current_vertex = verticeCount)


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
	gluPerspective(xml.cameraFov ,ratio, xml.cameraNear, xml.cameraFar);
    //printf("%f %f %f\n", xml.cameraFov, xml.cameraNear, xml.cameraFar);
    //gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

GLuint loadTexture(std::string file_path){
    GLuint texID;
    unsigned char *texData;
    unsigned int t, tw, th;

    ilInit();

    //coloca a origem da textura no canto inferior esquerdo do primeiro quadrante
    ilEnable(IL_ORIGIN_SET);
    ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

    //Coloca a textura na memória
    ilGenImages(1, &t);
    ilBindImage(t);
	if (ilLoadImage((ILstring)file_path.c_str()) == false) {
		printf("Erro: ilLoadImage(\"%s\")\n", file_path.c_str());
		return 0;
	}
    tw = ilGetInteger(IL_IMAGE_WIDTH);
    th = ilGetInteger(IL_IMAGE_HEIGHT);

    //Assegura que está em RGBA
	if (ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE) == false) {
		printf("Erro: ilConvertImage(\"%s\")\n", file_path.c_str());
		return 0;
	}
    texData = ilGetData();

    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    return texID;
}

void print_transformsMatVector(){
	for(int id = 0; id < transformsMatVector.size(); id++){
		printf("Model id: %d\n", id);
		for(int i = 0; i < 4; i++){
			for(int j = 0; j < 4; j++){
				printf("%f ", transformsMatVector[id][i*4+j]);
			}
			printf("\n");
		}
		printf("\n");
	}

}

void fillVectorsForVBOs(vector<Ponto> points, vector<Normal> normals, vector<Transform> transforms, int id, vector<TexCoord2D> texCoords){
	// preenche o vetor de vértices com os pontos do modelo, e o vetor de normais com as normais do modelo
	// e atualiza o vetor de matrizes de transformações com a matriz de transformações do modelo
	if(!points.empty()){
		tMat transformsM = calcTransformsMatrix(transforms, 0, showCRC, prev_y[id]);
		
		for(int i = 0; i < points.size(); i++){
			vertices.push_back(points[i].x);
			vertices.push_back(points[i].y);
			vertices.push_back(points[i].z);
			verticeCount++;

			if(normals.size() > 0){
				normalsV.push_back(normals[i].x);
				normalsV.push_back(normals[i].y);
				normalsV.push_back(normals[i].z);
			}
		}
		transformsMatVector[id] = tMatToVector(transformsM);
	}

	// Carregar vector de texturas
	for(auto t: texCoords) {
		textures.push_back(t.s);
		textures.push_back(t.t);
	}
}

void updateTransformationMatrices(vector<Transform> transforms, float current_time, int id){
	// apenas dá update às matrizes de transformações dos modelos com animações.
	// apenas é utilizada para current_time > 0
	if(!transforms.empty()){
		tMat transformsM = calcTransformsMatrix(transforms, current_time, showCRC, prev_y[id]);
		
		transformsMatVector[id] = tMatToVector(transformsM);
	}
}

void fillVerticesAndUpdateTransformsMatrices(vector<Figure> models, float current_time) {
    for(Figure f : models){
		//printf("Model id: %d\n", f.getId());
		if((current_time == 0) && f.getId() > -1){
			
			// carregar texturas
			if( f.getTexture().size() > 0){
				for(string s : f.getTexture()){
					if(s != ""){
						auto t = loadTexture(s);
						textures_ids[s] = t;
					}
				}
			}
			// in the start, it will fill the vertice array with all vertices, and the transformsMatVector with
			// the transformation matrices for each model. But after that, it will only update the transformation matrices
			// only for the dynamic models
			
        	fillVectorsForVBOs(f.getPontos(), f.getNormals(), f.getTransforms(), f.getId(), f.getTexCoords());
		}
		else if ((f.isDynamic() == 1) && f.getId() > -1){
			// fiz esta separação para que, após o instante 0, apenas os modelos com animação
			// tenhas as suas matrizes de transformações atualizadas
			updateTransformationMatrices(f.getTransforms(), current_time, f.getId());
		}
		
		if(!f.getFiguras().empty()){
			fillVerticesAndUpdateTransformsMatrices(f.getFiguras(), current_time);
		}
    }
}

int countModels(vector<Figure> models){
	int count = 0;
	for(Figure& f : models){
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

void init_transformsMatVector(vector<Figure> models){
	for(Figure& f : models){
		if(f.getId() > -1){
			transformsMatVector.push_back(tMatToVector(tMat_identity()));
		}
		if(!f.getFiguras().empty()){
			init_transformsMatVector(f.getFiguras());
		}
	}
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

void debugNormals(vector<Figure> models){// para debug das normais (funciona bem apenas em modelos estáticos)
	glDisable(GL_LIGHTING);
	for(Figure& f : models){ 
		if((f.getId() > -1) && (!f.getNormals().empty())){
			
			for(int i = current_vertex; i < current_vertex + (f.getPontos().size() * 3); i+=3){
				Ponto p = {vertices[i], vertices[i+1], vertices[i+2]};
				Ponto n = {normalsV[i], normalsV[i+1], normalsV[i+2]};
				tMat m = vectorTotMat(transformsMatVector[f.getId()]);
				applyMatrixToPoint(m, p, 0); 
				applyMatrixToPoint(m, n, 1);

				glBegin(GL_LINES);
					glVertex3f(p.x, p.y, p.z);
					glVertex3f(p.x + n.x, p.y + n.y, p.z + n.z);
				glEnd();
			}
		}
		current_vertex += f.getPontos().size() * 3;
		if(!f.getFiguras().empty()){
			debugNormals(f.getFiguras());
		}
	}
	glEnable(GL_LIGHTING);
}

void enableLights(vector<Light> lights){
	if(!lights.empty()){
		glEnable(GL_LIGHTING);
		//ISTO TAVA NO FICHEIRO "notes for phase IV" NOS FICHEIROS DE TESTE DO STOR
		glEnable(GL_RESCALE_NORMAL);
		float amb[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);

		int i = 0;
		for (Light light : lights) {
			if(i == 8){ // tem de ter no máximo 8 luzes
				break;
			}
			glEnable(GL_LIGHT0 + i);
			float white[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
			glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, white);
			glLightfv(GL_LIGHT0 + i, GL_SPECULAR, white);

			i++;
		}
	}
}

void setupLights(vector<Light> lights) {
	if(!lights.empty()){
		int i = 0;
		for (Light light : lights) {
			if(i == 8){ // tem de ter no máximo 8 luzes
				break;
			}
				
			if (light.getLightType() == "point") {
				GLfloat pos[4] = { light.getLightSettings().at(0), light.getLightSettings().at(1) ,light.getLightSettings().at(2), 1.0 };
				glLightfv(GL_LIGHT0 + i, GL_POSITION, pos);
			}
			else if (light.getLightType() == "directional") {
				GLfloat dir[4] = { light.getLightSettings().at(0), light.getLightSettings().at(1) ,light.getLightSettings().at(2), 0.0};
				glLightfv(GL_LIGHT0 + i, GL_POSITION, dir);
			}
			else {
				GLfloat pos[4] = { light.getLightSettings().at(0), light.getLightSettings().at(1) ,light.getLightSettings().at(2), 1.0 };
				GLfloat spotDir[4] = { light.getLightSettings().at(3), light.getLightSettings().at(4) ,light.getLightSettings().at(5), 0.0};
				GLfloat cutoff = light.getLightSettings().at(6);
				glLightfv(GL_LIGHT0 + i, GL_POSITION, pos);
				glLightfv(GL_LIGHT0 + i, GL_SPOT_DIRECTION, spotDir);
				glLightf(GL_LIGHT0 + i, GL_SPOT_CUTOFF, cutoff);
				glLightf(GL_LIGHT0 + i, GL_SPOT_EXPONENT, 0.0);
			}
			i++;
		}
	}
}

// VBOs ///////////////////////
void initGL(){
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	enableLights(xml.sceneLights);
	//////////////////////////////////////////////////////////////////

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	vertices.clear();
	normalsV.clear();

	int modelsCount = countModels(xml.models);
	set_prev_y(xml.models, modelsCount);
	init_transformsMatVector(xml.models);

	fillVerticesAndUpdateTransformsMatrices(xml.models, 0);

	//print_transformsMatVector();
	//startTimer();
	startTime = glutGet(GLUT_ELAPSED_TIME);
	glGenBuffers(3, buffers);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticeCount * 3, vertices.data(), GL_STATIC_DRAW);
	if(!normalsV.empty()){
		glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * normalsV.size(), normalsV.data(), GL_STATIC_DRAW);
	}
	if(!textures.empty()){
		glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * textures.size(), textures.data(), GL_STATIC_DRAW);
	}
}

void drawVBOs(vector<Figure> models){
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glNormalPointer(GL_FLOAT, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
	glTexCoordPointer(2, GL_FLOAT, 0, 0);

	for(Figure& f : models){ // coloquei a desenhar os modelos um a um,
		if(f.getId() > -1){ // de modo a que se possa aplicar diferentes texturas em diferentes modelos
			for(int i = 0; i < f.getCountPerModel().size(); i++){
				glPushMatrix();
					if (f.getTexture()[i] != "" && textures_ids.find(f.getTexture()[i]) != textures_ids.end()) {
						auto tid = textures_ids[f.getTexture()[i]];
						glBindTexture(GL_TEXTURE_2D, tid);
					} else {
						//esta condicao nunca deve acontecer, no entanto, se acontecer remove a textura
						glBindTexture(GL_TEXTURE_2D, 0);
					}

					float light_amb[4] = {f.getColor()[i].getAmbient().r, f.getColor()[i].getAmbient().g, f.getColor()[i].getAmbient().b, 1.0};
					glMaterialfv(GL_FRONT, GL_AMBIENT, light_amb);

					float light_dif[4] = {f.getColor()[i].getDiffuse().r, f.getColor()[i].getDiffuse().g, f.getColor()[i].getDiffuse().b, 1.0};
					glMaterialfv(GL_FRONT, GL_DIFFUSE, light_dif);

					float light_spe[4] = {f.getColor()[i].getSpecular().r, f.getColor()[i].getSpecular().g , f.getColor()[i].getSpecular().b, 1.0};
					glMaterialfv(GL_FRONT, GL_SPECULAR, light_spe);

					float light_em[4] = {f.getColor()[i].getEmissive().r, f.getColor()[i].getEmissive().g, f.getColor()[i].getEmissive().b, 1.0};
					glMaterialfv(GL_FRONT, GL_EMISSION, light_em);

					glMaterialf(GL_FRONT, GL_SHININESS, f.getColor()[i].getShininess());

					glMultMatrixf(transformsMatVector[f.getId()].data());
					glDrawArrays(GL_TRIANGLES, current_vertex, f.getCountPerModel()[i]);
					current_vertex += f.getCountPerModel()[i];

				glPopMatrix();
			}
			
		}
		if(!f.getFiguras().empty()){
			drawVBOs(f.getFiguras());
		}
	}	
}

void updateTMatricesAtRenderTime(){
	// Calculate time elapsed since the start
    float t_seg = (float)(glutGet(GLUT_ELAPSED_TIME) - startTime) / 1000.0f;

    fillVerticesAndUpdateTransformsMatrices(xml.models, t_seg);
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

    glDisable(GL_LIGHTING);
    glBegin(GL_LINES); //eixos x y z
		// X axis in red
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-10000.0f, 0.0f, 0.0f);
		glVertex3f( 10000.0f, 0.0f, 0.0f);
		// Y Axis in Green
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.0f, -10000.0f, 0.0f);
		glVertex3f(0.0f, 10000.0f, 0.0f);
		// Z Axis in Blue
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, -10000.0f);
		glVertex3f(0.0f, 0.0f, 10000.0f);
		//Reset color
		glColor3f(1.0f, 1.0f, 1.0f);
	glEnd();
	glEnable(GL_LIGHTING);

	setupLights(xml.sceneLights);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	updateTMatricesAtRenderTime();

	drawVBOs(xml.models);
	current_vertex = 0;

	// debugNormals(xml.models);
	// current_vertex = 0;

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