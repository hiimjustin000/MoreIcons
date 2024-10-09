#include "../MoreIcons.hpp"

using namespace geode::prelude;

#include <Geode/modify/LoadingLayer.hpp>
class $modify(MILoadingLayer, LoadingLayer) {
    void loadAssets() {
        LoadingLayer::loadAssets();

        if (m_loadStep == 1) {
            MoreIcons::LOADING_LAYER = this;
            MoreIcons::load();
        }

        if (m_loadStep == 14) MoreIcons::LOADING_LAYER = nullptr;
    }
};
