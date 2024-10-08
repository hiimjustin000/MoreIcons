#include "../MoreIcons.hpp"

using namespace geode::prelude;

#include <Geode/modify/MenuLayer.hpp>
class $modify(MIMenuLayer, MenuLayer) {
    static void onModify(auto& self) {
        (void)self.setHookPriority("MenuLayer::init", -1);
    }

    bool init() {
        if (!MenuLayer::init()) return false;

        if (!Loader::get()->isModLoaded("capeling.icon_profile")) return true;

        auto profileMenu = getChildByID("profile-menu");
        if (!profileMenu) return true;

        auto profileButton = static_cast<CCMenuItemSpriteExtra*>(profileMenu->getChildByID("profile-button"));
        if (!profileButton) return true;

        MoreIcons::changeSimplePlayer(getChildOfType<SimplePlayer>(profileButton->getNormalImage(), 0), GameManager::get()->m_playerIconType, false);

        return true;
    }
};
