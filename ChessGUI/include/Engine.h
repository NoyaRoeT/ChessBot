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

	void makeMove(const int& origin, const int& target);
	
	bool isSquareEmpty(int index);
	int getPieceColor(int index);
	void loadFen(const std::string& fen);

	Bitboard getColorPieceOccupancies(int color);
	Bitboard getOccupiedSquares();

	// Single Piece Move Generation (to highlight possible moves for the player)
	std::vector<Move> getPieceMove(int origin);

private:
	// Board state
	std::vector<int> board;
	std::vector<Bitboard> piecePositions;

	// Attack tables
	std::vector<std::vector<Bitboard>> pawnAttackMasks;
	std::vector<Bitboard> knightAttackMasks;
	std::vector<Bitboard> kingAttackMasks;

	std::vector<std::vector<Bitboard>> rayTable;

	// Precomputation
	Bitboard genPawnAttackMask(int color, int index);
	void precomputePawnAttacks();
	Bitboard genKnightAttackMask(int index);
	void precomputeKnightAttacks();
	Bitboard genKingAttackMask(int index);
	void precomputeKingAttacks();

	Bitboard genRay(int index, int dir);
	void fillRayTable();

	// Single Piece Move Generation
	std::vector<Move> getPawnMove(int origin, int color);

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