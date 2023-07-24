#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

// Black is differentiated by a +6
constexpr unsigned int KING = 1;
constexpr unsigned int QUEEN = 2;
constexpr unsigned int ROOK = 3;
constexpr unsigned int BISHOP = 4;
constexpr unsigned int KNIGHT = 5;
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

	// Rendering related
	int tileSize;
	sf::RectangleShape tile;
	std::vector<sf::Texture> pieceTextures;
	std::vector<sf::Sprite> pieceSprites;
	sf::Color lightSquareCol{255, 255, 255 };
	sf::Color darkSquareCol{ 0, 0, 0 };


};