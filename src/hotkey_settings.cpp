#include "hotkey_settings.h"

#include <algorithm>
#include <sstream>
#include <set>

HotkeySettings::HotkeySettings(bool win, bool ctrl, bool alt, bool shift, int code) {
    
    this->win = win;
    this->ctrl = ctrl;
    this->alt = alt;
    this->shift = shift;
    this->code = code;
}

HotkeySettings::HotkeySettings(std::string hotkeyString) {

    hotkeyString.erase(std::remove(hotkeyString.begin(), hotkeyString.end(), ' '), hotkeyString.end());
    
    std::stringstream hotkeyTokenStream(hotkeyString);
    std::string token;
    std::set<std::string> keys;
    while(std::getline(hotkeyTokenStream, token, '+')) {
        keys.insert(token);
    }

    if (keys.find("Alt") != keys.end()) {
        alt = true;
        keys.erase("Alt");
    }

    if (keys.find("Shift") != keys.end()) {
        shift = true;
        keys.erase("Shift");
    }

    if (keys.find("Win") != keys.end()) {
        win = true;
        keys.erase("Win");
    }

    if (keys.find("Ctrl") != keys.end()) {
        ctrl = true;
        keys.erase("Ctrl");
    }

    if (keys.size() > 0) {
        std::string charKey = *keys.begin();
        
        if (charKey == "SPACE") {
            code = VK_SPACE;

        } else {
            // char to virtual key code 
            code = (int)*charKey.c_str();
        }
    }
}

bool HotkeySettings::isValid() {
    return (alt || ctrl || win || shift) && code != 0;
}

std::string HotkeySettings::toString() {
    std::string string = "";
    if (win) {
        string += "Win + ";
    }
    if (ctrl) {
        string += "Ctrl + ";
    }
    if (alt) {
        string += "Alt + ";
    }
    if (shift) {
        string += "Shift + ";
    }

    if (code > 0) {

        if (code == VK_SPACE) {
            string += "SPACE";

        } else {
            UINT charInt = MapVirtualKeyA(code, MAPVK_VK_TO_CHAR);
            string += std::string((char*)&charInt);
        }
    }
    return string;
}

UINT HotkeySettings::getFSModifier() {
    UINT fsModifiers = 0;

    if (win) {
        fsModifiers |= MOD_WIN;
    }

    if (ctrl) {
        fsModifiers |= MOD_CONTROL;
    }

    if (alt) {
        fsModifiers |= MOD_ALT;
    }

    if (shift) {
        fsModifiers |= MOD_SHIFT;
    }
    return fsModifiers;
}

HotkeySettings HotkeySettings::clone() {

    return HotkeySettings(win, ctrl, alt, shift, code);
}