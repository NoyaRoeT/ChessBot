#include <Game.h>

Game::Game() : videoMode(600, 600), window(videoMode, "ChessGUI"), board(600)
{
    window.setFramerateLimit(60);
}

void Game::start()
{
    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        update();
        window.clear();
        render();
        window.display();
    }
}

void Game::update()
{
    board.update();
}

void Game::render()
{
    board.render(window);
}