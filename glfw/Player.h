#pragma once

#include <math.h>
#include "GameObject.h"

#define NOT_ON_WALL 0
#define LEFT_WALL -1
#define RIGHT_WALL 1

class Level;
class Enemy;

class Player : public GameObject {
	std::pair<float, float> spawn;
	float xVelocity;
	float yVelocity;
	bool onFloor;
	int onWall;
	float vision;
	bool dead;

public:
	Player();

	//getters
	float getXVelocity() const;
	float getYVelocity() const;
	bool isDead() const;
	bool isOnFloor() const;

	//setters
	void setX(float);
	void setY(float);
	void setXVelocity(float);
	void setYVelocity(float);
	void setSpawn(float, float);
	void setPos(float, float);
	void setOnFloor(bool);
	void setOnWall(int);
	void kill();
	void reset();

	void processKeys(bool, bool, bool);
	virtual void updatePos();
	void drawVision() const;
	virtual void drawObject() const;
};


inline Player::Player() 
	: GameObject(-30, -30, 60, 60) {
	this->onFloor = false;
	this->onWall = NOT_ON_WALL;
	this->xVelocity = 0;
	this->yVelocity = 0;
	this->vision = 500;
	this->dead = false;
}

inline void Player::updatePos() {
	x += xVelocity;
	y += yVelocity;
}

inline void Player::drawVision() const {
	float a, b, a2, b2;
	float frac = 2 * PI / 100;
	glPushMatrix();
	glColor3f(1, 1, 1);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; ++i) {
		a = (x + width / 2) + vision * cos(i*frac);
		b = (y + height / 2) + vision * sin(i*frac);
		glVertex2f(a, b);
	}
	glEnd();
	glPopMatrix();
}

inline void Player::drawObject() const {
	glPushMatrix();
	paintObject();
	glTranslatef(x+width/2, y+height/2, 0);
	glBegin(GL_POLYGON);
		if (xVelocity > 0) {
			glVertex2f(-width / 2 + xVelocity / 3, height / 2);
			glVertex2f(width / 2 + xVelocity / 3, height / 2 - xVelocity/2);
		}
		else if (xVelocity < 0) {
			glVertex2f(-width / 2 + xVelocity / 3, height / 2 + xVelocity / 2);
			glVertex2f(width / 2 + xVelocity / 3, height / 2);
		}
		else {
			glVertex2f(-width / 2, height / 2);
			glVertex2f(width / 2, height / 2);
		}

		if (yVelocity > 0) {
			glVertex2f(width / 2, -height / 2 - yVelocity);
			glVertex2f(-width / 2, -height / 2 - yVelocity);
		}
		else {
			glVertex2f(width / 2, -height / 2);
			glVertex2f(-width / 2, -height / 2);
		}
	glEnd();

	float eyeY = yVelocity;
	if (eyeY < -10) eyeY = -10;
	glColor3f(1, 1, 1);
	glTranslatef(xVelocity/1.5, eyeY/1.5, 0);
	glBegin(GL_POLYGON);
		glVertex2f(-5, -5);	
		glVertex2f(-5, 5);
		glVertex2f(5, 5);
		glVertex2f(5, -5);
	glEnd();
	glPopMatrix();
}

inline float Player::getXVelocity() const {
	return xVelocity;
}

inline float Player::getYVelocity() const {
	return yVelocity;
}

inline bool Player::isDead() const {
	return dead;
}

inline bool Player::isOnFloor() const {
	return onFloor;
}

inline void Player::setX(float x) {
	this->x = x;
}

inline void Player::setY(float y) {
	this->y = y;
}

inline void Player::setSpawn(float x, float y) {
	spawn.first = x;
	spawn.second = y;
}

inline void Player::setXVelocity(float xV) {
	xVelocity = xV;
}

inline void Player::setYVelocity(float yV) {
	yVelocity = yV;
}

inline void Player::setPos(float x, float y) {
	this->x = x;
	this->y = y;
}

inline void Player::setOnFloor(bool f){
	onFloor = f;
}

inline void Player::setOnWall(int w) {
	onWall = w;
}

inline void Player::kill() {
	dead = true;
}