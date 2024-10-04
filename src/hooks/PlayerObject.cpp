#include "../MoreIcons.hpp"

using namespace geode::prelude;

#include <Geode/modify/PlayerObject.hpp>
class $modify(MIPlayerObject, PlayerObject) {
    bool init(int player, int ship, GJBaseGameLayer* gameLayer, CCLayer* layer, bool highGraphics) {
        if (!PlayerObject::init(player, ship, gameLayer, layer, highGraphics)) return false;

        if (!gameLayer || (gameLayer->m_player1 && gameLayer->m_player2)) return true;

        useCustomIcon();
        useCustomShip();
        useCustomBall();
        useCustomUfo();
        useCustomWave();
        useCustomSwing();
        useCustomJetpack();

        return true;
    }

    void updatePlayerFrame(int frame) {
        PlayerObject::updatePlayerFrame(frame);

        if (m_gameLayer && (m_gameLayer->m_player1 == this || m_gameLayer->m_player2 == this)) useCustomIcon();
    }

    void useCustomIcon() {
        auto iconFile = Mod::get()->getSavedValue<std::string>("icon", "");
        if (iconFile.empty() || !MoreIcons::hasIcon(iconFile)) return;

        auto icon = fmt::format("{}_001.png"_spr, iconFile);
        auto icon2 = fmt::format("{}_2_001.png"_spr, iconFile);
        auto iconExtra = fmt::format("{}_extra_001.png"_spr, iconFile);
        auto iconGlow = fmt::format("{}_glow_001.png"_spr, iconFile);

        auto spriteFrameCache = CCSpriteFrameCache::get();
        m_iconSprite->setDisplayFrame(spriteFrameCache->spriteFrameByName(icon.c_str()));
        m_iconSpriteSecondary->setDisplayFrame(spriteFrameCache->spriteFrameByName(icon2.c_str()));
        m_iconSpriteSecondary->setPosition(m_iconSprite->getContentSize() / 2);
        m_iconGlow->setDisplayFrame(spriteFrameCache->spriteFrameByName(iconGlow.c_str()));

        auto iconExtraFrame = spriteFrameCache->spriteFrameByName(iconExtra.c_str());
        auto extraVisible = MoreIcons::doesExist(iconExtraFrame);
        m_iconSpriteWhitener->setVisible(extraVisible);
        if (extraVisible) {
            m_iconSpriteWhitener->setDisplayFrame(iconExtraFrame);
            m_iconSpriteWhitener->setPosition(m_iconSprite->getContentSize() / 2);
        }
    }

    void updatePlayerShipFrame(int frame) {
        PlayerObject::updatePlayerShipFrame(frame);

        if (m_gameLayer && (m_gameLayer->m_player1 == this || m_gameLayer->m_player2 == this)) useCustomShip();
    }

    void useCustomShip() {
        auto shipFile = Mod::get()->getSavedValue<std::string>("ship", "");
        if (shipFile.empty() || !MoreIcons::hasShip(shipFile)) return;

        auto ship = fmt::format("{}_001.png"_spr, shipFile);
        auto ship2 = fmt::format("{}_2_001.png"_spr, shipFile);
        auto shipExtra = fmt::format("{}_extra_001.png"_spr, shipFile);
        auto shipGlow = fmt::format("{}_glow_001.png"_spr, shipFile);

        auto spriteFrameCache = CCSpriteFrameCache::get();
        m_vehicleSprite->setDisplayFrame(spriteFrameCache->spriteFrameByName(ship.c_str()));
        m_vehicleSpriteSecondary->setDisplayFrame(spriteFrameCache->spriteFrameByName(ship2.c_str()));
        m_vehicleSpriteSecondary->setPosition(m_vehicleSprite->getContentSize() / 2);
        m_vehicleGlow->setDisplayFrame(spriteFrameCache->spriteFrameByName(shipGlow.c_str()));

        auto shipExtraFrame = spriteFrameCache->spriteFrameByName(shipExtra.c_str());
        auto extraVisible = MoreIcons::doesExist(shipExtraFrame);
        m_vehicleSpriteWhitener->setVisible(extraVisible);
        if (extraVisible) {
            m_vehicleSpriteWhitener->setDisplayFrame(shipExtraFrame);
            m_vehicleSpriteWhitener->setPosition(m_vehicleSprite->getContentSize() / 2);
        }
    }

    void updatePlayerRollFrame(int frame) {
        PlayerObject::updatePlayerRollFrame(frame);

        if (m_gameLayer && (m_gameLayer->m_player1 == this || m_gameLayer->m_player2 == this)) useCustomBall();
    }

    void useCustomBall() {
        auto ballFile = Mod::get()->getSavedValue<std::string>("ball", "");
        if (ballFile.empty() || !MoreIcons::hasBall(ballFile)) return;

        auto ball = fmt::format("{}_001.png"_spr, ballFile);
        auto ball2 = fmt::format("{}_2_001.png"_spr, ballFile);
        auto ballExtra = fmt::format("{}_extra_001.png"_spr, ballFile);
        auto ballGlow = fmt::format("{}_glow_001.png"_spr, ballFile);

        auto spriteFrameCache = CCSpriteFrameCache::get();
        m_iconSprite->setDisplayFrame(spriteFrameCache->spriteFrameByName(ball.c_str()));
        m_iconSpriteSecondary->setDisplayFrame(spriteFrameCache->spriteFrameByName(ball2.c_str()));
        m_iconSpriteSecondary->setPosition(m_iconSprite->getContentSize() / 2);
        m_iconGlow->setDisplayFrame(spriteFrameCache->spriteFrameByName(ballGlow.c_str()));

        auto ballExtraFrame = spriteFrameCache->spriteFrameByName(ballExtra.c_str());
        auto extraVisible = MoreIcons::doesExist(ballExtraFrame);
        m_iconSpriteWhitener->setVisible(extraVisible);
        if (extraVisible) {
            m_iconSpriteWhitener->setDisplayFrame(ballExtraFrame);
            m_iconSpriteWhitener->setPosition(m_iconSprite->getContentSize() / 2);
        }
    }

    void updatePlayerBirdFrame(int frame) {
        PlayerObject::updatePlayerBirdFrame(frame);

        if (m_gameLayer && (m_gameLayer->m_player1 == this || m_gameLayer->m_player2 == this)) useCustomUfo();
    }

    void useCustomUfo() {
        auto ufoFile = Mod::get()->getSavedValue<std::string>("ufo", "");
        if (ufoFile.empty() || !MoreIcons::hasUfo(ufoFile)) return;

        auto ufo = fmt::format("{}_001.png"_spr, ufoFile);
        auto ufo2 = fmt::format("{}_2_001.png"_spr, ufoFile);
        auto ufo3 = fmt::format("{}_3_001.png"_spr, ufoFile);
        auto ufoExtra = fmt::format("{}_extra_001.png"_spr, ufoFile);
        auto ufoGlow = fmt::format("{}_glow_001.png"_spr, ufoFile);

        auto spriteFrameCache = CCSpriteFrameCache::get();
        m_vehicleSprite->setDisplayFrame(spriteFrameCache->spriteFrameByName(ufo.c_str()));
        m_vehicleSpriteSecondary->setDisplayFrame(spriteFrameCache->spriteFrameByName(ufo2.c_str()));
        m_vehicleSpriteSecondary->setPosition(m_vehicleSprite->getContentSize() / 2);
        m_birdVehicle->setDisplayFrame(spriteFrameCache->spriteFrameByName(ufo3.c_str()));
        m_birdVehicle->setPosition(m_vehicleSprite->getContentSize() / 2);
        m_vehicleGlow->setDisplayFrame(spriteFrameCache->spriteFrameByName(ufoGlow.c_str()));

        auto ufoExtraFrame = spriteFrameCache->spriteFrameByName(ufoExtra.c_str());
        auto extraVisible = MoreIcons::doesExist(ufoExtraFrame);
        m_vehicleSpriteWhitener->setVisible(extraVisible);
        if (extraVisible) {
            m_vehicleSpriteWhitener->setDisplayFrame(ufoExtraFrame);
            m_vehicleSpriteWhitener->setPosition(m_vehicleSprite->getContentSize() / 2);
        }
    }

    void updatePlayerDartFrame(int frame) {
        PlayerObject::updatePlayerDartFrame(frame);

        if (m_gameLayer && (m_gameLayer->m_player1 == this || m_gameLayer->m_player2 == this)) useCustomWave();
    }

    void useCustomWave() {
        auto waveFile = Mod::get()->getSavedValue<std::string>("wave", "");
        if (waveFile.empty() || !MoreIcons::hasWave(waveFile)) return;

        auto wave = fmt::format("{}_001.png"_spr, waveFile);
        auto wave2 = fmt::format("{}_2_001.png"_spr, waveFile);
        auto waveExtra = fmt::format("{}_extra_001.png"_spr, waveFile);
        auto waveGlow = fmt::format("{}_glow_001.png"_spr, waveFile);

        auto spriteFrameCache = CCSpriteFrameCache::get();
        m_iconSprite->setDisplayFrame(spriteFrameCache->spriteFrameByName(wave.c_str()));
        m_iconSpriteSecondary->setDisplayFrame(spriteFrameCache->spriteFrameByName(wave2.c_str()));
        m_iconSpriteSecondary->setPosition(m_iconSprite->getContentSize() / 2);
        m_iconGlow->setDisplayFrame(spriteFrameCache->spriteFrameByName(waveGlow.c_str()));

        auto waveExtraFrame = spriteFrameCache->spriteFrameByName(waveExtra.c_str());
        auto extraVisible = MoreIcons::doesExist(waveExtraFrame);
        m_iconSpriteWhitener->setVisible(extraVisible);
        if (extraVisible) {
            m_iconSpriteWhitener->setDisplayFrame(waveExtraFrame);
            m_iconSpriteWhitener->setPosition(m_iconSprite->getContentSize() / 2);
        }
    }

    void updatePlayerSwingFrame(int frame) {
        PlayerObject::updatePlayerSwingFrame(frame);

        if (m_gameLayer && (m_gameLayer->m_player1 == this || m_gameLayer->m_player2 == this)) useCustomSwing();
    }

    void useCustomSwing() {
        auto swingFile = Mod::get()->getSavedValue<std::string>("swing", "");
        if (swingFile.empty() || !MoreIcons::hasSwing(swingFile)) return;

        auto swing = fmt::format("{}_001.png"_spr, swingFile);
        auto swing2 = fmt::format("{}_2_001.png"_spr, swingFile);
        auto swingExtra = fmt::format("{}_extra_001.png"_spr, swingFile);
        auto swingGlow = fmt::format("{}_glow_001.png"_spr, swingFile);

        auto spriteFrameCache = CCSpriteFrameCache::get();
        m_iconSprite->setDisplayFrame(spriteFrameCache->spriteFrameByName(swing.c_str()));
        m_iconSpriteSecondary->setDisplayFrame(spriteFrameCache->spriteFrameByName(swing2.c_str()));
        m_iconSpriteSecondary->setPosition(m_iconSprite->getContentSize() / 2);
        m_iconGlow->setDisplayFrame(spriteFrameCache->spriteFrameByName(swingGlow.c_str()));

        auto swingExtraFrame = spriteFrameCache->spriteFrameByName(swingExtra.c_str());
        auto extraVisible = MoreIcons::doesExist(swingExtraFrame);
        m_iconSpriteWhitener->setVisible(extraVisible);
        if (extraVisible) {
            m_iconSpriteWhitener->setDisplayFrame(swingExtraFrame);
            m_iconSpriteWhitener->setPosition(m_iconSprite->getContentSize() / 2);
        }
    }

    void updatePlayerJetpackFrame(int frame) {
        PlayerObject::updatePlayerJetpackFrame(frame);

        if (m_gameLayer && (m_gameLayer->m_player1 == this || m_gameLayer->m_player2 == this)) useCustomJetpack();
    }

    void useCustomJetpack() {
        auto jetpackFile = Mod::get()->getSavedValue<std::string>("jetpack", "");
        if (jetpackFile.empty() || !MoreIcons::hasJetpack(jetpackFile)) return;

        auto jetpack = fmt::format("{}_001.png", jetpackFile);
        auto jetpack2 = fmt::format("{}_2_001.png", jetpackFile);
        auto jetpackExtra = fmt::format("{}_extra_001.png", jetpackFile);
        auto jetpackGlow = fmt::format("{}_glow_001.png", jetpackFile);

        auto spriteFrameCache = CCSpriteFrameCache::get();
        m_vehicleSprite->setDisplayFrame(spriteFrameCache->spriteFrameByName(jetpack.c_str()));
        m_vehicleSpriteSecondary->setDisplayFrame(spriteFrameCache->spriteFrameByName(jetpack2.c_str()));
        m_vehicleSpriteSecondary->setPosition(m_vehicleSprite->getContentSize() / 2);
        m_vehicleGlow->setDisplayFrame(spriteFrameCache->spriteFrameByName(jetpackGlow.c_str()));

        auto jetpackExtraFrame = spriteFrameCache->spriteFrameByName(jetpackExtra.c_str());
        auto extraVisible = MoreIcons::doesExist(jetpackExtraFrame);
        m_vehicleSpriteWhitener->setVisible(extraVisible);
        if (extraVisible) {
            m_vehicleSpriteWhitener->setDisplayFrame(jetpackExtraFrame);
            m_vehicleSpriteWhitener->setPosition(m_vehicleSprite->getContentSize() / 2);
        }
    }
};
