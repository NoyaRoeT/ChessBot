#pragma once
#include <vector>
#include <string>
#include <Bitboard.h>


class Engine 
{
public:
	Engine();

	const std::vector<int>& getBoard() const;

	Bitboard getOccupiedSquares(int color);
	bool isSquareEmpty(int index);
	void makeMove(const int& origin, const int& target);
	int getPieceColor(int index);

	void loadFen(const std::string& fen);


private:
	std::vector<int> board;
	std::vector<Bitboard> piecePositions;

	static const std::string startingFen;

};