#include "../MoreIcons.hpp"

using namespace geode::prelude;

class $modify(MILoadingLayer, LoadingLayer) {
    static void onModify(auto& self) {
        (void)self.setHookPriority("LoadingLayer::init", -1);
    }

    bool init(bool refresh) {
        if (!LoadingLayer::init(refresh)) return false;

        MoreIcons::LOADING_LAYER = this;

        return true;
    }

    #ifndef GEODE_IS_ANDROID // Compiler optimizations that aren't present in the Android version
    void loadAssets() {
        LoadingLayer::loadAssets();

        if (m_loadStep == 14) MoreIcons::LOADING_LAYER = nullptr;
    }
    #else
    void loadingFinished() {
        LoadingLayer::loadingFinished();

        MoreIcons::LOADING_LAYER = nullptr;
    }
    #endif
};
