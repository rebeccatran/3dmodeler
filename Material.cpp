/* 3GC3 Assignment 3
Name: Sean McKay, Rebecca Tran
Student Numbers: 1423885, 1425611
MacID: mckaysm, tranr5
*/

#include "Material.h"

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

Material::Material(){

//storing each of the material values into vectors

//--------------bronze-------------------//
	bronze.mAmb.push_back(0.2125);
	bronze.mAmb.push_back(0.1275);
	bronze.mAmb.push_back(0.054);
	bronze.mAmb.push_back(1.0);

	bronze.mDiff.push_back(0.714);
	bronze.mDiff.push_back(0.4284);
	bronze.mDiff.push_back(0.18144);
	bronze.mDiff.push_back(1.0);

	bronze.mSpec.push_back(0.393548);
	bronze.mSpec.push_back(0.271906);
	bronze.mSpec.push_back(0.166721);
	bronze.mSpec.push_back(1.0);

	bronze.mShiny = 0.2f * 128;

//--------------chrome------------------//
	chrome.mAmb.push_back(0.25);
	chrome.mAmb.push_back(0.25);
	chrome.mAmb.push_back(0.25);
	chrome.mAmb.push_back(1.0);

	chrome.mDiff.push_back(0.4);
	chrome.mDiff.push_back(0.4);
	chrome.mDiff.push_back(0.4);
	chrome.mDiff.push_back(1.0);

	chrome.mSpec.push_back(0.774597);
	chrome.mSpec.push_back(0.774597);
	chrome.mSpec.push_back(0.774597);
	chrome.mSpec.push_back(1.0);

	chrome.mShiny = 0.6f * 128;

//--------------copper-------------------//
	copper.mAmb.push_back(0.19125);
	copper.mAmb.push_back(0.0735);
	copper.mAmb.push_back(0.0225);
	copper.mAmb.push_back(1.0);

	copper.mDiff.push_back(0.7038);
	copper.mDiff.push_back(0.27048);
	copper.mDiff.push_back(0.0828);
	copper.mDiff.push_back(1.0);

	copper.mSpec.push_back(0.256777);
	copper.mSpec.push_back(0.137622);
	copper.mSpec.push_back(0.086014);
	copper.mSpec.push_back(1.0);

	copper.mShiny = 0.1f * 128;

//--------------pearl-------------------//
	pearl.mAmb.push_back(0.25);
	pearl.mAmb.push_back(0.20725);
	pearl.mAmb.push_back(0.20725);
	pearl.mAmb.push_back(1.0);

	pearl.mDiff.push_back(1.0);
	pearl.mDiff.push_back(0.829);
	pearl.mDiff.push_back(0.829);
	pearl.mDiff.push_back(1.0);

	pearl.mSpec.push_back(0.296648);
	pearl.mSpec.push_back(0.296648);
	pearl.mSpec.push_back(0.296648);
	pearl.mSpec.push_back(1.0);

	pearl.mShiny = 0.088f * 128;

//--------------obsidian-------------------//
	obsidian.mAmb.push_back(0.05375);
	obsidian.mAmb.push_back(0.05);
	obsidian.mAmb.push_back(0.06625);
	obsidian.mAmb.push_back(1.0);

	obsidian.mDiff.push_back(0.18275);
	obsidian.mDiff.push_back(0.17);
	obsidian.mDiff.push_back(0.22525);
	obsidian.mDiff.push_back(1.0);

	obsidian.mSpec.push_back(0.332741);
	obsidian.mSpec.push_back(0.328634);
	obsidian.mSpec.push_back(0.346435);
	obsidian.mSpec.push_back(1.0);

	obsidian.mShiny= 0.3f * 128;

	setInitMaterial();
}

//the initial material with the objects will be set to bronze automatically
void Material::setInitMaterial() {	
	m = 1;
	amb = bronze.mAmb;
	diff = bronze.mDiff;
	spec = bronze.mSpec;
	shiny = bronze.mShiny;
}

//switching the materials according to the values 1-5, each object subsequently drawn will be drawn with the material it was switched to
void Material::switchMaterial(int n) {	
	//this->m = n;
	switch (n) {
		// printf("entering the switch\n");
		case (1): 
			// printf("Switching material values to 1\n");
			this->amb = bronze.mAmb;		//switching to bronze
			this->diff = bronze.mDiff;
			this->spec = bronze.mSpec;
			this->shiny = bronze.mShiny;
			break; 
		case (2): 
			// printf("Switching material values to 2\n");
			this->amb = chrome.mAmb;	//switching to chrome
			this->diff = chrome.mDiff;
			this->spec = chrome.mSpec;
			this->shiny = chrome.mShiny;
			break;
		case (3): 
			// printf("Switching material values to 3\n");
			this->amb = copper.mAmb;	//switching to copper
			this->diff = copper.mDiff;
			this->spec = copper.mSpec;
			this->shiny = copper.mShiny;
			break;
		case (4): 
			// printf("Switching material values to 4\n");
			this->amb = pearl.mAmb;	//switching to pearl
			this->diff = pearl.mDiff;
			this->spec = pearl.mSpec;
			this->shiny = pearl.mShiny;
			break;
		case (5): 
			// printf("Switching material values to 5\n");
			this->amb = obsidian.mAmb;	//switching to obsidian
			this->diff = obsidian.mDiff;
			this->spec = obsidian.mSpec;
			this->shiny = obsidian.mShiny;
			break;
	}
}
