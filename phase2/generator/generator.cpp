#include "generator.hpp"


void plane(float length, int divisions,char* filename){
    float x = length/2;
    float z = length/2;
    float iter = length/divisions;
    stringstream plane_points;

    for(int i = 0; i < divisions; i++){
        for(int j = 0; j < divisions; j++){
            plane_points << x << " " << 0 << " " << z << "\n";
            plane_points << x << " " << 0 << " " << z - iter << "\n";
            plane_points << x - iter << " " << 0 << " " << z << "\n";

            plane_points << x - iter << " " << 0 << " " << z << "\n";
            plane_points << x << " " << 0 << " " << z - iter << "\n";
            plane_points << x - iter << " " << 0 << " " << z - iter << "\n";

            x -= iter;

        }
        x = length/2;
        z -= iter;
    }

    ofstream file;
    string filename_cpp(filename);
    file.open("../3d_files/" + filename_cpp);
    file << plane_points.str();
    file.close();
}

void box(float length, int divisions,char* filename){
    double x, y, z, xx, yy, zz;
    double espc, espl, espal;
    int i, j;
    stringstream box_points;

    // Espaçamentos
    espc = length / divisions;
    espl = length / divisions;
    espal = length / divisions;

    /*
     * FACES DE CIMA E DE BAIXO
     */
    y = length / 2;
    yy = -y;
    z = length / 2;

    for (i = 0; i < divisions; i++)
    {
        x = -(length / 2);
        xx = x + espc;
        zz = z - espl;
        for (j = 0; j < divisions; j++)
        {

            box_points << x << " " << y << " " << z << "\n";
            box_points << xx << " " << y << " " << z << "\n";
            box_points << x << " " << y << " " << zz << "\n";

            box_points << xx << " " << y << " " << z << "\n";
            box_points << xx << " " << y << " " << zz << "\n";
            box_points << x << " " << y << " " << zz << "\n";

            box_points << x << " " << yy << " " << z << "\n";
            box_points << x << " " << yy << " " << zz << "\n";
            box_points << xx << " " << yy << " " << z << "\n";

            box_points << xx << " " << yy << " " << z << "\n";
            box_points << x << " " << yy << " " << zz << "\n";
            box_points << xx << " " << yy << " " << zz << "\n";

            x = x + espc;
            xx = xx + espc;
        }
        z = z - espl;
    }

    x = -(length / 2);
    xx = -x;
    y = -(length / 2);

    for (i = 0; i < divisions; i++)
    {
        z = length / 2;
        zz = z - espl;
        yy = y + espal;

        for (j = 0; j < divisions; j++)
        {

            box_points << x << " " << y << " " << z << "\n";
            box_points << x << " " << yy << " " << z << "\n";
            box_points << x << " " << yy << " " << zz << "\n";

            box_points << x << " " << y << " " << zz << "\n";
            box_points << x << " " << y << " " << z << "\n";
            box_points << x << " " << yy << " " << zz << "\n";

            box_points << xx << " " << y << " " << z << "\n";
            box_points << xx << " " << y << " " << zz << "\n";
            box_points << xx << " " << yy << " " << zz << "\n";

            box_points << xx << " " << y << " " << z << "\n";
            box_points << xx << " " << yy << " " << zz << "\n";
            box_points << xx << " " << yy << " " << z << "\n";

            z = z - espl;
            zz = zz - espl;
        }
        y = y + espal;
    }

    z = length / 2;
    zz = -z;
    y = -(length / 2);

    for (i = 0; i < divisions; i++)
    {
        x = -(length / 2);
        xx = x + espc;
        yy = y + espal;

        for (j = 0; j < divisions; j++)
        {

            box_points << x << " " << y << " " << z << "\n";
            box_points << xx << " " << y << " " << z << "\n";
            box_points << x << " " << yy << " " << z << "\n";

            box_points << xx << " " << y << " " << z << "\n";
            box_points << xx << " " << yy << " " << z << "\n";
            box_points << x << " " << yy << " " << z << "\n";

            box_points << x << " " << y << " " << zz << "\n";
            box_points << x << " " << yy << " " << zz << "\n";
            box_points << xx << " " << y << " " << zz << "\n";

            box_points << xx << " " << y << " " << zz << "\n";
            box_points << x << " " << yy << " " << zz << "\n";
            box_points << xx << " " << yy << " " << zz << "\n";

            x = x + espc;
            xx = xx + espc;
        }
        y = y + espal;
    }

    ofstream file;
    string filename_cpp(filename);
    file.open("../3d_files/" + filename_cpp);
    file << box_points.str();
    file.close();
}

void sphere(float r, int slices, int stacks,char* filename){

	float alpha = (2 * M_PI) / slices; //
	float beta = M_PI / stacks; //

    	stringstream sphere_points;

	for (int i = 0; i < slices; i += 1) { //divide a(circunferência da) esfera pelo numero de slices

		float newAlpha = alpha * i; //

		for (int j = 0; j < stacks; j += 1) { //divide a esfera pelo numero de slices (em varias circunferências)

			float newBeta = (-M_PI / 2) + beta * j;

			float x1 = r * sin(newAlpha) * cos(newBeta);
			float y1 = r * sin(newBeta);
			float z1 = r * cos(newAlpha) * cos(newBeta);

			float x2 = r * sin(newAlpha + alpha) * cos(newBeta);
			float y2 = r * sin(newBeta);
			float z2 = r * cos(newAlpha + alpha) * cos(newBeta);

			float x3 = r * sin(newAlpha) * cos(newBeta + beta);
			float y3 = r * sin(newBeta + beta);
			float z3 = r * cos(newAlpha) * cos(newBeta + beta);

			float x4 = r * sin(newAlpha + alpha) * cos(newBeta + beta);
			float y4 = r * sin(newBeta + beta);
			float z4 = r * cos(newAlpha + alpha) * cos(newBeta + beta);

			sphere_points << x1;sphere_points << " ";sphere_points << y1;sphere_points << " ";sphere_points << z1;sphere_points << '\n';
			sphere_points << x2;sphere_points << " ";sphere_points << y2;sphere_points << " ";sphere_points << z2;sphere_points << '\n';
			sphere_points << x3;sphere_points << " ";sphere_points << y3;sphere_points << " ";sphere_points << z3;sphere_points << '\n';

			sphere_points << x2;sphere_points << " ";sphere_points << y2;sphere_points << " ";sphere_points << z2;sphere_points << '\n';
			sphere_points << x4;sphere_points << " ";sphere_points << y4;sphere_points << " ";sphere_points << z4;sphere_points << '\n';
			sphere_points << x3;sphere_points << " ";sphere_points << y3;sphere_points << " ";sphere_points << z3;sphere_points << '\n';
		}
	}

	ofstream file;
    string filename_cpp(filename);
    file.open("../3d_files/" + filename_cpp);
    file << sphere_points.str();
    file.close();
}

void cone(float r, float height, int slices, int stacks, char* filename){
	
    float sliceAngle = (float) (2 * M_PI) / slices; //angulo interno de cada slice
    float stackSize = (float) height / stacks; //diferença de y entre cada stack
    float rAux = (float) r / stacks; //diferença do valor do raio entre stacks
    stringstream cone_points;
    //base
    for(float a = 0; a < (2 * M_PI); a += sliceAngle){ //divide a base pelo numero de slices
        //centro da base(origem)
        float x1 = 0;
        float y1 = 0;
        float z1 = 0;

        //pontos contidos na circunferência da base
        float x3 = (float) r * sin(a);
        float y3 = 0;
        float z3 = (float)r * cos(a);

        float x2 = (float) r * sin(a + sliceAngle);
        float y2 = 0;
        float z2 =(float) r * cos(a + sliceAngle);

        cone_points << x3; cone_points << " "; cone_points << y3; cone_points << " "; cone_points << z3; cone_points << '\n';
        cone_points << x1; cone_points << " "; cone_points << y1; cone_points << " "; cone_points << z1; cone_points << '\n';
        cone_points << x2; cone_points << " "; cone_points << y2; cone_points << " "; cone_points << z2; cone_points << '\n';

    }

    //altura
    for(int i = 0; i<stacks; i++){ //divide a altura pelo numero de stacks
        for(float a = 0; a < (2 * M_PI); a += sliceAngle){ //divide cada stack pelo numero de slices
            float yBaixo = i * stackSize; //y da margem inferior da stack
            float yCima = (i + 1) * stackSize; //y da margem inferior da stack

            float rBaixo = (float) r - (i * rAux); //raio da margem inferior da stack
            float rCima = (float) r - ((i + 1) * rAux); //raio da margem inferior da stack

            float x1 = rCima * sin(a);
            float y1 = yCima;
            float z1 = rCima * cos(a);

            float x2 = rBaixo * sin(a);
            float y2 = yBaixo;
            float z2 = rBaixo * cos(a);

            float x5 = rBaixo * sin(a + sliceAngle);
            float y5 = yBaixo;
            float z5 = rBaixo * cos(a + sliceAngle);


            float x3 = rCima * sin(a);
            float y3 = yCima;
            float z3 = rCima * cos(a);

            float x4 = rBaixo * sin(a + sliceAngle);
            float y4 = yBaixo;
            float z4 = rBaixo * cos(a + sliceAngle);

            float x6 = rCima * sin(a + sliceAngle);
            float y6 = yCima;
            float z6 = rCima * cos(a + sliceAngle);

            cone_points << x1; cone_points << " "; cone_points << y1; cone_points << " "; cone_points << z1; cone_points << '\n';
            cone_points << x2; cone_points << " "; cone_points << y2; cone_points << " "; cone_points << z2; cone_points << '\n';
            cone_points << x5; cone_points << " "; cone_points << y5; cone_points << " "; cone_points << z5; cone_points << '\n';
            cone_points << x3; cone_points << " "; cone_points << y3; cone_points << " "; cone_points << z3; cone_points << '\n';
            cone_points << x4; cone_points << " "; cone_points << y4; cone_points << " "; cone_points << z4; cone_points << '\n';
            cone_points << x6; cone_points << " "; cone_points << y6; cone_points << " "; cone_points << z6; cone_points << '\n';

        }
    }

    ofstream file;
    string filename_cpp(filename);
    file.open("../3d_files/" + filename_cpp);
    file << cone_points.str();
    file.close();
}

void torus(float r, float R, int slices, int stacks, char* filename) {
    ofstream file;
    string filename_cpp(filename);
    file.open("../3d_files/" + filename_cpp);

    if (!file.is_open()) {
        cerr << "Falha ao abrir " << filename << endl;
        return;
    }

    // Definições angulares para as fatias (slices) e pilhas (stacks)
    // Escreve os vértices
    for (int i = 0; i < slices; i++) {
        for (int j = 0; j < stacks; j++) {
            float theta = (float)i / slices * 2.0f * M_PI;
            float phi = (float)j / stacks * 2.0f * M_PI;
            float nextTheta = (float)(i + 1) / slices * 2.0f * M_PI;
            float nextPhi = (float)(j + 1) / stacks * 2.0f * M_PI;

            // Primeiro triângulo
            float x1 = cos(theta) * (R + r * cos(phi));
            float y1 = r * sin(phi);
            float z1 = sin(theta) * (R + r * cos(phi));

            float x2 = cos(nextTheta) * (R + r * cos(phi));
            float y2 = r * sin(phi);
            float z2 = sin(nextTheta) * (R + r * cos(phi));

            float x3 = cos(theta) * (R + r * cos(nextPhi));
            float y3 = r * sin(nextPhi);
            float z3 = sin(theta) * (R + r * cos(nextPhi));

            // Segundo triângulo
            float x4 = cos(nextTheta) * (R + r * cos(nextPhi));
            float y4 = r * sin(nextPhi);
            float z4 = sin(nextTheta) * (R + r * cos(nextPhi));

            file << x2 << " " << y2 << " " << z2 << std::endl;
            file << x1 << " " << y1 << " " << z1 << std::endl;
            file << x3 << " " << y3 << " " << z3 << std::endl;

            file << x2 << " " << y2 << " " << z2 << std::endl;
            file << x3 << " " << y3 << " " << z3 << std::endl;
            file << x4 << " " << y4 << " " << z4 << std::endl;

            /*//Com este código, a torus é desenhada com as faces viradas para dentro
            file << x1 << " " << y1 << " " << z1 << std::endl;
            file << x2 << " " << y2 << " " << z2 << std::endl;
            file << x3 << " " << y3 << " " << z3 << std::endl;

            file << x2 << " " << y2 << " " << z2 << std::endl;
            file << x4 << " " << y4 << " " << z4 << std::endl;
            file << x3 << " " << y3 << " " << z3 << std::endl;
            */
        }
    }

    file.close();
}

void cilinder(float radius, float height, int slices, char* filename){
    float angle = 2 * M_PI / slices;
    stringstream cilinder_points;
	for(int i = 0; i < slices; i++){
		//top
		cilinder_points << 0 << " " << height/2 << " 0\n";
		cilinder_points << radius * sin(i * angle) << " " << height/2 << " " << radius * cos(i * angle) << "\n";
        cilinder_points << radius * sin((i+1) * angle) << " " << height/2 << " " << radius * cos((i+1) * angle) << "\n";

		//bottom
        cilinder_points << radius * sin(i * angle) << " " << -(height/2) << " " << radius * cos(i * angle) << "\n";
        cilinder_points << 0 << " " << -(height/2) << " 0\n";
		cilinder_points << radius * sin((i+1) * angle) << " " << -(height/2) << " " << radius * cos((i+1) * angle) << "\n";

		//side-1
        cilinder_points << radius * sin(i * angle) << " " << height/2 << " " << radius * cos(i * angle) << "\n";
        cilinder_points << radius * sin(i * angle) << " " << -(height/2) << " " << radius * cos(i * angle) << "\n";
        cilinder_points << radius * sin((i+1) * angle) << " " << height/2 << " " << radius * cos((i+1) * angle) << "\n";

		//side-2
		cilinder_points << radius * sin((i+1) * angle) << " " << height/2 << " " << radius * cos((i+1) * angle) << "\n";
        cilinder_points << radius * sin(i * angle) << " " << -(height/2) << " " << radius * cos(i * angle) << "\n";
        cilinder_points << radius * sin((i+1) * angle) << " " << -(height/2) << " " << radius * cos((i+1) * angle) << "\n";
	}

    ofstream file;
    //converte filename de char* para string
    string filename_cpp(filename);
    file.open("../3d_files/" + filename_cpp);
    file << cilinder_points.str();
    file.close();
}

int main_generator(int argc, char *argv[]) {

    if(argc == 7 && string(argv[2]) == "cilinder"){ //certo
        cilinder(atof(argv[3]), atof(argv[4]), atoi(argv[5]), argv[6]);
    }
    else if (argc == 8 && string(argv[2]) == "torus"){//errado
        torus(atof(argv[3]), atof(argv[4]), atoi(argv[5]), atoi(argv[6]), argv[7]);
    }
    else if(argc == 6 && string(argv[2]) == "plane"){//certo
        plane(atof(argv[3]), atoi(argv[4]), argv[5]);
    }
    else if(argc == 6 && string(argv[2]) == "box"){//certo
        box(atof(argv[3]), atoi(argv[4]), argv[5]);
    }
    else if(argc == 7 && string(argv[2]) == "sphere"){//certo
        sphere(atof(argv[3]), atoi(argv[4]), atoi(argv[5]), argv[6]);
    }
    else if(argc == 8 && string(argv[2]) == "cone"){//certo
        cone(atof(argv[3]), atof(argv[4]), atoi(argv[5]), atoi(argv[6]), argv[7]);
    }
    
    else{
        printf("NUMERO DE ARGUMENTOS INCORRETOS OU FIGURA NÂO IMPLEMENTADA!\n");
    }
    return 0;
}
