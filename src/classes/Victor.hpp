#pragma once

#include <string>

using namespace geode::prelude;

class Victor : public CCObject {
    public:
    std::string m_nickname;
    std::string m_percent;
    std::string m_video;

    static CCArray* fromResponse(const std::string& data);
};


