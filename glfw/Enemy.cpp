#include "Enemy.h"
#include "Player.h"
#include <algorithm>

bool Enemy::checkCollision(Player &p) {
	float dx1 = std::max(x, p.getX());
	float dy1 = std::max(y, p.getY());
	float dx2 = std::min(x + width, p.getX() + p.getWidth());
	float dy2 = std::min(y + height, p.getY() + p.getHeight());

	if (dx2 - dx1 > (dy2 - dy1) - 1) {							//top or bottom (-1 to add a slight bias to the player to make enemies more forgiving)
		if (dy1 == p.getY()) {									//top
			p.setY(y + height - 1);
			p.setYVelocity(6);
			return true;
		}
		else p.kill();											//bottom
			
	}
	else if (!(dx2 - dx1 > dy2 - dy1)) p.kill();			//left or right
		
	return false;
}

void Enemy::followPlayer(Player p) {
	if (x > p.getX() && xVelocity >= -5) xVelocity -= 0.25, toEnd = false;
	else if (x < p.getX() && xVelocity <= 5) xVelocity += 0.25, toEnd = true;
}
