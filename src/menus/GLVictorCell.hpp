#pragma once

using namespace geode::prelude;

class GLVictorCell : public TableViewCell{
    GLVictorCell(const char* name, CCSize size) : TableViewCell(name, size.width, size.height) {
        auto label = CCLabelBMFont::create("123", "bigFont.fnt");
        label->setAnchorPoint({ 0.5f, 0.5f });
        label->setPosition(size.width / 2, size.height / 2);
        label->setScale(0.8f);
        this->m_mainLayer->addChild(label);
    }

    static GLVictorCell* create(const char* key, CCSize size) {
        auto ret = new GLVictorCell(key, size);
        ret->autorelease();
        return ret;
    }
};




