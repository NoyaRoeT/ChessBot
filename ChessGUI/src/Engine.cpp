#include "Engine.h"
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

bool Engine::isSquareEmpty(int index)
{
    return board[index] == 0;
}

int Engine::getTurn()
{
    return turn;
}

bool Engine::makeMove(Move move)
{
    // Fill in move info
    move.originPiece = board[move.originIndex];
    move.targetPiece = board[move.targetIndex];

    // Check if valid move
    std::vector<Move> validMoves = getPieceMoves(move.originIndex);

    bool foundMove = false;
    for (const Move& m : validMoves)
    {
        if (move.targetIndex == m.targetIndex) foundMove = true;
    }
    if (!foundMove) return false;

    // Update board state
    int originPiece = move.originPiece;
    int targetPiece = move.targetPiece;
    board[move.targetIndex] = originPiece;
    board[move.originIndex] = 0;

    if (targetPiece != 0) piecePositions[targetPiece - 1].setBit(move.targetIndex, 0);
    piecePositions[originPiece - 1].setBit(move.originIndex, 0);
    piecePositions[originPiece - 1].setBit(move.targetIndex, 1);

    return true;

}

void Engine::undoMove(const Move& move)
{
    // Doesn't care if move is invalid
    board[move.originIndex] = move.originPiece;
    board[move.targetIndex] = move.targetPiece;

    piecePositions[move.originPiece - 1].setBit(move.originIndex, 1);
    piecePositions[move.originPiece - 1].setBit(move.targetIndex, 0);

    if (move.targetPiece != 0) piecePositions[move.targetPiece - 1].setBit(move.targetIndex, 1);

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
    std::string turnStr;
    std::string castlingStr;
    std::string enPassantStr;;
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

    fenStream >> turnStr;
    if (turnStr == "w") turn = WHITE;
    else turn = BLACK;

    fenStream >> castlingStr;
    castlingRights = { false, false, false, false };
    for (const char& c : castlingStr)
    {
        if (c == '-') break;
        else if (c == 'Q') castlingRights[0] = true;
        else if (c == 'K') castlingRights[1] = true;
        else if (c == 'q') castlingRights[2] = true;
        else if (c == 'k') castlingRights[3] = true;
    }

    fenStream >> enPassantStr;
    if (enPassantStr != "-")
    {
        enPassantTarget = getBitboardFromAlg(enPassantStr);
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

Bitboard Engine::getBitboardFromAlg(const std::string& a)
{
    const char letter = a[0];
    const char number = a[1];

    int x, y = number - 1;

    switch (letter)
    {
    case 'a':
        x = 7;
        break;
    case 'b':
        x = 6;
        break;
    case 'c':
        x = 5;
        break;
    case 'd':
        x = 4;
        break;
    case 'e':
        x = 3;
        break;
    case 'f':
        x = 2;
        break;
    case 'g':
        x = 1;
        break;
    case 'h':
        x = 0;
        break;
    }

    Bitboard res;
    res.setBit(x + y * 8, 1);
    return res;
}

void Engine::precomputePawnAttacks()
{
    pawnAttackMasks = std::vector<std::vector<Bitboard>>(2);

    for (int color = 0; color != 2; ++color)
    for (int idx = 0; idx != 64; ++idx)
    {
        Bitboard attackMask;
        Bitboard pieceOccupancy;

        pieceOccupancy.setBit(idx, 1);

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
        
        pawnAttackMasks[color].push_back(attackMask);
    }
}

void Engine::precomputeKnightAttacks()
{
    knightAttackMasks = std::vector<Bitboard>();

    for (int i = 0; i != 64; ++i)
    {
        Bitboard attackMask;
        Bitboard pieceOccupancy;

        pieceOccupancy.setBit(i, 1);

        attackMask |= pieceOccupancy << 17 & (~Bitboard::hFile); //nnW
        attackMask |= pieceOccupancy << 10 & ~(Bitboard::gFile | Bitboard::hFile); //nwW
        attackMask |= pieceOccupancy << 15 & (~Bitboard::aFile); //nnE
        attackMask |= pieceOccupancy << 6 & ~(Bitboard::aFile | Bitboard::bFile);
        attackMask |= pieceOccupancy >> 15 & (~Bitboard::hFile); //ssW
        attackMask |= pieceOccupancy >> 6 & ~(Bitboard::hFile | Bitboard::gFile); //swW
        attackMask |= pieceOccupancy >> 17 & ~(Bitboard::aFile); //ssE
        attackMask |= pieceOccupancy >> 10 & ~(Bitboard::aFile | Bitboard::bFile); //seE

        knightAttackMasks.push_back(attackMask);
    }
    
}

void Engine::precomputeKingAttacks()
{
    kingAttackMasks = std::vector<Bitboard>();

    for (int i = 0; i != 64; ++i)
    {
        Bitboard attackMask;
        Bitboard pieceOccupancy;
        pieceOccupancy.setBit(i, 1);


        attackMask |= pieceOccupancy << HORIZONTAL & (~Bitboard::hFile) | pieceOccupancy >> HORIZONTAL & (~Bitboard::aFile);
        pieceOccupancy |= attackMask;
        attackMask |= pieceOccupancy << VERTICAL;
        attackMask |= pieceOccupancy >> VERTICAL;
        
        kingAttackMasks.push_back(attackMask);
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

void Engine::makePseudoLegalMove(Move move)
{
    // Update board state
    int originPiece = move.originPiece;
    int targetPiece = move.targetPiece;
    board[move.targetIndex] = originPiece;
    board[move.originIndex] = 0;

    if (targetPiece != 0) piecePositions[targetPiece - 1].setBit(move.targetIndex, 0);
    piecePositions[originPiece - 1].setBit(move.originIndex, 0);
    piecePositions[originPiece - 1].setBit(move.targetIndex, 1);
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

    if (piece == PAWN) getPawnMoves(pos, color, empty, oppColorPieces, moves);
    else if (piece == KNIGHT) getKnightMoves(pos, sameColorPieces, moves);
    else if (piece == KING) getKingMoves(pos, sameColorPieces, moves);
    else if (piece == BISHOP) getBishopMoves(pos, occupied, sameColorPieces, moves);
    else if (piece == ROOK) getRookMoves(pos, occupied, sameColorPieces, moves);
    else if (piece == QUEEN) getQueenMoves(pos, occupied, sameColorPieces, moves);

    for (const Move& m : moves)
    { 
        if (m.originIndex == origin) filtered.push_back(m);
    }

    std::vector<Move> legalMoves;
    filterOutIllegalMoves(color, filtered, legalMoves);

    return legalMoves;
}

void Engine::filterOutIllegalMoves(int color, std::vector<Move>& pseudoLegalMoves, std::vector<Move>& legalMoves)
{
    for (auto& move : pseudoLegalMoves)
    {
        makePseudoLegalMove(move);
        if (!isInCheck(color)) {
            legalMoves.push_back(move);
        }
        undoMove(move);
    }
}

void Engine::getPawnMoves(Bitboard pawnPositions, int color, const Bitboard& empty, const Bitboard& oppColorPieces, std::vector<Move>& moves)
{
    while (pawnPositions != 0)
    {
        const int originIdx = pawnPositions.bitScanForward();
        Bitboard currPawn = pawnPositions.isolateLSB();
        Bitboard targets = genPawnMoveMask(originIdx, color, currPawn, empty, oppColorPieces);

        while (targets != 0)
        {
            const int targetIdx = targets.bitScanForward();
            Move move(originIdx, targetIdx);
            move.originPiece = board[originIdx];
            move.targetPiece = board[targetIdx];
            moves.push_back(move);
            targets = targets.resetLSB();   
        }

        pawnPositions = pawnPositions.resetLSB();
    }
}

void Engine::getKnightMoves(Bitboard knightPositions, const Bitboard& sameColorPieces, std::vector<Move>& moves)
{
    
    while (knightPositions != 0)
    {
        const int originIdx = knightPositions.bitScanForward();
        Bitboard targets = genKnightMoveMask(originIdx, sameColorPieces);

        while (targets != 0)
        {
            const int targetIdx = targets.bitScanForward();
            Move move(originIdx, targetIdx);
            move.originPiece = board[originIdx];
            move.targetPiece = board[targetIdx];
            moves.push_back(move);
            targets = targets.resetLSB();
        }
        knightPositions = knightPositions.resetLSB();
    }
}

void Engine::getKingMoves(Bitboard kingPosition, const Bitboard& sameColorPieces, std::vector<Move>& moves)
{
    const int originIdx = kingPosition.bitScanForward();
    Bitboard targets = genKingMoveMask(kingPosition, sameColorPieces);

    while (targets != 0)
    {
        const int targetIdx = targets.bitScanForward();
        Move move(originIdx, targetIdx);
        move.originPiece = board[originIdx];
        move.targetPiece = board[targetIdx];
        moves.push_back(move);
        targets = targets.resetLSB();
    }
}

void Engine::getBishopMoves(Bitboard bishopPositions, const Bitboard& blockers, const Bitboard& sameColorPieces, std::vector<Move>& moves)
{

    while (bishopPositions != 0)
    {
        const int originIdx = bishopPositions.bitScanForward();
        Bitboard targets = genBishopMoveMask(originIdx, blockers, sameColorPieces);

        while (targets != 0)
        {
            const int targetIdx = targets.bitScanForward();
            Move move(originIdx, targetIdx);
            move.originPiece = board[originIdx];
            move.targetPiece = board[targetIdx];
            moves.push_back(move);
            targets = targets.resetLSB();
        }

        bishopPositions = bishopPositions.resetLSB();
    }
}

void Engine::getRookMoves(Bitboard rookPositions, const Bitboard& blockers, const Bitboard& sameColorPieces, std::vector<Move>& moves)
{
    while (rookPositions != 0)
    {
        const int originIdx = rookPositions.bitScanForward();
        Bitboard targets = genRookMoveMask(originIdx, blockers, sameColorPieces);

        while (targets != 0)
        {
            const int targetIdx = targets.bitScanForward();
            Move move(originIdx, targetIdx);
            move.originPiece = board[originIdx];
            move.targetPiece = board[targetIdx];
            moves.push_back(move);
            targets = targets.resetLSB();
        }

        rookPositions = rookPositions.resetLSB();
    }
}

void Engine::getQueenMoves(Bitboard queenPosition, const Bitboard& blockers, const Bitboard& sameColorPieces, std::vector<Move>& moves)
{
    getBishopMoves(queenPosition, blockers, sameColorPieces, moves);
    getRookMoves(queenPosition, blockers, sameColorPieces, moves);
}

Bitboard Engine::genPawnsMoveMask(int color, Bitboard pawnPositions, const Bitboard& empty, const Bitboard& oppColorPieces)
{
    Bitboard result;
    while (pawnPositions != 0)
    {
        const int originIdx = pawnPositions.bitScanForward();
        Bitboard currPawn = pawnPositions.isolateLSB();
        Bitboard targets = genPawnMoveMask(originIdx, color, currPawn, empty, oppColorPieces);
        result |= targets;

        pawnPositions = pawnPositions.resetLSB();
    }
    return result;
}

Bitboard Engine::genKnightsMoveMask(Bitboard knightPositions, const Bitboard& sameColorPieces)
{
    Bitboard result;
    while (knightPositions != 0)
    {
        const int originIdx = knightPositions.bitScanForward();
        Bitboard targets = genKnightMoveMask(originIdx, sameColorPieces);
        result |= targets;
        
        knightPositions = knightPositions.resetLSB();
    }

    return result;
}

Bitboard Engine::genBishopsMoveMask(Bitboard bishopPositions, const Bitboard& blockers, const Bitboard& sameColorPieces)
{
    Bitboard result;
    while (bishopPositions != 0)
    {
        const int originIdx = bishopPositions.bitScanForward();
        Bitboard targets = genBishopMoveMask(originIdx, blockers, sameColorPieces);

        result |= targets;

        bishopPositions = bishopPositions.resetLSB();
    }
    return result;
}

Bitboard Engine::genRooksMoveMask(Bitboard rookPositions, const Bitboard& blockers, const Bitboard& sameColorPieces)
{
    Bitboard result;
    while (rookPositions != 0)
    {
        const int originIdx = rookPositions.bitScanForward();
        Bitboard targets = genRookMoveMask(originIdx, blockers, sameColorPieces);

        result |= targets;

        rookPositions = rookPositions.resetLSB();
    }

    return result;
}

Bitboard Engine::genPawnMoveMask(int originIdx, int color, Bitboard currPawn, const Bitboard& empty, const Bitboard& oppColorPieces)
{
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

    return targets;
}

Bitboard Engine::genKnightMoveMask(int originIdx, const Bitboard& sameColorPieces)
{
    return knightAttackMasks[originIdx] & ~sameColorPieces;
}

Bitboard Engine::genKingMoveMask(Bitboard kingPosition, const Bitboard& sameColorPieces)
{
    const int originIdx = kingPosition.bitScanForward();
    if (originIdx == -1) return Bitboard();
    const Bitboard& kingAttacks = kingAttackMasks[originIdx];
    Bitboard targets = kingAttacks & ~(sameColorPieces);

    return targets;
}

Bitboard Engine::genBishopMoveMask(int originIdx, const Bitboard& blockers, const Bitboard& sameColorPieces)
{
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

    return targets;
}

Bitboard Engine::genRookMoveMask(int originIdx, const Bitboard& blockers, const Bitboard& sameColorPieces)
{
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

    return targets;
}

Bitboard Engine::genQueenMoveMask(Bitboard queenPosition, const Bitboard& blockers, const Bitboard& sameColorPieces)
{
    const int originIdx = queenPosition.bitScanForward();
    if (originIdx == -1) return Bitboard();
    return genBishopMoveMask(originIdx, blockers, sameColorPieces) | genRookMoveMask(originIdx, blockers, sameColorPieces);
}

Bitboard Engine::genAttackMask(int color)
{
    // Generate attack mask of opposite color (should refactor into a function)
    const int offset = color * 6;
    const Bitboard& occupied = getOccupiedSquares();
    const Bitboard& oppPos = getOccupancyByColor(color);
    const Bitboard& oppKingPos = piecePositions[offset + KING - 1];
    const Bitboard& oppQueenPos = piecePositions[offset + QUEEN - 1];
    const Bitboard& oppBishopPos = piecePositions[offset + BISHOP - 1];
    const Bitboard& oppKnightPos = piecePositions[offset + KNIGHT - 1];
    const Bitboard& oppRookPos = piecePositions[offset + ROOK - 1];
    Bitboard oppPawnPos = piecePositions[offset + PAWN - 1];

    Bitboard attacked;
    attacked |= genKingMoveMask(oppKingPos, oppPos);
    attacked |= genQueenMoveMask(oppQueenPos, occupied, oppPos);
    attacked |= genBishopsMoveMask(oppBishopPos, occupied, oppPos);
    attacked |= genKnightsMoveMask(oppKnightPos, oppPos);
    attacked |= genRooksMoveMask(oppRookPos, occupied, oppPos);

    while (oppPawnPos != 0)
    {
        const int originIdx = oppPawnPos.bitScanForward();
        Bitboard currPawn = oppPawnPos.isolateLSB();
        attacked |= pawnAttackMasks[color][originIdx];
        oppPawnPos = oppPawnPos.resetLSB();
    }
    return attacked;
}

bool Engine::isInCheck(int color)
{
    
    // Generate attack mask of opposite color (should refactor into a function)
    const int oppColor = color == WHITE ? BLACK : WHITE;
    const Bitboard attacked = genAttackMask(oppColor);

    // Check if king is in attacked squares
    const Bitboard& kingPos = piecePositions[color == WHITE ? 0 : 6];


    return (kingPos & attacked) != 0;
}
