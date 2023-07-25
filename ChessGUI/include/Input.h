#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

/*
* Responsbile for keeping track of user input.
* User input is received via polling events from the rendering window.
* Keeping track of the input in this class allows the information to be easily retrieved in other areas such as the update function.
*/
class Input {

public:

	std::vector<sf::Mouse::Button> mouseButtonsPressed;
	sf::Vector2i mousePos;

	bool isMbPressed(sf::Mouse::Button) const;

	// To update and store all occurences of user input this iteration
	void update(sf::Event, const sf::RenderWindow&);
	void clear(); // Reset every iteration
};