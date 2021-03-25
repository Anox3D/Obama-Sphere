#include <iostream>
#include <GL/glut.h>
#include <math.h>
#include <ctime>
#include <algorithm>
#include <vector>
#include <fstream>

using namespace std;

const double PI = 3.1415926535897932384626433832795;
float x_pos = 0;
float y_pos = 0;
float z_pos = 15;
float theta = PI/6;
float phi = PI/2;
float RR = 5;
GLuint texture;	// Stocking texture variable

typedef struct
{
	double X;
	double Y;
	double Z;
	double U;
	double V;
} VERTICES;

const int space = 10;
const int VertexCount = (90/space)*(360/space)*4;
VERTICES VERTEX[VertexCount];

GLuint LoadTextureRaw(const char* filename)
{
	GLuint texture;
	
	int width, height;
	
	unsigned char* data;
	
	FILE*file;
	file = fopen(filename, "rb");
	if(file == NULL) return 0;
	
	width = 512;
	height = 512;
	data = (unsigned char*)malloc(width*height*3);
	fread(data, width*height*3, 1, file);
	
	fclose(file);
	

	for(int i = 0; i < width*height; ++i)
	{
		int index = i*3;
		unsigned char B, R;
		B = data[index];
		R = data[index+2];
		data[index] = R;
		data[index+2] = B;
	}
	
	glGenTextures(1,&texture);
	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
	free(data);
	
	return texture;
}

// Display sphere using triangle strips
void DisplaySphere(double R, GLuint texture)
{
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	int b;
	
	glScalef(0.5*R, 0.5*R, 0.5*R);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_TRIANGLE_STRIP);
	
	// Assigning values to the triangle strips
	// Assigning values for the first half of the sphere on the triangle strips
	for(b = 0;b < VertexCount; b++)
	{
		glTexCoord2f(VERTEX[b].U, VERTEX[b].V);
		glVertex3f(VERTEX[b].X, VERTEX[b].Y, -VERTEX[b].Z);
	}

	// Assigning values for other half of the sphere on the triangle strips
	for(b = 0; b < VertexCount; b++)
	{
		glTexCoord2f(VERTEX[b].U, -VERTEX[b].V);
		glVertex3f(VERTEX[b].X, VERTEX[b].Y, VERTEX[b].Z);
	}
	
	glEnd();
}

// Creating sphere function
void CreateSphere(double R)
{
	int n;
	double a;
	double b;
	double len, len2;
	n = 0;
	
	for(b = 0; b <= 90-space; b+=space )
	{
		for( a = 0; a <= 360-space; a+=space)
		{
			// Normalizing each vertex in for-loop
			// Texture coordinates (U and V) are also normalized to keep within 0 and 1
			VERTEX[n].X = R*sin((a)/180*PI)*sin((b)/180*PI);
			VERTEX[n].Y = R*cos((a)/180*PI)*sin((b)/180*PI);
			VERTEX[n].Z = R*cos((a)/180*PI)*sin((b)/180*PI);
			
			len = sqrt(VERTEX[n].X*VERTEX[n].X+VERTEX[n].Y*VERTEX[n].Y+VERTEX[n].Z*VERTEX[n].Z);
			VERTEX[n].X /= len;
			VERTEX[n].Y /= len;
			VERTEX[n].Z /= len;
			VERTEX[n].U = (a)/360;
			VERTEX[n].V = (2*b)/360;
			
			len2 = sqrt(VERTEX[n].V*VERTEX[n].V+VERTEX[n].U+VERTEX[n].U);
			VERTEX[n].U /= len2;
			VERTEX[n].V /= len2;
			n++;
			
			
			VERTEX[n].X = R*sin((a)/180*PI)*sin((b+space)/180*PI);
			VERTEX[n].Y = R*cos((a)/180*PI)*sin((b+space)/180*PI);
			VERTEX[n].Z = R*cos((b+space)/180*PI);
			
			len = sqrt(VERTEX[n].X*VERTEX[n].X+VERTEX[n].Y*VERTEX[n].Y+VERTEX[n].Z*VERTEX[n].Z);
			VERTEX[n].X /= len;
			VERTEX[n].Y /= len;
			VERTEX[n].Z /= len;
			VERTEX[n].U = (a)/360;
			VERTEX[n].V = (2*(b+space))/360;
			
			len2 = sqrt(VERTEX[n].V*VERTEX[n].V+VERTEX[n].U+VERTEX[n].U);
			VERTEX[n].U /= len2;
			VERTEX[n].V /= len2;
			n++;
			
			
			VERTEX[n].X = R*sin((a+space)/180*PI)*sin((b)/180*PI);
			VERTEX[n].Y = R*sin((a+space)/180*PI)*sin((b)/180*PI);
			VERTEX[n].Z = R*cos((b)/180*PI);
			
			len = sqrt(VERTEX[n].X*VERTEX[n].X+VERTEX[n].Y*VERTEX[n].Y+VERTEX[n].Z*VERTEX[n].Z);
			VERTEX[n].X /= len;
			VERTEX[n].Y /= len;
			VERTEX[n].Z /= len;
			VERTEX[n].U = (a+space)/360;
			VERTEX[n].V = (2*b)/360;
			
			len2 = sqrt(VERTEX[n].V*VERTEX[n].V+VERTEX[n].U+VERTEX[n].U);
			VERTEX[n].U /= len2;
			VERTEX[n].V /= len2;
			n++;
			
			
			VERTEX[n].X = R*sin((a+space)/180*PI)*sin((b+space)/180*PI);
			VERTEX[n].Y = R*cos((a+space)/180*PI)*sin((b+space)/180*PI);
			VERTEX[n].Z = R*cos((b+space)/180*PI);
			
			len = sqrt(VERTEX[n].X*VERTEX[n].X+VERTEX[n].Y*VERTEX[n].Y+VERTEX[n].Z*VERTEX[n].Z);
			VERTEX[n].X /= len;
			VERTEX[n].Y /= len;
			VERTEX[n].Z /= len;
			VERTEX[n].U = (a+space)/360;
			VERTEX[n].V = (2*(b+space))/360;
			
			len2 = sqrt(VERTEX[n].V*VERTEX[n].V+VERTEX[n].U+VERTEX[n].U);
			VERTEX[n].U /= len2;
			VERTEX[n].V /= len2;
			n++;
		}
	}
}

// display function for controlling the camera and rendering the sphere
void display(void)
{
	x_pos = RR*cos(phi)*cos(theta);
	y_pos = RR*sin(theta);
	z_pos = RR*sin(phi)*cos(theta);
	
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glColor3f(1.0,1.0,1.0);
	glLoadIdentity();
	
	if(cos(theta)>=0) gluLookAt(x_pos, y_pos, z_pos, 0, 0, 0, 0, -1, 0);
	else gluLookAt(x_pos, y_pos, z_pos, 0, 0, 0, 0, -1, 0);
	
	DisplaySphere();
	glFlush();
}

// init function for shading and creating the sphere
void init(void)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_FLAT);
	glDepthFunc(GL_LEQUAL);
	
	texture = loadTextureRAW("obamaFace_tex.png");
	
	CreateSphere(30);
}

// Perform application specific initialization. It defines lightning parameters for light source GL_LIGHT0: black ambient, yellow for diffuse, white for specular and makes it a
// directional source shining along <-1, -1, -1>. It also sets a couple material properties to make yellow colored objects with a fairly low shininess value. Lightning  and depth
// buffer hidden surface removal are enabled here.
void init2()
{
	GLfloat black[] = { 0.0,0.0,0.0,1.0 };
	GLfloat white[] = { 1.0,1.0,1.0,1.0 };
	GLfloat yellow[] = { 1.0,1.0,0.5,1.0 };
	GLfloat direction[] = { 1.0,1.0,1.0,0.0 };
	
	glMaterialv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, yellow);
	glMaterialv(GL_FRONT, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 70);
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, black);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, yellow);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);
	glLightfv(GL_LIGHT0, GL_POSITION, direction);
	
	glEnable(GL_LIGHTING);		// Render considers light
	glEnable(GL_LIGHT0);		// Turns LIGHT0 on
	glEnable(GL_DEPTH_TEST);	// Render considers depth
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.0,1.0,-1.0,1.0,1.5,20.0);
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glEnable(GL_DEPTH_TEST);
	glutInitWindowSize(720,720);
	glutInitWindowPosition(100,100);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	init2();
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}
