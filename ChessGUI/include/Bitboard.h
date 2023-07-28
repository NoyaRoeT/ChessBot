#pragma once
#include <stdint.h>

/*
* Responsbile for managing a 64-bit bitboard.
* Different from board, the index 0 is at the bottom right of the board
*/
class Bitboard
{
public:
	Bitboard();
	Bitboard(int64_t bits);

	// Operator overloads
	Bitboard operator+(const Bitboard& rhs) const;
	Bitboard& operator+=(Bitboard const& rhs);
	Bitboard& operator++();
	Bitboard operator++(int);
	Bitboard operator-(Bitboard const& rhs) const;
	Bitboard& operator-=(Bitboard const& rhs);
	Bitboard& operator--();
	Bitboard operator--(int);
	Bitboard operator*(Bitboard const& rhs) const;
	Bitboard& operator*=(Bitboard const& rhs);
	Bitboard operator/(Bitboard const& rhs) const;
	Bitboard& operator/=(Bitboard const& rhs);
	Bitboard operator<<(size_t pos) const;
	Bitboard& operator<<=(size_t pos);
	Bitboard operator>>(size_t pos) const;
	Bitboard& operator>>=(size_t pos);
	Bitboard operator|(Bitboard const& rhs) const;
	Bitboard& operator|=(Bitboard const& rhs);
	Bitboard operator&(Bitboard const& rhs) const;
	Bitboard& operator&=(Bitboard const& rhs);
	Bitboard operator^(Bitboard const& rhs) const;
	Bitboard& operator^=(Bitboard const& rhs);
	Bitboard operator~() const;

	void setBit(int index, int value);
	void printBoard() const;

private:
	int64_t bitboard;
};