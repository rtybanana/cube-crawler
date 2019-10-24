#pragma once

#include <glfw3.h>
#include <string>
#include <math.h>
#include <iostream>

#define PI 3.14159

/*
Global variables to count time for use if you want some action to happen based on time.
	- implementation in .cpp
*/
namespace gameTime
{
	extern double limitFPS;
	extern int counter;
	extern double time;
}

class Player;

class GameObject {
protected:
	float height;
	float width;
	float x;
	float y;
	std::string colour;

public:
	GameObject(float, float, float, float, std::string);
	
	//getters
	float getX() const;
	float getY() const;
	float getHeight() const;
	float getWidth() const;
	virtual float getVision() const;

	void paintObject() const;
	virtual void drawObject() const;
	virtual bool checkCollision(Player&);
	virtual void updatePos();

	static GLuint createTexture(const char* filename);
};

inline GameObject::GameObject(float x = 0, float y = 0, float width = 0, float height = 0, std::string colour = "black") {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->colour = colour;
}

/*
Update position virtual method which has no implementation for the base class as base GameObject objects are just static platforms.
Derived classes like MovingPlatform and Enemy have an implementation for this method.
*/
inline void GameObject::updatePos() {
}

/*
Get object sight line virtual method which has no implementation for the base class as base GameObject objects are just static platforms.
Derived class Enemy has an implementation for this method.

Returns	:	radius of objects sightline.
*/
inline float GameObject::getVision() const {
	return 0;
}

/*
Virtual method to draw the object with OpenGL. All visible derived classes have an implementation fo this
*/
inline void GameObject::drawObject() const {
	glPushMatrix();
	paintObject();
	glBegin(GL_POLYGON);
		glVertex2f(x, y);
		glVertex2f(x+width, y);
		glVertex2f(x+width, y+height);
		glVertex2f(x, y+height);
	glEnd();
	glPopMatrix();
}

/*
Quickly paint objects different common colours by passing a colour string variable with the constuctor,
was used for debugging, not really used for final product as game functionality relies on things being
black to simulate sightlines.
*/
inline void GameObject::paintObject() const {
	if (colour == "white") glColor3f(1, 1, 1);
	if (colour == "black") glColor3f(0, 0, 0);
	if (colour == "blue") glColor3f(0, 0, 1);
	if (colour == "red") glColor3f(1, 0, 0);
}

//getters
inline float GameObject::getX() const {
	return this->x;
}

inline float GameObject::getY() const {
	return this->y;
}

inline float GameObject::getHeight() const {
	return this->height;
}

inline float GameObject::getWidth() const {
	return this->width;
}

//operator overload for collision detection
inline bool operator^=(const GameObject &o1, const GameObject &o2) {
	if (o1.getX() < o2.getX() + o2.getWidth() && o1.getX() + o1.getWidth() > o2.getX() &&
		o1.getY() < o2.getY() + o2.getHeight() && o1.getY() + o1.getHeight() > o2.getY())
	{
		return true;
	}
	return false;
}

//operator overload to check if one GameObject is in anothers line of sight
inline bool operator|=(const GameObject &o1, const GameObject &o2) {
	if ((pow(2, abs(o2.getX() - o1.getX()))) + pow(2, abs(o2.getY() - o1.getY())) < pow(2, o2.getVision())) return true;
	else return false;
}