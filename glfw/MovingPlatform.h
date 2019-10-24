#pragma once
#include "GameObject.h"

#define HORIZONTAL 0
#define VERTICAL 1

class MovingPlatform : public GameObject {
	int type;											//moving up or down
	float startCoord;									//start coordinate
	float endCoord;										//end coordinate where it bounces
	bool toEnd;											//boolean to indicate which direction it's moving in

public:
	MovingPlatform(float, float, int, float);

	virtual void updatePos();
	virtual bool checkCollision(Player&);
};

inline MovingPlatform::MovingPlatform(float x = 0, float y = 0, int t = VERTICAL, float d = 0)
	: GameObject(x, y, 300, 60) {
	this->type = t;
	this->toEnd = true;
	if (t == VERTICAL){
		this->startCoord = y;
		this->endCoord = y + d;
	}
	else if (t == HORIZONTAL){
		this->startCoord = x;
		this->endCoord = x + d;
	}
}

inline void MovingPlatform::updatePos() {
	if (toEnd) {
		if (type == HORIZONTAL) {
			if (x < endCoord) {
				x += 4;
			}
			else {
				toEnd = false;
				x -= 4;
			}
		}
		else if (type == VERTICAL) {
			if (y < endCoord) {
				y += 4;
			}
			else {
				toEnd = false;
				y -= 4;
			}
		}
	}
	else {
		if (type == HORIZONTAL) {
			if (x > startCoord) {
				x -= 4;
			}
			else {
				toEnd = true;
				x += 4;
			}
		}
		else if (type == VERTICAL) {
			if (y > startCoord) {
				y -= 4;
			}
			else {
				toEnd = true;
				y += 4;
			}
		}
	}
}

