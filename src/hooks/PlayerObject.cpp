#include "../MoreIcons.hpp"

using namespace geode::prelude;

#include <Geode/modify/PlayerObject.hpp>
class $modify(MIPlayerObject, PlayerObject) {
    bool init(int player, int ship, GJBaseGameLayer* gameLayer, CCLayer* layer, bool highGraphics) {
        if (!PlayerObject::init(player, ship, gameLayer, layer, highGraphics)) return false;

        if (!gameLayer || (gameLayer->m_player1 && gameLayer->m_player2)) return true;

        if (!gameLayer->m_player1) useCustomIcon(false);
        else if (!gameLayer->m_player2) useCustomIcon(true);

        return true;
    }

    void updatePlayerFrame(int frame) {
        PlayerObject::updatePlayerFrame(frame);

        if (!m_gameLayer || frame == 0) return;

        if (m_gameLayer->m_player1 == this) useCustomIcon(false);
        else if (m_gameLayer->m_player2 == this) useCustomIcon(true);
    }

    void useCustomIcon(bool dual) {
        auto iconFile = Mod::get()->getSavedValue<std::string>(MoreIcons::getDual("icon", dual), "");
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

        if (!m_gameLayer) return;

        if (m_gameLayer->m_player1 == this) useCustomShip(false);
        else if (m_gameLayer->m_player2 == this) useCustomShip(true);
    }

    void useCustomShip(bool dual) {
        auto shipFile = Mod::get()->getSavedValue<std::string>(MoreIcons::getDual("ship", dual), "");
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

        if (!m_gameLayer || frame == 0) return;

        if (m_gameLayer->m_player1 == this) useCustomBall(false);
        else if (m_gameLayer->m_player2 == this) useCustomBall(true);
    }

    void useCustomBall(bool dual) {
        auto ballFile = Mod::get()->getSavedValue<std::string>(MoreIcons::getDual("ball", dual), "");
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

        if (!m_gameLayer) return;

        if (m_gameLayer->m_player1 == this) useCustomUfo(false);
        else if (m_gameLayer->m_player2 == this) useCustomUfo(true);
    }

    void useCustomUfo(bool dual) {
        auto ufoFile = Mod::get()->getSavedValue<std::string>(MoreIcons::getDual("ufo", dual), "");
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

        if (!m_gameLayer) return;

        if (m_gameLayer->m_player1 == this) useCustomWave(false);
        else if (m_gameLayer->m_player2 == this) useCustomWave(true);
    }

    void useCustomWave(bool dual) {
        auto waveFile = Mod::get()->getSavedValue<std::string>(MoreIcons::getDual("wave", dual), "");
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

    void createRobot(int frame) {
        PlayerObject::createRobot(frame);

        if (!m_gameLayer) return;

        if (!m_gameLayer->m_player1 || m_gameLayer->m_player1 == this) {
            auto robotFile = Mod::get()->getSavedValue<std::string>(MoreIcons::getDual("robot", false), "");
            if (robotFile.empty() || !MoreIcons::hasRobot(robotFile)) return;
            MoreIcons::useCustomRobot(m_robotSprite, robotFile);
        }
        else if (!m_gameLayer->m_player2 || m_gameLayer->m_player2 == this) {
            auto robotFile = Mod::get()->getSavedValue<std::string>(MoreIcons::getDual("robot", true), "");
            if (robotFile.empty() || !MoreIcons::hasRobot(robotFile)) return;
            MoreIcons::useCustomRobot(m_robotSprite, robotFile);
        }
        else return;

        m_robotBatchNode->initWithTexture(m_robotSprite->getTexture(), 25);
        m_robotBatchNode->addChild(m_robotSprite);
    }

    void createSpider(int frame) {
        PlayerObject::createSpider(frame);

        if (!m_gameLayer) return;

        if (!m_gameLayer->m_player1 || m_gameLayer->m_player1 == this) {
            auto spiderFile = Mod::get()->getSavedValue<std::string>(MoreIcons::getDual("spider", false), "");
            if (spiderFile.empty() || !MoreIcons::hasSpider(spiderFile)) return;
            MoreIcons::useCustomSpider(m_spiderSprite, spiderFile);
        }
        else if (!m_gameLayer->m_player2 || m_gameLayer->m_player2 == this) {
            auto spiderFile = Mod::get()->getSavedValue<std::string>(MoreIcons::getDual("spider", true), "");
            if (spiderFile.empty() || !MoreIcons::hasSpider(spiderFile)) return;
            MoreIcons::useCustomSpider(m_spiderSprite, spiderFile);
        }
        else return;

        m_spiderBatchNode->initWithTexture(m_spiderSprite->getTexture(), 25);
        m_spiderBatchNode->addChild(m_spiderSprite);
    }

    void updatePlayerSwingFrame(int frame) {
        PlayerObject::updatePlayerSwingFrame(frame);

        if (!m_gameLayer) return;

        if (m_gameLayer->m_player1 == this) useCustomSwing(false);
        else if (m_gameLayer->m_player2 == this) useCustomSwing(true);
    }

    void useCustomSwing(bool dual) {
        auto swingFile = Mod::get()->getSavedValue<std::string>(MoreIcons::getDual("swing", dual), "");
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

        if (!m_gameLayer) return;

        if (m_gameLayer->m_player1 == this) useCustomJetpack(false);
        else if (m_gameLayer->m_player2 == this) useCustomJetpack(true);
    }

    void useCustomJetpack(bool dual) {
        auto jetpackFile = Mod::get()->getSavedValue<std::string>(MoreIcons::getDual("jetpack", dual), "");
        if (jetpackFile.empty() || !MoreIcons::hasJetpack(jetpackFile)) return;

        auto jetpack = fmt::format("{}_001.png"_spr, jetpackFile);
        auto jetpack2 = fmt::format("{}_2_001.png"_spr, jetpackFile);
        auto jetpackExtra = fmt::format("{}_extra_001.png"_spr, jetpackFile);
        auto jetpackGlow = fmt::format("{}_glow_001.png"_spr, jetpackFile);

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

    void setupStreak() {
        PlayerObject::setupStreak();

        if (!m_gameLayer) return;

        if (!m_gameLayer->m_player1 || m_gameLayer->m_player1 == this) useCustomTrail(false);
        else if (!m_gameLayer->m_player2 || m_gameLayer->m_player2 == this) useCustomTrail(true);
    }

    void useCustomTrail(bool dual) {
        auto trailFile = Mod::get()->getSavedValue<std::string>(MoreIcons::getDual("trail", dual), "");
        if (trailFile.empty() || !MoreIcons::hasTrail(trailFile)) return;

        m_streakRelated1 = 14.0f;
        m_streakRelated2 = true;
        m_streakRelated3 = false;
        if (m_regularTrail) m_regularTrail->removeFromParent();
        m_regularTrail = CCMotionStreak::create(0.3f, 5.0f, 14.0f, { 255, 255, 255 },
            CCTextureCache::get()->textureForKey(MoreIcons::TRAIL_TEXTURES[trailFile].c_str()));
        m_regularTrail->setM_fMaxSeg(50.0f);
        m_parentLayer->addChild(m_regularTrail, -2);
        m_regularTrail->stopStroke();
    }
};
