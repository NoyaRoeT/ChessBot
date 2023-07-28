#include "Engine.h"
#include <Piece.h>
#include <sstream>
#include <iostream>

const std::string Engine::startingFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

Engine::Engine() : board(64, 0)
{
    loadFen(startingFen);
}


const std::vector<int>& Engine::getBoard() const
{
    return board;
}

int Engine::getPieceColor(int index)
{
    int targetPiece = board[index];
    if (targetPiece == 0) return -1;
    else if (targetPiece < 7) return 0;
    else return 1;
}

Bitboard Engine::getOccupiedSquares(int color)
{
    int start = color * 6;
    int end = start + 6;
    Bitboard occupied;
    for (int i = start; i != end; ++i)
    {
        occupied |= piecePositions[i];
    }

    return occupied;
}

bool Engine::isSquareEmpty(int index)
{
    return board[index] == 0;
}

void Engine::makeMove(const int& origin, const int& target)
{
    // Makes moves without checking legality

    int originPiece = board[origin];
    int targetPiece = board[target];
    board[target] = originPiece;
    board[origin] = 0;

    if (targetPiece != 0) piecePositions[targetPiece - 1].setBit(target, 0);
    piecePositions[originPiece - 1].setBit(origin, 0);
    piecePositions[originPiece - 1].setBit(target, 1);

    Bitboard occupied = getOccupiedSquares(0) | getOccupiedSquares(1);
    occupied.printBoard();
    std::cout << std::endl;
}

void Engine::loadFen(const std::string& fen)
{
    // Reset boards
    piecePositions = std::vector<Bitboard>(12);
    board = std::vector<int>(64, 0);

    // FEN starts with rank 8 -> 1 and a->h
    // White pieces are uppercase letters
    std::stringstream fenStream(fen);

    // Read piece placement string
    std::string piecePlacementStr;
    fenStream >> piecePlacementStr;

    int x = 7, y = 7;
    for (const char& c : piecePlacementStr)
    {

        if (c == '/')
        {
            x = 7;
            --y;
            continue;
        }

        if (isdigit(c))
        {
            const int numEmptySqrs = c - '0';
            x -= numEmptySqrs;
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
            piecePositions[piece + color * 6 - 1].setBit(x + y * 8, 1);
            board[x + y * 8] = piece + color * 6;
            --x;
        }
    }
}