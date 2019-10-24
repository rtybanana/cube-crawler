#pragma once
#include <string>
#include <vector>

class menu {
	std::string title;
	std::vector<std::string> options;

public:
	menu(std::string, std::vector<std::string>);
};

inline menu::menu(std::string title, std::vector<std::string> options) {
	this->title = title;
	this->options = options;
}
