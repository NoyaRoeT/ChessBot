#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

// Black is differentiated by a +6
constexpr unsigned int KING = 1;
constexpr unsigned int QUEEN = 2;
constexpr unsigned int BISHOP = 3;
constexpr unsigned int KNIGHT = 4;
constexpr unsigned int ROOK = 5;
constexpr unsigned int PAWN = 6;

/*
* Responsible for the chess board
*/
class Board {
public:
	Board(int width);

	void update();
	void render(sf::RenderWindow& gameWindow);

	std::vector<int> board;


private:

	// RENDERING RELATED
	// Since sprite stores a pointer to texture internally, need to keep texture around.
	int tileSize;
	sf::RectangleShape tile;
	sf::Texture piecesTexture;
	std::vector<sf::Sprite> pieceSprites;
	sf::Color lightSquareCol{ 234,234,210 };
	sf::Color darkSquareCol{ 75,114,153 };


};