#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

// Black is differentiated by a +6
constexpr unsigned int KING = 1;
constexpr unsigned int QUEEN = 2;
constexpr unsigned int BISHOP = 3;
constexpr unsigned int KNIGHT = 4;
constexpr unsigned int ROOK = 5;
constexpr unsigned int PAWN = 6;


class Input;
/*
* Responsible for the chess board
*/
class Board {
public:
	std::vector<int> board;

	Board(const int& width);

	void update(const Input& input);
	void render(sf::RenderWindow& gameWindow, const Input& input);
	void loadFen(const std::string& fen);



private:
	int selectedPiece;
	std::vector<int> highlightedSquares;

	// Drawing related members
	int tileSize;
	sf::RectangleShape tile;
	sf::Texture piecesTexture; // Since sprite stores a pointer to texture internally, need to keep texture around.
	std::vector<sf::Sprite> pieceSprites;
	sf::Color lightSquareCol;
	sf::Color darkSquareCol;
	sf::Color highlightCol;

	// Static members
	static const std::string startingFen;

	// User input
	void dragDropPiece(const Input& input);

	// Draw functions
	void drawTiles(sf::RenderWindow& gameWindow);
	void drawPieces(sf::RenderWindow& gameWindow);
	void drawSelectedPiece(sf::RenderWindow& gameWindow, const Input& input);
	void drawHighlightedSquares(sf::RenderWindow& gameWindow);

	// Util functions
	int getPieceColor(int index);

};