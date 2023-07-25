#include <Board.h>
#include <Input.h>
#include <sstream>
#include <iostream>

const std::string Board::startingFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";


Board::Board(const int& width) : selectedPiece(-1), board(64, 0), tileSize(width / 8), lightSquareCol(234, 234, 210), darkSquareCol(75, 114, 153), highlightCol(240, 0, 0, 100)
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


void Board::update(const Input& input)
{

    int mouseTileX = input.mousePos.x / tileSize;
    int mouseTileY = input.mousePos.y / tileSize;
   
    if (selectedPiece == -1 && input.isMbPressed(sf::Mouse::Button::Left) && board[mouseTileX + mouseTileY * 8] != 0)
    {
        selectedPiece = mouseTileX + mouseTileY * 8;     
    }
    else if (selectedPiece != -1 && input.isMbPressed(sf::Mouse::Button::Left))
    {
        int targetPiece = mouseTileX + mouseTileY * 8;
        if (board[targetPiece] == 0 || getPieceColor(targetPiece) != getPieceColor(selectedPiece))
        {
            
            board[targetPiece] = board[selectedPiece];
            board[selectedPiece] = 0;

            // Every time a move is made, set the highlight squares appropriately
            highlightedSquares.clear();
            highlightedSquares.push_back(selectedPiece);
            highlightedSquares.push_back(targetPiece);
        }
        selectedPiece = -1;
    }
}

void Board::render(sf::RenderWindow& gameWindow, const Input& input)
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
        
        if (selectedPiece == index) continue;

		const int piece = board[index];

		if (piece == 0) continue;

		const int spriteIndex = piece - 1;

		pieceSprites[spriteIndex].setPosition(x * tileSize, y * tileSize);

		gameWindow.draw(pieceSprites[spriteIndex]);
	}

    // Draw selected piece
    if (selectedPiece != -1)
    {
        const int spriteIndex = board[selectedPiece] - 1;
        pieceSprites[spriteIndex].setPosition(input.mousePos.x - tileSize / 2, input.mousePos.y - tileSize / 2);
        gameWindow.draw(pieceSprites[spriteIndex]);
    }

    // Draw highlighted squares
    sf::RectangleShape highlight(sf::Vector2f(tileSize, tileSize));
    highlight.setFillColor(highlightCol);

    for (const int& i : highlightedSquares)
    {
        int x = i % 8;
        int y = i / 8;

        highlight.setPosition(sf::Vector2f(x * tileSize, y * tileSize));
        gameWindow.draw(highlight);
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

int Board::getPieceColor(int index)
{
    int targetPiece = board[index];
    if (targetPiece == 0) return -1;
    else if (targetPiece < 7) return 0;
    else return 1;
}
