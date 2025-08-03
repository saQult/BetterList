#include <Geode/Geode.hpp>
#include <Geode/modify/LevelLeaderboard.hpp>
#include "./menus/GLListView.hpp"

using namespace geode::utils::web;
using namespace geode::prelude;

class $modify(CustomLevelLeaderboard, LevelLeaderboard) {

    void showVictors(CCObject*) {

    }

    bool init(GJGameLevel * level, LevelLeaderboardType type, LevelLeaderboardMode mode) {
        if (!LevelLeaderboard::init(level, type, mode)) return false;

        auto scene = static_cast<CCNode*>(this->getChildren()->objectAtIndex(0));

        auto menu = scene->getChildByID("type-menu");

        auto sprite = CCSprite::createWithSpriteFrameName("GL_victorsButton.png"_spr);

        auto victorsButton = CCMenuItemSpriteExtra::create(sprite, this, menu_selector(CustomLevelLeaderboard::showVictors));
        victorsButton->setPosition({ 17.0f, 47.25f });
        victorsButton->setID("victors-button");

        menu->addChild(victorsButton);

        return true;
    }
};