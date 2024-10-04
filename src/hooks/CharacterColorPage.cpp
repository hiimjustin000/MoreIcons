#include "../MoreIcons.hpp"

using namespace geode::prelude;

#include <Geode/modify/CharacterColorPage.hpp>
class $modify(MICharacterColorPage, CharacterColorPage) {
    bool init() {
        if (!CharacterColorPage::init()) return false;

        for (int i = 0; i < m_playerObjects->count(); i++) {
            auto player = static_cast<SimplePlayer*>(m_playerObjects->objectAtIndex(i));
            MoreIcons::changeSimplePlayer(player, (IconType)i);
        }

        return true;
    }

    void toggleShip(CCObject* sender) {
        CharacterColorPage::toggleShip(sender);

        MoreIcons::changeSimplePlayer(static_cast<SimplePlayer*>(static_cast<CCMenuItemSpriteExtra*>(sender)->getNormalImage()), (IconType)sender->getTag());
    }
};
