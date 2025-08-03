#include <Geode/Geode.hpp>
#include <Geode/modify/LevelCell.hpp>
#include <Geode/utils/web.hpp>
#include "../Utils.hpp"

using namespace geode::utils::web;
using namespace geode::prelude;


class $modify(GLLevelCell, LevelCell) {
    struct Fields {
        EventListener<web::WebTask> m_listener;
    };

    void loadFromLevel(GJGameLevel * level) {
        LevelCell::loadFromLevel(level);
        if (!this->m_mainLayer) return;

        auto difficulty = level->m_difficulty;

        m_fields->m_listener.bind([this](web::WebTask::Event* e) {
            if (web::WebResponse* res = e->getValue()) {
                const auto data = res->string().unwrapOr("");
                if (data == "")
                    return;

                auto position = Utils::extractPlace(data);

                if (position == "")
                    return;

                auto menu = this->m_mainLayer;
                auto positionX = 315.0f;
                auto positionY = Loader::get()->isModLoaded("cvolton.compact_lists") ? 45.0f : 72.0f;

                auto positionText = "Global #" + position;
                auto label = CCLabelBMFont::create(positionText.c_str(), "chatFont.fnt");
                label->setAlignment(CCTextAlignment::kCCTextAlignmentRight);
                label->setPosition({ positionX, positionY });
                label->setScale(0.6f);
                label->setZOrder(999);
                label->setColor({ 255, 255, 255 });
                label->setOpacity(200);
                label->setID("position-label");

                menu->addChild(label);
            }
            });

        auto levelID = level->m_levelID.value();
        log::info("{}", levelID);
        auto url = "https://api.demonlist.org/levels/classic?search=" + std::to_string(levelID);
        auto request = web::WebRequest();
        m_fields->m_listener.setFilter(request.get(url));

        return;
    }
};
