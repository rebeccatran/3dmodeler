/* 3GC3 Assignment 3
Name: Sean McKay, Rebecca Tran
Student Numbers: 1423885, 1425611
MacID: mckaysm, tranr5
*/

//Header file for Material class
#ifndef __MATERIALS_H__
#define __MATERIALS_H__

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>	
#endif

#include <vector>

using namespace std; 

//defining struct 
typedef struct Mat{
	vector<float> mAmb;
	vector <float> mDiff;
	vector <float> mSpec;
	float mShiny;
}Mat;


class Material{
public:
	Material();	//constructor
	int m;
	vector<float> amb;	//declaring the vectors for the amb, diff, spec where the values will be stored 
	vector <float> diff;
	vector <float> spec;
	float shiny;	//shiny value for each material
	Mat bronze;	//type of materials
	Mat chrome;
	Mat copper;
	Mat pearl;
	Mat obsidian;

	void setInitMaterial();	//function to set initial material 
	void switchMaterial(int n);	//function to switch between materials
};



#endif