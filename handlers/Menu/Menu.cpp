#include "Menu.h"
#include <cstdint>

Menu::Menu(sf::RenderWindow& window, sf::Font& font)
    : window(window), font(font)
{
    cards.push_back({"STATIC",  "Targets stand still", "1 target  |  high focus", sf::Color(0,   180, 140)});
    cards.push_back({"MOVING",  "Targets move",         "2 targets  |  tracking",  sf::Color(60,  130, 255)});
    cards.push_back({"SHRINK",  "Targets shrink",       "3 targets  |  speed",     sf::Color(220, 80,  40 )});
}

int Menu::update(float dt) {
    animTime += dt;
    int ret  = selected;
    selected = -1;
    return ret;
}

void Menu::handleEvent(const sf::Event& event) {
    const float cardW  = 240.f, cardH = 180.f, gap = 30.f;
    const float startX = (float)WINDOW_W / 2.f - 1.5f * cardW - gap;
    const float cardY  = (float)WINDOW_H / 2.f - cardH / 2.f + 30.f;

    //кнопка настроек
    const float settBtnX = (float)WINDOW_W - 180.f;
    const float settBtnY = 20.f;

    if (const auto* e = event.getIf<sf::Event::MouseMoved>()) {
        sf::Vector2f m((float)e->position.x, (float)e->position.y);
        hovered = -1;
        for (int i = 0; i < 3; ++i) {
            float cx = startX + i * (cardW + gap);
            if (m.x >= cx && m.x <= cx + cardW && m.y >= cardY && m.y <= cardY + cardH)
                hovered = i;
        }
        //кнопка настроек: hovered = 10
        if (m.x >= settBtnX && m.x <= settBtnX + 150.f &&
            m.y >= settBtnY && m.y <= settBtnY + 44.f)
            hovered = 10;
    }

    if (const auto* e = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (e->button == sf::Mouse::Button::Left) {
            if (hovered >= 0 && hovered < 3) selected = hovered;
            if (hovered == 10)               selected = -2;
        }
    }
}

void Menu::draw() {
    //фон
    sf::VertexArray bg(sf::PrimitiveType::TriangleStrip, 4);
    bg[0] = sf::Vertex({sf::Vector2f(0.f,            0.f),           COL_BG_TOP});
    bg[1] = sf::Vertex({sf::Vector2f((float)WINDOW_W, 0.f),           COL_BG_TOP});
    bg[2] = sf::Vertex({sf::Vector2f(0.f,            (float)WINDOW_H), COL_BG_BOT});
    bg[3] = sf::Vertex({sf::Vector2f((float)WINDOW_W, (float)WINDOW_H), COL_BG_BOT});
    window.draw(bg);

    //сетка
    float horizon = (float)WINDOW_H * 0.55f;
    sf::Color gridCol(30, 50, 90, 40);
    for (int i = 0; i <= 10; ++i) {
        float tx = (float)WINDOW_W * i / 10.f;
        float hx = (float)WINDOW_W / 2.f + (tx - (float)WINDOW_W / 2.f) * 0.3f;
        sf::VertexArray line(sf::PrimitiveType::Lines, 2);
        line[0] = sf::Vertex({sf::Vector2f(tx, (float)WINDOW_H), gridCol});
        line[1] = sf::Vertex({sf::Vector2f(hx, horizon), gridCol});
        window.draw(line);
    }

    //заголовок
    {
        sf::Text t(font, "AIM TRAINER", 72u);
        t.setFillColor(COL_WHITE);
        sf::FloatRect r = t.getLocalBounds();
        t.setOrigin({r.size.x/2.f, r.size.y/2.f});
        t.setPosition({(float)WINDOW_W/2.f, 150.f});
        window.draw(t);
    }
    {
        sf::Text t(font, "Select mode", 24u);
        t.setFillColor(COL_GRAY);
        sf::FloatRect r = t.getLocalBounds();
        t.setOrigin({r.size.x/2.f, r.size.y/2.f});
        t.setPosition({(float)WINDOW_W/2.f, 220.f});
        window.draw(t);
    }

    // карточки режимов
    const float cardW  = 240.f, cardH = 180.f, gap = 30.f;
    const float startX = (float)WINDOW_W / 2.f - 1.5f * cardW - gap;
    const float cardY  = (float)WINDOW_H / 2.f - cardH / 2.f + 30.f;

    for (int i = 0; i < 3; ++i) {
        float cx   = startX + i * (cardW + gap);
        bool  hov  = (hovered == i);
        float lift = hov ? -10.f : 0.f;
        sf::Color cc = cards[i].color;

        sf::RectangleShape card({cardW, cardH});
        card.setPosition({cx, cardY + lift});
        card.setFillColor(sf::Color(
            (uint8_t)(cc.r/6), (uint8_t)(cc.g/6), (uint8_t)(cc.b/6), 220));
        card.setOutlineColor(cc);
        card.setOutlineThickness(hov ? 3.f : 1.5f);
        window.draw(card);

        auto centered = [&](const std::string& s, float y, unsigned sz, sf::Color col) {
            sf::Text t(font, s, sz);
            t.setFillColor(col);
            sf::FloatRect r = t.getLocalBounds();
            t.setOrigin({r.size.x/2.f, r.size.y/2.f});
            t.setPosition({cx + cardW/2.f, cardY + lift + y});
            window.draw(t);
        };

        centered(cards[i].title,  50.f, 32u, cc);
        centered(cards[i].desc,   95.f, 18u, COL_WHITE);
        centered(cards[i].detail, 130.f, 14u, COL_GRAY);
    }

    //кнопка настроек
    {
        const float bx = (float)WINDOW_W - 180.f, by = 20.f;
        bool hov = (hovered == 10);
        sf::RectangleShape btn({150.f, 44.f});
        btn.setPosition({bx, by});
        btn.setFillColor(sf::Color(20, 30, 50, 200));
        btn.setOutlineColor(hov ? COL_ACCENT : COL_GRAY);
        btn.setOutlineThickness(hov ? 2.f : 1.f);
        window.draw(btn);
        sf::Text t(font, "SETTNGS", 18u);
        t.setFillColor(hov ? COL_ACCENT : COL_GRAY);
        sf::FloatRect r = t.getLocalBounds();
        t.setOrigin({r.size.x/2.f, r.size.y/2.f});
        t.setPosition({bx + 75.f, by + 22.f});
        window.draw(t);
    }

    //подсказка
    {
        sf::Text t(font, "60 s  |  ESC — exit", 18u);
        t.setFillColor(COL_GRAY);
        sf::FloatRect r = t.getLocalBounds();
        t.setOrigin({r.size.x/2.f, r.size.y/2.f});
        t.setPosition({(float)WINDOW_W/2.f, (float)WINDOW_H - 50.f});
        window.draw(t);
    }
}
