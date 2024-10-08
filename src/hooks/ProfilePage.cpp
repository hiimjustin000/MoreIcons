#include "../MoreIcons.hpp"

using namespace geode::prelude;

#include <Geode/modify/ProfilePage.hpp>
class $modify(MIProfilePage, ProfilePage) {
    struct Fields {
        CCObject* m_originalSDIToggleTarget;
        SEL_MenuHandler m_originalSDIToggle;
        CCObject* m_originalSDIShipTarget;
        SEL_MenuHandler m_originalSDIShip;
        IconType m_shipType = IconType::Ship;
    };

    static void onModify(auto& self) {
        (void)self.setHookPriority("ProfilePage::loadPageFromUserInfo", -1);
    }

    void loadPageFromUserInfo(GJUserScore* score) {
        ProfilePage::loadPageFromUserInfo(score);

        if (!m_ownProfile) return;

        auto playerMenu = m_mainLayer->getChildByID("player-menu");
        if (!playerMenu) return;

        MoreIcons::changeSimplePlayer(getChildOfType<SimplePlayer>(playerMenu->getChildByID("player-icon"), 0), IconType::Cube);
        MoreIcons::changeSimplePlayer(getChildOfType<SimplePlayer>(playerMenu->getChildByID("player-ship"), 0), IconType::Ship);
        MoreIcons::changeSimplePlayer(getChildOfType<SimplePlayer>(playerMenu->getChildByID("player-ball"), 0), IconType::Ball);
        MoreIcons::changeSimplePlayer(getChildOfType<SimplePlayer>(playerMenu->getChildByID("player-ufo"), 0), IconType::Ufo);
        MoreIcons::changeSimplePlayer(getChildOfType<SimplePlayer>(playerMenu->getChildByID("player-wave"), 0), IconType::Wave);
        if (Loader::get()->isModLoaded("thesillydoggo.animatedprofiles") && !Loader::get()->isModLoaded("rynat.better_unlock_info")) {
            MoreIcons::changeSimplePlayer(
                getChildOfType<SimplePlayer>(playerMenu->getChildByID("player-robot")->getChildByID("player-robot"), 0), IconType::Robot);
            MoreIcons::changeSimplePlayer(
                getChildOfType<SimplePlayer>(playerMenu->getChildByID("player-spider")->getChildByID("player-spider"), 0), IconType::Spider);
        } else {
            MoreIcons::changeSimplePlayer(getChildOfType<SimplePlayer>(playerMenu->getChildByID("player-robot"), 0), IconType::Robot);
            MoreIcons::changeSimplePlayer(getChildOfType<SimplePlayer>(playerMenu->getChildByID("player-spider"), 0), IconType::Spider);
        }
        MoreIcons::changeSimplePlayer(getChildOfType<SimplePlayer>(playerMenu->getChildByID("player-swing"), 0), IconType::Swing);
        if (auto playerJetpack = playerMenu->getChildByID("player-jetpack"))
            MoreIcons::changeSimplePlayer(getChildOfType<SimplePlayer>(playerJetpack, 0), IconType::Jetpack);

        if (Loader::get()->isModLoaded("weebify.separate_dual_icons")) {
            auto twoPToggler = static_cast<CCMenuItemSpriteExtra*>(m_mainLayer->getChildByID("left-menu")->getChildByID("2p-toggler"));
            auto f = m_fields.self();
            f->m_originalSDIToggleTarget = twoPToggler->m_pListener;
            f->m_originalSDIToggle = twoPToggler->m_pfnSelector;
            twoPToggler->setTarget(this, menu_selector(MIProfilePage::newOn2PToggle));

            if (!Loader::get()->isModLoaded("rynat.better_unlock_info")) {
                auto shipToggler = static_cast<CCMenuItemSpriteExtra*>(playerMenu->getChildByID("player-ship"));
                f->m_originalSDIShipTarget = shipToggler->m_pListener;
                f->m_originalSDIShip = shipToggler->m_pfnSelector;
                shipToggler->setTarget(this, menu_selector(MIProfilePage::newOnShipToggle));
            }
        }
    }

    void newOn2PToggle(CCObject* sender) {
        auto f = m_fields.self();
        (f->m_originalSDIToggleTarget->*f->m_originalSDIToggle)(sender);
        auto playerMenu = m_mainLayer->getChildByID("player-menu");
        MoreIcons::changeSimplePlayer(getChildOfType<SimplePlayer>(playerMenu->getChildByID("player-icon"), 0), IconType::Cube);
        MoreIcons::changeSimplePlayer(getChildOfType<SimplePlayer>(playerMenu->getChildByID("player-ship"), 0), IconType::Ship);
        MoreIcons::changeSimplePlayer(getChildOfType<SimplePlayer>(playerMenu->getChildByID("player-ball"), 0), IconType::Ball);
        MoreIcons::changeSimplePlayer(getChildOfType<SimplePlayer>(playerMenu->getChildByID("player-ufo"), 0), IconType::Ufo);
        MoreIcons::changeSimplePlayer(getChildOfType<SimplePlayer>(playerMenu->getChildByID("player-wave"), 0), IconType::Wave);
        if (Loader::get()->isModLoaded("thesillydoggo.animatedprofiles") && !Loader::get()->isModLoaded("rynat.better_unlock_info")) {
            MoreIcons::changeSimplePlayer(
                getChildOfType<SimplePlayer>(playerMenu->getChildByID("player-robot")->getChildByID("player-robot"), 0), IconType::Robot);
            MoreIcons::changeSimplePlayer(
                getChildOfType<SimplePlayer>(playerMenu->getChildByID("player-spider")->getChildByID("player-spider"), 0), IconType::Spider);
        } else {
            MoreIcons::changeSimplePlayer(getChildOfType<SimplePlayer>(playerMenu->getChildByID("player-robot"), 0), IconType::Robot);
            MoreIcons::changeSimplePlayer(getChildOfType<SimplePlayer>(playerMenu->getChildByID("player-spider"), 0), IconType::Spider);
        }
        MoreIcons::changeSimplePlayer(getChildOfType<SimplePlayer>(playerMenu->getChildByID("player-swing"), 0), IconType::Swing);
        if (auto playerJetpack = playerMenu->getChildByID("player-jetpack"))
            MoreIcons::changeSimplePlayer(getChildOfType<SimplePlayer>(playerJetpack, 0), IconType::Jetpack);
    }

    void toggleShip(CCObject* sender) {
        ProfilePage::toggleShip(sender);

        if (!m_ownProfile) return;

        MoreIcons::changeSimplePlayer(static_cast<SimplePlayer*>(static_cast<CCMenuItemSpriteExtra*>(sender)->getNormalImage()), (IconType)sender->getTag());
    }

    void newOnShipToggle(CCObject* sender) {
        auto f = m_fields.self();
        (f->m_originalSDIShipTarget->*f->m_originalSDIShip)(sender);

        f->m_shipType = f->m_shipType == IconType::Ship ? IconType::Jetpack : IconType::Ship;
        MoreIcons::changeSimplePlayer(static_cast<SimplePlayer*>(static_cast<CCMenuItemSpriteExtra*>(sender)->getNormalImage()), f->m_shipType);
    }
};
