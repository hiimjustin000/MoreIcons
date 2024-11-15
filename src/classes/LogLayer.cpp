#include "LogCell.hpp"
#include "LogLayer.hpp"

using namespace geode::prelude;

LogLayer* LogLayer::create() {
    auto ret = new LogLayer();
    if (ret->initAnchored(440.0f, 290.0f, "GJ_square04.png")) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool LogLayer::setup() {
    setTitle("More Icons");

    auto background = CCScale9Sprite::create("square02_001.png", { 0, 0, 80, 80 });
    background->setContentSize({ 400.0f, 230.0f });
    background->setPosition({ 220.0f, 135.0f });
    background->setOpacity(127);
    m_mainLayer->addChild(background);

    auto scrollLayer = ScrollLayer::create({ 400.0f, 230.0f });
    scrollLayer->setPosition({ 20.0f, 20.0f });
    scrollLayer->m_contentLayer->setLayout(
        ColumnLayout::create()
            ->setAxisReverse(true)
            ->setAxisAlignment(AxisAlignment::End)
            ->setAutoGrowAxis(230.0f)
            ->setGap(0.0f)
    );
    m_mainLayer->addChild(scrollLayer);

    auto& vec = MoreIcons::LOGS;
    auto size = std::min((int)vec.size(), 100);
    auto dark = Loader::get()->isModLoaded("bitz.darkmode_v4");
    for (int i = 0; i < size; i++) {
        scrollLayer->m_contentLayer->addChild(LogCell::create(vec[i], i, size, dark));
    }

    scrollLayer->m_contentLayer->updateLayout();
    scrollLayer->scrollToTop();

    auto topButtons = CCMenu::create();
    topButtons->setPosition({ 420.0f, 270.0f });
    topButtons->setContentSize({ 100.0f, 30.0f });
    topButtons->setAnchorPoint({ 1.0f, 0.5f });
    topButtons->setLayout(
        RowLayout::create()
            ->setAxisAlignment(AxisAlignment::End)
            ->setAxisReverse(true)
    );
    m_mainLayer->addChild(topButtons);

    topButtons->addChild(CCMenuItemExt::createSpriteExtraWithFrameName("GJ_infoIcon_001.png", 1.0f, [this](auto) {
        MoreIcons::showInfoPopup();
    }));
    topButtons->addChild(CCMenuItemExt::createSpriteExtraWithFrameName("folderIcon_001.png", 0.7f, [this](auto) {
        file::openFolder(Mod::get()->getConfigDir());
    }));
    topButtons->updateLayout();

    return true;
}
