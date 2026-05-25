#include <SFML/Graphics.hpp>
#include "handlers/Constants/Constants.h"
#include "handlers/Menu/Menu.h"
#include "handlers/Game/Game.h"
#include "handlers/Result/ResultScreen.h"

int main() {
    // SFML 3: sf::VideoMode(width, height), sf::State::Windowed
    sf::RenderWindow window(
        sf::VideoMode({(unsigned)WINDOW_W, (unsigned)WINDOW_H}),
        "Aim Trainer",
        sf::State::Windowed
    );
    window.setMouseCursorVisible(false);
    window.setFramerateLimit(144);

    sf::Font font;
    if (!font.openFromFile("font.ttf")) return 1;  // SFML 3: openFromFile

    enum class State { MENU, GAME, RESULT };
    State state = State::MENU;

    Menu* menu       = new Menu(window, font);
    Game* game       = nullptr;
    ResultScreen* rs = nullptr;
    GameMode selectedMode = GameMode::STATIC;

    sf::Clock clock;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        // SFML 3: pollEvent возвращает std::optional<sf::Event>
        while (auto eventOpt = window.pollEvent()) {
            const sf::Event& event = *eventOpt;

            // Закрытие окна
            if (event.is<sf::Event::Closed>())
                window.close();

            // ESC выход/возврат в меню
            if (const auto* e = event.getIf<sf::Event::KeyPressed>()) {
                if (e->code == sf::Keyboard::Key::Escape) {
                    if (state == State::GAME) {
                        delete game; game = nullptr;
                        state = State::MENU;
                        menu = new Menu(window, font);
                    } else if (state == State::MENU) {
                        window.close();
                    }
                }
            }

            if (state == State::MENU)   menu->handleEvent(event);
            if (state == State::GAME)   game->handleEvent(event);
            if (state == State::RESULT) rs->handleEvent(event);
        }

        if (state == State::MENU) {
            int sel = menu->update(dt);
            if (sel >= 0) {
                selectedMode = (GameMode)sel;
                delete menu; menu = nullptr;
                game  = new Game(window, font, selectedMode);
                state = State::GAME;
            }
        } else if (state == State::GAME) {
            bool playing = game->update(dt);
            if (!playing) {
                rs    = new ResultScreen(window, font, game->getStats());
                delete game; game = nullptr;
                state = State::RESULT;
            }
        } else if (state == State::RESULT) {
            int action = rs->update(dt);
            if (action == 0) {  // играть снова
                delete rs; rs = nullptr;
                game  = new Game(window, font, selectedMode);
                state = State::GAME;
            } else if (action == 1) {   // меню
                delete rs; rs = nullptr;
                menu  = new Menu(window, font);
                state = State::MENU;
            }
        }

        //отрисовка
        window.clear();
        if (state == State::MENU)   menu->draw();
        if (state == State::GAME)   game->draw();
        if (state == State::RESULT) rs->draw();
        window.display();
    }

    delete menu;
    delete game;
    delete rs;
    return 0;
}
