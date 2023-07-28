#include <Board.h>
#include <Input.h>
#include <sstream>
#include <iostream>
#include <Piece.h>
#include <Engine.h>




Board::Board(const int& width) : selectedIndex(-1), board(64, 0), tileSize(width / 8), lightSquareCol(234, 234, 210), darkSquareCol(75, 114, 153), mostRecentMoveCol(240, 0, 0, 100), selectedOutlineCol(240, 240, 0)
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

}


void Board::update(const Input& input, Engine& engine)
{

    movePieceManually(input, engine);
}

void Board::render(sf::RenderWindow& gameWindow, const Engine& engine)
{
    drawTiles(gameWindow);
    drawMostRecentMove(gameWindow);
    drawSelectedOutline(gameWindow);
    drawPieces(gameWindow, engine);
}


void Board::movePieceManually(const Input& input, Engine& engine)
{
    if (input.mousePos.x >= tileSize * 8 || input.mousePos.x < 0) return;

  
    const int boardX = 7 - input.mousePos.x / tileSize;
    const int boardY = 7 - input.mousePos.y / tileSize;

    if (selectedIndex == -1 && input.isMbPressed(sf::Mouse::Button::Left) && !engine.isSquareEmpty(boardX + boardY * 8))
    {
        selectedIndex = boardX + boardY * 8;
    }
    else if (selectedIndex != -1 && input.isMbPressed(sf::Mouse::Button::Left))
    {
        int targetIndex = boardX + boardY * 8;
        if (engine.isSquareEmpty(targetIndex) || engine.getPieceColor(targetIndex) != engine.getPieceColor(selectedIndex))
        {

            engine.makeMove(selectedIndex, targetIndex);

            // Every time a move is made, set the highlight squares appropriately
            mostRecentMove.clear();
            mostRecentMove.push_back(selectedIndex);
            mostRecentMove.push_back(targetIndex);
        }
        selectedIndex = -1;
    }
}

void Board::drawTiles(sf::RenderWindow& gameWindow)
{
    for (int x = 0; x != 8; ++x)
    for (int y = 0; y != 8; ++y)
    {

        tile.setPosition(x * tileSize, y * tileSize);
        sf::Color tileColor = ((x + y) % 2 == 0) ? lightSquareCol : darkSquareCol;
        tile.setFillColor(tileColor);

        gameWindow.draw(tile);
    }
}
void Board::drawPieces(sf::RenderWindow& gameWindow, const Engine& engine)
{
    // Pieces are drawn from 'top to bottom'.
    // The piece at index 0 is at the top left of the board.
    const std::vector<int>& board = engine.getBoard();

    for (int i = 0; i != 8; ++i)
    for (int j = 0; j != 8; ++j)
    {
        const int x = 7 - i;
        const int y = 7 - j;

        const int piece = board[i + j * 8];

        if (piece == 0) continue;

        const int spriteIndex = piece - 1;

        pieceSprites[spriteIndex].setPosition(x * tileSize, y * tileSize);

        gameWindow.draw(pieceSprites[spriteIndex]);
    }
}

void Board::drawMostRecentMove(sf::RenderWindow& gameWindow)
{
    sf::RectangleShape highlight(sf::Vector2f(tileSize, tileSize));
    highlight.setFillColor(mostRecentMoveCol);

    for (const int& i : mostRecentMove)
    {
        int x = 7 - i % 8;
        int y = 7 - i / 8;

        highlight.setPosition(sf::Vector2f(x * tileSize, y * tileSize));
        gameWindow.draw(highlight);
    }
}

void Board::drawSelectedOutline(sf::RenderWindow& gameWindow)
{
    if (selectedIndex != -1)
    {
        sf::RectangleShape outline(sf::Vector2f(tileSize, tileSize));
        outline.setFillColor(sf::Color::Transparent);
        outline.setOutlineThickness(-5);
        outline.setOutlineColor(selectedOutlineCol);
        outline.setPosition(sf::Vector2f((7 - selectedIndex % 8) * tileSize, (7 - selectedIndex / 8) * tileSize));
        gameWindow.draw(outline);
    }
}

