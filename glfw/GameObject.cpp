#include "GameObject.h"
#include "Player.h"
#include <algorithm>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

/*
Implementation of the global game time variables
here because GameObject is included everywhere
*/
namespace gameTime
{
	double limitFPS = 1.0 / 144.0;
	int counter = 0;
	double time = counter * limitFPS;
}

GLuint GameObject::createTexture(const char* filename)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load and generate the texture
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	else std::cout << "Failed to load texture" << std::endl;

	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
}

/*
Virtual method to check if an object is colliding with the player. All derived classes have a version of
this method implemented to check collisions for their object type.
*/
bool GameObject::checkCollision(Player &p){
	bool cornerCase = false;
	float dx1 = std::max(x, p.getX());
	float dy1 = std::max(y, p.getY());
	float dx2 = std::min(x + width, p.getX() + p.getWidth());
	float dy2 = std::min(y + height, p.getY() + p.getHeight());

	if (dx2 - dx1 > dy2 - dy1) {							//top or bottom
		if (dy1 == p.getY()) {									//top
			if (p.getYVelocity() < 0) {
				p.setY(y + height - 1);
				p.setOnFloor(true);
				p.setOnWall(NOT_ON_WALL);
			}
			else cornerCase = true;
		}
		else {													//bottom
			if (p.getYVelocity() > 0) {
				p.setY(y - p.getHeight() - 1);
				p.setOnWall(NOT_ON_WALL);

			}
			else cornerCase = true;
		}
	}
	else if (!(dx2 - dx1 > dy2 - dy1) || cornerCase) {		//left or right
		if (dx1 == p.getX()) {									//right
			p.setX(x + width);
			p.setOnWall(LEFT_WALL);								//left wall because it is on player's left
			if (p.getXVelocity() < 0) p.setXVelocity(0);
		}
		else {													//left
			p.setX(x - p.getWidth());
			p.setOnWall(RIGHT_WALL);							//right wall because it is on player's right
			if (p.getXVelocity() > 0) p.setXVelocity(0);
		}
		p.setOnFloor(false);
	}
	p.setYVelocity(0);

	return true;
}
