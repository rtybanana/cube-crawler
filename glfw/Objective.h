#pragma once
#include "GameObject.h"

class Objective : public GameObject {
	GLuint texture;
	float radius;

public:
	Objective(float, float);

	void drawLight() const;
	virtual void drawObject() const;
	virtual bool checkCollision(Player &p);
};

inline Objective::Objective(float x, float y)
	: GameObject(x, y, 60, 60) {
	this->radius = 20;
	this->texture = GameObject::createTexture("Include/stb/objective.png");
}

inline void Objective::drawLight() const {
	float a, b;
	float frac = 2 * PI / 25;
	glPushMatrix();
	glColor3f(1, 1, 1);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 25; ++i) {
		a = (x + width / 2) + 100 * cos(i*frac);
		b = (y + height / 2) + 100 * sin(i*frac);
		glVertex2f(a, b);
	}
	glEnd();
	glPopMatrix();
}

/*
draw the textured 'objective' collectible object
*/
inline void Objective::drawObject() const {
	float a, b;
	float frac = 2 * PI / 25;
	glPushMatrix();
	glColor3f(1, 0.7, 0);
	glBindTexture(GL_TEXTURE_2D, texture);						//bind texture objective.png
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 25; ++i) {
		a = (x + width / 2) + 20 * cos(i*frac);
		b = (y + height / 2) + 20 * sin(i*frac);
		glTexCoord2f(0.5 + 0.5*cos(i*frac), 0.5 + 0.5*sin(i*frac)); glVertex2f(a, b);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);							//unbind texture
	glPopMatrix();
}

inline bool operator==(const Objective &o1 , const Objective &o2 ) {
	if (o1.getX() == o2.getX() && o1.getY() == o2.getY()) return true;
	else return false;
}
