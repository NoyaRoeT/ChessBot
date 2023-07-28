#include "Bitboard.h"
#include <bitset>
#include <iostream>

Bitboard::Bitboard() : bitboard(0) {}

Bitboard::Bitboard(int64_t bits) : bitboard(bits)
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

void Bitboard::setBit(int index, int value)
{
	int64_t one = 1;
	if (value == 1) bitboard |= (one << index);
	else bitboard ^= (one << index);
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
