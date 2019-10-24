#pragma once
#include "Objective.h"

class End : public Objective {
public:
	End(float, float);

	virtual void drawObject() const;
	virtual bool checkCollision(Player&);
};

inline End::End(float x, float y)
	: Objective(x, y) {
}

inline void End::drawObject() const {
	float a, b;
	float frac = 2 * PI / 25;
	glPushMatrix();
	glColor3f(0.9, 0, 0);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 25; ++i) {
		a = (x + width / 2) + 20 * cos(i*frac);
		b = (y + height / 2) + 20 * sin(i*frac);
		glVertex2f(a, b);
	}
	glEnd();
	glPopMatrix();
}