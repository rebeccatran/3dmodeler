/* 3GC3 Assignment 3
Name: Sean McKay, Rebecca Tran
Student Numbers: 1423885, 1425611
MacID: mckaysm, tranr5
*/

#ifndef __SCENE_GRAPH_H__
#define __SCENE_GRAPH_H__

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

//Scenegraph class for storing all the nodes in the graph
//With functions for accesing info from nodes, and changing the node pointed to
class SceneGraph{
public:
	SceneGraph();	//Constructor


	void toRoot();	//Go to the root of the scenegraph
	void toChild(int i);	//Go the ith child of the current object
	bool search(int objID);	//Search for a given node ID
	void drawHitboxes();	//Draw hitboxes for all currently selected objects
	int intersect(vec3D r0, vec3D rd);	//Ray intersection test for objects
	void toParent();	//Go to the parent of the current node
	void insertNode(Node *node);	//Insert a new node as the child of the current node
	void deleteNode();	//Delete the current node

	void traverse();	//Recusively draw each node
	void deleteScene();	//Delete all nodes other than the root


	Node *current;	//Pointer to the current node
	Node *root;	//Pointer to the root node

};

#endif

