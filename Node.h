/* 3GC3 Assignment 3
Name: Sean McKay, Rebecca Tran
Student Numbers: 1423885, 1425611
MacID: mckaysm, tranr5
*/

#ifndef NODE_H
#define NODE_H

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>	
#endif

#include <stdlib.h>
#include "math3D.h"
#include <vector>
#include "Material.h"

using namespace std;

enum NodeType {root, transform, object};
enum ObjectType {cube, sphere, cone, torus, dodecahedron};
enum TransformType {translate, rotate, scale};

class Node {

public:
	Node();
	vector<Node*> *children;
	Node* parent;
	NodeType type;
	int ID;
	vec3D amount;
	vec4D amount4;
	bool intersects;
	int material;


	float xRot = 0;
	float yRot = 0;	
	float zRot = 0;

	float xScale = 0;
	float yScale = 0;
	float zScale = 0;

	float xTran = 0;
	float yTran = 0;
	float zTran = 0;


	ObjectType o_type;
	TransformType t_type;
	void traverse();
	int intersect(vec3D r0, vec3D rd);
	void drawHitboxes();

	virtual void draw();
	virtual bool testIntersection(vec3D r0, vec3D rd);
	virtual void translateBounds(float x, float y, float z);
	virtual void scaleBounds(float x, float y, float z);
	virtual void switchMaterial(int n);
	virtual void drawHitbox();
};

#endif