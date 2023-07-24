#pragma once
#include <vector>

/*
* Responsbile for keeping track of user input.
* User input is received via polling events from the rendering window.
* Keeping track of the input in this class allows the information to be easily retrieved in other areas such as the update function.
*/
class Input {

public:
	// To update and store all occurences of user input this iteration
	void update();

	// Not sure what input we need yet so leaving this empty
	std::vector<int> test;
};