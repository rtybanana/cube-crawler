#pragma once
#include "GameObject.h"

class Boost : public GameObject {
	float boostAmount;

public:
	Boost(float, float, float);
	
	virtual bool checkCollision(Player&);
};

inline Boost::Boost(float x = 0, float y = 0, float boost = 25)
	: GameObject(x, y, 300, 15) {
	this->boostAmount = boost;
}

