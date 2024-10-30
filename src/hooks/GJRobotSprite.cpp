#include "../api/MoreIconsAPI.hpp"

using namespace geode::prelude;

#include <Geode/modify/GJRobotSprite.hpp>
class $modify(MIRobotSprite, GJRobotSprite) {
    void updateFrame(int frame) {
        GJRobotSprite::updateFrame(frame);

        MoreIconsAPI::removeUserObject(this);
    }
};
