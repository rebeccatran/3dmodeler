/* 3GC3 Assignment 3
Name: Sean McKay, Rebecca Tran
Student Numbers: 1423885, 1425611
MacID: mckaysm, tranr5
*/

#include "SceneGraph.h"

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
#include <stdio.h>

//Boolean for recursive search
bool found = false;

//Constructor for a scenegraph
SceneGraph::SceneGraph(){
	//Create a root node for the graph, setting it's ID to 0 and making it the currently selected node
	root = new Node();
	current = root;
	current->ID = 0;
}

//Traverse through the tree recusively starting at the root node, drawing each node
void SceneGraph::traverse() {
	root->traverse();
}

//Set the currently selected node to the root
void SceneGraph::toRoot() {
	current = root;
}

//Go the parent of the currently selected node if it exists
void SceneGraph::toParent() {
	if (current->parent != NULL) {
		current = current->parent;
	}
}

//Go to the ith child of the current node, if it exists
void SceneGraph::toChild(int i) {
	if (i < current->children->size() && i >=0) {
		current = current->children->at(i);
	}
}

//Recursive search from current node down for object with given ID
//Boolean return value is useless, the found value
bool SceneGraph::search(int objID) {
	//Reset the boolean 
	if (found) found = false;

	//If the currently selected object's ID is equal to the search ID, return
	//and set found to true 
	if (current->ID == objID) {
		found = true;
		return true;
	}

	//If the current node has children, recursively call search with each child
	int numChildren = current->children->size();
	if (numChildren > 0) {
		for (int i = 0; i < numChildren; i++) {
			//Save the current current node
			Node* temp = current;
			current = current->children->at(i);
			search(objID);
			//Reset to the saved current node so the for loop works properly
			if (!found) current = temp;
			//If the object has been found, break out of the loop
			else break;
		}
	}
	//Default return value
	return false;
}

//Draw hitboxes around all selected objects
void SceneGraph::drawHitboxes() {
	root -> drawHitboxes();
}

//Check if a ray interscts with any of the objects in the scenegraph
//Return the ID of the last object that intersects with the ray, -1 if no objects intersect 
int SceneGraph::intersect(vec3D r0, vec3D rd) {
	int objectID = root->intersect(r0, rd);
	return objectID;
}

//Insert a new node into the scenegraph as a child of the current node
void SceneGraph::insertNode(Node *node) {
	node->parent = current;

	current->children->push_back(node);

	current = node;
}

//Delete the current node from the scenegraph
void SceneGraph::deleteNode() {
	Node *temp = current;
	SceneGraph::toParent();
	for (int i = 0; i < current->children->size(); i++) { 
		if (current->children->at(i)->ID == temp->ID) {
			current->children->erase(current->children->begin() + i);
		}
	}
}

//Delete the entire scenegraph, leaving only the root node
void SceneGraph::deleteScene(){
	current = root;
	root->children->clear();
}


