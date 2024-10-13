#include "../MoreIcons.hpp"
#include "../classes/LogLayer.hpp"

using namespace geode::prelude;

#include <Geode/modify/GJGarageLayer.hpp>
class $modify(MIGarageLayer, GJGarageLayer) {
    struct Fields {
        ListButtonBar* m_pageBar;
        CCMenu* m_navMenu;
        std::map<IconType, int> m_pages;
        int m_page;
        bool m_custom;
        CCObject* m_originalSDISwitchTarget;
        SEL_MenuHandler m_originalSDISwitch;
        CCObject* m_originalSDISwapTarget;
        SEL_MenuHandler m_originalSDISwap;
    };

    static void onModify(auto& self) {
        (void)self.setHookPriority("GJGarageLayer::init", -1);
        (void)self.setHookPriority("GJGarageLayer::setupPage", 1);
    }

    bool init() {
        if (!GJGarageLayer::init()) return false;

        auto iconType = GameManager::get()->m_playerIconType;
        MoreIcons::changeSimplePlayer(m_playerObject, Mod::get()->getSavedValue<std::string>(MoreIcons::savedForType(iconType, false), ""), iconType);
        auto sdi = Loader::get()->getLoadedMod("weebify.separate_dual_icons");
        if (sdi) {
            auto lastmode = (IconType)sdi->getSavedValue("lastmode", 0);
            MoreIcons::changeSimplePlayer(static_cast<SimplePlayer*>(getChildByID("player2-icon")),
                Mod::get()->getSavedValue<std::string>(MoreIcons::savedForType(lastmode, true), ""), lastmode);
        }

        auto customIcon = Mod::get()->getSavedValue<std::string>("icon", "");
        if (!customIcon.empty() && MoreIcons::hasIcon(customIcon)) setupCustomPage(MoreIcons::findIconPage(IconType::Cube));
        else createNavMenu();

        if (sdi) {
            auto f = m_fields.self();
            auto p1Button = static_cast<CCMenuItemSpriteExtra*>(getChildByID("player-buttons-menu")->getChildByID("player1-button"));
            auto p2Button = static_cast<CCMenuItemSpriteExtra*>(getChildByID("player-buttons-menu")->getChildByID("player2-button"));
            f->m_originalSDISwitchTarget = p1Button->m_pListener;
            f->m_originalSDISwitch = p2Button->m_pfnSelector;
            p1Button->setTarget(this, menu_selector(MIGarageLayer::newOn2PToggle));
            p2Button->setTarget(this, menu_selector(MIGarageLayer::newOn2PToggle));

            auto swap2PButton = static_cast<CCMenuItemSpriteExtra*>(getChildByID("shards-menu")->getChildByID("swap-2p-button"));
            f->m_originalSDISwapTarget = swap2PButton->m_pListener;
            f->m_originalSDISwap = swap2PButton->m_pfnSelector;
            swap2PButton->setTarget(this, menu_selector(MIGarageLayer::newSwap2PKit));
        }

        auto moreIconsSprite = CircleButtonSprite::createWithSprite("MI_moreIcons_001.png"_spr, 1.0f, CircleBaseColor::Gray, CircleBaseSize::Small);
        if (!MoreIcons::LOGS.empty()) {
            auto severityFrame = "";
            switch (MoreIcons::HIGHEST_SEVERITY) {
                case LogType::Info: severityFrame = "GJ_infoIcon_001.png"; break;
                case LogType::Warn: severityFrame = "geode.loader/info-warning.png"; break;
                case LogType::Error: severityFrame = "geode.loader/info-alert.png"; break;
            }
            auto severitySprite = CCSprite::createWithSpriteFrameName(severityFrame);
            severitySprite->setPosition(moreIconsSprite->getContentSize() - CCPoint { 6.0f, 6.0f });
            severitySprite->setScale(0.6f);
            moreIconsSprite->addChild(severitySprite, 1);
        }
        auto moreIconsButton = CCMenuItemExt::createSpriteExtra(moreIconsSprite, [this](auto) {
            if (!MoreIcons::LOGS.empty()) LogLayer::create()->show();
            else MoreIcons::showInfoPopup(true);
        });
        moreIconsButton->setID("more-icons-button"_spr);
        if (auto shardsMenu = getChildByID("shards-menu")) {
            shardsMenu->addChild(moreIconsButton);
            shardsMenu->updateLayout();
        }

        return true;
    }

    void onSelect(CCObject* sender) {
        GJGarageLayer::onSelect(sender);

        auto sdi = Loader::get()->getLoadedMod("weebify.separate_dual_icons");
        auto savedType = MoreIcons::savedForType(sdi && sdi->getSavedValue("2pselected", false) ? (IconType)sdi->getSavedValue("lasttype", 0) : m_selectedIconType);
        if (!savedType.empty()) Mod::get()->setSavedValue<std::string>(savedType, "");
    }

    void newOn2PToggle(CCObject* sender) {
        auto f = m_fields.self();
        (f->m_originalSDISwitchTarget->*f->m_originalSDISwitch)(sender);
        if (f->m_custom) setupCustomPage(f->m_page);
        else createNavMenu();
    }

    void newSwap2PKit(CCObject* sender) {
        auto f = m_fields.self();
        (f->m_originalSDISwapTarget->*f->m_originalSDISwap)(sender);
        MoreIcons::swapDual("icon");
        MoreIcons::swapDual("ship");
        MoreIcons::swapDual("ball");
        MoreIcons::swapDual("ufo");
        MoreIcons::swapDual("wave");
        MoreIcons::swapDual("robot");
        MoreIcons::swapDual("spider");
        MoreIcons::swapDual("swing");
        MoreIcons::swapDual("jetpack");
        MoreIcons::swapDual("trail");
        auto iconType = GameManager::get()->m_playerIconType;
        MoreIcons::changeSimplePlayer(m_playerObject, Mod::get()->getSavedValue<std::string>(MoreIcons::savedForType(iconType, false), ""), iconType);
        auto lastmode = (IconType)Loader::get()->getLoadedMod("weebify.separate_dual_icons")->getSavedValue("lastmode", 0);
        MoreIcons::changeSimplePlayer(static_cast<SimplePlayer*>(getChildByID("player2-icon")),
            Mod::get()->getSavedValue<std::string>(MoreIcons::savedForType(lastmode, true), ""), lastmode);
        if (f->m_custom) setupCustomPage(f->m_page);
        else createNavMenu();
    }

    void updatePlayerColors() {
        GJGarageLayer::updatePlayerColors();

        if (m_iconSelection && m_fields->m_custom && !MoreIcons::vectorForType(m_iconType).empty()) m_iconSelection->setVisible(false);
    }

    void createNavMenu() {
        auto f = m_fields.self();
        auto winSize = CCDirector::get()->getWinSize();
        if (!f->m_navMenu) {
            f->m_navMenu = CCMenu::create();
            f->m_navMenu->setPosition(winSize.width / 2, 15.0f);
            f->m_navMenu->setLayout(RowLayout::create()->setGap(6.0f)->setAxisAlignment(AxisAlignment::Center));
            f->m_navMenu->setContentSize({ winSize.width - 60.0f, 20.0f });
            f->m_navMenu->setID("navdot-menu"_spr);
            addChild(f->m_navMenu, 1);
        }

        auto& vec = MoreIcons::vectorForType(m_iconType);
        m_navDotMenu->setPositionY(vec.empty() ? 25.0f : 35.0f);
        if (GameManager::get()->countForType(m_iconType) <= 36) {
            m_navDotMenu->setVisible(true);
            m_navDotMenu->setEnabled(true);
            m_navDotMenu->removeAllChildren();
            auto firstDot = static_cast<CCMenuItemSpriteExtra*>(m_pageButtons->objectAtIndex(0));
            static_cast<CCSprite*>(firstDot->getNormalImage())->setDisplayFrame(CCSpriteFrameCache::get()->spriteFrameByName(
                f->m_custom && !vec.empty() ? "gj_navDotBtn_off_001.png" : "gj_navDotBtn_on_001.png"
            ));
            m_navDotMenu->addChild(firstDot);
            m_navDotMenu->updateLayout();
        }
        f->m_navMenu->setVisible(!vec.empty());
        if (vec.empty()) return;

        f->m_navMenu->removeAllChildren();
        auto navDotAmount = (vec.size() + 35) / 36;
        for (int i = 0; i < navDotAmount; i++) {
            auto dot = CCMenuItemExt::createSpriteExtraWithFrameName(
                f->m_custom && i == f->m_page ? "gj_navDotBtn_on_001.png" : "gj_navDotBtn_off_001.png",
                0.9f,
                [this, i](auto) { setupCustomPage(i); }
            );
            dot->setSizeMult(1.1f);
            f->m_navMenu->addChild(dot);
        }

        f->m_navMenu->updateLayout();
    }

    void setupPage(int page, IconType type) {
        GJGarageLayer::setupPage(page, type);

        auto f = m_fields.self();
        if (f->m_pageBar) {
            f->m_pageBar->removeFromParent();
            f->m_pageBar = nullptr;
        }
    }

    void onNavigate(CCObject* sender) {
        GJGarageLayer::onNavigate(sender);

        m_fields->m_custom = false;

        createNavMenu();
    }

    void onArrow(CCObject* sender) {
        GJGarageLayer::onArrow(sender);

        auto f = m_fields.self();
        if (f->m_custom) setupCustomPage(f->m_page + sender->getTag());
        else createNavMenu();
    }

    void onSelectTab(CCObject* sender) {
        GJGarageLayer::onSelectTab(sender);

        auto f = m_fields.self();
        if (f->m_custom) setupCustomPage(f->m_pages.contains(m_iconType) ? f->m_pages[m_iconType] : MoreIcons::findIconPage(m_iconType));
        else createNavMenu();
    }

    void setupCustomPage(int page) {
        if (m_iconType == IconType::Special) {
            setupCustomSpecialPage(page);
            return;
        }

        auto& vec = MoreIcons::vectorForType(m_iconType);
        auto f = m_fields.self();
        if (f->m_pageBar) {
            f->m_pageBar->removeFromParent();
            f->m_pageBar = nullptr;
        }
        if (vec.empty()) {
            createNavMenu();
            return;
        }

        m_iconSelection->setVisible(false);

        f->m_custom = true;
        f->m_page = MoreIcons::wrapPage(m_iconType, page);
        f->m_pages[m_iconType] = f->m_page;
        createNavMenu();

        auto iconType = GameManager::get()->m_playerIconType;
        MoreIcons::changeSimplePlayer(m_playerObject, Mod::get()->getSavedValue<std::string>(MoreIcons::savedForType(iconType, false), ""), iconType);
        auto sdi = Loader::get()->getLoadedMod("weebify.separate_dual_icons");
        if (sdi) {
            auto lastmode = (IconType)sdi->getSavedValue("lastmode", 0);
            MoreIcons::changeSimplePlayer(static_cast<SimplePlayer*>(getChildByID("player2-icon")),
                Mod::get()->getSavedValue<std::string>(MoreIcons::savedForType(lastmode, true), ""), lastmode);
        }

        m_leftArrow->setVisible(vec.size() > 36);
        m_rightArrow->setVisible(vec.size() > 36);

        auto spriteFrameCache = CCSpriteFrameCache::get();
        for (auto navDot : CCArrayExt<CCMenuItemSpriteExtra*>(m_navDotMenu->getChildren())) {
            static_cast<CCSprite*>(navDot->getNormalImage())->setDisplayFrame(spriteFrameCache->spriteFrameByName("gj_navDotBtn_off_001.png"));
        }

        auto winSize = CCDirector::get()->getWinSize();
        auto gameManager = GameManager::get();
        auto unlockType = gameManager->iconTypeToUnlockType(m_iconType);
        auto playerSquare = CCSprite::createWithSpriteFrameName("playerSquare_001.png");
        auto objs = CCArray::create();
        CCMenuItemSpriteExtra* current = nullptr;
        auto savedType = MoreIcons::savedForType(m_iconType);
        auto dual = sdi && sdi->getSavedValue("2pselected", false);
        int i = 1;
        auto hasAnimProf = Loader::get()->isModLoaded("thesillydoggo.animatedprofiles");
        for (auto name : MoreIcons::getPage(m_iconType, f->m_page)) {
            auto itemIcon = GJItemIcon::createBrowserItem(unlockType, 1);
            itemIcon->setScale(GJItemIcon::scaleForType(unlockType));
            MoreIcons::changeSimplePlayer(itemIcon->m_player, name, m_iconType);
            if (hasAnimProf) {
                if (auto robotSprite = itemIcon->m_player->m_robotSprite) robotSprite->runAnimation("idle01");
                if (auto spiderSprite = itemIcon->m_player->m_spiderSprite) spiderSprite->runAnimation("idle01");
            }
            auto iconButton = CCMenuItemExt::createSpriteExtra(itemIcon, [this, f, name, sdi, dual, savedType, gameManager, unlockType](CCMenuItemSpriteExtra* sender) {
                m_cursor1->setPosition(sender->getParent()->convertToWorldSpace(sender->getPosition()));
                m_cursor1->setVisible(true);
                auto player = dual ? static_cast<SimplePlayer*>(getChildByID("player2-icon")) : m_playerObject;
                player->updatePlayerFrame(1, m_iconType);
                player->updateColors();
                MoreIcons::changeSimplePlayer(player, name, m_iconType);
                if (!dual) gameManager->m_playerIconType = m_iconType;
                player->setScale(m_iconType == IconType::Jetpack ? 1.5f : 1.6f);
                auto selectedIconType = dual ? (IconType)sdi->getSavedValue("lasttype", 0) : m_selectedIconType;
                if (Mod::get()->setSavedValue<std::string>(savedType, name) == name && selectedIconType == m_iconType) {
                    auto popup = ItemInfoPopup::create(1, unlockType);
                    if (auto nameLabel = getChildOfType<CCLabelBMFont>(popup->m_mainLayer, 0)) nameLabel->setString(name.c_str());
                    if (auto achLabel = getChildOfType<CCLabelBMFont>(popup->m_mainLayer, 1)) achLabel->setString("Custom");
                    if (Loader::get()->isModLoaded("thesillydoggo.animatedprofiles") && (m_iconType == IconType::Robot || m_iconType == IconType::Spider)) {
                        for (auto buttonChild : CCArrayExt<CCNode*>(popup->m_buttonMenu->getChildren())) {
                            if (auto possibleButton = typeinfo_cast<CCMenuItemSpriteExtra*>(buttonChild)) {
                                if (auto possibleIcon = typeinfo_cast<GJItemIcon*>(possibleButton->getNormalImage()))
                                    MoreIcons::changeSimplePlayer(possibleIcon->m_player, name, m_iconType);
                            }
                        }
                    }
                    else if (auto popupIcon = getChildOfType<GJItemIcon>(popup->m_mainLayer, 0)) MoreIcons::changeSimplePlayer(popupIcon->m_player, name, m_iconType);
                    if (auto descText = getChildOfType<TextArea>(popup->m_mainLayer, 0)) descText->setString(
                        fmt::format("This <cg>{}</c> is added by the <cl>More Icons</c> mod.", std::string(ItemInfoPopup::nameForUnlockType(1, unlockType))));
                    popup->show();
                }
                if (dual) {
                    sdi->setSavedValue("lastmode", (int)m_iconType);
                    sdi->setSavedValue("lasttype", (int)m_iconType);
                }
                else m_selectedIconType = m_iconType;
            });
            iconButton->setContentSize(playerSquare->getContentSize());
            itemIcon->setPosition(iconButton->getContentSize() / 2);
            iconButton->setTag(i++);
            objs->addObject(iconButton);
            if (name == Mod::get()->getSavedValue<std::string>(savedType, "")) current = iconButton;
        }

        f->m_pageBar = ListButtonBar::create(objs, winSize / 2 - CCPoint { 0.0f, 65.0f }, 12, 3, 5.0f, 5.0f, 25.0f, 220.0f, 1);
        f->m_pageBar->m_scrollLayer->togglePageIndicators(false);
        f->m_pageBar->setID("icon-selection-bar"_spr);
        addChild(f->m_pageBar, 101);

        m_cursor1->setVisible(current != nullptr);
        if (current) m_cursor1->setPosition(current->getParent()->convertToWorldSpace(current->getPosition()));
    }

    void setupCustomSpecialPage(int page) {
        auto& vec = MoreIcons::vectorForType(m_iconType);
        auto f = m_fields.self();
        if (f->m_pageBar) {
            f->m_pageBar->removeFromParent();
            f->m_pageBar = nullptr;
        }
        if (vec.empty()) {
            createNavMenu();
            return;
        }

        m_iconSelection->setVisible(false);

        f->m_custom = true;
        f->m_page = MoreIcons::wrapPage(m_iconType, page);
        f->m_pages[m_iconType] = f->m_page;
        createNavMenu();

        m_leftArrow->setVisible(vec.size() > 36);
        m_rightArrow->setVisible(vec.size() > 36);

        auto spriteFrameCache = CCSpriteFrameCache::get();
        for (auto navDot : CCArrayExt<CCMenuItemSpriteExtra*>(m_navDotMenu->getChildren())) {
            static_cast<CCSprite*>(navDot->getNormalImage())->setDisplayFrame(spriteFrameCache->spriteFrameByName("gj_navDotBtn_off_001.png"));
        }

        auto winSize = CCDirector::get()->getWinSize();
        auto unlockType = GameManager::get()->iconTypeToUnlockType(m_iconType);
        auto objs = CCArray::create();
        CCMenuItemSpriteExtra* current = nullptr;
        auto savedType = MoreIcons::savedForType(m_iconType);
        auto sdi = Loader::get()->getLoadedMod("weebify.separate_dual_icons");
        auto dual = sdi && sdi->getSavedValue("2pselected", false);
        int i = 1;
        for (auto name : MoreIcons::getPage(m_iconType, f->m_page)) {
            auto square = CCSprite::createWithSpriteFrameName("playerSquare_001.png");
            square->setColor({ 150, 150, 150 });
            auto texture = CCTextureCache::get()->textureForKey(MoreIcons::TRAIL_INFO[name].texture.c_str());
            auto streak = CCSprite::createWithTexture(texture);
            limitNodeWidth(streak, 27.0f, 999.0f, 0.001f);
            streak->setRotation(-90.0f);
            square->addChild(streak);
            streak->setPosition(square->getContentSize() / 2);
            square->setScale(0.8f);
            auto iconButton = CCMenuItemExt::createSpriteExtra(square, [this, name, texture, winSize, sdi, dual, savedType, unlockType](CCMenuItemSpriteExtra* sender) {
                m_cursor1->setPosition(sender->getParent()->convertToWorldSpace(sender->getPosition()));
                m_cursor1->setVisible(true);
                auto selectedIconType = dual ? (IconType)sdi->getSavedValue("lasttype", 0) : m_selectedIconType;
                if (Mod::get()->setSavedValue<std::string>(savedType, name) == name && selectedIconType == m_iconType) {
                    auto popup = ItemInfoPopup::create(1, unlockType);
                    if (auto nameLabel = getChildOfType<CCLabelBMFont>(popup->m_mainLayer, 0)) nameLabel->setString(name.c_str());
                    if (auto achLabel = getChildOfType<CCLabelBMFont>(popup->m_mainLayer, 1)) achLabel->setString("Custom");
                    if (auto popupIcon = getChildOfType<GJItemIcon>(popup->m_mainLayer, 0)) {
                        popupIcon->setVisible(false);
                        auto square = CCSprite::createWithSpriteFrameName("playerSquare_001.png");
                        square->setColor({ 150, 150, 150 });
                        auto streak = CCSprite::createWithTexture(texture);
                        limitNodeWidth(streak, 27.0f, 999.0f, 0.001f);
                        streak->setRotation(-90.0f);
                        square->addChild(streak);
                        streak->setPosition(square->getContentSize() / 2);
                        square->setPosition(popupIcon->getPosition());
                        square->setScale(popupIcon->getScale());
                        square->setID("trail-square"_spr);
                        popup->m_mainLayer->addChild(square);
                    }
                    if (auto descText = getChildOfType<TextArea>(popup->m_mainLayer, 0)) descText->setString(
                        fmt::format("This <cg>{}</c> is added by the <cl>More Icons</c> mod.", std::string(ItemInfoPopup::nameForUnlockType(1, unlockType))));
                    auto blendToggler = CCMenuItemExt::createTogglerWithStandardSprites(0.5f, [this, name](CCMenuItemToggler* sender) {
                        MoreIcons::TRAIL_INFO[name].blend = !sender->isToggled();
                    });
                    blendToggler->setPosition(popup->m_buttonMenu->convertToNodeSpace(winSize / 2 - CCPoint { 123.0f, 78.0f }));
                    blendToggler->toggle(MoreIcons::TRAIL_INFO[name].blend);
                    blendToggler->setID("blend-toggler"_spr);
                    popup->m_buttonMenu->addChild(blendToggler);
                    auto blendLabel = CCLabelBMFont::create("Blend", "bigFont.fnt");
                    blendLabel->setPosition(winSize / 2 - CCPoint { 112.0f, 78.0f });
                    blendLabel->setAnchorPoint({ 0.0f, 0.5f });
                    blendLabel->setScale(0.3f);
                    blendLabel->setID("blend-label"_spr);
                    popup->m_mainLayer->addChild(blendLabel);
                    auto tintToggler = CCMenuItemExt::createTogglerWithStandardSprites(0.5f, [this, name](CCMenuItemToggler* sender) {
                        MoreIcons::TRAIL_INFO[name].tint = !sender->isToggled();
                    });
                    tintToggler->setPosition(popup->m_buttonMenu->convertToNodeSpace(winSize / 2 - CCPoint { 123.0f, 98.0f }));
                    tintToggler->toggle(MoreIcons::TRAIL_INFO[name].tint);
                    tintToggler->setID("tint-toggler"_spr);
                    popup->m_buttonMenu->addChild(tintToggler);
                    auto tintLabel = CCLabelBMFont::create("Tint", "bigFont.fnt");
                    tintLabel->setPosition(winSize / 2 - CCPoint { 112.0f, 98.0f });
                    tintLabel->setAnchorPoint({ 0.0f, 0.5f });
                    tintLabel->setScale(0.3f);
                    tintLabel->setID("tint-label"_spr);
                    popup->m_mainLayer->addChild(tintLabel);
                    popup->show();
                }
                if (dual) sdi->setSavedValue("lasttype", (int)m_iconType);
                else m_selectedIconType = m_iconType;
            });
            iconButton->setTag(i++);
            objs->addObject(iconButton);
            if (name == Mod::get()->getSavedValue<std::string>(savedType, "")) current = iconButton;
        }

        f->m_pageBar = ListButtonBar::create(objs, winSize / 2 - CCPoint { 0.0f, 65.0f }, 12, 3, 5.0f, 5.0f, 25.0f, 220.0f, 1);
        f->m_pageBar->m_scrollLayer->togglePageIndicators(false);
        f->m_pageBar->setID("icon-selection-bar"_spr);
        addChild(f->m_pageBar, 101);

        m_cursor1->setVisible(current != nullptr);
        if (current) m_cursor1->setPosition(current->getParent()->convertToWorldSpace(current->getPosition()));
        m_cursor2->setVisible(false);
    }
};
