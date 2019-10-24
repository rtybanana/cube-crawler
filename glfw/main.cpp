#include <iostream>
#include <glfw3.h>
#include "Level.h"
#include "Player.h"
#include "GameObject.h"
#include "stb_image.h"

//global polling variables
bool left = false;
bool right = false;
bool up = false;
bool space = false;
bool restart = false;
bool left_click = false;
bool mouseIn = false;

//global enum to assist with seperating what opengl renders
enum screens {menu, instructions, pause, game};
screens screen;

/* INPUT CALLBACKS AND PROCESSING */
static void size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-width/2, width/2, -height/2, height/2, 0, 1);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		if(screen == game) screen = pause;
		else if (screen == pause) screen = game;
	}
	if (key == GLFW_KEY_LEFT || key == GLFW_KEY_A) {
		if (action == GLFW_PRESS) left = true;
		else if (action == GLFW_RELEASE) left = false;
	}
	if (key == GLFW_KEY_RIGHT || key == GLFW_KEY_D) {
		if (action == GLFW_PRESS) right = true;
		else if (action == GLFW_RELEASE) right = false;
	}
	if (key == GLFW_KEY_R) {
		if (action == GLFW_PRESS) restart = true;
		else if (action == GLFW_RELEASE) restart = false;
	}
	if (key == GLFW_KEY_UP || key == GLFW_KEY_W) {
		if (!up) {
			if (action == GLFW_PRESS) up = true;
		}
	}
	if (key == GLFW_KEY_SPACE) {
		if (action == GLFW_PRESS) space = true;
		else if (action == GLFW_RELEASE) space = false;
	}
}

static void mouse_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT)
		if (action == GLFW_PRESS) left_click = true;
		else left_click = false;
}

inline void drawGame(Player player, Level &level, float width, float height) {
	//setting 'camera' to follow the character (translates based on player coords)
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glScalef(0.6, 0.6, 1);
	glTranslatef(-(player.getX() + player.getWidth() / 2) / (width / 2), -(player.getY() + player.getHeight() / 2) / (height / 2), 0);
	glOrtho(-width / 2, width / 2, -height / 2, height / 2, 0, 1);

	//draw the frame
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	player.drawVision();
	level.drawLevel();
	level.drawHUD(player, width, height);
	player.drawObject();
}

inline void drawMainMenu(float x, float y, float width, float height, GLuint texture, FTPixmapFont &font) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-width / 2, width / 2, -height / 2, height / 2, 0, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glColor3f(1, 1, 1);
	glBindTexture(GL_TEXTURE_2D, texture);						//bind texture title.png
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_POLYGON);
		glTexCoord2f(0, 0); glVertex2f(-380, 180);
		glTexCoord2f(0, 1); glVertex2f(-380, 270);
		glTexCoord2f(1, 1); glVertex2f(380, 270);
		glTexCoord2f(1, 0); glVertex2f(380, 180);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);							//unbind texture 
	glPopMatrix();

	glPushMatrix();
	if ((x > -150 && x < 150) && (y > 40 && y < 100)) {
		glColor3f(0.8, 0.6, 0.6);
		if (left_click) screen = game;
	}
	else glColor3f(1, 1, 1);
	glBegin(GL_POLYGON);
		glVertex2f(-150, 40);
		glVertex2f(-150, 100);
		glVertex2f(150, 100);
		glVertex2f(150, 40);
	glEnd();
	font.FaceSize(35);
	glColor3f(0, 0, 0);
	glRasterPos2f(-45, 60);
	font.Render("Play");
	glPopMatrix();

	glPushMatrix();
	if ((x > -150 && x < 150) && (y > -40 && y < 20)) {
		glColor3f(0.8, 0.6, 0.6);
		if (left_click) screen = instructions;
	}
	else glColor3f(1, 1, 1);
	glBegin(GL_POLYGON);
		glVertex2f(-150, -40);
		glVertex2f(-150, 20);
		glVertex2f(150, 20);
		glVertex2f(150, -40);
	glEnd();
	font.FaceSize(35);
	glColor3f(0, 0, 0);
	glRasterPos2f(-125, -20);
	font.Render("Instructions");
	glPopMatrix();

	glPushMatrix();
	font.FaceSize(35);
	glColor3f(1, 1, 1);
	glRasterPos2f(-190, -200);
		font.Render("arrow keys to move");
	glRasterPos2f(-130, -250);
		font.Render("r to restart");
	glRasterPos2f(-130, -300);
		font.Render("esc to pause");
	glPopMatrix();
}

inline void drawPauseMenu(float x, float y, float width, float height, FTPixmapFont &font) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-width / 2, width / 2, -height / 2, height / 2, 0, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	font.FaceSize(35);
	glColor3f(0.9, 0, 0);
	glRasterPos2f(-60, 100);
	font.Render("Paused");
	glPopMatrix();

	glPushMatrix();
	if (x > -width / 2 + 20 && x < -width / 2 + 170 && y > height / 2 - 60 && y < height / 2 - 20) {
		glColor3f(0.8, 0.6, 0.6);
		if (left_click) screen = menu;
	}
	else glColor3f(0.3, 0.3, 0.3);
	glBegin(GL_POLYGON);
		glVertex2f(-width/2 + 20, height/2 - 60);
		glVertex2f(-width/2 + 20, height/2 - 20);
		glVertex2f(-width/2 + 170, height/2 - 20);
		glVertex2f(-width/2 + 170, height/2 - 60);
	glEnd();
	font.FaceSize(25);
	glColor3f(1, 1, 1);
	glRasterPos2f(-width/2 + 40, height/2 - 48);
	font.Render("<< Quit");
	glPopMatrix();
}

inline void drawInstructions(float x, float y, float width, float height, FTPixmapFont &font) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-width / 2, width / 2, -height / 2, height / 2, 0, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	if (x > -width / 2 + 20 && x < -width / 2 + 170 && y > height / 2 - 60 && y < height / 2 - 20) {
		glColor3f(0.8, 0.6, 0.6);
		if (left_click) screen = menu;
	}
	else glColor3f(0.3, 0.3, 0.3);
	glBegin(GL_POLYGON);
		glVertex2f(-width / 2 + 20, height / 2 - 60);
		glVertex2f(-width / 2 + 20, height / 2 - 20);
		glVertex2f(-width / 2 + 170, height / 2 - 20);
		glVertex2f(-width / 2 + 170, height / 2 - 60);
	glEnd();
	font.FaceSize(25);
	glColor3f(1, 1, 1);
	glRasterPos2f(-width / 2 + 40, height / 2 - 48);
	font.Render("<< Back");
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 1);
	glBegin(GL_POLYGON);
		glVertex2f(-400, -160);
		glVertex2f(-400, 300);
		glVertex2f(-240, 300);
		glVertex2f(-240, -160);
	glEnd();
	glPopMatrix();

	Objective objective(-350, 200);
	objective.drawObject();
	glPushMatrix();
	font.FaceSize(35);
	glColor3f(1, 1, 1);
	glRasterPos2f(-200, 240);
		font.Render("Objective");
	font.FaceSize(25);
	glRasterPos2f(-180, 200);
		font.Render("- Collect them all to open the end");
	glPopMatrix();

	Secret secret(-350, 50);
	secret.drawObject();
	glPushMatrix();
	font.FaceSize(35);
	glColor3f(1, 1, 1);
	glRasterPos2f(-200, 90);
		font.Render("Secret");
	font.FaceSize(25);
	glRasterPos2f(-180, 50);
		font.Render("- Collect these for a time bonus of -5s each");
	glRasterPos2f(-180, 20);
		font.Render("- Very well hidden");
	glPopMatrix();

	Secret light(-350, -100);
	Enemy enemy(-350, -100, 0, 0);
	enemy.drawObject();
	glPushMatrix();
	font.FaceSize(35);
	glColor3f(1, 1, 1);
	glRasterPos2f(-200, -60);
		font.Render("Enemy");
	font.FaceSize(25);
	glRasterPos2f(-180, -100);
		font.Render("- Will follow you if they enter your light");
	glRasterPos2f(-180, -130);
		font.Render("- Landing on them kills them with a +5s penalty");

	font.FaceSize(35);
	glColor3f(1, 1, 1);
	glRasterPos2f(-260, -270);
		font.Render("Finish as fast as you can!");
	glPopMatrix();
}

/*
setup and execution of the main loop
*/
int main() {
	glfwInit();

	//setting up glfw window and callbacks
	GLFWwindow* window = glfwCreateWindow(1280, 720, "Cube Crawler", NULL, NULL);
	GLFWimage images[1]; 
	images[0].pixels = stbi_load("Include/stb/ico.png", &images[0].width, &images[0].height, 0, 4); //rgba channels 
	glfwSetWindowIcon(window, 1, images); 
	stbi_image_free(images[0].pixels);

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_callback);
	glfwSetWindowSizeCallback(window, size_callback);

	//instantiating and initialialising the player and level objects
	Player player;
	Level level; 
	level.initLevel1(player);
	screen = menu;

	FTPixmapFont font = FTPixmapFont("./Fonts/cour.ttf");
	GLuint title = GameObject::createTexture("Include/stb/title.png");

	glClearColor(0, 0, 0, 1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-640, 640, -360, 360, 0, 1);

	double lastTime = glfwGetTime(), timer = lastTime;
	double deltaTime = 0, nowTime = 0;

	while (!glfwWindowShouldClose(window)) { 
		//polling window size for use in the next opengl frame render
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		//measure time
		nowTime = glfwGetTime();
		deltaTime += (nowTime - lastTime) / gameTime::limitFPS;
		lastTime = nowTime;
		int updates = 0;

		while (deltaTime >= 1.0) {
			//std::cout << deltaTime << std::endl;
			glfwPollEvents();								//poll keypresses
			if (screen == game) {
				player.processKeys(left, right, up);
				if (updates < (int)(1 / gameTime::limitFPS)) {
					level.updateLevel(player);
					updates++;
				}
				if (player.isDead()) {
					player.reset();
					right = false;
					left = false;
					lastTime += 0.4;						//this corrects the lastTime because the player.reset() function sleeps the thread for 400 milliseconds
					break;
				}
				if (level.checkCollision(player) == false) up = false;
			}

			deltaTime--;
		}
		
		glClear(GL_COLOR_BUFFER_BIT);
		if (screen == pause || screen == game) {								//if in game or paused
			drawGame(player, level, width, height);
			if (restart) level.reInitLevel(player), screen = game;
			if (screen == pause) {												//if game is paused
				double x, y;
				glfwGetCursorPos(window, &x, &y);								//poll cursor position for mouse interaction
				x -= width / 2, y -= height / 2;								//convert mouse coordinates to viewport coordinates
				y *= -1;
				drawPauseMenu(x, y, width, height, font);						//draw pause menu
			}
			else if (screen == game) {											//if currently playing game
				gameTime::counter++;
				if (nowTime - timer > 1.0) {									//increments level timer once every second
					timer = nowTime;
					level.incrTimer();
				}
				if (level.isComplete()) {										//checks if level is complete
					Level::endText(player);
					if (space) {
						level.incrLevel();
						if (!level.reInitLevel(player)) glfwSetWindowShouldClose(window, 1);
					}
					if (restart) level.reInitLevel(player);
				}
			}
		}
		else if (screen == menu || screen == instructions) {
			double x, y;
			glfwGetCursorPos(window, &x, &y);											//poll cursor position for mouse interaction
			x -= width / 2, y -= height / 2;											//convert mouse coordinates to viewport coordinates
			y *= -1;
			if (screen == menu) drawMainMenu(x, y, width, height, title, font);
			if (screen == instructions) drawInstructions(x, y, width, height, font);

			if (screen == game) level.setLevel(1), level.reInitLevel(player);			//if play clicked in either menu
		}
		
		glFlush();
		glfwSwapBuffers(window); 
	}

	glfwTerminate();
}