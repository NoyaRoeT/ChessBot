#include "Engine.h"
#include <Piece.h>
#include <sstream>
#include <iostream>
#include <algorithm>

const std::string Engine::startingFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";


Engine::Engine() : board(64, 0)
{
    loadFen(startingFen);

    precomputePawnAttacks();
    precomputeKnightAttacks();
    precomputeKingAttacks();
    fillRayTable();
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

Bitboard Engine::getOccupancyByColor(int color)
{
    int start = 0 + color * 6;
    int end = 6 + color * 6;

    Bitboard result;

    for (int i = start; i != end; ++i)
    {
        result |= piecePositions[i];
    }

    return result;
}

Bitboard Engine::getOccupiedSquares()
{
    Bitboard result;
    for (const auto& i : piecePositions) result |= i;
    return result;
}

Bitboard Engine::genPawnAttackMask(int color, int index)
{
    Bitboard attackMask;
    Bitboard pieceOccupancy;

    pieceOccupancy.setBit(index, 1);

    if (color == WHITE)
    {
        attackMask |= pieceOccupancy << DIAG_TL_BR & (~Bitboard::hFile);
        attackMask |= pieceOccupancy << DIAG_BL_TR & (~Bitboard::aFile);
    }
    else
    {
        attackMask |= pieceOccupancy >> DIAG_BL_TR & (~Bitboard::hFile);
        attackMask |= pieceOccupancy >> DIAG_TL_BR & (~Bitboard::aFile);
    }

    return attackMask;
}

void Engine::precomputePawnAttacks()
{
    pawnAttackMasks = std::vector<std::vector<Bitboard>>(2);

    for (int i = 0; i != 2; ++i)
    for (int idx = 0; idx != 64; ++idx)
    {
        pawnAttackMasks[i].push_back(genPawnAttackMask(i, idx));
    }
}

Bitboard Engine::genKnightAttackMask(int index)
{
    Bitboard attackMask;
    Bitboard pieceOccupancy;

    pieceOccupancy.setBit(index, 1);

    attackMask |= pieceOccupancy << 17 & (~Bitboard::hFile); //nnW
    attackMask |= pieceOccupancy << 10 & ~(Bitboard::gFile | Bitboard::hFile); //nwW
    attackMask |= pieceOccupancy << 15 & (~Bitboard::aFile); //nnE
    attackMask |= pieceOccupancy << 6 & ~(Bitboard::aFile | Bitboard::bFile);
    attackMask |= pieceOccupancy >> 15 & (~Bitboard::hFile); //ssW
    attackMask |= pieceOccupancy >> 6 & ~(Bitboard::hFile | Bitboard::gFile); //swW
    attackMask |= pieceOccupancy >> 17 & ~(Bitboard::aFile); //ssE
    attackMask |= pieceOccupancy >> 10 & ~(Bitboard::aFile| Bitboard::bFile); //seE

    return attackMask;
}

void Engine::precomputeKnightAttacks()
{
    knightAttackMasks = std::vector<Bitboard>();

    for (int i = 0; i != 64; ++i)
    {
        knightAttackMasks.push_back(genKnightAttackMask(i));
    }
    
}

Bitboard Engine::genKingAttackMask(int index)
{
    Bitboard attackMask;
    Bitboard pieceOccupancy;
    pieceOccupancy.setBit(index, 1);

    
    attackMask |= pieceOccupancy << HORIZONTAL & (~Bitboard::hFile) | pieceOccupancy >> HORIZONTAL & (~Bitboard::aFile);
    pieceOccupancy |= attackMask;
    attackMask |= pieceOccupancy << VERTICAL;
    attackMask |= pieceOccupancy >> VERTICAL;

    return attackMask;
}

void Engine::precomputeKingAttacks()
{
    kingAttackMasks = std::vector<Bitboard>();

    for (int i = 0; i != 64; ++i)
    {
        kingAttackMasks.push_back(genKingAttackMask(i));
    }
}

Bitboard Engine::genRay(int dir, int index)
{
    Bitboard rayMask;
    Bitboard target;
    target.setBit(index, 1);

    int numShifts, shiftAmt, x = index % 8, y = index / 8;

    if (dir == NORTH) numShifts = 7 - y;
    else if (dir == SOUTH) numShifts = y;
    else if (dir == EAST) numShifts = x;
    else if (dir == WEST) numShifts = 7 - x;
    else if (dir == NORTH_EAST) numShifts = std::min(x, 7 - y);
    else if (dir == SOUTH_EAST) numShifts = std::min(x, y);
    else if (dir == SOUTH_WEST) numShifts = std::min(7-x, y);
    else numShifts = std::min(7-x, 7-y);

    if (dir == NORTH || dir == SOUTH) shiftAmt = VERTICAL;
    else if (dir == EAST || dir == WEST) shiftAmt = HORIZONTAL;
    else if (dir == NORTH_EAST || dir == SOUTH_WEST) shiftAmt = DIAG_BL_TR;
    else shiftAmt = DIAG_TL_BR;

    for (int i = 0; i != numShifts; ++i)
    {
        if (dir < 4)
        {
            target <<= shiftAmt;
            rayMask |= target;
        }
        else
        {
            target >>= shiftAmt;
            rayMask |= target;
        }
    }

    return rayMask;
}

void Engine::fillRayTable()
{
    rayTable = std::vector<std::vector<Bitboard>>(8, std::vector<Bitboard>(64));

    for (int dir = 0; dir != 8; ++dir)
    {
        for (int i = 0; i != 64; ++i)
        {
            rayTable[dir][i] = genRay(dir, i);
        }
    }

}

std::vector<Move> Engine::getPieceMoves(int origin)
{
    const int originPiece = board[origin];

    if (originPiece == 0) return std::vector<Move>();

    const int color = (originPiece > 6) ? BLACK : WHITE;
    const int piece = originPiece - color * 6;

    const Bitboard& pos = piecePositions[originPiece - 1];
    const Bitboard& occupied = getOccupiedSquares();
    const Bitboard& empty = ~occupied;
    const Bitboard& sameColorPieces = getOccupancyByColor(color);
    const Bitboard& oppColorPieces = getOccupancyByColor(1 - color);

    std::vector<Move> moves;
    std::vector<Move> filtered;

    if (piece == PAWN) getAllPawnMoves(pos, color, empty, oppColorPieces, moves);
    else if (piece == KNIGHT) getAllKnightMoves(pos, sameColorPieces, moves);
    else if (piece == KING) getKingMoves(pos, sameColorPieces, moves);
    else if (piece == BISHOP) getAllBishopMoves(pos, occupied, sameColorPieces, moves);
    else if (piece == ROOK) getAllRookMoves(pos, occupied, sameColorPieces, moves);
    else if (piece == QUEEN) getQueenMoves(pos, occupied, sameColorPieces, moves);

    for (const Move& m : moves)
    {
        if (m.originIndex == origin) filtered.push_back(m);
    }

    return filtered;
}

void Engine::getAllPawnMoves(Bitboard pawnPositions, int color, const Bitboard& empty, const Bitboard& oppColorPieces, std::vector<Move>& moves)
{
    while (pawnPositions != 0)
    {
        const int originIdx = pawnPositions.bitScanForward();
        Bitboard currPawn = pawnPositions.isolateLSB();
        Bitboard targets = pawnAttackMasks[color][originIdx] & oppColorPieces;

        if (color == 0)
        {
            const Bitboard& singlePushTargets = (currPawn << VERTICAL) & empty;
            const Bitboard& doublePushTargets = (singlePushTargets << VERTICAL) & empty & Bitboard::rank4;
            targets |= singlePushTargets | doublePushTargets;
        }
        else
        {
            const Bitboard& singlePushTargets = (currPawn >> VERTICAL) & empty;
            const Bitboard& doublePushTargets = (singlePushTargets >> VERTICAL) & empty & Bitboard::rank5;
            targets |= singlePushTargets | doublePushTargets;
        }


        while (targets != 0)
        {
            const int targetIndex = targets.bitScanForward();
            moves.push_back(Move(originIdx, targetIndex));
            targets = targets.resetLSB();   
        }

        pawnPositions = pawnPositions.resetLSB();
    }
}

void Engine::getAllKnightMoves(Bitboard knightPositions, const Bitboard& sameColorPieces, std::vector<Move>& moves)
{
    
    while (knightPositions != 0)
    {
        const int originIdx = knightPositions.bitScanForward();
        Bitboard targets = knightAttackMasks[originIdx] & ~sameColorPieces;

        while (targets != 0)
        {
            const int targetIndex = targets.bitScanForward();
            moves.push_back(Move(originIdx, targetIndex));
            targets = targets.resetLSB();
        }
        knightPositions = knightPositions.resetLSB();
    }
}

void Engine::getKingMoves(Bitboard kingPosition, const Bitboard& sameColorPieces, std::vector<Move>& moves)
{
    const int originIdx = kingPosition.bitScanForward();
    const Bitboard& kingAttacks = kingAttackMasks[originIdx];

    Bitboard targets = kingAttacks & ~(sameColorPieces);

    while (targets != 0)
    {
        const int targetIdx = targets.bitScanForward();
        moves.push_back(Move(originIdx, targetIdx));
        targets = targets.resetLSB();
    }
}

void Engine::getAllBishopMoves(Bitboard bishopPositions, const Bitboard& blockers, const Bitboard& sameColorPieces, std::vector<Move>& moves)
{

    while (bishopPositions != 0)
    {
        const int originIdx = bishopPositions.bitScanForward();
        Bitboard targets;

        targets |= rayTable[NORTH_EAST][originIdx];
        if (rayTable[NORTH_EAST][originIdx] & blockers)
        {
            const int blockerIdx = (rayTable[NORTH_EAST][originIdx] & blockers).bitScanForward();
            targets &= ~rayTable[NORTH_EAST][blockerIdx];
        }

        targets |= rayTable[NORTH_WEST][originIdx];
        if (rayTable[NORTH_WEST][originIdx] & blockers)
        {
            const int blockerIdx = (rayTable[NORTH_WEST][originIdx] & blockers).bitScanForward();
            targets &= ~rayTable[NORTH_WEST][blockerIdx];
        }

        targets |= rayTable[SOUTH_EAST][originIdx];
        if (rayTable[SOUTH_EAST][originIdx] & blockers)
        {
            const int blockerIdx = (rayTable[SOUTH_EAST][originIdx] & blockers).bitScanReverse();
            targets &= ~rayTable[SOUTH_EAST][blockerIdx];
        }

        targets |= rayTable[SOUTH_WEST][originIdx];
        if (rayTable[SOUTH_WEST][originIdx] & blockers)
        {
            const int blockerIdx = (rayTable[SOUTH_WEST][originIdx] & blockers).bitScanReverse();
            targets &= ~rayTable[SOUTH_WEST][blockerIdx];
        }

        targets &= ~sameColorPieces;

        while (targets != 0)
        {
            const int targetIndex = targets.bitScanForward();
            moves.push_back(Move(originIdx, targetIndex));
            targets = targets.resetLSB();
        }

        bishopPositions = bishopPositions.resetLSB();
    }
}

void Engine::getAllRookMoves(Bitboard rookPositions, const Bitboard& blockers, const Bitboard& sameColorPieces, std::vector<Move>& moves)
{
    while (rookPositions != 0)
    {
        const int originIdx = rookPositions.bitScanForward();
        Bitboard targets;

        targets |= rayTable[NORTH][originIdx];
        if (rayTable[NORTH][originIdx] & blockers)
        {
            const int blockerIdx = (rayTable[NORTH][originIdx] & blockers).bitScanForward();
            targets &= ~rayTable[NORTH][blockerIdx];
        }

        targets |= rayTable[WEST][originIdx];
        if (rayTable[WEST][originIdx] & blockers)
        {
            const int blockerIdx = (rayTable[WEST][originIdx] & blockers).bitScanForward();
            targets &= ~rayTable[WEST][blockerIdx];
        }

        targets |= rayTable[SOUTH][originIdx];
        if (rayTable[SOUTH][originIdx] & blockers)
        {
            const int blockerIdx = (rayTable[SOUTH][originIdx] & blockers).bitScanReverse();
            targets &= ~rayTable[SOUTH][blockerIdx];
        }

        targets |= rayTable[EAST][originIdx];
        if (rayTable[EAST][originIdx] & blockers)
        {
            const int blockerIdx = (rayTable[EAST][originIdx] & blockers).bitScanReverse();
            targets &= ~rayTable[EAST][blockerIdx];
        }

        targets &= ~sameColorPieces;

        while (targets != 0)
        {
            const int targetIndex = targets.bitScanForward();
            moves.push_back(Move(originIdx, targetIndex));
            targets = targets.resetLSB();
        }

        rookPositions = rookPositions.resetLSB();
    }
}

void Engine::getQueenMoves(Bitboard queenPosition, const Bitboard& blockers, const Bitboard& sameColorPieces, std::vector<Move>& moves)
{
    getAllBishopMoves(queenPosition, blockers, sameColorPieces, moves);
    getAllRookMoves(queenPosition, blockers, sameColorPieces, moves);
}
