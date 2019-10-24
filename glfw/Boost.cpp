#include "Boost.h"
#include "Player.h"
#include <algorithm>

bool Boost::checkCollision(Player &p) {
	bool cornerCase = false;
	float dx1 = std::max(x, p.getX());
	float dy1 = std::max(y, p.getY());
	float dx2 = std::min(x + width, p.getX() + p.getWidth());
	float dy2 = std::min(y + height, p.getY() + p.getHeight());

	if (dx2 - dx1 > dy2 - dy1) {							//top or bottom
		p.setYVelocity(boostAmount);
	}

	return true;
}
