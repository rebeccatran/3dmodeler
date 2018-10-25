/* 3GC3 Assignment 3
Name: Sean McKay, Rebecca Tran
Student Numbers: 1423885, 1425611
MacID: mckaysm, tranr5
*/

#include "NodeObject.h"


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
#include "math.h"
#include <stdio.h>


using namespace std;

NodeObject::NodeObject(ObjectType obj) {
	type = object;
	o_type = obj;

	min.x = -0.5f;
	min.y = -0.5f;
	min.z = -0.5f;

	max.x = 0.5f;
	max.y = 0.5f;
	max.z = 0.5f;

	material = 1;
	mat = new Material();

	intersects = true;

	setPoints();
	calcBounds();
}

plane NodeObject::calcPlane(point3D points[4]) {
	vec3D temp1, temp2, norm;
	float d;
	plane p;

	temp1 = temp1.createVector(points[1], points[0]);
	temp2 = temp2.createVector(points[3], points[0]);


	norm = temp1.crossProduct(temp2);

	if (norm.x == 0.0f) norm.x = 0.0f;
	if (norm.y == 0.0f) norm.y = 0.0f;
	if (norm.z == 0.0f) norm.z = 0.0f;

	norm = norm.normalize();

	d = (norm.x * points[0].x) + (norm.y * points[0].y) + (norm.z * points[0].z);
	d = -d;

	p.a = norm.x; p.b = norm.y; p.c = norm.z; p.d = d;
	return p;
}

void NodeObject::translateBounds(float x, float y, float z) {

	p0.x += x; p0.y+= y; p0.z += z;
	p1.x += x; p1.y+= y; p1.z += z;
	p2.x += x; p2.y+= y; p2.z += z;
	p3.x += x; p3.y+= y; p3.z += z;
	p4.x += x; p4.y+= y; p4.z += z;
	p5.x += x; p5.y+= y; p5.z += z;
	p6.x += x; p6.y+= y; p6.z += z;
	p7.x += x; p7.y+= y; p7.z += z;

	calcBounds();
}

void NodeObject::scaleBounds(float x, float y, float z) {

	x = x/2; y = y/2; z = z/2;
	p0.x -= x; p0.y -= y; p0.z -= z;
	p1.x -= x; p1.y -= y; p1.z += z;
	p2.x += x; p2.y -= y; p2.z += z;
	p3.x += x; p3.y -= y; p3.z -= z;
	p4.x -= x; p4.y += y; p4.z -= z;
	p5.x -= x; p5.y += y; p5.z += z;
	p6.x += x; p6.y += y; p6.z += z;
	p7.x += x; p7.y += y; p7.z -= z;

	calcBounds();
}


void NodeObject::setPoints() {
	//Specify all points on bounding rectangular prism for object
	p0.x = min.x; p0.y = min.y; p0.z = min.z;
	p1.x = min.x; p1.y = min.y; p1.z = max.z;
	p2.x = max.x; p2.y = min.y; p2.z = max.z;
	p3.x = max.x; p3.y = min.y; p3.z = min.z;
	p4.x = min.x; p4.y = max.y; p4.z = min.z;
	p5.x = min.x; p5.y = max.y; p5.z = max.z;
	p6.x = max.x; p6.y = max.y; p6.z = max.z;
	p7.x = max.x; p7.y = max.y; p7.z = min.z;
}

void NodeObject::calcBounds() {
	//Define points that make up planes of bounding rectangular prism
	frontPlane[0] = p1; frontPlane[1] = p2; frontPlane[2] = p6; frontPlane[3] = p5;
	backPlane[0] = p0; backPlane[1] = p3; backPlane[2] = p7; backPlane[3] = p4;
	leftPlane[0] = p0; leftPlane[1] = p1; leftPlane[2] = p5; leftPlane[3] = p4;
	rightPlane[0] = p2; rightPlane[1] = p3; rightPlane[2] = p7; rightPlane[3] = p6;
	topPlane[0] = p4; topPlane[1] = p5; topPlane[2] = p6; topPlane[3] = p7; 
	bottomPlane[0] = p0; bottomPlane[1] = p1; bottomPlane[2] = p2; bottomPlane[3] = p3; 

	//Find formula of each plane (Ax + By + Cz + D = 0) given points on the plane
	planes.at(0) = calcPlane(frontPlane);
	planes.at(1) = calcPlane(backPlane);
	planes.at(2) = calcPlane(rightPlane);
	planes.at(3) = calcPlane(leftPlane);
	planes.at(4) = calcPlane(topPlane);
	planes.at(5) = calcPlane(bottomPlane);
}

bool NodeObject::testIntersection(vec3D r0, vec3D rd) {
	float t;
	float dot0, dotD;
	vec3D is;
	//For each plane, test whether the ray intersects them;
	for (int i = 0; i < 6; i++) {
		dot0 = (planes[i].a * r0.x) + (planes[i].b * r0.y) + (planes[i].c * r0.z); 
		dotD = (planes[i].a * rd.x) + (planes[i].b * rd.y) + (planes[i].c * rd.z);

		if (dot0 == 0) return false;

		t = (dot0 + planes[i].d) / dotD;
		t = -t;

		is = r0 + rd.vectorMultiply(t);

		switch (i) {
			case 0: 
				if ((is.x > p1.x) && (is.x < p2.x) && (is.y > p1.y) && (is.y < p5.y)) {
					intersects = true;
					return true;
				}
				break;
			case 1:
				if ((is.x > p0.x) && (is.x < p3.x) && (is.y > p0.y) && (is.y < p4.y)) {
					intersects = true;
					return true;
				}
				break;
			case 2:
				if ((is.z > p3.z) && (is.z < p2.z) && (is.y > p3.y) && (is.y < p7.y)) {
					intersects = true;
					return true;
				}
				break;
			case 3:
				if ((is.z > p0.z) && (is.z < p1.z) && (is.y > p0.y) && (is.y < p4.y)) {
					intersects = true;
					return true;
				}
				break;
			case 4: 
				if ((is.x > p4.x) && (is.x < p7.x) && (is.z > p4.z) && (is.z < p5.z)) {
					intersects = true;
					return true;
				}
				break;
			case 5:
				if ((is.x > p0.x) && (is.x < p3.x) && (is.z > p0.z) && (is.z < p1.z)) {
					intersects = true;
					return true;
				}
				break;
		}
	}
	intersects = false;
	return false;
}

void NodeObject::drawHitbox() {
	glPushMatrix();
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glColor3f(0,1,0);
		glBegin(GL_QUADS);	
			//Draw the front of the bounding box
			glVertex3f(p1.x,p1.y,p1.z);
			glVertex3f(p2.x,p2.y,p2.z);
			glVertex3f(p6.x,p6.y,p6.z);
			glVertex3f(p5.x,p5.y,p5.z);

			//Draw the back of the bounding box
			glVertex3f(p0.x,p0.y,p0.z);
			glVertex3f(p3.x,p3.y,p3.z);
			glVertex3f(p7.x,p7.y,p7.z);
			glVertex3f(p4.x,p4.y,p4.z);
		
			//Draw the bottom of the bounding box
			glVertex3f(p0.x,p0.y,p0.z);
			glVertex3f(p1.x,p1.y,p1.z);
			glVertex3f(p2.x,p2.y,p2.z);
			glVertex3f(p3.x,p3.y,p3.z);

			//Draw the top of the bounding box
			glVertex3f(p4.x,p4.y,p4.z);
			glVertex3f(p5.x,p5.y,p5.z);
			glVertex3f(p6.x,p6.y,p6.z);
			glVertex3f(p7.x,p7.y,p7.z);

			//Draw the left side of the bounding box
			glVertex3f(p0.x,p0.y,p0.z);
			glVertex3f(p1.x,p1.y,p1.z);
			glVertex3f(p5.x,p5.y,p5.z);
			glVertex3f(p4.x,p4.y,p4.z);


			//Draw the right side of the bounding box
			glVertex3f(p2.x,p2.y,p2.z);
			glVertex3f(p3.x,p3.y,p3.z);
			glVertex3f(p7.x,p7.y,p7.z);
			glVertex3f(p6.x,p6.y,p6.z);

		glEnd();
	glPopMatrix();
}

void NodeObject::switchMaterial(int n)  {

	material = n; 
	mat->switchMaterial(n);
}

void NodeObject::draw() {
	glEnable(GL_DEPTH_TEST);
	//Cull the back face of objects
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	
	float* tAmb = &mat->amb[0];
	float* tDiff = &mat->diff[0];
	float* tSpec = &mat->spec[0];
	//printf("values for amb %f %f %f %f\n", tAmb[0], tAmb[1], tAmb[2], tAmb[3]);
	
	glMaterialfv(GL_FRONT, GL_AMBIENT, tAmb);	//Setting the material for the ground plane, currently on chrome
	glMaterialfv(GL_FRONT, GL_DIFFUSE, tDiff);
	glMaterialfv(GL_FRONT, GL_SPECULAR, tSpec);
	glMaterialf(GL_FRONT, GL_SHININESS, mat->shiny);


	switch (o_type) {
		case cube:
			glutSolidCube(1);
			break;

		case sphere:
			glutSolidSphere(0.6, 100, 100);
			break;

		case cone:
			glutSolidCone(1, 1, 100, 100);
			break;

		case torus:
			glutSolidTorus(0.25, 0.5, 100, 100);
			break;

		case dodecahedron:
			glutSolidDodecahedron();
			break;
	}
}