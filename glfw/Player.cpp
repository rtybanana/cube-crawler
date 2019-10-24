#include "Player.h"
#include "Level.h"
#include <algorithm>
#include <chrono>
#include <thread>

void Player::processKeys(bool left, bool right, bool up) {
	//y axis key process
	if (up && onFloor) {
		yVelocity = 10;
		onFloor = false;
	}
	else if (up && onWall != NOT_ON_WALL) {
		yVelocity = 10;
		if (onWall == LEFT_WALL) xVelocity = 16;
		else if (onWall == RIGHT_WALL) xVelocity = -16;
		onWall = NOT_ON_WALL;
	}
	else if (onWall != NOT_ON_WALL && !onFloor) yVelocity = -3;
	else if (!onFloor && yVelocity > -40) yVelocity -= 0.25;

	//x axis key process
	if (left && right && xVelocity > 0) xVelocity -= 0.5;
	else if (left && right && xVelocity < 0) xVelocity += 0.5;

	if (left && xVelocity > -10) xVelocity -= 0.5;
	else if (!left && xVelocity < 0) xVelocity += 0.5;

	if (right && xVelocity < 10) xVelocity += 0.5;
	else if (!right && xVelocity > 0) xVelocity -= 0.5;

	if (xVelocity > 10) xVelocity -= 0.5;
	else if (xVelocity < -10) xVelocity += 0.5;

	updatePos();
}

void Player::reset() {
	dead = false;
	onFloor = false;
	onWall = NOT_ON_WALL;
	xVelocity = 0;
	yVelocity = 0;
	x = spawn.first;
	y = spawn.second;
	std::this_thread::sleep_for(std::chrono::milliseconds(400));
}

