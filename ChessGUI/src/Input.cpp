#include <Input.h>
#include <iostream>

bool Input::isMbPressed(sf::Mouse::Button button) const
{
	for (const sf::Mouse::Button& b : mouseButtonsPressed)
	{
		if (b == button) return true;
	}
	return false;
}

void Input::update(sf::Event event, const sf::RenderWindow& window)
{
	// Update the object with all user input occurences this iteration.
	if (event.type == sf::Event::MouseButtonPressed)
	{
		mouseButtonsPressed.push_back(event.mouseButton.button);
	}

	mousePos = sf::Mouse::getPosition(window);
	
	
}

void Input::clear()
{
	mouseButtonsPressed.clear();
}
