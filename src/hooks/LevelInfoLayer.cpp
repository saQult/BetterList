#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/utils/web.hpp>
#include "../Utils.hpp"

using namespace geode::utils::web;
using namespace geode::prelude;

class $modify(LevelInfoLayer) {

    struct Fields {
        EventListener<web::WebTask> m_listener;
    };

    bool init(GJGameLevel * level, bool challenge) {
        if (!LevelInfoLayer::init(level, challenge))
            return false;

        m_fields->m_listener.bind([this](web::WebTask::Event* e) {
            if (web::WebResponse* res = e->getValue()) {
                const auto data = res->string().unwrapOr("");
                if (data == "")
                    return;

                auto position = Utils::extractPlace(data);

                if (position == "")
                    return;

                auto positionText = "#" + position;
                auto label = CCLabelBMFont::create(positionText.c_str(), "goldFont.fnt");

                auto positionY = this->getChildByID("difficulty-sprite")->getPositionY() + 40;
                label->setPosition({ 185, positionY });
                label->setScale(0.6f);
                label->setZOrder(999);
                label->setID("demoin-position-label");

                this->addChild(label);
            }
            });

        auto levelID = level->m_levelID.value();

        auto url = "https://api.demonlist.org/levels/classic?search=" + std::to_string(levelID);
        auto request = web::WebRequest();
        m_fields->m_listener.setFilter(request.get(url));

        return true;
    }
};