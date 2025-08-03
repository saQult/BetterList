#include <Geode/Geode.hpp>
#include <Geode/modify/CreatorLayer.hpp>
#include <string>
#include <cctype>
#include "../menus/GLLayer.hpp"

using namespace geode::utils::web;
using namespace geode::prelude;

class $modify(CustomCreatorLayer, CreatorLayer) {

    bool init() {
        if (!CreatorLayer::init()) {
            return false;
        }

        auto menu = this->getChildByID("creator-buttons-menu");

        auto sprite = CCSprite::createWithSpriteFrameName("GL_demonlistButton.png"_spr);
        sprite->setScale(0.8);

        auto demonlistButton = CCMenuItemSpriteExtra::create(sprite, this, menu_selector(GLLayer::callback));
        demonlistButton->setPosition({ 450.f, 63.4f });
        demonlistButton->setID("global-list-button");

        menu->addChild(demonlistButton);

        menu->getChildByID("featured-button")->setPositionX(2.9);
        menu->getChildByID("lists-button")->setPositionX(91.5);
        menu->getChildByID("paths-button")->setPositionX(181);
        menu->getChildByID("map-packs-button")->setPositionX(270.5);
        menu->getChildByID("search-button")->setPositionX(360.1);

        auto bottomRightMenu = this->getChildByID("bottom-right-menu");
        bottomRightMenu->getChildByID("treasure-room-button")->setScale(0.5);
        bottomRightMenu->getChildByID("treasure-room-button")->setPositionY(10.25);

        return true;
    }
};