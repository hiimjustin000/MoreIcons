#include "../MoreIcons.hpp"

using namespace geode::prelude;

#include <Geode/modify/ProfilePage.hpp>
class $modify(MIProfilePage, ProfilePage) {
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
    }

    void toggleShip(CCObject* sender) {
        ProfilePage::toggleShip(sender);

        if (!m_ownProfile) return;

        MoreIcons::changeSimplePlayer(static_cast<SimplePlayer*>(static_cast<CCMenuItemSpriteExtra*>(sender)->getNormalImage()), (IconType)sender->getTag());
    }
};
