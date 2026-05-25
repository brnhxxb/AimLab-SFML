#include "Menu.h"
#include <cstdint>

Menu::Menu(sf::RenderWindow& window, sf::Font& font)
    : window(window), font(font), hovered(-1), animTime(0.f), selected(-1)
{
    cards.push_back({"STATIC", "Мишени стоят на месте", sf::Color(0,   180, 140)});
    cards.push_back({"MOVING", "Мишени двигаются",      sf::Color(60,  130, 255)});
    cards.push_back({"SHRINK", "Мишени исчезают",       sf::Color(220, 80,  40 )});
}

int Menu::update(float dt) {
    animTime += dt;
    int ret  = selected;
    selected = -1;
    return ret;
}

void Menu::handleEvent(const sf::Event& event) {
    const float cardW  = 240.f, cardH = 160.f;
    const float startX = (float)WINDOW_W / 2.f - 1.5f * cardW - 30.f;
    const float cardY  = (float)WINDOW_H / 2.f - cardH / 2.f + 40.f;

    if (const auto* e = event.getIf<sf::Event::MouseMoved>()) {
        sf::Vector2f m((float)e->position.x, (float)e->position.y);
        hovered = -1;
        for (int i = 0; i < 3; ++i) {
            float cx = startX + i * (cardW + 30.f);
            if (m.x >= cx && m.x <= cx + cardW &&
                m.y >= cardY && m.y <= cardY + cardH)
                hovered = i;
        }
    }

    if (const auto* e = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (e->button == sf::Mouse::Button::Left && hovered >= 0)
            selected = hovered;
    }
}

void Menu::draw() {
    // Фон
    sf::VertexArray bg(sf::PrimitiveType::TriangleStrip, 4);
    bg[0] = sf::Vertex({sf::Vector2f(0.f,          0.f),          COL_BG_TOP});
    bg[1] = sf::Vertex({sf::Vector2f((float)WINDOW_W, 0.f),          COL_BG_TOP});
    bg[2] = sf::Vertex({sf::Vector2f(0.f,          (float)WINDOW_H), COL_BG_BOT});
    bg[3] = sf::Vertex({sf::Vector2f((float)WINDOW_W, (float)WINDOW_H), COL_BG_BOT});
    window.draw(bg);

    // Заголовок цsf::Text(font, string, charSize)
    {
        sf::Text title(font, "AIM TRAINER", 72u);
        title.setFillColor(COL_WHITE);
        sf::FloatRect r = title.getLocalBounds();
        title.setOrigin({r.size.x / 2.f, r.size.y / 2.f});
        title.setPosition({(float)WINDOW_W / 2.f, 160.f});
        window.draw(title);
    }
    {
        sf::Text sub(font, "Выбери режим и нажми", 24u);
        sub.setFillColor(COL_GRAY);
        sf::FloatRect r = sub.getLocalBounds();
        sub.setOrigin({r.size.x / 2.f, r.size.y / 2.f});
        sub.setPosition({(float)WINDOW_W / 2.f, 240.f});
        window.draw(sub);
    }

    const float cardW  = 240.f, cardH = 160.f;
    const float startX = (float)WINDOW_W / 2.f - 1.5f * cardW - 30.f;
    const float cardY  = (float)WINDOW_H / 2.f - cardH / 2.f + 40.f;

    for (int i = 0; i < 3; ++i) {
        float     cx    = startX + i * (cardW + 30.f);
        bool      isHov = (hovered == i);
        float     lift  = isHov ? -8.f : 0.f;
        sf::Color cc    = cards[i].color;

        sf::RectangleShape card({cardW, cardH});
        card.setPosition({cx, cardY + lift});
        card.setFillColor(sf::Color(
            static_cast<uint8_t>(cc.r / 5),
            static_cast<uint8_t>(cc.g / 5),
            static_cast<uint8_t>(cc.b / 5),
            static_cast<uint8_t>(isHov ? 200 : 120)
        ));
        card.setOutlineColor(cc);
        card.setOutlineThickness(isHov ? 3.f : 1.5f);
        window.draw(card);

        {
            sf::Text t1(font, cards[i].title, 32u);
            t1.setFillColor(cc);
            sf::FloatRect r1 = t1.getLocalBounds();
            t1.setOrigin({r1.size.x / 2.f, r1.size.y / 2.f});
            t1.setPosition({cx + cardW / 2.f, cardY + lift + 50.f});
            window.draw(t1);
        }
        {
            sf::Text t2(font, cards[i].desc, 18u);
            t2.setFillColor(COL_GRAY);
            sf::FloatRect r2 = t2.getLocalBounds();
            t2.setOrigin({r2.size.x / 2.f, r2.size.y / 2.f});
            t2.setPosition({cx + cardW / 2.f, cardY + lift + 100.f});
            window.draw(t2);
        }
    }

    {
        sf::Text info(font, "60 секунд | нажми ESC для выхода", 20u);
        info.setFillColor(COL_GRAY);
        sf::FloatRect r = info.getLocalBounds();
        info.setOrigin({r.size.x / 2.f, r.size.y / 2.f});
        info.setPosition({(float)WINDOW_W / 2.f, (float)WINDOW_H - 60.f});
        window.draw(info);
    }
}
