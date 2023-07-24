#include <Board.h>

Board::Board(int width) : board(64, 0), tileSize(width / 8) 
{
	tile = sf::RectangleShape(sf::Vector2f(tileSize, tileSize));
	piecesTexture.loadFromFile("assets/chess_pieces_sprites.png");

	const sf::Vector2u textureDims = piecesTexture.getSize();
	const int length = textureDims.x / 6, height = textureDims.y / 2;

	for (int i = 0; i != 2; ++i)
	for (int j = 0; j != 6; ++j)
	{
		sf::Sprite sprite(piecesTexture, sf::IntRect(j * length, i * height, length, height));
		sprite.setScale(sf::Vector2f(tileSize / sprite.getLocalBounds().width, tileSize / sprite.getLocalBounds().height));
		pieceSprites.push_back(sprite);
	}

	board[0] = KING + 6;
	board[7] = BISHOP;
	board[16] = PAWN + 6;
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

	// Draw pieces
	for (int x = 0; x != 8; ++x)
	for (int y = 0; y != 8; ++y)
	{
		const int index = x + y * 8;
		const int piece = board[index];

		if (piece == 0) continue;

		const int spriteIndex = piece - 1;

		pieceSprites[spriteIndex].setPosition(x * tileSize, y * tileSize);

		gameWindow.draw(pieceSprites[spriteIndex]);
	}
}
