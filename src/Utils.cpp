#include "Utils.hpp"
#include <cctype>

std::string Utils::extractPlace(const std::string& json) {
    std::string key = "\"place\":";
    size_t pos = json.find(key);
    if (pos == std::string::npos) return "";

    pos += key.length();

    while (pos < json.size() && std::isspace(json[pos]))
        ++pos;

    std::string value;
    while (pos < json.size() && std::isdigit(json[pos]))
        value += json[pos++];

    return value;
}
