/* 3GC3 Assignment 3
Name: Sean McKay, Rebecca Tran
Student Numbers: 1423885, 1425611
MacID: mckaysm, tranr5
*/

//Node Transform header file
#ifndef __NODE_TRANSFORM_H__	
#define __NODE_TRANSFORM_H__

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>	
#endif

#include "Node.h"
#include "math3D.h"
#include <string>

class NodeTransform:public Node{
public: 
		NodeTransform(TransformType transformation, vec3D params);	//getting the type and the parameters of the transform type (for 3 params)
		NodeTransform(TransformType transformation, vec4D params);	//getting the tpe and the parameters of the transform type (for 4 params)
		virtual void draw();	//taking care of the rotation, translation and scaling of an object
		virtual bool testIntersection(vec3D r0, vec3D rd);	//testing for intersection of object and ray
		virtual void drawHitbox();	//drawing the hitbox function
};

#endif