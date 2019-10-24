//#pragma once
//
//#include <GLFW/glfw3.h>
//#include <string>
//#include <sstream>
//#include <ftgl.h>
//
//class HUD {
//	int objectives;
//	int secrets;
//	int oCollected;
//	//int sCollected;
//	static FTPixmapFont courier;
//
//public:
//	HUD(int, int);
//
//	//setters
//	void objective();
//	//void secret();
//
//	void printHUD() const;
//};
//
//inline HUD::HUD(int objectives = 0, int secrets = 0) {
//	this->objectives = objectives;
//	this->secrets = secrets;
//	oCollected = 0;
//	//sCollected = 0;
//	courier.FaceSize(30);
//}
//
//inline void HUD::objective(){
//	oCollected++;
//}
//
////inline void HUD::secret() {
////	sCollected++;
////}
//
//inline void HUD::printHUD() const {
//	std::ostringstream oss;
//	oss << "Objectives " << oCollected << "/" << objectives;
//	std::string s = oss.str();
//	const char* p = s.c_str();
//
//	glPushMatrix();
//	glColor3f(1.0f, 0, 0);
//	glTrans(300000, 9999999, 0);
//	courier.Render(p);
//	glPopMatrix();
//}