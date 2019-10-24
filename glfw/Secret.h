#pragma once
#include "Objective.h"

class Secret : public Objective {
	GLuint texture;
	float radius;

public:
	Secret(float, float);

	virtual void drawObject() const;
	virtual bool checkCollision(Player&);
};

inline Secret::Secret(float x, float y)
	: Objective(x, y) {
	this->radius = 20;
	this->texture = GameObject::createTexture("Include/stb/secret.png");
}

/*
draw the textured 'secret' collectible object
*/
inline void Secret::drawObject() const {
	float a, b;
	float frac = 2 * PI / 25;
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texture);						//bind texture secret.png
	glEnable(GL_TEXTURE_2D);
	glColor3f(1, 1, 1);
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