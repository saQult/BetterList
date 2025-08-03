#include "GLLayer.hpp"
#include <Geode/binding/CCContentLayer.hpp>
#include <Geode/binding/CCScrollLayerExt.hpp>
#include <Geode/binding/BoomListView.hpp>

// a lof of logic has been taking from demon progression and integrated demonlist >.<
// todo add page stepper
GLLayer* GLLayer::create() {
    auto ret = new GLLayer();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void GLLayer::callback(CCObject*)
{
    auto scene = CCScene::create();
    auto glLayer = GLLayer::create();

    scene->addChild(glLayer);

    CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(0.5f, scene));

    return;
}

GLLayer* GLLayer::scene() {
    auto layer = GLLayer::create();
    auto scene = CCScene::create();
    scene->addChild(layer);
    return layer;
}
bool GLLayer::init() {
    if (!CCLayer::init()) return false;

    auto menu = CCMenu::create();
    auto director = CCDirector::sharedDirector();
    auto size = director->getWinSize();
    auto background = createLayerBG();
    background->setZOrder(-10);
    this->addChild(background);

    m_loadCircle = LoadingCircle::create();
    m_loadCircle->m_parentLayer = this;
    m_loadCircle->show();

    auto leftCornerSprite = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
    leftCornerSprite->setAnchorPoint({ 0, 0 });
    this->addChild(leftCornerSprite);

    auto rightCornerSprite = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
    rightCornerSprite->setAnchorPoint({ 1, 0 });
    rightCornerSprite->setPosition({ size.width, 0 });
    rightCornerSprite->setFlipX(true);
    this->addChild(rightCornerSprite);

    auto backSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    auto backButton = CCMenuItemSpriteExtra::create(backSprite, this, menu_selector(GLLayer::backButtonClicked));
    auto backMenu = CCMenu::create();
    backMenu->addChild(backButton);
    backMenu->setPosition({ 25, size.height - 25 });
    backMenu->setID("back-menu");
    this->addChild(backMenu);
    m_backMenu = backMenu;


    m_pagesMenu = CCMenu::create();
    m_pagesMenu->setPosition({ 0, 0 });
    m_pagesMenu->setID("pages-menu");

    m_left = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png"), this, menu_selector(GLLayer::pageLeftClicked));
    m_left->setPosition(24.f, size.height / 2);
    m_left->setVisible(false);
    m_pagesMenu->addChild(m_left);

    auto rightBtnSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    rightBtnSpr->setFlipX(true);
    m_right = CCMenuItemSpriteExtra::create(rightBtnSpr, this, menu_selector(GLLayer::pageRightClicked));
    m_right->setPosition(size.width - 24.0f, size.height / 2);
    m_right->setVisible(false);
    m_pagesMenu->addChild(m_right);

    auto reloadMenu = CCMenu::create();
    reloadMenu->setPosition({ size.width - 30, size.height - 30 });
    auto reloadBtnSprite = CCSprite::createWithSpriteFrameName("GJ_updateBtn_001.png");
    auto reloadBtn = CCMenuItemSpriteExtra::create(reloadBtnSprite, this, menu_selector(GLLayer::reloadLevels));
    reloadBtn->setPosition({ 0, 0 });
    reloadMenu->addChild(reloadBtn);
    reloadMenu->setID("reload-menu");
    this->addChild(reloadMenu);

    m_list = GJListLayer::create(CustomListView::create(CCArray::create(), BoomListType::Level, 220.0f, 358.0f), "", { 194, 114, 62, 255 }, 358.0f, 220.0f, 0);
    m_list->setZOrder(2);
    m_list->setPosition(size / 2 - m_list->getContentSize() / 2);
    this->addChild(m_list);

    this->addChild(m_pagesMenu);

    this->addChild(menu);

    reloadData(true);

    this->setKeyboardEnabled(true);
    this->setKeypadEnabled(true);

    return true;
}

void GLLayer::keyBackClicked() {
    CCDirector::sharedDirector()->popSceneWithTransition(0.5f, PopTransition::kPopTransitionFade);
}

void GLLayer::backButtonClicked(CCObject* sender) {
    keyBackClicked();
}
void GLLayer::reloadLevels(CCObject* sender) {

    if (m_levelsLoaded) {
        loadLevels(m_page);
    }

    return;
}
void GLLayer::pageRightClicked(CCObject*) {
    m_page += 1;

    loadLevels(m_page);

    return;

}
void GLLayer::pageLeftClicked(CCObject*) {
    m_page -= 1;

    loadLevels(m_page);

    return;
}
void GLLayer::loadLevels(int page) {
    m_loadCircle->show();

    m_right->setVisible(true);
    auto glm = GameLevelManager::sharedState();
    glm->m_levelManagerDelegate = this;
    auto results = std::vector<std::string>(m_IDs.begin() + m_page * 10,
        m_IDs.begin() + std::min(static_cast<int>(m_IDs.size()), (m_page + 1) * 10));
    auto searchObject = GJSearchObject::create(SearchType::Type19, string::join(results, ","));
    auto storedLevels = glm->getStoredOnlineLevels(searchObject->getKey());

    if (storedLevels) {
        loadLevelsFinished(storedLevels, "");
    }
    else
    {
        glm->getOnlineLevels(searchObject);
    }
}
void GLLayer::reloadData(bool isInit) {
    auto url = fmt::format("https://api.demonlist.org/levels/classic?levels_type={}", m_type);
    m_listener.bind([&](web::WebTask::Event* e) {
        if (web::WebResponse* res = e->getValue()) {

            log::info("{}", res->string().unwrapOr(""));
            m_IDs.clear();
            const auto data = res->string().unwrapOr("");
            const std::string key = "\"level_id\": ";
            size_t pos = 0;

            while ((pos = data.find(key, pos)) != std::string::npos) {
                pos += key.length();
                size_t end = data.find_first_of(",}", pos);
                if (end != std::string::npos) {
                    std::string id = data.substr(pos, end - pos);
                    id.erase(remove_if(id.begin(), id.end(), ::isspace), id.end());
                    m_IDs.push_back(id);
                }
            }

            loadLevels(0);
            return;
        }
        });

    auto listReq = web::WebRequest();
    m_listener.setFilter(listReq.get(url));
};
void GLLayer::loadLevelsFinished(CCArray* levels, const char*) {
    if (m_loadingCancelled) { return; }

    auto listSize = m_IDs.size();
    auto maxPage = (listSize % 10 == 0 ? listSize : listSize + (10 - (listSize % 10))) / 10 - 1;
    m_left->setVisible(m_page > 0);
    m_right->setVisible(m_page < maxPage);
    m_pagesMenu->setVisible(true);

    m_levelsLoaded = true;
    m_list->m_listView->setVisible(true);

    m_loadCircle->fadeAndRemove();

    auto director = CCDirector::sharedDirector();
    auto size = director->getWinSize();

    if (m_list->getParent() == this) { this->removeChild(m_list); }

    auto data = Mod::get()->getSavedValue<matjson::Value>("cached-data");

    m_list = GJListLayer::create(CustomListView::create(levels, 
        BoomListType::Level, 220.0f, 358.0f), 
        "Global DemonList", 
        { 194, 114, 62, 255 }, 
        358.0f, 220.0f, 0);
    m_list->setZOrder(2);
    m_list->setPosition(size / 2 - m_list->getContentSize() / 2);
    this->addChild(m_list);
}

void GLLayer::loadLevelsFailed(const char*) {
    if (m_loadingCancelled) { return; }

    m_levelsLoaded = true;

    m_loadCircle->fadeAndRemove();

    FLAlertLayer::create("Oopsie", "Failed to load data from demon list", "OK")->show();

    return;
}