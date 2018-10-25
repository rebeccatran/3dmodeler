/* 3GC3 Assignment 3
Name: Sean McKay, Rebecca Tran
Student Numbers: 1423885, 1425611
MacID: mckaysm, tranr5
*/

#include <stdio.h>
#include <stdlib.h>
#include "math3D.h"
#include "NodeObject.h"
#include "NodeTransform.h"
#include "Node.h"
#include "SceneGraph.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>

#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

#include "math.h"
#include "Material.h"

#define PI 3.14159265;

using namespace std;

//Global variables
vec3D tran;	//3D vectors for translating, rotating and scaling
vec3D rota;
vec3D scal;

//Current material to set objects to
int mate = 1;

bool textureToggle = false;	//toggle to switch texture on and off

bool light = true;	//toggle for switching the light on and off
bool lightSwitch = true;	//toggle for switching between light0 and light1

float light_pos[] = {0,5,-10,1};	//light location, where light0 and light1 is at 
float light_pos1[] = {-10,5,0,1};	

float amb0[4]  = {1, 1, 1, 1}; //setting the ambient, diffuse and specular values for light0
float diff0[4] = {1, 1, 1, 1}; 
float spec0[4] = {1, 1, 1, 1};

float amb1[4]  = {0.9, 0.9, 0.9, 1}; //setting the ambient, diffuse and specular values for light0
float diff1[4] = {0.9, 0.9, 0.9, 1}; 
float spec1[4] = {0.9, 0.9, 0.9, 1};

float camPos[] = {10, 5, 10};	//camera position
float camPos2[3];	//temp camera array for camera movement

string fName = "";	//for saving file
string lName = "";	//for loading file


//ID of the selected object in the scene, -1 for no selection, 0 for root node
int selected = -1;

//ID for identifying nodes, increments when new nodes are created and assigned an ID
int ID = 1;

//Scenegraph storing object and transformation nodes from the root node
SceneGraph *sGraph;

//Vector of object IDs
vector <int> objIDs;

//declaring the image for textures, using three different textures
GLubyte* img_data1;	
GLubyte* img_data2;
GLubyte* img_data3;

GLuint textures[3];	//array for storing the three different textures
int width, height, ma;	//width, height, max variables for the file of the texures

/* LoadPPM -- loads the specified ppm file, and returns the image data as a GLubyte 
 *  (unsigned byte) array. Also returns the width and height of the image, and the
 *  maximum colour value by way of arguments
 *  usage: GLubyte myImg = LoadPPM("myImg.ppm", &width, &height, &max);
 */
GLubyte* LoadPPM(char* file, int* width, int* height, int* ma)
{
	GLubyte* img;
	FILE *fd;
	int n, m;
	int  k, nm;
	char c;
	int i;
	char b[100];
	float s;
	int red, green, blue;
	
	fd = fopen(file, "r");
	fscanf(fd,"%[^\n] ",b);
	if(b[0]!='P'|| b[1] != '3')
	{
		printf("%s is not a PPM file!\n",file); 
		exit(0);
	}
	printf("%s is a PPM file\n", file);
	fscanf(fd, "%c",&c);
	while(c == '#') 
	{
		fscanf(fd, "%[^\n] ", b);
		printf("%s\n",b);
		fscanf(fd, "%c",&c);
	}
	ungetc(c,fd); 
	fscanf(fd, "%d %d %d", &n, &m, &k);

	printf("%d rows  %d columns  max value= %d\n",n,m,k);

	nm = n*m;

	img = (GLubyte*)malloc(3*sizeof(GLuint)*nm);


	s=255.0/k;


	for(i=0;i<nm;i++) 
	{
		fscanf(fd,"%d %d %d",&red, &green, &blue );
		img[3*nm-3*i-3]=red*s;
		img[3*nm-3*i-2]=green*s;
		img[3*nm-3*i-1]=blue*s;
	}

	*width = n;
	*height = m;
	*ma = k;

	return img;
}

//textures function
void initTexture(void) {
	img_data1 = LoadPPM("hay.ppm", &width, &width, &ma); 	//loading the texture by using the LoadPPM function, into img_data1
	img_data2 = LoadPPM("red.ppm", &width, &height, &ma);	//loading the texture by using the LoadPPM function, into img_data2
	img_data3 = LoadPPM("tree.ppm", &width, &height, &ma);	//loading the texture by using the LoadPPM function, into img_data3

	//enabling textures
	glEnable(GL_TEXTURE_2D);	

	glGenTextures(3, textures); //using three textures 
	glBindTexture(GL_TEXTURE_2D, textures[0]); 	//binding the first texture stored in the textures array

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img_data1); //getting the necessary information of the texture

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 	//setting parameters
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT ); 
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 

	glBindTexture(GL_TEXTURE_2D, textures[1]); //same as above but for second texture

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img_data2); 

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 	//setting parameters
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT ); 
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 

	glBindTexture(GL_TEXTURE_2D, textures[2]); //same as above but for third textures

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img_data3); 

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 	//setting parameters
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT ); 
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

//Init for lighting and other declarations of the screen
void init(void){
	glClearColor(0, 0, 0, 0);
	glColor3f(1, 1, 1);

	//enabling lighting
	glEnable(GL_LIGHTING); 	
	glEnable(GL_LIGHT0);	//turning on light0
	glEnable(GL_LIGHT1);	//turning on light2

	//setting the amb, diff, and spec for light0
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb0); 
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff0); 
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec0);

	//setting the amb, diff, and spec for light0
	glLightfv(GL_LIGHT1, GL_AMBIENT, amb1); 
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diff1); 
	glLightfv(GL_LIGHT1, GL_SPECULAR, spec1);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 1, 100);
	sGraph = new SceneGraph();
}

//Draws the x, y, and z axis to show orientation in 3D space
void drawAxis()
{
	glPushMatrix();	
		glBegin(GL_LINES);
			glColorMaterial(GL_FRONT, GL_AMBIENT);	
			glEnable(GL_COLOR_MATERIAL);
			glColor3f(1, 0, 0);
			glVertex3f(-50,0,0);	//drawing -x axis
			glVertex3f(50,0,0);		//drawing x axis
			glColor3f(0,1,0);
			glVertex3f(0,-50,0);	//drawing -y axis
			glVertex3f(0,50,0);		//drawing y axis
			glColor3f(0,0,1);
			glVertex3f(0,0,-50);	//drawing -z axis
			glVertex3f(0,0,50);		//drawing z axis
		glEnd();
	glPopMatrix();
}

//drawing the ground plane
void drawPlane(){
	glPushMatrix();	
		glPolygonMode(GL_FRONT, GL_FILL);	//colouring in the groundplane
		glBegin(GL_QUADS);
			glNormal3f(0,10,0);
			glVertex3f(-50,-10,-50);	//far back right vertex
			glVertex3f(-50,-10,50);		//far back left vertex
			glVertex3f(50,-10,50);		//near front left vertex
			glVertex3f(50,-10,-50);		//near front right vertex
		glEnd();
	glPopMatrix();

	glutPostRedisplay();
}

//Assigns an ID to a given node so it can be found by searching
void assignID(Node *node) {
	node->ID = ID;
	//Increment the ID counter
	ID++;
}

//Adds a new object node to the scene with initial translate, scale, and rotate nodes
void addObj(ObjectType t) {
	vec3D initT = vec3D(0, 0, 0);
	vec4D initRX = vec4D(0, 1, 0, 0);
	vec4D initRY = vec4D(0, 0, 1, 0);
	vec4D initRZ = vec4D(0, 0, 0, 1);
	vec3D initS = vec3D(1, 1, 1);

	//Create and assign IDs to the object and transformation nodes
	Node *tNode = new NodeTransform(translate, initT);
	Node *rXNode = new NodeTransform(rotate, initRX);
	Node *rYNode = new NodeTransform(rotate, initRY);
	Node *rZNode = new NodeTransform(rotate, initRZ);
	Node *sNode = new NodeTransform(scale, initS);
	Node *oNode = new NodeObject(t);
	assignID(tNode);
	assignID(rXNode);
	assignID(rYNode);
	assignID(rZNode);
	assignID(sNode);
	assignID(oNode);

	oNode->switchMaterial(mate);	//switching material for the object node
	objIDs.push_back(oNode->ID);

	//Insert the new object into the scenegraph at the root
	sGraph->toRoot();
	sGraph->insertNode(tNode);
	sGraph->insertNode(rXNode);
	sGraph->insertNode(rYNode);
	sGraph->insertNode(rZNode);
	sGraph->insertNode(sNode);
	sGraph->insertNode(oNode);

	//Set the selected ID to the newly created object's ID
	//printf("%i\n",sGraph->current->ID);
	selected = sGraph->current->ID;

}

//drawing the display
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);	//putting light0 and light1 into position
	glLightfv(GL_LIGHT1, GL_POSITION, light_pos1);

	gluLookAt(camPos[0], camPos[1], camPos[2], 0, 0, 0, 0, 1, 0);

	glDisable(GL_TEXTURE_GEN_S); //disable texture coordinate generation
 	glDisable(GL_TEXTURE_GEN_T);
	drawPlane();	//drawing the ground plane
	drawAxis();		//drawing the axis

	glPushMatrix();	//drawing sphere for light0 source
		glTranslatef(light_pos[0], light_pos[1], light_pos[2]);	//translating the sphere according to the position movement
		glutSolidSphere(0.2,100,100);	
	glPopMatrix();

	glPushMatrix();	//sphere for lightsource 1 and translating sphere accordingly
		glTranslatef(light_pos1[0], light_pos1[1], light_pos1[2]);
		glutSolidSphere(0.2,100,100);
	glPopMatrix();

	if(textureToggle == true){	//if the texture is on
		glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
	    glEnable(GL_TEXTURE_GEN_T);
	}

	if (light == false){	//turning the light off
		glDisable(GL_LIGHTING);
	}
	if(light == true){	//turning the light on 
		glEnable(GL_LIGHTING);
	}

	//Recursively generate the scene graph
	sGraph->traverse();

	//Draw hitboxes for every object
	sGraph->drawHitboxes();

	glutPostRedisplay();

	glutSwapBuffers();
}

//Calculates the equation of a ray passing through the eye in world coordinates
//from the given x and y coordinate in screen coordinates
//Tests if any of the objects in the scene intersect with the ray, and if so,
//sets the last of the intersecting objects as the currently selected object
void calcRay(float x, float y) {
	GLint viewport[4];			//declaring arrays and variables
	GLdouble modelview[16];
	GLdouble projection[16];
	float winX;
	float winY;
	GLdouble pos1X, pos1Y, pos1Z;
	GLdouble pos2X, pos2Y, pos2Z;
	GLdouble dirX, dirY, dirZ;
	vec3D r0, rd;

	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);

	glGetDoublev(GL_PROJECTION_MATRIX, projection);

	//Flip the y coordinate to have the proper opengl screen coords
	winX = x;
	winY = (float)viewport[3] - y;

	//Unproject the coordinates from screen to world coordinates on the near clipping plane
	gluUnProject(winX, winY, 0.0, modelview, projection, viewport, &pos1X, &pos1Y, &pos1Z);

	//Unproject the coordinates from screen to world coordinates on the far clipping plane
	gluUnProject(winX, winY, 1.0, modelview, projection, viewport, &pos2X, &pos2Y, &pos2Z);

	//Calculate a normalized ray between the far and near clipping planes
	dirX = pos2X - pos1X;
	dirY = pos2Y - pos1Y;
	dirZ = pos2Z - pos1Z;

	r0.x = pos1X; r0.y = pos1Y; r0.z = pos1Z;
	rd.x = dirX; rd.y = dirY; rd.z = dirZ;

	rd = rd.normalize();

	//Go to the root of the scene
	sGraph -> toRoot();

	//Test for intersection of the ray and any of the objects in the scene,
	//Return the ID of the last object that intersects, -1 if no objects intersect
	int z = sGraph -> intersect(r0, rd);

	//Go back to the root of the scene
	sGraph -> toRoot();
	//Select the object
	sGraph -> search(z);
	selected = z;

}

//Delete the selected object from the scene
void deleteSelectedObject() {
	if (selected == -1 or selected == 0) {
		return;	//do nothing
	}
	else {
		if (sGraph->current->type == object) {
			while (sGraph->current->type == object || sGraph->current->type == transform) {	//if it is an object node or transformation node
				sGraph->deleteNode();	//delete the node until root is reached
			}
		}
	}
}

//selecting objects and deleting with left and right mouse clicks
void mouse(int btn, int state, int x, int y){
	//Select objects using ray intersection test
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		calcRay((float)x,  (float)y);
	}
	glutPostRedisplay();

	//Delete object from the scene
	if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
		calcRay((float)x,  (float)y);
		deleteSelectedObject();
	}
}

//modifying each object
void transformObj(TransformType t, float x, float y, float z, float w = 0) {
	//Go the selected object in the scene 
	sGraph->toRoot();
	sGraph->search(selected);

	float wR = w * 3.14159265358979 / 180;

	//Transform the hitbox for the object
	switch (t) {
		//Translate the object's hitbox, and store the translation coordinates
		case (translate):
			sGraph->current->translateBounds(x, y, z);
			sGraph->current->xTran += x;
			sGraph->current->yTran += y;
			sGraph->current->zTran += z;

			break;
		//Store rotation coordinates, hitbox rotation not implemented
		case (rotate): 
			if (x == 1) {
				sGraph->current->xRot += wR;
			}
			if (y == 1) {
				sGraph->current->yRot += wR;
			}
			if (z == 1) {
				sGraph->current->zRot += wR;
			}
			break;
		//Scale the object's hitbox, and store the scale coordinates
		case (scale):
			sGraph->current->scaleBounds(x, y, z);
			sGraph->current->xScale += x;
			sGraph->current->yScale += y;
			sGraph->current->zScale += z;
			break;
	}

	//Go to the parent node until the correct translation node is selected
	while (sGraph->current->type == object || (sGraph->current->type == transform && sGraph->current->t_type != t)) {
		sGraph->toParent();
	}

	//Modify the existing transformation node for the object
	//Rotate the object
	if (t == rotate) {
		if (sGraph->current->amount4.z == z) {
			sGraph->current->amount4.w += w;
		}
		else if (sGraph->current->amount4.y == y) {
			sGraph->toParent();
			sGraph->current->amount4.w += w;

		}
		else if (sGraph->current->amount4.x == x) {
			sGraph->toParent();
			sGraph->toParent();
			sGraph->current->amount4.w += w;
		}
	}
	
	//Translate or scale the object
	else {
		sGraph->current->amount.x += x;
		sGraph->current->amount.y += y;
		sGraph->current->amount.z += z;
	}
}

//Save the info to file
void saveNodeInfo(string fName) {
	fName = fName + ".txt";	//saving file as .txt
	const char *fChar = fName.c_str();	//changing from string to char

	//opening the file
	ofstream saveFile;	
	saveFile.open(fChar);

	if (objIDs.size() != 0) {	//iterating through the scene objects (which each have an id)
		for (int i = 0; i < objIDs.size(); i++) {	//for each object, do the following
			sGraph->toRoot();	//go to the root of scene graph
			sGraph->search(objIDs.at(i));	//searching for the object with a given id
			// printf("The current object's ID is %i\n", sGraph->current->ID);	
			saveFile << sGraph->current->o_type << ",";		//storing the type, x, y and z translation, rotation ange and the scale, and material
			saveFile << sGraph->current->xTran << "," << sGraph->current->yTran << "," << sGraph->current->zTran << ",";
			saveFile << sGraph->current->xRot << "," << sGraph->current->yRot << "," << sGraph->current->zRot << ",";
			saveFile << sGraph->current->xScale << "," << sGraph->current->yScale << "," << sGraph->current->zScale << ",";
			saveFile << sGraph->current->material;
			saveFile << "\n";
		}
	}
	saveFile.close();	//close the file
}

//Loading a previously saved file onto the screen
void loadNodeInfo(string lName) {
	lName = lName + ".txt";	//taking the existing name from user input

	sGraph->deleteScene();	//loading, therefore delete whatever is on the screen currently

	const char *lChar = lName.c_str();	//changing from string to char
	float pX, pY, pZ, rX, rY, rZ, sX, sY, sZ;	//declaring float variables for translate, rotate and scale -- for conversion from str to float
	int mat, type;	//declaring variables for material and type
	string tMat, tType, tpX, tpY, tpZ, trX, trY, trZ, tsX, tsY, tsZ;	//declaring string variables for translate, rotate and scale for when retrieving numbers from file

	//opening file and declaring temp variable and line variable
	ifstream loadFile;	
	loadFile.open(lChar);
	string line;
	NodeObject *temp;

	while (getline(loadFile, line)) {
			stringstream currentLine(line);		//reading each line and retrieving translationg position, rotation, scale sizes 
			getline(currentLine, tType, ',');	//retrieving the type of the object and retrieving the material
			getline(currentLine, tpX, ',');
			getline(currentLine, tpY, ',');
			getline(currentLine, tpZ, ',');
			getline(currentLine, trX, ',');
			getline(currentLine, trY, ',');
			getline(currentLine, trZ, ',');
			getline(currentLine, tsX, ',');
			getline(currentLine, tsY, ',');
			getline(currentLine, tsZ, ',');
			getline(currentLine, tMat, ',');

			type = atoi(tType.c_str());		//converting each of the retrieved values from str to floats or ints
			mat = atoi(tMat.c_str());
			pX = atof(tpX.c_str());
			pY = atof(tpY.c_str());
			pZ = atof(tpZ.c_str());
			rX = atof(trX.c_str()) / 3.14159265358979 * 180;
			rY = atof(trY.c_str()) / 3.14159265358979 * 180;
			rZ = atof(trZ.c_str()) / 3.14159265358979 * 180;
			sX = atof(tsX.c_str());
			sY = atof(tsY.c_str());
			sZ = atof(tsZ.c_str());
			// printf("values are %i %i %f %f %f\n", type, mat, pX, pY, pZ);

			//setting the material
			mate = mat;		
			switch(type) {	//setting the type of the object
				case (0):
					addObj(cube);	//drawing each of the object depending on the type
				break;

				case (1):
					addObj(sphere);
				break;

				case (2):
					addObj(cone);
				break;

				case (3):
					addObj(torus);
				break;

				case (4):
					addObj(dodecahedron);
				break;
			}
			transformObj(scale, sX, sY, sZ);	//scaling the object according to the values retrieved
			transformObj(rotate, 1, 0, 0, rX);	//rotating about the angle retrieved for the x, y and z axis 
			transformObj(rotate, 0, 1, 0, rY);
			transformObj(rotate, 0, 0, 1, rZ);
			transformObj(translate, pX, pY, pZ);	//translating according to the values retrieved
	}
	loadFile.close();	//close the file 
}

//special for the arrow keys, ctrl, shift and alt keys -- responsible for camera movement, rotating/translating/scaling objects
void special(int key, int x, int y)
{
	int mod = glutGetModifiers();
	switch(key)
	{
		//Move the selected object left in the x-axis
		case GLUT_KEY_RIGHT: 

			if ( mod == GLUT_ACTIVE_SHIFT) transformObj(rotate, 0, 1, 0, 1);	//Rotate the object around the y-axis
			else if ( mod == GLUT_ACTIVE_CTRL) transformObj(scale, 0.1, 0, 0);	//Scale object positively in the x direction 
			else if (mod == GLUT_ACTIVE_ALT){	//Changing the camera position
				camPos2[0] = camPos[0];	//Using another array to store the old values into a temp array
				camPos2[2] = camPos[2];
				camPos[0] = camPos2[0]*cos(0.05)+camPos2[2]*sin(0.05);	//Moving the camera around about the axes, in the positive for x and z
        		camPos[2] = camPos2[2]*cos(0.05)-camPos2[0]*sin(0.05);	//storing new values back into camPos
			}
			else transformObj(translate, 1, 0, 0);	//Translate the selected object in x axis

		break;

		case GLUT_KEY_LEFT: 
			if ( mod == GLUT_ACTIVE_SHIFT) transformObj(rotate, 0, 1, 0, -1);	//opposite rotation for y
			else if ( mod == GLUT_ACTIVE_CTRL) transformObj(scale, -0.1, 0, 0);	//opposite scaling for x
			else if (mod == GLUT_ACTIVE_ALT){
				camPos2[0] = camPos[0];
				camPos2[2] = camPos[2];
 				camPos[0] = camPos2[0]*cos(-0.05)+camPos2[2]*sin(-0.05);	//oposite camera rotation, in the negative for x and z
        		camPos[2] = camPos2[2]*cos(-0.05)-camPos2[0]*sin(-0.05);
			}
			else transformObj(translate, -1, 0, 0);

		break;

		case GLUT_KEY_UP:
			if ( mod == GLUT_ACTIVE_SHIFT) transformObj(rotate, 1, 0, 0, 1);	//rotate about x axis
			else if ( mod == GLUT_ACTIVE_CTRL) transformObj(scale, 0, 0.1, 0);	//scale about y axis
			else if (mod == GLUT_ACTIVE_ALT){
				camPos2[1] = camPos[1];
				camPos2[2] = camPos[2];
				camPos[1] = camPos2[1]*cos(0.05)-camPos2[2]*sin(0.05); //camera movement, positive direction in y and z
				camPos[2] = camPos2[1]*sin(0.05)+camPos2[2]*cos(0.05);

			}
			else transformObj(translate, 0, 0, 1);	//translate object in z axis

		break;

		case GLUT_KEY_DOWN:
			if ( mod == GLUT_ACTIVE_SHIFT) transformObj(rotate, 1, 0, 0, -1);	//rotate in -x axis
			else if ( mod == GLUT_ACTIVE_CTRL) transformObj(scale, 0, -0.1, 0);	//scale in -y axis
			else if (mod == GLUT_ACTIVE_ALT){
				camPos2[1] = camPos[1];
				camPos2[2] = camPos[2];
				camPos[1] = camPos2[1]*cos(-0.05)-camPos2[2]*sin(-0.05);	//camera movement, negative in y and z
				camPos[2] = camPos2[1]*sin(-0.05)+camPos2[2]*cos(0.05);
			}
			else transformObj(translate, 0, 0, -1);	//translate in -z axis

		break;
	}

	glutPostRedisplay();
}

//keyboard actions to be done when 'key' is called
void keyboard(unsigned char key, int xIn, int yIn)
{
	int mod = glutGetModifiers();	
	switch (key)
	{
		case 'q':
		case 27:	//27 is the esc key
			exit(0);
			break;

		case 'z':
			addObj(cube);	//calling cube to be drawn
			break;

		case 'x':
			addObj(sphere);	//calling sphere object to be drawm
			break;

		case 'c':
			addObj(cone);	//calling cone object to be drawn
			break;

		case 'v':
			addObj(torus);	//calling the object torus to be drawn
			break;

		case 'b':
			addObj(dodecahedron);	//calling the object dodecahedron to be drawn
			break;

		case 'o':
			if (light == false){	//light is "off"
				light = true;		//turn light "on"
			} else {
				light = false;	//light is on, turn it off
			}
		break;

		case 'i':
			if (lightSwitch == true){	//if the light0 is current light moving
				lightSwitch = false;	//make light1 moving
			} else {
				lightSwitch = true; //light1 is currently light moving, make light0 the light moving
			}
		break;

		case 'u':
			if(lightSwitch == true){
				// printf("I'm moving!\n");	
	 			light_pos[1]+=2.5;	//moving light0, inc y value
	 		} else {		
	 			light_pos1[1]+=2.5; 	//moving light1, inc y value	
	 		}
		break;

		case 'j':
			if(lightSwitch == true){
	 			light_pos[1]-=2.5;	//moving light0, dec y value
	 		} else {
	 			light_pos1[1]-=2.5;		//moving light1, dec y value
	 		}
		break;
		
		case 'h':
			if(lightSwitch == true){	//if lightswitch is true, moving light0
	 			light_pos[0]-=2.5;	//decrementing the light0 x value
	 		} else {				//lightswitch is false, moving light1
	 			light_pos1[0]-=2.5;	 //decrementing the light1 x value
	 		}
		break;
		
		case 'k':
			if(lightSwitch == true){	//light switch is true, moving light0
	 			light_pos[0]+=2.5;	//incrementing light0 x value
	 		} else {
	 			light_pos1[0]+=2.5;	 	//moving light1, incrementing x value
	 		}
		break;

		case 'r':
			sGraph->deleteScene();	//delete all the nodes and objects on the scene graph
		break;

		case 's':	//saving current scene
			fName = "";
			while(fName == "") {	//if there is no file name
				cout << "Enter file name for saving\n";	//prompt for file name
				cin >> fName;	//store the name
			}
			saveNodeInfo(fName);	//saving information function, passing in the filename user inputted

		break;

		case 'l':	//loading a saved file 
			lName = "";
			while (lName == "") {	//if there is no file name inputted
				cout << "Enter file name to load from\n";	//prompt for file name to be loaded
				cin >> lName;	//store the file name that is being loaded
				loadNodeInfo(lName);	//load information function, passing in the filename for which file to be loaded from user input
			}

		break;

		case 'm':
			sGraph->current->switchMaterial(mate);	//switch material for the current selected object to the current material
		break;

		case '.':
			if ( mod == GLUT_ACTIVE_CTRL) transformObj(scale, 0, 0, 0.1);	//scaling up in the z axis
			else transformObj(translate, 0, 1, 0);	//transing in y axis (going up)
		break;

		case ',':
			if ( mod == GLUT_ACTIVE_CTRL) transformObj(scale, 0, 0, -0.1);	//scaling down in the z axis
			else transformObj(translate, 0, -1, 0);	//translating in the -y axis (going down)
		break;

		case '<':
			if ( mod == GLUT_ACTIVE_SHIFT) transformObj(rotate, 0, 0, 1, 1);	//rotate object in z axis
		break;

		case '>':
			if ( mod == GLUT_ACTIVE_SHIFT) transformObj(rotate, 0, 0, 1, -1);	//rotate object in -z axis
		break;

		case '1':
			printf("SET to 1\n");
			mate = 1;	//setting the material to bronze to be switched
		break;

		case '2':
			printf("SET to 2\n");
			mate = 2;	//setting material to chrome 
		break;

		case '3':
			printf("SET to 3\n");
			mate = 3; 	//setting material to copper
		break;

		case '4':
			printf("SET to 4\n");
			mate = 4;	//setting material to pearl
		break;	

		case '5':
			printf("SET to 5\n");
			mate = 5;	//setting material to obsidian
		break;

		case '6':
		    glBindTexture(GL_TEXTURE_2D, textures[0]);	//setting the texture to the first texture stored in the array when 6 is hit
		break;

		case '7':
		    glBindTexture(GL_TEXTURE_2D, textures[1]);	//setting the texture to the second texture stored when 7 is hit 
		break;

		case '8':
		    glBindTexture(GL_TEXTURE_2D, textures[2]);	//setting the texture to the third texture stored when 8 is hit 
		break;

		case '9':	//turning texture on and off with toggle
			if(textureToggle == true){	//if texture is currently applied
				textureToggle = false;	//turn the texture off
				glDisable(GL_TEXTURE_GEN_S); //disable texture coordinate generation
    			glDisable(GL_TEXTURE_GEN_T);
			} else {	//if the texture is off
				textureToggle = true;	//turn texture toggle on
				glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
		    	glEnable(GL_TEXTURE_GEN_T);
			}
		break;
	}
	glutPostRedisplay();
}

//declaring keyboard functions for user
void instructions(){	
	printf("'1'-------------------------Change current material to bronze \n");
	printf("'2'-------------------------Change current material to chrome \n");
	printf("'3'-------------------------Change current material to copper \n");
	printf("'4'-------------------------Change current material to pearl \n");
	printf("'5'-------------------------Change current material to obsidian \n");
	printf("'6'-------------------------Change texture to hay\n");
	printf("'7'-------------------------Change texture to prism\n");
	printf("'8'-------------------------Change texture to tree\n");
	printf("'9'-------------------------Turn texture on and off\n");
	printf("'m'-------------------------Change the selected object material to the current material\n");
	printf("'z'-------------------------Draw cube object\n");
	printf("'x'-------------------------Draw sphere object\n");
	printf("'c'-------------------------Draw cone object\n");
	printf("'v'-------------------------Draw torus object\n");
	printf("'b'-------------------------Draw dodecahedron object\n");
	printf("'r'-------------------------Clear the objects on the scene\n");
	printf("'s'-------------------------Save scene, requires text file name\n");
	printf("'l'-------------------------Load scene\n");
	printf("'o'-------------------------Turn light on and off\n");
	printf("'i'-------------------------Switch between Light0 and Light1\n");
	printf("'h','k','u','j'-------------Move whichever light you are on (based on 'i') about x and y axis\n");
	printf("Arrow Keys------------------Translate selected object in the x and z axes\n");
	printf("Arrow Keys + SHIFT----------Rotate selected object in the x and z axes\n");
	printf("Arrow Keys + CTRL-----------Scale selected object in the x and z axes\n");
	printf("Arrow Keys + ALT------------Move camera position\n");
	printf("',', '.'--------------------Translate selected object in y axis\n");
	printf("',', '.' + SHIFT ('<','>')--Rotate selected object in y axis\n");
	printf("',', '.' + CTRL-------------Scale selected object in y axis\n");
	printf("Mouse Left Click------------Select an object\n");
	printf("Mouse Right Click-----------Delete an object\n");
}

int main(int argc, char** argv){
	glutInit(&argc, argv);		//starts up GLUT
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowSize(800, 800);	//declaring window position
	glutInitWindowPosition(50, 50);	//declaring window size 

	glutCreateWindow("A3 Modeller -- mckaysm, tranr5");	//creates the window
	init();	//calling init function for lighting and other declarations 
	initTexture();	//calling texture functions	
	instructions();	//displaying instructions to user on the command prompt
	glutDisplayFunc(display);	//calling display function 
	glutMouseFunc(mouse);	//calling mouse function

	glutKeyboardFunc(keyboard);	//calling keyboard keys
	glutSpecialFunc(special);	//calling special keys, arrow keys and such  

	glutMainLoop();				//starts the event glutMainLoop
	return(0);					//return may not be necessary on all compilers
}