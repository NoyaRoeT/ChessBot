#include <Board.h>
#include <Input.h>
#include <sstream>
#include <iostream>
#include <Engine.h>




Board::Board(const int& width) : selectedIndex(-1), board(64, 0), tileSize(width / 8), lightSquareCol(234, 234, 210), darkSquareCol(75, 114, 153), mostRecentMoveCol(240, 0, 0, 100), selectedOutlineCol(240, 240, 0), selectedMovesOutlineCol(0, 240, 0), playerColor(Engine::WHITE)
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
    drawSelectedPieceMoves(gameWindow);
    drawPieces(gameWindow, engine);
}


void Board::movePieceManually(const Input& input, Engine& engine)
{

    int boardX = input.mousePos.x / tileSize;
    int boardY = input.mousePos.y / tileSize;
    if (boardX < 0 || boardX > 7 || boardY < 0 || boardY > 7) return;

    if (playerColor == Engine::WHITE)
    {
        boardX = 7 - boardX;
        boardY = 7 - boardY;
    }

    if (selectedIndex == -1 && input.isMbPressed(sf::Mouse::Button::Left) && !engine.isSquareEmpty(boardX + boardY * 8))
    {
        selectedIndex = boardX + boardY * 8;
        selectedPieceMoves = engine.getPieceMoves(selectedIndex);
    }
    else if (selectedIndex != -1 && input.isMbPressed(sf::Mouse::Button::Left))
    {
        int targetIndex = boardX + boardY * 8;
        Move move(selectedIndex, targetIndex);
        
        if (engine.makeMove(move))
        {
            // Every time a move is made, set the highlight squares appropriately
            mostRecentMove.clear();
            mostRecentMove.push_back(selectedIndex);
            mostRecentMove.push_back(targetIndex);
         };

        // Clear possible moves for selected piece
        selectedPieceMoves.clear();
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
    
    std::vector<int> board;
    if (playerColor == Engine::WHITE)
    {
        board = engine.getBoard();
    }
    else
    {
        board = rotateBoard(engine.getBoard());
    }

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
        const int index = (playerColor == Engine::WHITE) ? 63 - i : i;
        int x = index % 8;
        int y = index / 8;

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
        const int selected = (playerColor == Engine::WHITE) ? 63 - selectedIndex : selectedIndex;
        outline.setPosition(sf::Vector2f((selected % 8) * tileSize, (selected / 8) * tileSize));
        gameWindow.draw(outline);
    }
}

void Board::drawSelectedPieceMoves(sf::RenderWindow& gameWindow)
{
    if (selectedIndex != -1)
    {
        sf::RectangleShape outline(sf::Vector2f(tileSize, tileSize));
        outline.setFillColor(sf::Color::Transparent);
        outline.setOutlineThickness(-5);
        outline.setOutlineColor(selectedMovesOutlineCol);
        for (const Move& m : selectedPieceMoves)
        {
            const int targetIndex = (playerColor == Engine::WHITE) ? 63 - m.targetIndex : m.targetIndex;
            outline.setPosition(sf::Vector2f((targetIndex % 8) * tileSize, (targetIndex / 8 ) * tileSize));
            gameWindow.draw(outline);
        }
    }
}

std::vector<int> Board::rotateBoard(const std::vector<int>& board)
{
    std::vector<int> rotated(64);

    for (int x = 0; x != 8; ++x)
    for (int y = 0; y != 8; ++y)
    {
        rotated[(7-x) + (7-y) * 8] = board[x + y * 8]; 
    }

    return rotated;
}

