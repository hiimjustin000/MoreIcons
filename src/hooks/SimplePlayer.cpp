#include "../api/MoreIconsAPI.hpp"

using namespace geode::prelude;

#include <Geode/modify/SimplePlayer.hpp>
class $modify(MISimplePlayer, SimplePlayer) {
    void updatePlayerFrame(int frame, IconType type) {
        SimplePlayer::updatePlayerFrame(frame, type);

        MoreIconsAPI::removeUserObject(this);
    }
};
