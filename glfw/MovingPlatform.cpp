#include "MovingPlatform.h"
#include "Player.h"
#include <algorithm>

bool MovingPlatform::checkCollision(Player &p){
	float dx1 = std::max(x, p.getX());
	float dy1 = std::max(y, p.getY());
	float dx2 = std::min(x + width, p.getX() + p.getWidth());
	float dy2 = std::min(y + height, p.getY() + p.getHeight());

	if (dx2 - dx1 > dy2 - dy1) {							//top or bottom
		if (dy1 == p.getY()) {								//top
			p.setOnFloor(true);
			if (type == VERTICAL) {
				if (toEnd) {
					p.setY(y + height - 5);
					p.setYVelocity(0);
				}
				else {
					p.setY(y + height - 5);
					p.setYVelocity(0);
				}
			}
			else if (type == HORIZONTAL){
				if (toEnd) {
					p.setYVelocity(0);
					p.setY(y + height - 1);
					p.setX(p.getX() + 4);
				}
				else {
					p.setYVelocity(0);
					p.setY(y + height - 1);
					p.setX(p.getX() - 4);
				}
			}
		}
		else {												//bottom
			p.setY(y - p.getHeight());
			if (type == HORIZONTAL) p.setYVelocity(0);
			else if (type == VERTICAL) p.setYVelocity(-4);
		}
	}
	else {													//left or right
		if (dx1 == p.getX()) {								//right
			p.setX(x + width);
			if (type == HORIZONTAL) {
				p.setOnWall(LEFT_WALL);
				if (!toEnd) p.setXVelocity(-4);
			}
		}
		else {												//left
			p.setX(x - p.getWidth());
			if (type == HORIZONTAL) {
				p.setOnWall(RIGHT_WALL);
				if (toEnd) p.setXVelocity(4);
			}
		}
		p.setOnFloor(false);
	}
	
	return true;
}
