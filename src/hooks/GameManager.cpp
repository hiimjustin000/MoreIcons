#include "../MoreIcons.hpp"

using namespace geode::prelude;

#include <Geode/modify/GameManager.hpp>
class $modify(MIGameManager, GameManager) {
    void reloadAllStep2() {
        GameManager::reloadAllStep2();

        if (m_unkBool1) MoreIcons::clear();
    }

    void reloadAllStep5() {
        GameManager::reloadAllStep5();

        if (m_unkBool1) MoreIcons::load();
    }
};
