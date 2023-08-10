#pragma once
#include <vector>
#include <string>
#include <Bitboard.h>
#include <Move.h>


class Engine 
{
public:
	Engine();

	const std::vector<int>& getBoard() const;

	bool makeMove(const int& origin, const int& target);
	bool isSquareEmpty(int index);

	void loadFen(const std::string& fen);

	// Single Piece Move Generation (to highlight possible moves for the player)
	std::vector<Move> getPieceMoves(int origin);

private:
	// Board state
	std::vector<int> board;
	std::vector<Bitboard> piecePositions;
	std::vector<bool> castlingRights; // {WQ, WK, BQ, BK}
	Bitboard enPassantTarget;
	int turn;

	// Attack tables
	std::vector<std::vector<Bitboard>> pawnAttackMasks;
	std::vector<Bitboard> knightAttackMasks;
	std::vector<Bitboard> kingAttackMasks;
	std::vector<std::vector<Bitboard>> rayTable;

	// Utility
	Bitboard getOccupancyByColor(int color);
	Bitboard getOccupiedSquares();

	// Precomputation
	Bitboard genPawnAttackMask(int color, int index);
	void precomputePawnAttacks();
	Bitboard genKnightAttackMask(int index);
	void precomputeKnightAttacks();
	Bitboard genKingAttackMask(int index);
	void precomputeKingAttacks();

	Bitboard genRay(int index, int dir);
	void fillRayTable();

	// Piece Move Generation
	void getPawnMoves(Bitboard pawnPositions, int color, const Bitboard& empty, const Bitboard& oppColorPieces, std::vector<Move>& moves);
	void getKnightMoves(Bitboard knightPositions, const Bitboard& sameColorPieces, std::vector<Move>& moves);
	void getKingMoves(Bitboard kingPosition, const Bitboard& sameColorPieces, std::vector<Move>& moves);
	void getBishopMoves(Bitboard bishopPositions, const Bitboard& blockers, const Bitboard& sameColorPieces, std::vector<Move>& moves);
	void getRookMoves(Bitboard rookPositions, const Bitboard& blockers, const Bitboard& sameColorPieces, std::vector<Move>& moves);
	void getQueenMoves(Bitboard queenPosition, const Bitboard& blockers, const Bitboard& sameColorPieces, std::vector<Move>& moves);


	// Preset positions
	static const std::string startingFen;
	
	// Directions
	static const int NORTH = 0;
	static const int NORTH_EAST = 1;
	static const int NORTH_WEST = 2;
	static const int WEST = 3;
	static const int EAST = 4;
	static const int SOUTH_EAST = 5;
	static const int SOUTH_WEST = 6;
	static const int SOUTH = 7;

	// Offsets
	static const int DIAG_TL_BR = 9;
	static const int DIAG_BL_TR = 7;
	static const int VERTICAL = 8;
	static const int HORIZONTAL = 1;
	
};