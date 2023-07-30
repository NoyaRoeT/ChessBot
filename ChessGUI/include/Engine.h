#pragma once
#include <vector>
#include <string>
#include <Bitboard.h>


class Engine 
{
public:
	Engine();

	const std::vector<int>& getBoard() const;

	void makeMove(const int& origin, const int& target);
	

	bool isSquareEmpty(int index);
	int getPieceColor(int index);
	void loadFen(const std::string& fen);


	std::vector<Bitboard> kingAttackMasks;
private:
	std::vector<int> board;
	std::vector<Bitboard> piecePositions;

	std::vector<std::vector<Bitboard>> pawnAttackMasks;
	std::vector<Bitboard> knightAttackMasks;

	Bitboard genPawnAttackMask(int color, int index);
	void precomputePawnAttacks();

	Bitboard genKnightAttackMask(int index);
	void precomputeKnightAttacks();

	Bitboard genKingAttackMask(int index);
	void precomputeKingAttacks();

	// Class members
	static const std::string startingFen;
};