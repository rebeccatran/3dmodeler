/* 3GC3 Assignment 3
Name: Sean McKay, Rebecca Tran
Student Numbers: 1423885, 1425611
MacID: mckaysm, tranr5
*/

#include "NodeTransform.h"

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>	
#endif

#include <stdio.h>

//For the transform types with three parameters
NodeTransform::NodeTransform(TransformType transformation, vec3D params) {
	type = transform;	//setting the type of node for transformations
	t_type = transformation;	//getting the type of transformation (rotate, scale, translate)
	amount = params;	//getting the parameters for the corresponding transformation type
}

//For the transform type with four parameters
NodeTransform::NodeTransform(TransformType transformation, vec4D params) {
	type = transform;	//setting the type, parameters of the object
	t_type = transformation;
	amount4 = params;
}

//drawing the translation, rotation and scaling according the the type
void NodeTransform::draw() {
	switch(t_type) {
		case translate:
			//printf("Translating by %f, %f, %f\n", amount.x, amount.y, amount.z);
			glTranslatef(amount.x, amount.y, amount.z);	//translate according to the values in the x, y and z axis
			break;

		case rotate:
			//printf("Rotating by %f, %f, %f, %f\n", amount4.w, amount4.x, amount4.y, amount4.z);
			glRotatef(amount4.w, amount4.x, amount4.y, amount4.z);	//rotating according to the angle value about the x y and z
			break;

		case scale:
			//printf("Scaling by %f, %f, %f\n", amount.x, amount.y, amount.z);
			glScalef(amount.x, amount.y, amount.z);	//scaling according to the values in the x y and z axis
			break;
	}
}

//returns false since there is no transformation node will intersect with a ray
bool NodeTransform::testIntersection(vec3D r0, vec3D rd) {
	return false;
}

void NodeTransform::drawHitbox() {};