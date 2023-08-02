#include <Game.h>
#include <Engine.h>
#include <Bitboard.h>
#include <iostream>

int main()
{
    /*Game game;
    game.start();*/
    Engine e;


    for (int dir = 0; dir != 8; ++dir)
    {
        for (int i = 0; i != 24; i+= 4)
        {
            e.rayTable[dir][i].printBoard();
            std::cout << std::endl;
        }
    }
}