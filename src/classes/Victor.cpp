#include "Victor.hpp"
#include <cctype>

using namespace geode::prelude;


CCArray* Victor::fromResponse(const std::string& data) {
    CCArray* result = CCArray::create();
    size_t pos = 0;

    while ((pos = data.find("{", pos)) != std::string::npos) {
        size_t end = data.find("}", pos);
        if (end == std::string::npos) break;

        std::string entry = data.substr(pos, end - pos + 1);

        auto extract = [](const std::string& source, const std::string& key) -> std::string {
            std::string search = "\"" + key + "\":";
            size_t start = source.find(search);
            if (start == std::string::npos) return "";

            start += search.length();
            while (start < source.size() && std::isspace(source[start])) ++start;

            if (source[start] == '"') {
                size_t end = source.find("\"", start + 1);
                if (end == std::string::npos) return "";
                return source.substr(start + 1, end - start - 1);
            } else {
                size_t end = start;
                while (end < source.size() && (std::isdigit(source[end]) || source[end] == '.')) ++end;
                return source.substr(start, end - start);
            }
        };

        auto v = new Victor();
        v->m_nickname = extract(entry, "username");
        v->m_percent = extract(entry, "percent");
        v->m_video = extract(entry, "video");

        result->addObject(v);
        pos = end + 1;
    }

    return result;
}