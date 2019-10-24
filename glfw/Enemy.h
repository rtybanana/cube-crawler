#pragma once
#include "GameObject.h"

class Enemy : public GameObject {
	float xVelocity;
	float vision;
	float startCoord;
	float endCoord;
	bool toEnd;
	bool onFloor;
	bool detected;

public:
	Enemy(float, float, float, float);

	float getXVelocity() const;
	bool getToEnd() const;
	virtual float getVision() const;

	void updatePos();
	void incrXVelocity();
	void decrXVelocity();
	void setDetected(bool);

	void followPlayer(Player);
	virtual void drawObject() const;
	virtual bool checkCollision(Player&);
};

inline Enemy::Enemy(float x, float y, float startCoord, float endCoord)
	: GameObject(x, y, 60, 60) {
	this->xVelocity = 0;
	this->vision = 500;
	this->detected = false;
	this->startCoord = startCoord;
	this->endCoord = endCoord;
}

/*
draws the enemy object (a black square with a yellow eye until player is detected, red after)
*/
inline void Enemy::drawObject() const {
	glPushMatrix();
	glColor3f(0, 0, 0);
	glBegin(GL_POLYGON);
		glVertex2f(x, y);
		glVertex2f(x + width, y);
		glVertex2f(x + width, y + height);
		glVertex2f(x, y + height);
	glEnd();
	

	if (detected) glColor3f(0.9, 0, 0);
	else glColor3f(1, 0.7, 0);
	glTranslatef(xVelocity*2, 0, 0);
	glBegin(GL_POLYGON);
		glVertex2f(x+25, y+25);
		glVertex2f(x+25, y+35);
		glVertex2f(x+35, y+35);
		glVertex2f(x+35, y+25);
	glEnd();
	glPopMatrix();
}

//getters
inline float Enemy::getXVelocity() const {
	return xVelocity;
}

inline float Enemy::getVision() const {
	return vision;
}

inline bool Enemy::getToEnd() const {
	return toEnd;
}

/*
move the enemy backward and forward within it's designated domain until it detects the player
when it will follow as much as it can without leaving it's area (startCoord < x < endCoord)
*/
inline void Enemy::updatePos() {
	if (!detected) {
		if (toEnd) {
			if (x + xVelocity < endCoord - width) x += xVelocity;
			else {
				toEnd = false;
				xVelocity = -1;
			}
		}
		else {
			if (x + xVelocity > startCoord) x += xVelocity;
			else {
				toEnd = true;
				xVelocity = 1;
			}
		}
	}
	else if (x + xVelocity > startCoord && x + xVelocity < endCoord - width) x += xVelocity;
}

//setters (increments locked to changing by 0.25)
inline void Enemy::incrXVelocity() {
	xVelocity += 0.25;
}

inline void Enemy::decrXVelocity() {
	xVelocity -= 0.25;
}

inline void Enemy::setDetected(bool d) {
	detected = d;
}

