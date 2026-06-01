#include <SFML/Graphics.hpp>
#include <array>

#include "handlers/Constants/Constants.h"
#include "handlers/Menu/Menu.h"
#include "handlers/Game/Game.h"
#include "handlers/Result/ResultScreen.h"
#include "handlers/SettingsScreen/SettingsScreen.h"

int main() {

    sf::RenderWindow window(
        sf::VideoMode({(unsigned)WINDOW_W, (unsigned)WINDOW_H}),
        "Aim Trainer",
        sf::State::Windowed
    );
    window.setFramerateLimit(144);

    sf::Font font;
    if (!font.openFromFile("font.ttf")) return 1;

    // Курсоры
    auto defaultCursorOpt = sf::Cursor::createFromSystem(sf::Cursor::Type::Arrow);
    auto defaultCursor = std::move(defaultCursorOpt.value());

    std::array<std::uint8_t, 4> emptyPixels{0, 0, 0, 0};
    auto emptyCursorOpt = sf::Cursor::createFromPixels(emptyPixels.data(), {1u, 1u}, {0u, 0u});
    auto emptyCursor = std::move(emptyCursorOpt.value());

    window.setMouseCursor(defaultCursor);  // по умолчанию видимый

    Settings settings;
    settings.load();

    enum class State { MENU, GAME, RESULT, SETTINGS };
    State state = State::MENU;

    Menu*           menu    = new Menu(window, font);
    Game*           game    = nullptr;
    ResultScreen*   rs      = nullptr;
    SettingsScreen* settScr = nullptr;
    GameMode        selMode = GameMode::STATIC;
    GameStats       lastStats;

    sf::Clock clock;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        while (auto eventOpt = window.pollEvent()) {
            const sf::Event& event = *eventOpt;

            if (event.is<sf::Event::Closed>())
                window.close();

            if (const auto* e = event.getIf<sf::Event::KeyPressed>()) {
                if (e->code == sf::Keyboard::Key::Escape) {
                    if (state == State::GAME) {
                        delete game; game = nullptr;
                        state = State::MENU;
                        menu = new Menu(window, font);
                        window.setMouseCursor(defaultCursor);
                    } else if (state == State::MENU) {
                        window.close();
                    }
                }
            }

            if (state == State::MENU)     menu->handleEvent(event);
            if (state == State::GAME)     game->handleEvent(event);
            if (state == State::RESULT)   rs->handleEvent(event);
            if (state == State::SETTINGS) settScr->handleEvent(event);
        }

        if (state == State::MENU) {
            int sel = menu->update(dt);
            if (sel == -2) {
                delete menu; menu = nullptr;
                settScr = new SettingsScreen(window, font, settings);
                state   = State::SETTINGS;
                window.setMouseCursor(defaultCursor);
            } else if (sel >= 0) {
                selMode = (GameMode)sel;
                delete menu; menu = nullptr;
                game  = new Game(window, font, selMode, settings,
                                 lastStats.reactCount > 0 ? &lastStats : nullptr);
                state = State::GAME;
                window.setMouseCursor(emptyCursor);
            }
        } else if (state == State::GAME) {
            if (!game->update(dt)) {
                lastStats = game->getStats();
                rs    = new ResultScreen(window, font, lastStats);
                delete game; game = nullptr;
                state = State::RESULT;
                window.setMouseCursor(defaultCursor);
            }
        } else if (state == State::RESULT) {
            int action = rs->update(dt);
            if (action == 0) {
                delete rs; rs = nullptr;
                game  = new Game(window, font, selMode, settings, &lastStats);
                state = State::GAME;
                window.setMouseCursor(emptyCursor);
            } else if (action == 1) {
                delete rs; rs = nullptr;
                menu  = new Menu(window, font);
                state = State::MENU;
                window.setMouseCursor(defaultCursor);
            }
        } else if (state == State::SETTINGS) {
            int act = settScr->update(dt);
            if (act == 0) {
                delete settScr; settScr = nullptr;
                menu  = new Menu(window, font);
                state = State::MENU;
                window.setMouseCursor(defaultCursor);
            }
        }

        window.clear();
        if (state == State::MENU)     menu->draw();
        if (state == State::GAME)     game->draw();
        if (state == State::RESULT)   rs->draw();
        if (state == State::SETTINGS) settScr->draw();
        window.display();
    }

    delete menu;
    delete game;
    delete rs;
    delete settScr;
    return 0;
}