#include <Game.h>
#include <Engine.h>
#include <Bitboard.h>
#include <iostream>

int main()
{
    /*Game game;
    game.start();*/
    Bitboard a;
    a.setBit(19, 1);
    a.setBit(19 - 8, 1);
    a.printBoard();

    std::cout << std::endl;

    Engine e;
    for (const auto& i : e.kingAttackMasks)
    {
        i.printBoard();
        std::cout << std::endl;
    }


    // 17, 10, 15, 6
}