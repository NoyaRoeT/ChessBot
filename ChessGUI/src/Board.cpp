#include <Board.h>
#include <sstream>

const std::string Board::startingFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

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

    loadFen(Board::startingFen);
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
	// Pieces are drawn from 'top to bottom'.
	// The piece at index 0 is at the top left of the board.
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

void Board::loadFen(const std::string& fen)
{
	// FEN starts with rank 8 -> 1 and a->h
	// White pieces are uppercase letters
    std::stringstream fenStream(fen);

    // RESET BOARD
    for (int& piece : board) piece = 0;

    // READ PIECE PLACEMENT DATA
    std::string piecePlacementStr;
    fenStream >> piecePlacementStr;

    int x = 0, y = 0;
    for (const char& c : piecePlacementStr)
    {

        if (c == '/')
        {
            x = 0;
            ++y;
            continue;
        }

        if (isdigit(c))
        {
            const int numEmptySqrs = c - '0';
            x += numEmptySqrs;
        }
        else
        {
            int piece = 0;
            switch (tolower(c)) {
            case 'k':
                piece = KING;
                break;
            case 'q':
                piece = QUEEN;
                break;
            case 'b':
                piece = BISHOP;
                break;
            case 'n':
                piece = KNIGHT;
                break;
            case 'r':
                piece = ROOK;
                break;
            case 'p':
                piece = PAWN;
                break;
            }
            int color = (islower(c)) ? 1 : 0;
            board[x + y * 8] = piece + color * 6;
            ++x;
        }
    }
}
