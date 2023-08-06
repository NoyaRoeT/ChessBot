#include "Bitboard.h"
#include <bitset>
#include <iostream>

const uint64_t Bitboard::debruijn = 0x03f79d71b4cb0a89;
const int Bitboard::debruijnIndex[64] = {
	0, 47,  1, 56, 48, 27,  2, 60,
   57, 49, 41, 37, 28, 16,  3, 61,
   54, 58, 35, 52, 50, 42, 21, 44,
   38, 32, 29, 23, 17, 11,  4, 62,
   46, 55, 26, 59, 40, 36, 15, 53,
   34, 51, 20, 43, 31, 22, 10, 45,
   25, 39, 14, 33, 19, 30,  9, 24,
   13, 18,  8, 12,  7,  6,  5, 63
};

const Bitboard Bitboard::hFile(0x0101010101010101);
const Bitboard Bitboard::aFile(0x8080808080808080);
const Bitboard Bitboard::gFile(0x0202020202020202);
const Bitboard Bitboard::bFile(0x4040404040404040);

const Bitboard Bitboard::rank4(0x00000000FF000000);
const Bitboard Bitboard::rank5(0x000000FF00000000);

Bitboard::Bitboard() : bitboard(0) {}

Bitboard::Bitboard(uint64_t bits) : bitboard(bits)
{
}

Bitboard Bitboard::operator+(const Bitboard& rhs) const
{
	return Bitboard(this->bitboard + rhs.bitboard);
}

Bitboard& Bitboard::operator+=(Bitboard const& rhs)
{
	this->bitboard += rhs.bitboard;
	return *this;
}

Bitboard& Bitboard::operator++()
{
	++(this->bitboard);
	return *this;
}

Bitboard Bitboard::operator++(int)
{
	Bitboard temp = *this;
	++(this->bitboard);
	return temp;
}

Bitboard Bitboard::operator-(Bitboard const& rhs) const
{
	return Bitboard(this->bitboard - rhs.bitboard);
}

Bitboard& Bitboard::operator-=(Bitboard const& rhs)
{
	this->bitboard -= rhs.bitboard;
	return *this;
}

Bitboard& Bitboard::operator--()
{
	--(this->bitboard);
	return *this;
}

Bitboard Bitboard::operator--(int)
{
	Bitboard temp(this->bitboard);
	--(this->bitboard);
	return temp;
}

Bitboard Bitboard::operator*(Bitboard const& rhs) const
{
	return Bitboard(this->bitboard * rhs.bitboard);
}

Bitboard& Bitboard::operator*=(Bitboard const& rhs)
{
	this->bitboard *= rhs.bitboard;
	return *this;
}

Bitboard Bitboard::operator/(Bitboard const& rhs) const
{
	return Bitboard(this->bitboard / rhs.bitboard);
}

Bitboard& Bitboard::operator/=(Bitboard const& rhs)
{
	this->bitboard /= rhs.bitboard;
	return *this;
}

Bitboard Bitboard::operator<<(size_t pos) const
{
	return Bitboard(this->bitboard << pos);
}

Bitboard& Bitboard::operator<<=(size_t pos)
{
	this->bitboard <<= pos;
	return *this;
}

Bitboard Bitboard::operator>>(size_t pos) const
{
	return Bitboard(this->bitboard >> pos);
}

Bitboard& Bitboard::operator>>=(size_t pos)
{
	this->bitboard >>= pos;
	return *this;
}

Bitboard Bitboard::operator|(Bitboard const& rhs) const
{
	return Bitboard(this->bitboard | rhs.bitboard);
}

Bitboard& Bitboard::operator|=(Bitboard const& rhs)
{
	this->bitboard |= rhs.bitboard;
	return *this;
}

Bitboard Bitboard::operator&(Bitboard const& rhs) const
{
	return Bitboard(this->bitboard & rhs.bitboard);
}

Bitboard& Bitboard::operator&=(Bitboard const& rhs)
{
	this->bitboard &= rhs.bitboard;
	return *this;
}

Bitboard Bitboard::operator^(Bitboard const& rhs) const
{
	return Bitboard(this->bitboard ^ rhs.bitboard);
}

Bitboard& Bitboard::operator^=(Bitboard const& rhs)
{
	this->bitboard ^= rhs.bitboard;
	return *this;
}

Bitboard Bitboard::operator~() const
{
	return Bitboard(~(this->bitboard));
}

bool Bitboard::operator==(Bitboard const& rhs) const
{
	return this->bitboard == rhs.bitboard;
}

bool Bitboard::operator!=(Bitboard const& rhs) const
{
	return this->bitboard != rhs.bitboard;
}

Bitboard::operator bool() const
{
	return bitboard;
}

void Bitboard::setBit(int index, int value)
{
	int64_t one = 1;
	if (value == 1) bitboard |= (one << index);
	else bitboard ^= (one << index);
}

int64_t Bitboard::get() const
{
	return bitboard;
}

void Bitboard::printBoard() const
{
	const std::string s = std::bitset<64>(bitboard).to_string();
	for (int i = 0; i != 8; ++i)
	{
		for (int j = 0; j != 8; ++j)
		{
			std::cout << s[j + i*8];
		}
		std::cout << std::endl;
	}
}

int Bitboard::bitScanForward() const
{
	// Returns index of LSB
	if (bitboard == 0) return -1;
	return Bitboard::debruijnIndex[((bitboard ^ (bitboard - 1)) * Bitboard::debruijn) >> 58];
}

int Bitboard::bitScanReverse() const
{
	if (bitboard == 0) return -1;
	uint64_t bb = bitboard;
	bb |= bb >> 1;
	bb |= bb >> 2;
	bb |= bb >> 4;
	bb |= bb >> 8;
	bb |= bb >> 16;
	bb |= bb >> 32;
	return Bitboard::debruijnIndex[(bb * Bitboard::debruijn) >> 58];
}

Bitboard Bitboard::resetLSB()
{
	return bitboard & (bitboard - 1);
}

Bitboard Bitboard::isolateLSB()
{
	return Bitboard((int64_t)bitboard & (-(int64_t)bitboard));
}
