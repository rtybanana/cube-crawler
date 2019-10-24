#pragma once
#include "GameObject.h"
#include <iostream>

#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3


class Spikes : public GameObject {
	int type;

public:
	Spikes(float, float, int, int);

	virtual bool checkCollision(Player&);
	virtual void drawObject() const;
};

inline Spikes::Spikes(float x, float y, int t, int n) 
	: GameObject(x, y) {
	this->type = t;

	//horizontal - up/down
	if (t == NORTH || t == SOUTH) {
		this->width = n * 60;
		this->height = 60 - 1;							//-1 so that platform will take precedent if the objects are at the same level
		if (t == NORTH) this->y = y - 60;
	}
	//vertical - right/left
	else if (t == EAST || t == WEST) {
		this->height= (n * 60) - 1;						//-1 as above
		this->width = 60;
		if (t == EAST) this->x = x - 60;
	}
}

//draws the spike(s)
inline void Spikes::drawObject() const {
	for (int ix = x; ix < x + width; ix += 60){
		for (int iy = y; iy < y + height; iy += 60) {
			glPushMatrix();
			glBegin(GL_POLYGON);
				if (type == NORTH) {
					glVertex2f(ix, iy + 60);
					glVertex2f(ix + 60, iy + 60);
					glVertex2f(ix + 30, iy);
				}
				else if (type == EAST) {
					glVertex2f(ix + 60, iy + 60);
					glVertex2f(ix + 60, iy);
					glVertex2f(ix, iy + 30); 
				}
				else if (type == SOUTH) {
					glVertex2f(ix + 60, iy);
					glVertex2f(ix, iy);
					glVertex2f(ix + 30, iy + 60);
				}
				else if (type == WEST) {
					glVertex2f(ix, iy);
					glVertex2f(ix, iy + 60);
					glVertex2f(ix + 60, iy + 30);
				}
			glEnd();
			glPopMatrix();
		}
	}
}

