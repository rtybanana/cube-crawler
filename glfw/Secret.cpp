#include "Secret.h"
#include "Player.h"
#include <algorithm>

bool Secret::checkCollision(Player &p) {
	float xCir = x + width / 2;
	float yCir = y + height / 2;
	float xPla = p.getX() + p.getWidth() / 2;
	float yPla = p.getY() + p.getHeight() / 2;
	float xMax = p.getX() + p.getWidth();
	float yMax = p.getY() + p.getHeight();

	if (xCir >= p.getX() && xCir <= xMax) {
		if (yCir >= p.getY() && yCir <= yMax) return true;
		else {
			float d = abs(yCir - yPla);
			if (d < p.getHeight() / 2 + radius) return true;
		}
	}
	else if (yCir >= p.getY() && yCir <= yMax) {
		if (xCir >= p.getX() && xCir <= xMax) return true;
		else {
			float d = abs(xCir - xPla);
			if (d < p.getWidth() / 2 + radius) return true;
		}
	}
	else {
		float d;
		if (xCir < p.getX() && yCir < p.getY()) {
			d = pow(2, p.getX() - xCir) + pow(2, p.getY() - yCir);
		}
		else if (xCir > xMax && yCir < p.getY()) {
			d = pow(2, xCir - xMax) + pow(2, p.getY() - yCir);
		}
		else if (xCir > xMax && yCir > yMax) {
			d = pow(2, xCir - xMax) + pow(2, yCir - yMax);
		}
		else if (xCir < p.getX() && yCir > yMax) {
			d = pow(2, p.getX() - xCir) + pow(2, yCir - yMax);
		}
		if (d < pow(2, radius)) return true;
	}

	return false;
}
