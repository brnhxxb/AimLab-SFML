#include "Constants.h"

#include <fstream>
#include <sstream>

void Settings::save() const {
    std::ofstream f(SETTINGS_PATH);
    if (!f) return;
    f << "crosshairStyle " << (int)crosshairStyle << "\n";
    f << "crosshairR "     << (int)crosshairColor.r << "\n";
    f << "crosshairG "     << (int)crosshairColor.g << "\n";
    f << "crosshairB "     << (int)crosshairColor.b << "\n";
    f << "crosshairSize "  << crosshairSize << "\n";
    f << "showFps "        << (int)showFps << "\n";
}

void Settings::load() {
    std::ifstream f(SETTINGS_PATH);
    if (!f) return;
    std::string key;
    while (f >> key) {
        if      (key == "crosshairStyle") { int v; f >> v; crosshairStyle = (CrosshairStyle)v; }
        else if (key == "crosshairR")     { int v; f >> v; crosshairColor.r = (uint8_t)v; }
        else if (key == "crosshairG")     { int v; f >> v; crosshairColor.g = (uint8_t)v; }
        else if (key == "crosshairB")     { int v; f >> v; crosshairColor.b = (uint8_t)v; }
        else if (key == "crosshairSize")  { f >> crosshairSize; }
        else if (key == "showFps")        { int v; f >> v; showFps = (bool)v; }
    }
}
