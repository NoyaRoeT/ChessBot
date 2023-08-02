#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <Move.h>

class Input;
class Engine;
/*
* Responsible for the chess board
*/
class Board {
public:
	std::vector<int> board;

	Board(const int& width);

	void update(const Input& input, Engine& engine);
	void render(sf::RenderWindow& gameWindow, const Engine& engine);

private:
	// Manual piece movement
	int selectedIndex;
	std::vector<Move> selectedPieceMoves;

	// Highlighted
	std::vector<int> mostRecentMove;

	// Drawing related members
	int tileSize;
	sf::RectangleShape tile;
	sf::Texture piecesTexture; // Since sprite stores a pointer to texture internally, need to keep texture around.
	std::vector<sf::Sprite> pieceSprites;
	sf::Color lightSquareCol;
	sf::Color darkSquareCol;
	sf::Color mostRecentMoveCol;
	sf::Color selectedOutlineCol;
	sf::Color selectedMovesOutlineCol;


	// User input
	void movePieceManually(const Input& input, Engine& engine);

	// Draw functions
	void drawTiles(sf::RenderWindow& gameWindow);
	void drawMostRecentMove(sf::RenderWindow& gameWindow);
	void drawSelectedOutline(sf::RenderWindow& gameWindow);
	void drawPieces(sf::RenderWindow& gameWindow, const Engine& engine);
	void drawSelectedPieceMoves(sf::RenderWindow& gameWindow);

};