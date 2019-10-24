#include "Level.h"
#include "Player.h"
#include "Spikes.h"
#include "Boost.h"
#include "MovingPlatform.h"
#include <algorithm>
#include <string>
#include <sstream>

FTPixmapFont Level::font = FTPixmapFont("./Fonts/cour.ttf");

//destructor
Level::~Level() {
	for (auto obj : level) {
		delete(obj);
	}
	for (auto obj : objectives) {
		delete(obj);
	}
	for (auto enm : enemies) {
		delete(enm);
	}
	for (auto srt : secrets) {
		delete(srt);
	}
	for (auto hid : hides) {
		delete(hid);
	}
}

//copy constructor
Level::Level(const Level &lvl) {
	this->level = lvl.level;
	this->objectives = lvl.objectives;
	this->enemies = lvl.enemies;
	this->secrets = lvl.secrets;
	this->hides = lvl.hides;
}

/*
method to check all collisions between the player and any GameObject and then deals with
that collision appropriately by calling another function or changing this levels member 
variables to reflect the correct collision response
*/
bool Level::checkCollision(Player &p){
	if (level.empty()) {
		return false;
	}
	GameObject* object = nullptr;
	float area = 0;
	for (GameObject* obj : level) {
		if (p ^= *obj) {											//overloaded operator for collisions (AABB)
			float dx1 = std::max(p.getX(), obj->getX());										//further checks to determine which object the player is colliding with more
			float dy1 = std::max(p.getY(), obj->getY());
			float dx2 = std::min(p.getX() + p.getWidth(), obj->getX() + obj->getWidth());
			float dy2 = std::min(p.getY() + p.getHeight(), obj->getY() + obj->getHeight());

			float intersect = (dx2 - dx1)*(dy2 - dy1);											//area of intersection
			if (intersect > area) {
				object = obj;
				area = intersect;
			}
		}
	}

	//checks for collisions with objectives (yellow dots)
	if (!objectives.empty()) {
		Objective* objective = nullptr;
		for (Objective* obj : objectives) {
			if (p ^= *obj) {
				if (obj->checkCollision(p) == true) {
					objective = obj;
					break;
				}
			}
		}
		if (objective != nullptr) {
			objCollected++;
			this->objectives.remove(objective);
			delete objective;					//remove objective and delete pointer
			if (objCollected == objCount) {
				level.pop_back();				//'pop_back' the last GameObject in the 'level' list member variable which is the wall blocking the exit
			}
		}
	}
	else if (p.getY() < -6000){		//signifies the player has dropped through the finish which is blocked until all objectives are collected
		finish();
	}

	//checks for collisions with enemies
	Enemy* enemy = nullptr;
	for (Enemy* enm : enemies) {
		if (p ^= *enm) {
			if (enm->checkCollision(p) == true) {
				enemy = enm;
				break;
			}
		}
	}
	if (enemy != nullptr) {
		this->enemies.remove(enemy);
		delete enemy;					//remove enemy and delete pointer
		timer += 5;						//5 second bonus for finding a secret
	}

	//checks for collisions with hidden walls which disappear on impact
	GameObject* hide = nullptr;
	for (GameObject* hid : hides) {
		if (p ^= *hid) {
			hide = hid;
			break;
		}
	}
	if (hide != nullptr) {
		this->hides.remove(hide);
		delete hide;					//remove wall and delete pointer
	}

	//checks for collisions with secret objectives (blue dots)
	Secret* secret = nullptr;
	for (Secret* srt : secrets) {
		if (p ^= *srt) {
			if (srt->checkCollision(p) == true) {
				secret = srt;
				break;
			}
		}
	}
	if (secret != nullptr) {
		srtCollected++;
		this->secrets.remove(secret);
		delete secret;					//remove secret and delete pointer
		timer -= 5;						//5 second penalty for killing an enemy
	}

	if (object != nullptr) {
		if (object->checkCollision(p) == true) {
			if (p.isDead()) {
				deathCount++;
			}
			return true;
		}
	}
	else {
		p.setOnFloor(false);
		p.setOnWall(NOT_ON_WALL);
	}

	return false;	
}

/*
updates the movable objects in the level (moving platforms and enemies) by calling their virtual updatePos(); method
*/
void Level::updateLevel(Player &p) const {
	for (GameObject* obj : level) {
		obj->updatePos();
	}
	for (Enemy* enm : enemies) {
		if (p |= *enm) {
			enm->setDetected(true);
			enm->followPlayer(p);
		}
		else {
			enm->setDetected(false);
			if (enm->getToEnd()) {
				if (enm->getXVelocity() >= 1) enm->decrXVelocity();
				else if (enm->getXVelocity() <= 1) enm->incrXVelocity();
			}
			else {
				if (enm->getXVelocity() >= -1) enm->decrXVelocity();
				else if (enm->getXVelocity() <= -1) enm->incrXVelocity();
			}
		}

		enm->updatePos();
	}
}

/*
draw the level by calling virtual drawObject(); and drawLight(); methods.
these are seperate because the lights for objectives and secrets need to 
be on the background layer so that they don't overlap with other foreground
objects
*/
void Level::drawLevel() const {
	for (Objective* obj : objectives) {
		obj->drawLight();
	}
	for (Secret* srt : secrets) {
		srt->drawLight();
	}
	for (Objective* obj : objectives) {
		obj->drawObject();
	}
	for (Secret* srt : secrets) {
		srt->drawObject();
	}
	for (Enemy* enm : enemies) {
		enm->drawObject();
	}
	for (GameObject* obj : level) {
		obj->drawObject();
	}
	for (GameObject* hid : hides) {
		hid->drawObject();
	}
}

/*
draws the heads-up display which contains information about the current level playthrough such as:
	level number
	number of objectives collected/total
	number of secrets collected/total
	current time elapsed
	number of deaths
*/
void Level::drawHUD(Player p, float width, float height) const {
	std::ostringstream oss;
	std::string s;
	const char* c;
	font.FaceSize(30);

	glPushMatrix();
	glColor3f(0.3, 0.3, 0.3);
	glTranslatef(width * 3 / 4 - 360, height * 3/4 - 125, 0);
	glBegin(GL_POLYGON);
		glVertex2f(p.getX(), p.getY());
		glVertex2f(p.getX(), p.getY() + 180);
		glVertex2f(p.getX() + 600, p.getY() + 180);
		glVertex2f(p.getX() + 600, p.getY());
	glEnd();
	glPopMatrix();

	oss << "Objectives: " << objCollected << "/" << objCount;
	s = oss.str();
	c = s.c_str();
	glPushMatrix();
	glColor3f(1, 1, 1);
	glRasterPos2f(p.getX() + width * 3 / 4 - 350, p.getY() + height * 3 / 4);
	font.Render(c);
	glPopMatrix();

	oss.str(std::string());
	oss << "Secrets: " << srtCollected << "/" << srtCount;
	s = oss.str();
	c = s.c_str();
	glPushMatrix();
	glColor3f(1, 1, 1);
	glRasterPos2f(p.getX() + width * 3 / 4 - 260, p.getY() + height * 3 / 4 - 50);
	font.Render(c);
	glPopMatrix();

	oss.str(std::string());
	oss << "Time:";
	s = oss.str();
	c = s.c_str();
	glPushMatrix();
	glColor3f(1, 1, 1);
	glRasterPos2f(p.getX() + width * 3 / 4 - 170, p.getY() + height * 3 / 4 - 100);
	font.Render(c);
	glPopMatrix();

	oss.str(std::string());
	if (!level.empty()) oss << timer;
	else oss << finalTime;
	s = oss.str();
	c = s.c_str();
	glPushMatrix();
	glColor3f(1, 1, 1);
	glRasterPos2f(p.getX() + width * 3 / 4 + 10, p.getY() + height * 3 / 4 - 100);
	font.Render(c);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.3, 0.3, 0.3);
	glTranslatef(width * 3 / 4 - 190, -height * 3 / 4 + 5, 0);
	glBegin(GL_POLYGON);
		glVertex2f(p.getX(), p.getY());
		glVertex2f(p.getX(), p.getY() + 60);
		glVertex2f(p.getX() + 600, p.getY() + 60);
		glVertex2f(p.getX() + 600, p.getY());
	glEnd();
	glPopMatrix();

	oss.str(std::string());
	oss << "Deaths: " << deathCount;
	s = oss.str();
	c = s.c_str();
	glPushMatrix();
	glColor3f(1, 1, 1);
	glRasterPos2f(p.getX() + width * 3 / 4 - 180, p.getY() - height * 3 / 4 + 20);
	font.Render(c);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.3, 0.3, 0.3);
	glTranslatef(0, height * 3 / 4 + 25, 0);
	glBegin(GL_POLYGON);
		glVertex2f(p.getX() - 130, p.getY() - 30);
		glVertex2f(p.getX() - 130, p.getY() + 30);
		glVertex2f(p.getX() + 190, p.getY() + 30);
		glVertex2f(p.getX() + 190, p.getY() - 30);
	glEnd();
	glPopMatrix();

	oss.str(std::string());
	oss << "Level: " << lvln;
	s = oss.str();
	c = s.c_str();
	glPushMatrix();
	glColor3f(1, 1, 1);
	glRasterPos2f(p.getX() - 85, p.getY() + height * 3 / 4 + 10);
	font.Render(c);
	glPopMatrix();
}

void Level::finish() {
	for (auto obj : level) {
		delete(obj);
	}
	level.clear();
	std::cout << level.size() << std::endl;
	for (auto enm : enemies) {
		delete(enm);
	}
	enemies.clear();
	for (auto srt : secrets) {
		delete(srt);
	}
	secrets.clear();
	for (auto hid : hides) {
		delete(hid);
	}
	hides.clear();
	finalTime = timer;
	complete = true;
}

/*
reinitialise the current level number (used for restarts and loading the next level)
returns false if there is no level associated with the current level number (used to exit at the end)
*/
bool Level::reInitLevel(Player &player) {
	for (auto obj : level) {
		delete(obj);
	}
	level.clear();
	for (auto obj : objectives) {
		delete(obj);
	}
	objectives.clear();
	for (auto enm : enemies) {
		delete(enm);
	}
	enemies.clear();
	for (auto srt : secrets) {
		delete(srt);
	}
	secrets.clear();
	for (auto hid : hides) {
		delete(hid);
	}
	hides.clear();

	if (lvln == 1) this->initLevel1(player);
	else if (lvln == 2) this->initLevel2(player);
	else return false;

	player.setYVelocity(0);
	player.setXVelocity(0);

	this->objCollected = 0;
	this->srtCollected = 0;
	this->deathCount = 0;
	this->timer = 0;
	this->complete = false;

	return true;
}

//Initialises the level 1 world, spikes, enemies, moving platforms, objectives and sets the spawn point for the player.
void Level::initLevel1(Player &player) {
	//boundaries
	level.push_back(new GameObject(-4700, -2400, 2300, 6800));
	level.push_back(new GameObject(-2700, 2100, 7400, 2300));
	level.push_back(new GameObject(2400, -4400, 2300, 6800));
	level.push_back(new GameObject(-4700, -4400, 6800, 2300));

	//static platforms
	level.push_back(new GameObject(-2400, -2100, 600, 300));
	level.push_back(new GameObject(-1500, -2100, 600, 300));
	level.push_back(new GameObject(-1500, -1500, 600, 600));
	level.push_back(new GameObject(-1500, -600, 1200, 600));
	level.push_back(new GameObject(-1500, 150, 1200, 1050));
	level.push_back(new GameObject(-1200, 1200, 300, 150));
	level.push_back(new GameObject(-900, 1200, 600, 300));

	level.push_back(new GameObject(-300, -2100, 1200, 1200));
	level.push_back(new GameObject(300, -900, 600, 600));
	level.push_back(new GameObject(-300, -600, 600, 300));
	level.push_back(new GameObject(0, -300, 600, 300));
	level.push_back(new GameObject(0, 0, 900, 900));
	level.push_back(new GameObject(900, -2100, 300, 900));

	level.push_back(new GameObject(1200, -2100, 900, 1200));
	level.push_back(new GameObject(1200, -600, 300, 300));
	level.push_back(new GameObject(1800, -600, 300, 300));
	level.push_back(new GameObject(1200, -300, 300, 300));
	level.push_back(new GameObject(1200, 0, 1500, 2100));

	//spikes
	level.push_back(new Spikes(-900, -2100, SOUTH, 10));
	level.push_back(new Spikes(-900, -1500, WEST, 10));
	level.push_back(new Spikes(-300, -1500, EAST, 10));
	level.push_back(new Spikes(-1500, -1500, EAST, 5));
	level.push_back(new Spikes(-1800, -2100, SOUTH, 5));
	level.push_back(new Spikes(-900, -600, NORTH, 10));
	level.push_back(new Spikes(-300, 1200, WEST, 4));
	level.push_back(new Spikes(600, 0, NORTH, 5));
	level.push_back(new Spikes(-2400, -1500, WEST, 45));
	level.push_back(new Spikes(-1500, -300, EAST, 5));
	level.push_back(new Spikes(-1500, 150, EAST, 8));
	level.push_back(new Spikes(1200, 1200, EAST, 4));
	level.push_back(new Spikes(-300, -300, WEST, 5));
	level.push_back(new Spikes(-300, -300, SOUTH, 5));
	level.push_back(new Spikes(-300, 150, WEST, 5));
	level.push_back(new Spikes(0, 600, EAST, 5));
	level.push_back(new Spikes(900, 600, WEST, 5));
	level.push_back(new Spikes(1200, 300, EAST, 3));
	level.push_back(new Spikes(900, 0, WEST, 3));
	level.push_back(new Spikes(1200, -600, EAST, 2));
	level.push_back(new Spikes(900, -1200, WEST, 5));
	level.push_back(new Spikes(900, -1200, SOUTH, 5));
	level.push_back(new Spikes(1500, -600, WEST, 10));
	level.push_back(new Spikes(1500, 0, NORTH, 15));
	level.push_back(new Spikes(2400, -180, EAST, 3));
	level.push_back(new Spikes(1800, -600, EAST, 5));
	level.push_back(new Spikes(1800, -300, SOUTH, 5));
	level.push_back(new Spikes(2100, -600, WEST, 5));

	//moving platforms
	level.push_back(new MovingPlatform(-750, -1860, VERTICAL, 1200));
	level.push_back(new MovingPlatform(-2100, -960, VERTICAL, 1500));
	level.push_back(new MovingPlatform(-300, 1440, HORIZONTAL, 1200));

	//end
	level.push_back(new GameObject(2100, -1860, 300, 60));

	//enemies
	enemies.push_back(new Enemy(-1200, -1800, -1500, -900));
	enemies.push_back(new Enemy(-1200, -900, -1500, -900));
	enemies.push_back(new Enemy(-900, 1500, -900, -300));
	enemies.push_back(new Enemy(300, 900, 0, 900));
	enemies.push_back(new Enemy(1500, -900, 1200, 2100));

	//objectives
	objectives.push_back(new Objective(-1980, 600));
	objectives.push_back(new Objective(-900, 45));
	objectives.push_back(new Objective(-180, 495));
	objectives.push_back(new Objective(720, -255));
	objectives.push_back(new Objective(1020, 1650));
	objCount = objectives.size();

	//secrets
	hides.push_back(new GameObject(-300, -900, 600, 300));
	secrets.push_back(new Secret(120, -780));
	hides.push_back(new GameObject(1500, -600, 300, 600));
	secrets.push_back(new Secret(1620, -480));
	srtCount = secrets.size();

	//level number
	lvln = 1;

	//setting the spawn point in the level and moving the character there initially
	player.setSpawn(-2100, -1500);
	player.setPos(-2100, -1500);
	//player.setSpawn(720, -255);
	//player.setPos(720, -255);
}

//Initialises the level 2 world, spikes, enemies, moving platforms, objectives and sets the spawn point for the player.
void Level::initLevel2(Player &player) {
	//boundaries
	level.push_back(new GameObject(-4700, -2400, 2300, 6800));
	level.push_back(new GameObject(-2700, 2100, 7700, 2300));
	level.push_back(new GameObject(2700, -4400, 2300, 6800));
	level.push_back(new GameObject(-4700, -4400, 7100, 2300));

	//static platforms
	level.push_back(new GameObject(-2400, -600, 900, 300));
	level.push_back(new GameObject(-2100, -2100, 300, 900));
	level.push_back(new GameObject(-2100, 0, 1500, 300));
	level.push_back(new GameObject(-2100, 900, 900, 300));
	level.push_back(new GameObject(-2100, 1500, 900, 300));
	level.push_back(new GameObject(-1500, -2100, 300, 600));
	level.push_back(new GameObject(-1500, -900, 300, 600));
	level.push_back(new GameObject(-900, -1800, 300, 1500));
	level.push_back(new GameObject(-600, -900, 1200, 600));
	level.push_back(new GameObject(-300, -2100, 300, 900));
	
	level.push_back(new GameObject(-300, 900, 600, 300));
	level.push_back(new GameObject(0, 600, 300, 300));
	level.push_back(new GameObject(300, -1800, 300, 900));
	level.push_back(new GameObject(300, 0, 300, 300));
	level.push_back(new GameObject(600, -1800, 300, 300));
	level.push_back(new GameObject(600, -600, 900, 300));
	level.push_back(new GameObject(600, 0, 300, 300));
	level.push_back(new GameObject(600, 300, 1500, 300));
	level.push_back(new GameObject(600, 900, 600, 300));
	level.push_back(new GameObject(900, -1200, 300, 300));
	level.push_back(new GameObject(1200, -2100, 300, 600));
	level.push_back(new GameObject(1200, -1500, 900, 600));
	level.push_back(new GameObject(1200, -300, 300, 300));
	level.push_back(new GameObject(1500, 1500, 600, 300));
	level.push_back(new GameObject(1800, -900, 600, 600));
	level.push_back(new GameObject(1800, 900, 600, 300));
	level.push_back(new GameObject(2100, -1800, 300, 900));
	level.push_back(new GameObject(2100, -300, 300, 900));
	level.push_back(new GameObject(2100, 1200, 300, 600));
	

	//spikes
	level.push_back(new Spikes(-2400, -1800, WEST, 5));
	level.push_back(new Spikes(-2400, -900, WEST, 5));
	level.push_back(new Spikes(-2400, -600, NORTH, 15));
	level.push_back(new Spikes(-2400, 600, WEST, 10));
	level.push_back(new Spikes(-2100, -2100, EAST, 2));
	level.push_back(new Spikes(-2100, -1320, EAST, 2));
	level.push_back(new Spikes(-2100, -1200, SOUTH, 5));
	level.push_back(new Spikes(-2100, 0, EAST, 5));
	level.push_back(new Spikes(-2100, 1500, EAST, 5));
	level.push_back(new Spikes(-2100, 1800, SOUTH, 2));
	level.push_back(new Spikes(-1800, -2100, WEST, 15));
	level.push_back(new Spikes(-1800, -2100, SOUTH, 5));
	level.push_back(new Spikes(-1200, -2100, SOUTH, 5));
	level.push_back(new Spikes(-1200, 900, WEST, 5));
	level.push_back(new Spikes(-600, -2100, SOUTH, 5));
	level.push_back(new Spikes(-300, -1500, EAST, 5));
	level.push_back(new Spikes(300, -1800, NORTH, 10));
	level.push_back(new Spikes(300, 0, NORTH, 5));
	level.push_back(new Spikes(600, -1500, SOUTH, 5));
	level.push_back(new Spikes(900, -1800, WEST, 5));
	level.push_back(new Spikes(900, -1200, EAST, 5));
	level.push_back(new Spikes(900, -900, SOUTH, 2));
	level.push_back(new Spikes(900, 0, WEST, 5));
	level.push_back(new Spikes(900, 300, NORTH, 20));
	level.push_back(new Spikes(1200, -300, EAST, 5));
	level.push_back(new Spikes(1200, 900, WEST, 5));
	level.push_back(new Spikes(1800, -300, SOUTH, 5));
	level.push_back(new Spikes(2700, 300, EAST, 15));

	//moving platforms
	level.push_back(new MovingPlatform(-1200, 1740, HORIZONTAL, 2400));

	//boosts
	level.push_back(new Boost(-900, 300, 23));

	//end
	level.push_back(new GameObject(2400, -2160, 300, 60));

	//enemies
	enemies.push_back(new Enemy(-2100, -300, -2400, -1200));
	enemies.push_back(new Enemy(-1800, 300, -2100, -900));
	enemies.push_back(new Enemy(300, -300, -900, 1140));
	enemies.push_back(new Enemy(600, -2100, 0, 1200));
	enemies.push_back(new Enemy(1200, 600, 600, 2400));
	enemies.push_back(new Enemy(1800, 1800, 1500, 2400));

	//objectives
	objectives.push_back(new Objective(-1680, 1200));
	objectives.push_back(new Objective(-1380, -1500));
	objectives.push_back(new Objective(-1380, 300));
	objectives.push_back(new Objective(-1080, -150));
	objectives.push_back(new Objective(420, 300));
	objectives.push_back(new Objective(870, 1200));
	objectives.push_back(new Objective(960, -300));
	objectives.push_back(new Objective(1920, 1200));
	objCount = objectives.size();

	//hides
	hides.push_back(new GameObject(-2400, 1500, 600, 600));
	hides.push_back(new GameObject(0, -2100, 2100, 1500));
	hides.push_back(new GameObject(1500, -600, 300, 300));
	hides.push_back(new GameObject(2100, -2100, 300, 300));

	//secrets
	secrets.push_back(new Secret(120, -1500));
	secrets.push_back(new Secret(720, -1080));
	secrets.push_back(new Secret(1770, -1830));
	secrets.push_back(new Secret(1920, -60));
	secrets.push_back(new Secret(1920, 1800));
	srtCount = secrets.size();

	//level number
	lvln = 2;

	//setting the spawn point in the level and moving the character there initially
	player.setSpawn(-2280, -1800);
	player.setPos(-2280, -1800);
}

/*
level complete text to display when the current level is complete
*/
void Level::endText(Player& p) {
	font.FaceSize(35);
	glPushMatrix();
	glColor3f(0, 0, 0);
	glRasterPos2f(p.getX() - 105, p.getY() + 200);
	font.Render("Finished");
	glPopMatrix();

	font.FaceSize(25);
	glPushMatrix();
	glColor3f(0, 0, 0);
	glRasterPos2f(p.getX() - 190, p.getY() - 200);
	font.Render("Press R to Restart");
	glPopMatrix();

	font.FaceSize(25);
	glPushMatrix();
	glColor3f(0.9, 0, 0);
	glRasterPos2f(p.getX() - 245, p.getY() - 250);
	font.Render("Press Space to Continue");
	glPopMatrix();
}