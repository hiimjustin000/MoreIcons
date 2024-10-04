#include "../MoreIcons.hpp"

using namespace geode::prelude;

#include <Geode/modify/GJGarageLayer.hpp>
class $modify(MIGarageLayer, GJGarageLayer) {
    struct Fields {
        ListButtonBar* m_pageBar;
        CCMenu* m_navMenu;
        int m_page;
        bool m_custom;
    };

    bool init() {
        if (!GJGarageLayer::init()) return false;

        auto iconType = GameManager::get()->m_playerIconType;
        MoreIcons::changeSimplePlayer(m_playerObject, Mod::get()->getSavedValue<std::string>(MoreIcons::savedForType(iconType), ""), iconType);

        createNavMenu();

        return true;
    }

    void onSelect(CCObject* sender) {
        GJGarageLayer::onSelect(sender);

        Mod::get()->setSavedValue<std::string>(MoreIcons::savedForType(m_iconType), "");
    }

    void updatePlayerColors() {
        GJGarageLayer::updatePlayerColors();

        if (m_iconSelection && m_fields->m_custom && !MoreIcons::vectorForType(m_iconType).empty()) m_iconSelection->setVisible(true);
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
            addChild(f->m_navMenu);
        }

        auto& vec = MoreIcons::vectorForType(m_iconType);
        m_navDotMenu->setPositionY(vec.empty() ? 25.0f : 35.0f);
        f->m_navMenu->setVisible(!vec.empty());
        if (vec.empty()) return;

        f->m_navMenu->removeAllChildren();
        auto navDotAmount = (vec.size() + 35) / 36;
        for (int i = 0; i < navDotAmount; i++) {
            auto dot = CCMenuItemExt::createSpriteExtraWithFrameName(
                f->m_custom && i == f->m_page ? "gj_navDotBtn_on_001.png" : "gj_navDotBtn_off_001.png",
                0.9f,
                [this, i](auto) {
                    setupCustomPage(i);
                }
            );
            dot->setSizeMult(1.1f);
            f->m_navMenu->addChild(dot);
        }

        f->m_navMenu->updateLayout();
    }

    void onNavigate(CCObject* sender) {
        GJGarageLayer::onNavigate(sender);

        auto f = m_fields.self();
        if (f->m_pageBar) {
            f->m_custom = false;
            f->m_pageBar->setVisible(false);
        }

        createNavMenu();
    }

    void onArrow(CCObject* sender) {
        GJGarageLayer::onArrow(sender);

        auto f = m_fields.self();
        if (f->m_custom) setupCustomPage(f->m_page + sender->getTag());

        createNavMenu();
    }

    void onSelectTab(CCObject* sender) {
        GJGarageLayer::onSelectTab(sender);

        if (m_fields->m_custom) setupCustomPage(0);

        createNavMenu();
    }

    void setupCustomPage(int page) {
        auto& vec = MoreIcons::vectorForType(m_iconType);
        auto f = m_fields.self();
        if (vec.empty()) {
            if (f->m_pageBar) f->m_pageBar->setVisible(false);
            return;
        }

        m_iconSelection->setVisible(false);

        f->m_custom = true;
        f->m_page = MoreIcons::wrapPage(m_iconType, page);
        createNavMenu();

        m_leftArrow->setVisible(vec.size() > 36);
        m_rightArrow->setVisible(vec.size() > 36);

        auto spriteFrameCache = CCSpriteFrameCache::get();
        for (auto navDot : CCArrayExt<CCMenuItemSpriteExtra*>(m_navDotMenu->getChildren())) {
            static_cast<CCSprite*>(navDot->getNormalImage())->setDisplayFrame(spriteFrameCache->spriteFrameByName("gj_navDotBtn_off_001.png"));
        }

        auto winSize = CCDirector::get()->getWinSize();
        auto unlockType = GameManager::get()->iconTypeToUnlockType(m_iconType);
        auto playerSquare = CCSprite::createWithSpriteFrameName("playerSquare_001.png");
        auto objs = CCArray::create();
        CCMenuItemSpriteExtra* current = nullptr;
        auto savedType = MoreIcons::savedForType(m_iconType);
        int i = 1;
        for (auto name : MoreIcons::getPage(m_iconType, f->m_page)) {
            auto itemIcon = GJItemIcon::createBrowserItem(unlockType, 1);
            itemIcon->setScale(GJItemIcon::scaleForType(unlockType));
            MoreIcons::changeSimplePlayer(itemIcon->m_player, name, m_iconType);
            auto iconButton = CCMenuItemExt::createSpriteExtra(itemIcon, [this, f, name, savedType](CCMenuItemSpriteExtra* sender) {
                Mod::get()->setSavedValue<std::string>(savedType, name);
                m_cursor1->setPosition(sender->getParent()->convertToWorldSpace(sender->getPosition()));
                m_cursor1->setVisible(true);
                m_playerObject->updatePlayerFrame(1, m_iconType);
                MoreIcons::changeSimplePlayer(m_playerObject, name, m_iconType);
                GameManager::get()->m_playerIconType = m_iconType;
                m_playerObject->setScale(m_iconType == IconType::Jetpack ? 1.5f : 1.6f);
            });
            iconButton->setContentSize(playerSquare->getContentSize());
            itemIcon->setPosition(iconButton->getContentSize() / 2);
            iconButton->setTag(i++);
            objs->addObject(iconButton);
            if (name == Mod::get()->getSavedValue<std::string>(savedType, "")) current = iconButton;
        }

        if (f->m_pageBar) {
            f->m_pageBar->removeFromParent();
            f->m_pageBar = nullptr;
        }

        f->m_pageBar = ListButtonBar::create(objs, winSize / 2 - CCPoint { 0.0f, 65.0f }, 12, 3, 5.0f, 5.0f, 25.0f, 220.0f, 1);
        f->m_pageBar->m_scrollLayer->togglePageIndicators(false);
        f->m_pageBar->setID("icon-selection-bar"_spr);
        addChild(f->m_pageBar, 100);

        m_cursor1->setVisible(current != nullptr);
        if (current) m_cursor1->setPosition(current->getParent()->convertToWorldSpace(current->getPosition()));
    }
};
