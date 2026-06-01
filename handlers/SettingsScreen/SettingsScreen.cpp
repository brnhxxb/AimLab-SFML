#include "SettingsScreen.h"
#include "../Crosshair/Crosshair.h"
#include <cmath>
#include <sstream>

SettingsScreen::SettingsScreen(sf::RenderWindow& window, sf::Font& font, Settings& settings)
    : window(window), font(font), settings(settings)
{
    colorPresets = {
        { sf::Color(0,   230, 180), "Cyan"   },
        { sf::Color(255, 80,  40 ), "Red"    },
        { sf::Color(255, 220, 0  ), "Yellow" },
        { sf::Color(100, 180, 255), "Blue"   },
        { sf::Color(180, 255, 100), "Green"  },
        { sf::Color(255, 255, 255), "White"  },
    };

    // инициализируем позицию ползунка из текущего значения
    sizeSliderX = (settings.crosshairSize - 0.5f) / 1.5f;   // 0..1
}

int SettingsScreen::update(float dt) {
    animTime += dt;
    int ret = action;
    action = -1;
    return ret;
}

void SettingsScreen::handleEvent(const sf::Event& event) {
    // стили: 4 кнопки
    const float btnW = 160.f, btnH = 60.f, btnGap = 20.f;
    const float styleX0 = (float)WINDOW_W / 2.f - 2.f * btnW - 1.5f * btnGap;
    const float styleY  = 200.f;

    // цвета: 6 кнопок
    const float colR = 28.f, colGap = 20.f;
    const float colX0 = (float)WINDOW_W / 2.f - 3.f * (colR*2 + colGap) + colGap;
    const float colY  = 360.f;

    // слайдер размера
    const float sliderX0 = (float)WINDOW_W / 2.f - 200.f;
    const float sliderX1 = (float)WINDOW_W / 2.f + 200.f;
    const float sliderY  = 460.f;

    if (const auto* e = event.getIf<sf::Event::MouseMoved>()) {
        float mx = (float)e->position.x, my = (float)e->position.y;
        hoveredStyle = -1;
        for (int i = 0; i < 4; ++i) {
            float bx = styleX0 + i * (btnW + btnGap);
            if (mx >= bx && mx <= bx + btnW && my >= styleY && my <= styleY + btnH)
                hoveredStyle = i;
        }
        hoveredColor = -1;
        for (int i = 0; i < (int)colorPresets.size(); ++i) {
            float cx = colX0 + i * (colR*2 + colGap);
            float cy = colY;
            float dx = mx - cx, dy = my - cy;
            if (dx*dx + dy*dy <= colR*colR) hoveredColor = i;
        }
        if (dragging) {
            float t = (mx - sliderX0) / (sliderX1 - sliderX0);
            t = std::max(0.f, std::min(1.f, t));
            sizeSliderX = t;
            settings.crosshairSize = 0.5f + t * 1.5f;
        }
    }

    if (const auto* e = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (e->button == sf::Mouse::Button::Left) {
            float mx = (float)e->position.x, my = (float)e->position.y;

            // стиль
            for (int i = 0; i < 4; ++i) {
                float bx = styleX0 + i * (btnW + btnGap);
                if (mx >= bx && mx <= bx + btnW && my >= styleY && my <= styleY + btnH)
                    settings.crosshairStyle = (CrosshairStyle)i;
            }
            // цвет
            for (int i = 0; i < (int)colorPresets.size(); ++i) {
                float cx = colX0 + i * (colR*2 + colGap);
                float dx = mx - cx, dy = my - colY;
                if (dx*dx + dy*dy <= colR*colR)
                    settings.crosshairColor = colorPresets[i].col;
            }
            // слайдер
            if (my >= sliderY - 12 && my <= sliderY + 12 &&
                mx >= sliderX0 && mx <= sliderX1) {
                dragging = true;
            }

            // кнопка назад
            if (mx >= 20.f && mx <= 160.f && my >= (float)WINDOW_H - 80.f &&
                my <= (float)WINDOW_H - 30.f) {
                settings.save();
                action = 0;
            }
        }
    }

    if (const auto* e = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (e->button == sf::Mouse::Button::Left) dragging = false;
    }

    if (const auto* e = event.getIf<sf::Event::KeyPressed>()) {
        if (e->code == sf::Keyboard::Key::Escape) {
            settings.save();
            action = 0;
        }
    }
}

void SettingsScreen::drawBackground() {
    sf::VertexArray bg(sf::PrimitiveType::TriangleStrip, 4);
    bg[0] = sf::Vertex({sf::Vector2f(0.f,            0.f),           COL_BG_TOP});
    bg[1] = sf::Vertex({sf::Vector2f((float)WINDOW_W, 0.f),           COL_BG_TOP});
    bg[2] = sf::Vertex({sf::Vector2f(0.f,            (float)WINDOW_H), COL_BG_BOT});
    bg[3] = sf::Vertex({sf::Vector2f((float)WINDOW_W, (float)WINDOW_H), COL_BG_BOT});
    window.draw(bg);
}

void SettingsScreen::drawStyleButtons() {
    const float btnW = 160.f, btnH = 60.f, btnGap = 20.f;
    const float styleX0 = (float)WINDOW_W / 2.f - 2.f * btnW - 1.5f * btnGap;
    const float styleY  = 200.f;

    const char* names[] = { "CLASSIC", "DOT", "CROSS", "CIRCLE" };

    for (int i = 0; i < 4; ++i) {
        float bx = styleX0 + i * (btnW + btnGap);
        bool  sel = ((int)settings.crosshairStyle == i);
        bool  hov = (hoveredStyle == i);

        sf::RectangleShape btn({btnW, btnH});
        btn.setPosition({bx, styleY});
        btn.setFillColor(sel ? sf::Color(0, 80, 60, 200) : sf::Color(20, 30, 50, 200));
        btn.setOutlineColor(sel ? COL_ACCENT : (hov ? COL_GRAY : sf::Color(60, 70, 100)));
        btn.setOutlineThickness(sel ? 2.5f : 1.f);
        window.draw(btn);

        sf::Text t(font, names[i], 20u);
        t.setFillColor(sel ? COL_ACCENT : COL_WHITE);
        sf::FloatRect r = t.getLocalBounds();
        t.setOrigin({r.size.x/2.f, r.size.y/2.f});
        t.setPosition({bx + btnW/2.f, styleY + btnH/2.f});
        window.draw(t);
    }
}

void SettingsScreen::drawColorPicker() {
    const float colR = 28.f, colGap = 20.f;
    const float colX0 = (float)WINDOW_W / 2.f - 3.f * (colR*2 + colGap) + colGap;
    const float colY  = 360.f;

    for (int i = 0; i < (int)colorPresets.size(); ++i) {
        float cx = colX0 + i * (colR*2 + colGap);
        bool  sel = (settings.crosshairColor.r == colorPresets[i].col.r &&
                     settings.crosshairColor.g == colorPresets[i].col.g);

        sf::CircleShape c(colR);
        c.setOrigin({colR, colR});
        c.setPosition({cx, colY});
        c.setFillColor(colorPresets[i].col);
        if (sel) {
            c.setOutlineThickness(3.f);
            c.setOutlineColor(COL_WHITE);
        }
        window.draw(c);
    }
}

void SettingsScreen::drawSizeSlider() {
    const float sliderX0 = (float)WINDOW_W / 2.f - 200.f;
    const float sliderX1 = (float)WINDOW_W / 2.f + 200.f;
    const float sliderY  = 460.f;

    // дорожка
    sf::RectangleShape track({sliderX1 - sliderX0, 4.f});
    track.setPosition({sliderX0, sliderY - 2.f});
    track.setFillColor(sf::Color(60, 70, 100));
    window.draw(track);

    // заполненная часть
    float filled = sizeSliderX * (sliderX1 - sliderX0);
    sf::RectangleShape fill({filled, 4.f});
    fill.setPosition({sliderX0, sliderY - 2.f});
    fill.setFillColor(COL_ACCENT);
    window.draw(fill);

    // ручка
    float handleX = sliderX0 + sizeSliderX * (sliderX1 - sliderX0);
    sf::CircleShape handle(10.f);
    handle.setOrigin({10.f, 10.f});
    handle.setPosition({handleX, sliderY});
    handle.setFillColor(COL_ACCENT);
    window.draw(handle);

    // подпись
    std::ostringstream ss; ss << "x" << std::fixed;
    ss.precision(1); ss << settings.crosshairSize;
    sf::Text label(font, ss.str(), 18u);
    label.setFillColor(COL_GRAY);
    label.setPosition({handleX + 14.f, sliderY - 10.f});
    window.draw(label);
}

void SettingsScreen::drawPreview() {
    sf::Vector2f previewPos = {(float)WINDOW_W * 0.82f, 380.f};

    sf::RectangleShape frame({200.f, 200.f});
    frame.setOrigin({100.f, 100.f});
    frame.setPosition(previewPos);
    frame.setFillColor(sf::Color(10, 15, 30, 180));
    frame.setOutlineColor(sf::Color(60, 70, 100));
    frame.setOutlineThickness(1.f);
    window.draw(frame);

    Settings tmp = settings;
    Crosshair previewCh;
    previewCh.update(previewPos, 0.f);
    previewCh.draw(window, tmp);

    sf::Text lbl(font, "PREVIEW", 14u);
    lbl.setFillColor(COL_GRAY);
    sf::FloatRect r = lbl.getLocalBounds();
    lbl.setOrigin({r.size.x/2.f, 0.f});
    lbl.setPosition({previewPos.x, previewPos.y + 108.f});
    window.draw(lbl);
}

void SettingsScreen::draw() {
    drawBackground();

    // заголовок
    {
        sf::Text t(font, "НАСТРОЙКИ ПРИЦЕЛА", 48u);
        t.setFillColor(COL_WHITE);
        sf::FloatRect r = t.getLocalBounds();
        t.setOrigin({r.size.x/2.f, r.size.y/2.f});
        t.setPosition({(float)WINDOW_W/2.f, 100.f});
        window.draw(t);
    }

    // подписи секций
    auto sectionLabel = [&](const std::string& s, float x, float y) {
        sf::Text t(font, s, 18u);
        t.setFillColor(COL_GRAY);
        t.setPosition({x, y});
        window.draw(t);
    };

    sectionLabel("Стиль:", (float)WINDOW_W/2.f - 360.f, 170.f);
    sectionLabel("Цвет:", (float)WINDOW_W/2.f - 360.f, 330.f);
    sectionLabel("Размер:", (float)WINDOW_W/2.f - 360.f, 440.f);

    drawStyleButtons();
    drawColorPicker();
    drawSizeSlider();
    drawPreview();

    // кнопка назад
    {
        sf::RectangleShape btn({140.f, 50.f});
        btn.setPosition({20.f, (float)WINDOW_H - 80.f});
        btn.setFillColor(sf::Color(20, 30, 50, 200));
        btn.setOutlineColor(COL_GRAY);
        btn.setOutlineThickness(1.f);
        window.draw(btn);

        sf::Text t(font, "< НАЗАД", 20u);
        t.setFillColor(COL_WHITE);
        t.setPosition({34.f, (float)WINDOW_H - 66.f});
        window.draw(t);
    }

    // подсказка
    {
        sf::Text t(font, "ESC — вернуться, настройки сохраняются автоматически", 16u);
        t.setFillColor(COL_GRAY);
        sf::FloatRect r = t.getLocalBounds();
        t.setOrigin({r.size.x/2.f, 0.f});
        t.setPosition({(float)WINDOW_W/2.f, (float)WINDOW_H - 40.f});
        window.draw(t);
    }
}
