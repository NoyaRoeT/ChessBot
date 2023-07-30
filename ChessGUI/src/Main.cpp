#include <Game.h>
#include <Engine.h>
#include <Bitboard.h>
#include <iostream>

int main()
{
    /*Game game;
    game.start();*/
    Engine e;

    for (const auto& color : e.pawnAttackMasks)
    {
        for (const auto& mask : color)
        {
            mask.printBoard();
            std::cout << std::endl;
        }
    }
}