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


	std::vector<std::vector<Bitboard>> pawnAttackMasks;
private:
	std::vector<int> board;
	std::vector<Bitboard> piecePositions;


	Bitboard genPawnAttackMask(int color, int index);
	void precomputePawnAttacks();

	// Class members
	static const std::string startingFen;
};