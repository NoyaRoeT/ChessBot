#include <Board.h>

Board::Board(int width) : board(64, 0), tileSize(width / 8) 
{
	tile = sf::RectangleShape(sf::Vector2f(tileSize, tileSize));
}

void Board::update()
{
}

void Board::render(sf::RenderWindow& gameWindow)
{
	// Draw board tiles
	for (int x = 0; x !=8; ++x)
	for (int y = 0; y != 8; ++y)
	{
		tile.setPosition(x * tileSize, y * tileSize);
		sf::Color tileColor = ((x + y) % 2 == 0) ? lightSquareCol : darkSquareCol;
		tile.setFillColor(tileColor);
		gameWindow.draw(tile);
	}
}
