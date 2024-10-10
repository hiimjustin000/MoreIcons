#include "../MoreIcons.hpp"

using namespace geode::prelude;

#include <Geode/modify/LoadingLayer.hpp>
class $modify(MILoadingLayer, LoadingLayer) {
    void loadAssets() {
        LoadingLayer::loadAssets();

        if (m_loadStep == 1) {
            if (auto smallLabel2 = static_cast<CCLabelBMFont*>(getChildByID("geode-small-label-2"))) smallLabel2->setString("More Icons: Loading Icons");
        }
        else if (m_loadStep == 2) MoreIcons::load(this);
    }
};
