#pragma once

#include <vector>
#include <list>
#include <ftgl.h>
#include "GameObject.h"
#include "Objective.h"
#include "Enemy.h"
#include "Secret.h"

class Player;

class Level {
	static FTPixmapFont font;
	std::list<GameObject*> level;
	std::list<Objective*> objectives;
	std::list<Enemy*> enemies;
	std::list<GameObject*> hides;
	std::list<Secret*> secrets;
	int deathCount;
	int objCount;
	int objCollected;
	int srtCount;
	int srtCollected;
	int timer;
	int finalTime;
	bool complete;
	int lvln;

public:
	Level(std::list<GameObject*>, std::list<Objective*>, std::list<Enemy*>, 
		  std::list<GameObject*>, std::list<Secret*>, int);
	~Level();
	Level(const Level&);
	Level operator=(const Level&);

	bool isComplete() const;

	void incrDeaths();
	void incrTimer();
	void incrLevel();
	void setLevel(int);

	bool checkCollision(Player&);
	void updateLevel(Player&) const;
	void drawLevel() const;
	void drawHUD(Player, float, float) const;
	void finish();

	bool reInitLevel(Player&);
	void initLevel1(Player&);
	void initLevel2(Player&);
	static void endText(Player&);
};

inline Level::Level(std::list<GameObject*> objects = std::list<GameObject*>(),
					std::list<Objective*> objectives = std::list<Objective*>(),
					std::list<Enemy*> enemies = std::list<Enemy*>(),
					std::list<GameObject*> hides = std::list<GameObject*>(),
					std::list<Secret*> secrets = std::list<Secret*>(), 
					int lvln = 1) {
	this->level = objects;
	this->objectives = objectives;
	this->enemies = enemies;
	this->hides = hides;
	this->secrets = secrets;
	this->objCount = objectives.size();
	this->srtCount = secrets.size();
	
	this->objCollected = 0;
	this->srtCollected = 0;
	this->deathCount = 0;
	this->timer = 0;
	this->complete = false;
	this->lvln = lvln;
}

//getters
inline bool Level::isComplete() const {
	return complete;
}

//setters
inline void Level::incrDeaths() {
	deathCount++;
}

inline void Level::incrTimer() {
	timer++;
}

inline void Level::incrLevel() {
	lvln++;
}

inline void Level::setLevel(int n) {
	lvln = n;
}

//copy assignment operator
inline Level Level::operator=(const Level& l) {
	if (this != &l) {
		std::list<GameObject*> newObjects;
		std::list<Objective*> newObjectives;
		std::list<Enemy*> newEnemies;
		std::list<GameObject*> newHides;
		std::list<Secret*> newSecrets;

		for (GameObject* obj : l.level) {
			newObjects.push_back(obj);
		}
		for (Objective* obj : l.objectives) {
			newObjectives.push_back(obj);
		}
		for (Enemy* obj : l.enemies) {
			newEnemies.push_back(obj);
		}
		for (GameObject* obj : l.hides) {
			newHides.push_back(obj);
		}
		for (Secret* obj : l.secrets) {
			newSecrets.push_back(obj);
		}

		while (!level.empty()) delete level.front(), level.pop_front();
		while (!objectives.empty()) delete objectives.front(), objectives.pop_front();
		while (!enemies.empty()) delete enemies.front(), enemies.pop_front();
		while (!hides.empty()) delete hides.front(), hides.pop_front();
		while (!secrets.empty()) delete secrets.front(), secrets.pop_front();

		this->level = newObjects;
		this->objectives = newObjectives;
		this->enemies = newEnemies;
		this->hides = newHides;
		this->secrets = newSecrets;
		this->objCount = newObjectives.size();
		this->srtCount = newSecrets.size();

		this->objCollected = 0;
		this->srtCollected = 0;
		this->deathCount = 0;
		this->timer = 0;
		this->complete = false;
	}
	return *this;
}
