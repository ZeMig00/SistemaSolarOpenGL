#include "generator.hpp"

void normalize_vector(float *a) {

	float l = sqrt(a[0]*a[0] + a[1] * a[1] + a[2] * a[2]);
	a[0] = a[0]/l;
	a[1] = a[1]/l;
	a[2] = a[2]/l;
}



void plane(float length, int divisions,char* filename){
    float x = length/2;
    float z = length/2;
    float xt = 1.0f;
    float zt = 1.0f;
    float iter = length/divisions;
    float texiter = 1.0f/divisions;
    stringstream plane_points;

    for(int i = 0; i < divisions; i++){
        for(int j = 0; j < divisions; j++){
            plane_points << x        << " " << 0 << " " << z        << " " << 0 << " " << 1 << " " << 0 << " " << xt         << " " << zt << "\n";
            plane_points << x        << " " << 0 << " " << z - iter << " " << 0 << " " << 1 << " " << 0 << " " << xt         << " " << zt-texiter << "\n";
            plane_points << x - iter << " " << 0 << " " << z        << " " << 0 << " " << 1 << " " << 0 << " " << xt-texiter << " " << zt << "\n";

            plane_points << x - iter << " " << 0 << " " << z        << " " << 0 << " " << 1 << " " << 0 << " " << xt-texiter << " " << zt << "\n";
            plane_points << x        << " " << 0 << " " << z - iter << " " << 0 << " " << 1 << " " << 0 << " " << xt         << " " << zt-texiter << "\n";
            plane_points << x - iter << " " << 0 << " " << z - iter << " " << 0 << " " << 1 << " " << 0 << " " << xt-texiter << " " << zt-texiter << "\n";

            x -= iter;
            xt -= texiter; 

        }
        x = length/2;
        xt = 1.0f;
        z -= iter;
        zt -= texiter;

        
    }

    ofstream file;
    string filename_cpp(filename);
    file.open("../3d_files/" + filename_cpp);
    file << plane_points.str();
    file.close();
}

void box(float length, int divisions,char* filename){
    double x, y, z, xx, yy, zz, xt, yt, zt, xxt, yyt, zzt;
    double espc, espl, espal;
    int i, j;
    stringstream box_points;
    float texiter = 1.0f/divisions;

    // Espaçamentos
    espc = length / divisions;
    espl = length / divisions;
    espal = length / divisions;

    /*
     * FACES DE CIMA E DE BAIXO
     */
    y = length / 2;
    yt = 1.0f;
    yy = -y;
    yyt = -yt;
    z = length / 2;
    zt = 1.0f;

    for (i = 0; i < divisions; i++)
    {
        x = -(length / 2);
        xt = -1.0f;
        xx = x + espc;
        xxt = xt + texiter;
        zz = z - espl;
        zzt = zt - texiter;
        for (j = 0; j < divisions; j++)
        {

            box_points << x << " " << y << " " << z << " " << 0 << " " << 1 << " " << 0 << " " <<  xt << " " << zt << "\n";
            box_points << xx << " " << y << " " << z << " " <<  0 << " " << 1 << " " << 0 << " " <<  xxt << " " << zt << "\n";
            box_points << x << " " << y << " " << zz << " " <<  0 << " " << 1 << " " << 0 << " " <<  xt << " " << zzt << "\n";

            box_points << xx << " " << y << " " << z << " " <<  0 << " " << 1 << " " << 0 << " " <<  xxt << " " << zt << "\n";
            box_points << xx << " " << y << " " << zz << " " <<  0 << " " << 1 << " " << 0 << " " <<  xxt << " " << zzt << "\n";
            box_points << x << " " << y << " " << zz << " " <<  0 << " " << 1 << " " << 0 << " " <<  xt << " " << zzt << "\n";

            box_points << x << " " << yy << " " << z << " " <<  0 << " " << -1 << " " << 0 << " " <<  xt << " " << zt << "\n";
            box_points << x << " " << yy << " " << zz << " " <<  0 << " " << -1 << " " << 0 << " " <<  xt << " " << zzt << "\n";
            box_points << xx << " " << yy << " " << z << " " <<  0 << " " << -1 << " " << 0 << " " <<  xxt << " " << zt << "\n";

            box_points << xx << " " << yy << " " << z << " " <<  0 << " " << -1 << " " << 0 << " " <<  xxt << " " << zt << "\n";
            box_points << x << " " << yy << " " << zz << " " <<  0 << " " << -1 << " " << 0 << " " <<  xt << " " << zzt << "\n";
            box_points << xx << " " << yy << " " << zz << " " <<  0 << " " << -1 << " " << 0 << " " <<  xxt << " " << zzt << "\n";

            x = x + espc;
            xt = xt + texiter;
            xx = xx + espc;
            xxt = xxt + texiter;
        }
        z = z - espl;
        zt = zt - texiter;
    }

    x = -(length / 2);
    xt = -1.0f;
    xx = -x;
    xxt = -xt;
    y = -(length / 2);
    yt = -1.0f;

    for (i = 0; i < divisions; i++)
    {
        z = length / 2;
        xt = 1.0f;
        zz = z - espl;
        xxt = xt + texiter;
        yy = y + espal;
        yyt = yt + texiter;

        for (j = 0; j < divisions; j++)
        {

            box_points << x << " " << y << " " << z << " " << -1 << " " << 0 << " " << 0 << " " << xt << " " << yt << "\n";
            box_points << x << " " << yy << " " << z << " " << -1 << " " << 0 << " " << 0 << " " << xt << " " << yyt << "\n";
            box_points << x << " " << yy << " " << zz << " " << -1 << " " << 0 << " " << 0 << " " << xxt << " " << yyt << "\n";

            box_points << x << " " << y << " " << zz << " " << -1 << " " << 0 << " " << 0 << " " << xxt << " " << yt << "\n";
            box_points << x << " " << y << " " << z << " " << -1 << " " << 0 << " " << 0 << " " << xt << " " << yt << "\n";
            box_points << x << " " << yy << " " << zz << " " << -1 << " " << 0 << " " << 0 << " " << xxt << " " << yyt << "\n";

            box_points << xx << " " << y << " " << z << " " << 1 << " " << 0 << " " << 0 << " " << xt << " " << yt << "\n";
            box_points << xx << " " << y << " " << zz << " " << 1 << " " << 0 << " " << 0 << " " << xxt << " " << yt << "\n";
            box_points << xx << " " << yy << " " << zz << " " << 1 << " " << 0 << " " << 0 << " " << xxt << " " << yyt << "\n";

            box_points << xx << " " << y << " " << z << " " << 1 << " " << 0 << " " << 0 << " " << xt << " " << yt << "\n";
            box_points << xx << " " << yy << " " << zz << " " << 1 << " " << 0 << " " << 0 << " " << xxt << " " << yyt << "\n";
            box_points << xx << " " << yy << " " << z << " " << 1 << " " << 0 << " " << 0 << " " << xt << " " << yyt << "\n";

            z = z - espl;
            xt = xt + texiter;
            zz = zz - espl;
            xxt = xxt + texiter;
        }
        y = y + espal;
        yt = yt + texiter;
    }

    z = length / 2;
    zt = 1.0f;
    zz = -z;
    zzt = -zt;
    y = -(length / 2);
    yt = -1.0f;

    for (i = 0; i < divisions; i++)
    {
        x = -(length / 2);
        xt = -1.0f;
        xx = x + espc;
        xxt = xt + texiter;
        yy = y + espal;
        yyt = yt + texiter;

        for (j = 0; j < divisions; j++)
        {

            box_points << x << " " << y << " " << z << " " << 0 << " " << 0 << " " << 1 << " " << xt << " " << yt << "\n";
            box_points << xx << " " << y << " " << z << " " << 0 << " " << 0 << " " << 1 << " " << xxt << " " << yt << "\n";
            box_points << x << " " << yy << " " << z << " " << 0 << " " << 0 << " " << 1 << " " << xt << " " << yyt << "\n";

            box_points << xx << " " << y << " " << z << " " << 0 << " " << 0 << " " << 1 << " " << xxt << " " << yt << "\n";
            box_points << xx << " " << yy << " " << z << " " << 0 << " " << 0 << " " << 1 << " " << xxt << " " << yyt << "\n";
            box_points << x << " " << yy << " " << z << " " << 0 << " " << 0 << " " << 1 << " " << xt << " " << yyt << "\n";

            box_points << x << " " << y << " " << zz << " " << 0 << " " << 0 << " " << -1 << " " << xt << " " << yt << "\n";
            box_points << x << " " << yy << " " << zz << " " << 0 << " " << 0 << " " << -1 << " " << xt << " " << yyt << "\n";
            box_points << xx << " " << y << " " << zz << " " << 0 << " " << 0 << " " << -1 << " " << xxt << " " << yt << "\n";

            box_points << xx << " " << y << " " << zz << " " << 0 << " " << 0 << " " << -1 << " " << xxt << " " << yt << "\n";
            box_points << x << " " << yy << " " << zz << " " << 0 << " " << 0 << " " << -1 << " " << xt << " " << yyt << "\n";
            box_points << xx << " " << yy << " " << zz << " " << 0 << " " << 0 << " " << -1 << " " << xxt << " " << yyt << "\n";

            x = x + espc;
            xt = xt + texiter;
            xx = xx + espc;
            xxt = xxt + texiter;
        }
        y = y + espal;
        yt = yt + texiter;
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
            float normal1[3] = {x1,y1,z1};
            normalize_vector(normal1);
            float u1 = newAlpha / (2 * M_PI);
            float v1 = (newBeta + (M_PI / 2)) / M_PI;

			float x2 = r * sin(newAlpha + alpha) * cos(newBeta);
			float y2 = r * sin(newBeta);
			float z2 = r * cos(newAlpha + alpha) * cos(newBeta);
            float normal2[3] = {x2,y2,z2};
            normalize_vector(normal2);
            float u2 = (newAlpha + alpha) / (2 * M_PI);
            float v2 = v1;

			float x3 = r * sin(newAlpha) * cos(newBeta + beta);
			float y3 = r * sin(newBeta + beta);
			float z3 = r * cos(newAlpha) * cos(newBeta + beta);
            float normal3[3] = {x3,y3,z3};
            normalize_vector(normal3);
            float u3 = u1;
            float v3 = (newBeta + beta + (M_PI / 2)) / M_PI;

			float x4 = r * sin(newAlpha + alpha) * cos(newBeta + beta);
			float y4 = r * sin(newBeta + beta);
			float z4 = r * cos(newAlpha + alpha) * cos(newBeta + beta);
            float normal4[3] = {x4,y4,z4};
            normalize_vector(normal4);
            float u4 = u2;
            float v4 = v3;

			sphere_points << x1 << " " << y1 << " " << z1 << " " <<  normal1[0] << " " << normal1[1] << " " << normal1[2] << " " << u1 << " " << v1 << '\n';
			sphere_points << x2 << " " << y2 << " " << z2 << " " <<  normal2[0] << " " << normal2[1] << " " << normal2[2] << " " << u2 << " " << v2 << '\n';
			sphere_points << x3 << " " << y3 << " " << z3 << " " <<  normal3[0] << " " << normal3[1] << " " << normal3[2] << " " << u3 << " " << v3 << '\n';

			sphere_points << x2 << " " << y2 << " " << z2 << " " <<  normal2[0] << " " << normal2[1] << " " << normal2[2] << " " << u2 << " " << v2 << '\n';
			sphere_points << x4 << " " << y4 << " " << z4 << " " <<  normal4[0] << " " << normal4[1] << " " << normal4[2] << " " << u4 << " " << v4 << '\n';
			sphere_points << x3 << " " << y3 << " " << z3 << " " <<  normal3[0] << " " << normal3[1] << " " << normal3[2] << " " << u3 << " " << v3 << '\n';
		}
	}

	ofstream file;
    string filename_cpp(filename);
    file.open("../3d_files/" + filename_cpp);
    file << sphere_points.str();
    file.close();
}

/*
COLOCAR NAS REFS DO RELATORIO (Cálculo das normais do cone)
https://stackoverflow.com/questions/19245363/opengl-glut-surface-normals-of-cone
*/

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

        cone_points << x3 << " " << y3 << " " << z3 << " 0 -1 0" << " " << (float)a/slices     << " " << 0 << '\n';
        cone_points << x1 << " " << y1 << " " << z1 << " 0 -1 0" << " " << (float)a/slices     << " " << 1 << '\n';
        cone_points << x2 << " " << y2 << " " << z2 << " 0 -1 0" << " " << (float)(a+1)/slices << " " << 0 << '\n';

    }

    //lados
    float coneAngle = atan((float)r/height); //angulo do cone
    for(int i = 0; i<stacks; i++){ //divide a altura pelo numero de stacks
        for(float a = 0; a < (2 * M_PI); a += sliceAngle){ //divide cada stack pelo numero de slices
            float yBaixo = i * stackSize; //y da margem inferior da stack
            float yCima = (i + 1) * stackSize; //y da margem inferior da stack

            float rBaixo = (float) r - (i * rAux); //raio da margem inferior da stack
            float rCima = (float) r - ((i + 1) * rAux); //raio da margem inferior da stack

            float x2 = rBaixo * sin(a);
            float y2 = yBaixo;
            float z2 = rBaixo * cos(a);
            float normal2[3];
            normal2[0] = cos(coneAngle) * sin(a);
            normal2[1] = sin(coneAngle);
            normal2[2] = cos(coneAngle) * cos(a);
            normalize_vector(normal2);

            float x5 = rBaixo * sin(a + sliceAngle);
            float y5 = yBaixo;
            float z5 = rBaixo * cos(a + sliceAngle);
            float normal5[3];
            normal5[0] = cos(coneAngle) * sin(a + sliceAngle);
            normal5[1] = sin(coneAngle);
            normal5[2] = cos(coneAngle) * cos(a + sliceAngle);
            normalize_vector(normal5);

            float x4 = rBaixo * sin(a + sliceAngle);
            float y4 = yBaixo;
            float z4 = rBaixo * cos(a + sliceAngle);
            float normal4[3];
            normal4[0] = cos(coneAngle) * sin(a + sliceAngle);
            normal4[1] = sin(coneAngle);
            normal4[2] = cos(coneAngle) * cos(a + sliceAngle);
            normalize_vector(normal4);

            float x1 = rCima * sin(a);
            float y1 = yCima;
            float z1 = rCima * cos(a);
            float normal1[3];
            normal1[0] = cos(coneAngle) * sin(a);
            normal1[1] = sin(coneAngle);
            normal1[2] = cos(coneAngle) * cos(a);
            normalize_vector(normal1);


            float x3 = rCima * sin(a);
            float y3 = yCima;
            float z3 = rCima * cos(a);
            float normal3[3];
            normal3[0] = cos(coneAngle) * sin(a);
            normal3[1] = sin(coneAngle);
            normal3[2] = cos(coneAngle) * cos(a);
            normalize_vector(normal3);

            float x6 = rCima * sin(a + sliceAngle);
            float y6 = yCima;
            float z6 = rCima * cos(a + sliceAngle);
            float normal6[3];
            normal6[0] = cos(coneAngle) * sin(a + sliceAngle);
            normal6[1] = sin(coneAngle);
            normal6[2] = cos(coneAngle) * cos(a + sliceAngle);
            normalize_vector(normal6);

            cone_points << x1 << " " << y1 << " " << z1 << " " << normal1[0] << " " << normal1[1] << " " << normal1[2] << " " << (float)a/slices     << " " << (float)(i+1)/stacks << '\n';
            cone_points << x2 << " " << y2 << " " << z2 << " " << normal2[0] << " " << normal2[1] << " " << normal2[2] << " " << (float)a/slices     << " " << (float)i/stacks << '\n';
            cone_points << x5 << " " << y5 << " " << z5 << " " << normal5[0] << " " << normal5[1] << " " << normal5[2] << " " << (float)(a+1)/slices << " " << (float)i/stacks << '\n';
            cone_points << x3 << " " << y3 << " " << z3 << " " << normal3[0] << " " << normal3[1] << " " << normal3[2] << " " << (float)a/slices     << " " << (float)(i+1)/stacks << '\n';
            cone_points << x4 << " " << y4 << " " << z4 << " " << normal4[0] << " " << normal4[1] << " " << normal4[2] << " " << (float)(a+1)/slices << " " << (float)i/stacks << '\n';
            cone_points << x6 << " " << y6 << " " << z6 << " " << normal6[0] << " " << normal6[1] << " " << normal6[2] << " " << (float)(a+1)/slices << " " << (float)(i+1)/stacks << '\n';

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
            float normal1[3] = {x1 - R * cos(theta), y1, z1 - R * sin(theta)};
            normalize_vector(normal1);
            float u1 = (float)i / slices;
            float v1 = (float)j / stacks;

            float x2 = cos(nextTheta) * (R + r * cos(phi));
            float y2 = r * sin(phi);
            float z2 = sin(nextTheta) * (R + r * cos(phi));
            float normal2[3] = {x2 - R * cos(nextTheta), y2, z2 - R * sin(nextTheta)};
            normalize_vector(normal2);
            float u2 = (float)(i + 1) / slices;
            float v2 = (float)j / stacks;

            float x3 = cos(theta) * (R + r * cos(nextPhi));
            float y3 = r * sin(nextPhi);
            float z3 = sin(theta) * (R + r * cos(nextPhi));
            float normal3[3] = {x3 - R * cos(theta), y3, z3 - R * sin(theta)};
            normalize_vector(normal3);
            float u3 = (float)i / slices;
            float v3 = (float)(j + 1) / stacks;

            // Segundo triângulo
            float x4 = cos(nextTheta) * (R + r * cos(nextPhi));
            float y4 = r * sin(nextPhi);
            float z4 = sin(nextTheta) * (R + r * cos(nextPhi));
            float normal4[3] = {x4 - R * cos(nextTheta), y4, z4 - R * sin(nextTheta)};
            normalize_vector(normal4);
            float u4 = (float)(i + 1) / slices;
            float v4 = (float)(j + 1) / stacks;

            file << x2 << " " << y2 << " " << z2 << " " << normal2[0] << " " << normal2[1] << " " << normal2[2] << " " << u2 << " " << v2 << std::endl;
            file << x1 << " " << y1 << " " << z1 << " " << normal1[0] << " " << normal1[1] << " " << normal1[2] << " " << u1 << " " << v1 << std::endl;
            file << x3 << " " << y3 << " " << z3 << " " << normal3[0] << " " << normal3[1] << " " << normal3[2] << " " << u3 << " " << v3 << std::endl;

            file << x2 << " " << y2 << " " << z2 << " " << normal2[0] << " " << normal2[1] << " " << normal2[2] << " " << u2 << " " << v2 << std::endl;
            file << x3 << " " << y3 << " " << z3 << " " << normal3[0] << " " << normal3[1] << " " << normal3[2] << " " << u3 << " " << v3 << std::endl;
            file << x4 << " " << y4 << " " << z4 << " " << normal4[0] << " " << normal4[1] << " " << normal4[2] << " " << u4 << " " << v4 << std::endl;
        }
    }

    file.close();
}

void cilinder(float radius, float height, int slices, char* filename){
    float angle = 2 * M_PI / slices;
    stringstream cilinder_points;
	for(int i = 0; i < slices; i++){
        float sin1 = radius * sin(i * angle);
        float cos1 = radius * cos(i * angle);
        float sin2 = radius * sin((i+1) * angle);
        float cos2 = radius * cos((i+1) * angle);
		//top
		cilinder_points << 0    << " " << height/2 << " " << 0    << " " << 0 << " " << 1 << " " << 0 << " " << (float)i/slices     << " " << 1 <<  "\n";
		cilinder_points << sin1 << " " << height/2 << " " << cos1 << " " << 0 << " " << 1 << " " << 0 << " " << (float)i/slices     << " " << 0 <<  "\n";
        cilinder_points << sin2 << " " << height/2 << " " << cos2 << " " << 0 << " " << 1 << " " << 0 << " " << ((float)i+1)/slices << " " << 0 <<  "\n";

		//bottom
        cilinder_points << sin1 << " " << -(height/2) << " " << cos1 << " " << 0 << " " << -1 << " " << 0 << " " << (float)i/slices     << " " << 0 <<  "\n";
        cilinder_points << 0    << " " << -(height/2) << " " << 0    << " " << 0 << " " << -1 << " " << 0 << " " << (float)i/slices     << " " << 1 <<  "\n";
		cilinder_points << sin2 << " " << -(height/2) << " " << cos2 << " " << 0 << " " << -1 << " " << 0 << " " << (float)(i+1)/slices << " " << 0 <<  "\n";

		//side-1
        float normal[3] = {sin(i * angle),0,cos(i * angle)};
        normalize_vector(normal);
        float normal3[3] = {sin((i+1) * angle),0,cos((i+1) * angle)};
        normalize_vector(normal3);
        float normal2[3] = {sin(i * angle),0,cos(i * angle)};
        normalize_vector(normal2);
        cilinder_points << sin1 << " " << height/2    << " " << cos1 << " " << normal[0]  << " " << normal[1]  << " " << normal[2]  << " " << ((float)i/slices)     << " " << 1 << "\n";        
        cilinder_points << sin1 << " " << -(height/2) << " " << cos1 << " " << normal2[0] << " " << normal2[1] << " " << normal2[2] << " " << ((float)i/slices)     << " " << 0 << "\n";        
        cilinder_points << sin2 << " " << height/2    << " " << cos2 << " " << normal3[0] << " " << normal3[1] << " " << normal3[2] << " " << ((float)(i+1)/slices) << " " << 1 << "\n";

		//side-2
        float normal4[3] = {sin((i+1) * angle),0,cos((i+1) * angle)};
        normalize_vector(normal4);
        float normal5[3] = {sin(i * angle),0,cos(i * angle)};
        normalize_vector(normal5);
        float normal6[3] = {sin((i+1) * angle),0,cos((i+1) * angle)};
        normalize_vector(normal6);
		cilinder_points << sin2 << " " << height/2    << " " << cos2 << " " << normal4[0] << " " << normal4[1] << " " << normal4[2] << " " << ((float)(i+1)/slices) << " " << 1 << "\n";
        cilinder_points << sin1 << " " << -(height/2) << " " << cos1 << " " << normal5[0] << " " << normal5[1] << " " << normal5[2] << " " << ((float)i/slices)     << " " << 0 << "\n";
        cilinder_points << sin2 << " " << -(height/2) << " " << cos2 << " " << normal6[0] << " " << normal6[1] << " " << normal6[2] << " " << ((float)(i+1)/slices) << " " << 0 << "\n";
	}

    ofstream file;
    //converte filename de char* para string
    string filename_cpp(filename);
    file.open("../3d_files/" + filename_cpp);
    file << cilinder_points.str();
    file.close();
}

void multMatVec(float* a, float* b, float* res){
    for(int i = 0; i < 4; i++){
        res[i] = 0;
        for(int j = 0; j < 4; j++){
            res[i] += a[i * 4 + j] * b[j];
        }
    }
}


void multMatrix(float a[4][4], float b[4][4], float res[4][4]){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            res[i][j] = 0;
            for(int k = 0; k < 4; k++){
                res[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}


float bez(float a,float b,float mat[4][4]){

    float vec_a[4] = {powf(a,3), powf(a,2),a,1};
    float vec_b[4] = {powf(b,3), powf(b,2),b,1};

    float aux[4];
    multMatVec((float*)mat,vec_a,aux);

    float r = 0;
    for(int i = 0; i < 4; i++){
        r += aux[i] * vec_b[i];
    }
    return r;
}


float tanBezU(float a,float b,float mat[4][4]){

    float vec_a[4] = {3*powf(a,2), 2*powf(a,1),1,0};
    float vec_b[4] = {powf(b,3), powf(b,2),b,1};

    float aux[4];
    multMatVec((float*)mat,vec_a,aux);

    float r = 0;
    for(int i = 0; i < 4; i++){
        r += aux[i] * vec_b[i];
    }
    return r;
}

float tanBezV(float a,float b,float mat[4][4]){

    float vec_a[4] = {powf(a,3), powf(a,2),a,1};
    float vec_b[4] = {3*powf(b,2), 2*powf(b,1),1,0};

    float aux[4];
    multMatVec((float*)mat,vec_a,aux);

    float r = 0;
    for(int i = 0; i < 4; i++){
        r += aux[i] * vec_b[i];
    }
    return r;
}

void crossVectors(float *a, float *b, float *res) {

	res[0] = a[1]*b[2] - a[2]*b[1];
	res[1] = a[2]*b[0] - a[0]*b[2];
	res[2] = a[0]*b[1] - a[1]*b[0];
}


void bezier(char* bezier_file,float tesselation,char* out_filename){

    std::string file_bezier(bezier_file);
    std::ifstream b_file(file_bezier);
    std::vector<std::vector<int>> patches;
    int nPatches;
    int nPoints;

    float bezier_matrix[4][4] = {{-1.0f, 3.0f, -3.0f, 1.0f},{3.0f, -6.0f, 3.0f, 0.0f},{-3.0f, 3.0f, 0.0f, 0.0f},{1.0f, 0.0f, 0.0f, 0.0f}};
    
    std::vector<std::tuple<float, float, float>> controlPoints;
    stringstream bezier_points;

    if(b_file.is_open()){ // load dos pontos de controlo por patch para a estrutura de dados
        string line;
        getline(b_file, line);
        nPatches = stoi(line);

        while(nPatches>0){
            getline(b_file, line);
            nPoints = stoi(line);
            std::vector<int> patch;
            char* token = strtok((char*)(line.c_str()), ", ");
            while(token != NULL){
                patch.push_back(atof(token));
                token = strtok(NULL, ", ");
            }
            patches.push_back(patch);
            nPatches--;
        }

        getline(b_file, line);
        nPoints = stoi(line);

        while(nPoints>0){ //load dos pontos de controlo para a estrutura de dados
            getline(b_file, line);
            std::vector<float> controlPoint;
            char* token = strtok((char*)line.c_str(), ", ");                        
            while(token != nullptr){
                controlPoint.push_back(atof(token));
                token = strtok(nullptr, ", ");
            }
            tuple<float, float, float> t(controlPoint[0], controlPoint[1], controlPoint[2]);
            controlPoints.push_back(t);
            nPoints--;
        }

        float a[4][4], b[4][4], c[4][4];

        for(std::vector<int> indexes : patches){ // calcula as matrizes de pontos de controlo para cada patch, uma por coordenada
                                                 // a = x, b = y, c = z
            for(int i = 0; i < 4; i++){
                for(int j = 0; j < 4; j++){
                    a[i][j] = get<0>(controlPoints[indexes[i*4+j]]);
                    b[i][j] = get<1>(controlPoints[indexes[i*4+j]]);
                    c[i][j] = get<2>(controlPoints[indexes[i*4+j]]);
                }
            }

            float temp[4][4];
            // calcula M * P * M^T, para cada coordenada
            multMatrix(bezier_matrix,a,temp);
            multMatrix(temp,bezier_matrix,a);
            multMatrix(bezier_matrix, b, temp);
            multMatrix(temp, bezier_matrix, b);
            multMatrix(bezier_matrix, c, temp);
            multMatrix(temp, bezier_matrix, c);

            float tess = 1/tesselation; // saber o número de pontos que vão ser calculados para o patch
            for(float i = 0.0;i<1.0;i += tess){
                for(float j = 0.0;j<1.0;j += tess){
                    float xt = j;
                    float yt = i;
                    float x = bez(i,j,a);
                    float y = bez(i,j,b);
                    float z = bez(i,j,c);
                    float nu1[3] = {tanBezU(i,j,a),tanBezU(i,j,b),tanBezU(i,j,c)};
                    float nv1[3] = {tanBezV(i,j,a),tanBezV(i,j,b),tanBezV(i,j,c)};
                    float normal1[3];
                    crossVectors(nu1,nv1,normal1);
                    normalize_vector(normal1);

                    bezier_points << x << " " << y << " " << z << " " << normal1[0] << " " << normal1[1] << " " << normal1[2] << " " << xt << " " << yt << "\n";

                    float xt2 = j;
                    float yt2 = i+tess;
                    float x2 = bez(i+tess,j,a);
                    float y2 = bez(i+tess,j,b);
                    float z2 = bez(i+tess,j,c);
                    float nu2[3] = {tanBezU(i+tess,j,a),tanBezU(i+tess,j,b),tanBezU(i+tess,j,c)};
                    float nv2[3] = {tanBezV(i+tess,j,a),tanBezV(i+tess,j,b),tanBezV(i+tess,j,c)};
                    float normal2[3];
                    crossVectors(nu2,nv2,normal2);
                    normalize_vector(normal2);

                    bezier_points << x2 << " " << y2 << " " << z2 << " " << normal2[0] << " " << normal2[1] << " " << normal2[2] << " " << xt2 << " " << yt2 <<  "\n";

                    float xt3 = j+tess;
                    float yt3 = i;
                    float x3 = bez(i,j+tess,a);
                    float y3 = bez(i,j+tess,b);
                    float z3 = bez(i,j+tess,c);
                    float nu3[3] = {tanBezU(i,j+tess,a),tanBezU(i,j+tess,b),tanBezU(i,j+tess,c)};
                    float nv3[3] = {tanBezV(i,j+tess,a),tanBezV(i,j+tess,b),tanBezV(i,j+tess,c)};
                    float normal3[3];
                    crossVectors(nu3,nv3,normal3);
                    normalize_vector(normal3);

                    bezier_points << x3 << " " << y3 << " " << z3 << " " << normal3[0] << " " << normal3[1] << " " << normal3[2] << " " << xt3 << " " << yt3 << "\n";

                    xt = j+tess;
                    yt = i+tess;
                    x = bez(i+tess,j+tess,a);
                    y = bez(i+tess,j+tess,b);
                    z = bez(i+tess,j+tess,c);
                    float nu4[3] = {tanBezU(i+tess,j+tess,a),tanBezU(i+tess,j+tess,b),tanBezU(i+tess,j+tess,c)};
                    float nv4[3] = {tanBezV(i+tess,j+tess,a),tanBezV(i+tess,j+tess,b),tanBezV(i+tess,j+tess,c)};
                    float normal4[3];
                    crossVectors(nu4,nv4,normal4);
                    normalize_vector(normal4);

                    bezier_points << x2 << " " << y2 << " " << z2 << " " << normal2[0] << " " << normal2[1] << " " << normal2[2] << " " << xt2 << " " << yt2 <<  "\n";

                    bezier_points << x << " " << y << " " << z << " " << normal4[0] << " " << normal4[1] << " " << normal4[2] << " " << xt << " " << yt << "\n";

                    bezier_points << x3 << " " << y3 << " " << z3 << " " << normal3[0] << " " << normal3[1] << " " << normal3[2] << " " << xt3 << " " << yt3 << "\n";

                }
            }
        }
    }

    ofstream file;
    string filename_cpp(out_filename);
    file.open("../3d_files/" + filename_cpp);
    file << bezier_points.str();
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
    else if(argc == 6 && string(argv[2]) == "box"){//certo generator box 2 3 box_2_3.3d
        box(atof(argv[3]), atoi(argv[4]), argv[5]);
    }
    else if(argc == 7 && string(argv[2]) == "sphere"){//certo generator sphere 1 8 8 sphere_1_8_8.3d
        sphere(atof(argv[3]), atoi(argv[4]), atoi(argv[5]), argv[6]);
    }
    else if(argc == 8 && string(argv[2]) == "cone"){//certo generator cone 1 2 4 3 cone_1_2_4_3.3d
        cone(atof(argv[3]), atof(argv[4]), atoi(argv[5]), atoi(argv[6]), argv[7]);
    }
    else if(argc == 6 && string(argv[2]) == "patch"){//generator patch <caminho para o ficheiro .patch> 10 bezier_10.3d
        bezier(argv[3], atof(argv[4]),argv[5]);
    }    
    else{
        printf("NUMERO DE ARGUMENTOS INCORRETOS OU FIGURA NÂO IMPLEMENTADA!\n");
    }
    return 0;
}
