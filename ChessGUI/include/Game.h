#pragma once

#include <SFML/Graphics.hpp>
#include <Input.h>
#include <Board.h>
#include <Engine.h>

/*
* Responbile for top-level management of the application.
* This includes the rendering window, the game loop and the final draw call.
*/
class Game {
public:
	Game();

	// Getters
	int getWidth() const {
		return videoMode.width;
	}

	int getHeight() const {
		return videoMode.height;
	}

	// To start the application
	void start();

	// Manage all updates per iteration
	void update();

	// Manage all draw calls per iteration
	void render();

private:

	sf::VideoMode videoMode;
	sf::RenderWindow window;
	sf::Event event;
	Input input;
	Board board;
	Engine engine;
};