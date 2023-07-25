#include <Game.h>

Game::Game() : videoMode(600, 600), window(videoMode, "ChessGUI"), board(600)
{
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);
}

void Game::start()
{
    while (window.isOpen())
    {
        input.clear();
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            input.update(event, window);

        }

        update();
        window.clear();
        render();
        window.display();
    }
}

void Game::update()
{
    board.update(input);
}

void Game::render()
{
    board.render(window, input);
}