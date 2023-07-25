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
	Board(const int& width);


	void update(const Input& input);
	void render(sf::RenderWindow& gameWindow, const Input& input);

	void dragDropPiece(const Input& input);

	void drawTiles(sf::RenderWindow& gameWindow);
	void drawPieces(sf::RenderWindow& gameWindow);
	void drawSelectedPiece(sf::RenderWindow& gameWindow, const Input& input);
	void drawHighlightedSquares(sf::RenderWindow& gameWindow);

	int getPieceColor(int index);
	void loadFen(const std::string& fen);

	std::vector<int> board;


private:
	int selectedPiece;
	std::vector<int> highlightedSquares;

	// RENDERING RELATED
	int tileSize;
	sf::RectangleShape tile;
	// Since sprite stores a pointer to texture internally, need to keep texture around.
	sf::Texture piecesTexture;
	std::vector<sf::Sprite> pieceSprites;
	sf::Color lightSquareCol;
	sf::Color darkSquareCol;
	sf::Color highlightCol;

	static const std::string startingFen;

};