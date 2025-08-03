#pragma once

#include <Geode/Geode.hpp>
#include <Geode/binding/FLAlertLayerProtocol.hpp>
#include <Geode/binding/GJListLayer.hpp>

using namespace geode::prelude;

class GLLayer : public CCLayer, LevelManagerDelegate {
protected:
    EventListener<web::WebTask> m_listener;
    CCMenu* m_buttonMenu;
    CCMenu* m_backMenu;
    CCMenu* m_pagesMenu;
    GJListLayer* m_list;
    CCLabelBMFont* m_pageLabel;
    CCMenuItemSpriteExtra* m_left;
    CCMenuItemSpriteExtra* m_right;
    CCMenuItemSpriteExtra* m_pageButton;
    LoadingCircle* m_loadCircle;
    std::vector<std::string> m_IDs;
    bool m_levelsLoaded = true;
    int m_page = 0;
    std::string m_type = "all";
    bool m_loadingCancelled = false;

    bool init() override;
    void keyBackClicked() override;
    void backButtonClicked(CCObject* sender);

public:
    static GLLayer* create();
    static GLLayer* scene();
    void callback(CCObject*);

    void reloadData(bool isInit);
    void loadLevels(int page);
    void loadLevelsFinished(CCArray*, const char*) override;
    void loadLevelsFailed(const char*) override;
    void loadLevelsFinished(CCArray* levels, const char* key, int) override {
        loadLevelsFinished(levels, key);
    }
    void loadLevelsFailed(const char* key, int) override {
        loadLevelsFailed(key);
    }

    void reloadLevels(CCObject*);
    void pageRightClicked(CCObject*);
    void pageLeftClicked(CCObject*);
};