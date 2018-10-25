/* 3GC3 Assignment 3
Name: Sean McKay, Rebecca Tran
Student Numbers: 1423885, 1425611
MacID: mckaysm, tranr5
*/

#include "Node.h"
#include "math3D.h"
#include <stdio.h>

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>	
#endif

int intersectID = 0;

Node::Node() {
	type = root;
	children = new vector<Node*>();
	intersects = false;
}

void Node::traverse() {
	glPushMatrix();
	draw();
	const int numChildren = children->size();
	if (numChildren > 0) {
		for (int i = 0; i < numChildren; i++) {
			children->at(i)->traverse();
		}
	}
	glPopMatrix();
}

void Node::draw(){}

void Node::drawHitboxes() {
	if (type == object && intersects) {
		drawHitbox();
	}
	const int numChildren = children->size();
	if (numChildren > 0) {
		for (int i = 0; i < numChildren; i++) {
			children->at(i)->drawHitboxes();
		}
	}
}

int Node::intersect(vec3D r0, vec3D rd){

	bool intersecting = testIntersection(r0, rd);
	if (type == object && intersecting) {
		intersects = true;
		intersectID = this->ID;
	}
	else if (type == object) {
		intersects = false;
		return 0;
	}
	const int numChildren = children->size();
	if (numChildren > 0) {
		for (int i = 0; i < numChildren; i++) {
			children->at(i)->intersect(r0, rd);
		}
	}
	return intersectID;
}

bool Node::testIntersection(vec3D r0, vec3D rd) {}

void Node::translateBounds(float x, float y, float z) {}

void Node::scaleBounds(float x, float y, float z) {}

void Node::switchMaterial(int n) {};

void Node::drawHitbox() {}



