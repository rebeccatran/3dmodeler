/* 3GC3 Assignment 3
Name: Sean McKay, Rebecca Tran
Student Numbers: 1423885, 1425611
MacID: mckaysm, tranr5
*/

#ifndef __NODE_OBJECT_H__
#define __NODE_OBJECT_H__

#include "Node.h"

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>	
#endif

#include "math3D.h"
#include <vector>
#include "Material.h"

struct plane {
	float a;
	float b;
	float c; 
	float d;
} ;

class NodeObject:public Node{
public:
	NodeObject(ObjectType obj);

		point3D min;
		point3D max;

		Material *mat;
		//int material;

		point3D p0, p1, p2, p3, p4, p5, p6, p7;

		//declaring point3D arrays
		point3D frontPlane[4];
		point3D backPlane[4];
		point3D leftPlane[4];
		point3D rightPlane[4];
		point3D topPlane[4];
		point3D bottomPlane[4];

		//declaring vec3D variables for the normals on each of the faces
		vec3D frontNorm;
		vec3D backNorm;
		vec3D leftNorm;
		vec3D rightNorm;
		vec3D topNorm;
		vec3D bottomNorm;

		vector<plane> planes = vector<plane>(6);

		void setPoints();	//setting the points for the bounding prism around object
		void calcBounds();	//calculating the planes and bounds for each of the planes
		virtual void translateBounds(float x, float y, float z);	//translating the hitbox
		virtual void scaleBounds(float x, float y, float z);	//scaling the hitbox
		plane calcPlane(point3D points[4]);	//calculating the equation of the planes that contain each face

		
		virtual void draw();	//drawing the objects
		virtual void drawHitbox();	//drawing the hitbox
		virtual void switchMaterial(int n);		//switching between materials
		virtual bool testIntersection(vec3D r0, vec3D rd);	//testing to see if object intersects with ray


};

#endif