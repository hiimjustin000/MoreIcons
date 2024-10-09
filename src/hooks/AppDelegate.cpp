#include "../MoreIcons.hpp"

using namespace geode::prelude;

#include <Geode/modify/AppDelegate.hpp>
class $modify(MIAppDelegate, AppDelegate) {
    static void onModify(auto& self) {
        (void)self.setHookPriority("AppDelegate::trySaveGame", -1);
    }

    void trySaveGame(bool closing) {
        MoreIcons::removeSaved();

        AppDelegate::trySaveGame(closing);

        MoreIcons::restoreSaved();
    }
};
