#include "../MoreIcons.hpp"
#include "../classes/DummyNode.hpp"

using namespace geode::prelude;

#include <Geode/modify/PlayerObject.hpp>
class $modify(MIPlayerObject, PlayerObject) {
    static void onModify(auto& self) {
        (void)self.setHookPriority("PlayerObject::setupStreak", -1);
    }

    bool init(int player, int ship, GJBaseGameLayer* gameLayer, CCLayer* layer, bool highGraphics) {
        if (!PlayerObject::init(player, ship, gameLayer, layer, highGraphics)) return false;

        if (!gameLayer || (gameLayer->m_player1 && gameLayer->m_player2)) return true;

        if (!gameLayer->m_player1) {
            useCustomIcon(false);
            useCustomShip(false);
        }
        else if (!gameLayer->m_player2) {
            useCustomIcon(true);
            useCustomShip(true);
        }

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
        auto hasExisted = m_robotSprite != nullptr;

        PlayerObject::createRobot(frame);

        if (!m_gameLayer) return;

        if (!m_gameLayer->m_player1 || m_gameLayer->m_player1 == this) {
            auto robotFile = Mod::get()->getSavedValue<std::string>(MoreIcons::getDual("robot", false), "");
            if (robotFile.empty() || !MoreIcons::hasRobot(robotFile)) return;
            MoreIconsAPI::updateRobotSprite(m_robotSprite, robotFile, IconType::Robot);
        }
        else if (!m_gameLayer->m_player2 || m_gameLayer->m_player2 == this) {
            auto robotFile = Mod::get()->getSavedValue<std::string>(MoreIcons::getDual("robot", true), "");
            if (robotFile.empty() || !MoreIcons::hasRobot(robotFile)) return;
            MoreIconsAPI::updateRobotSprite(m_robotSprite, robotFile, IconType::Robot);
        }
        else return;

        m_robotSprite->retain();
        if (m_robotBatchNode) {
            m_robotSprite->removeFromParent();
            m_robotBatchNode->removeFromParent();
            m_robotBatchNode->release();
        }
        m_robotBatchNode = DummyNode::createWithTexture(m_robotSprite->getTexture(), 25);
        m_robotBatchNode->retain();
        m_robotBatchNode->addChild(m_robotSprite);
        if (hasExisted && m_isRobot) m_mainLayer->addChild(m_robotBatchNode, 2);
        m_robotSprite->release();
    }

    void createSpider(int frame) {
        auto hasExisted = m_spiderSprite != nullptr;

        PlayerObject::createSpider(frame);

        if (!m_gameLayer) return;

        if (!m_gameLayer->m_player1 || m_gameLayer->m_player1 == this) {
            auto spiderFile = Mod::get()->getSavedValue<std::string>(MoreIcons::getDual("spider", false), "");
            if (spiderFile.empty() || !MoreIcons::hasSpider(spiderFile)) return;
            MoreIconsAPI::updateRobotSprite(m_spiderSprite, spiderFile, IconType::Spider);
        }
        else if (!m_gameLayer->m_player2 || m_gameLayer->m_player2 == this) {
            auto spiderFile = Mod::get()->getSavedValue<std::string>(MoreIcons::getDual("spider", true), "");
            if (spiderFile.empty() || !MoreIcons::hasSpider(spiderFile)) return;
            MoreIconsAPI::updateRobotSprite(m_spiderSprite, spiderFile, IconType::Spider);
        }
        else return;

        m_spiderSprite->retain();
        if (m_spiderBatchNode) {
            m_spiderSprite->removeFromParent();
            m_spiderBatchNode->removeFromParent();
            m_spiderBatchNode->release();
        }
        m_spiderBatchNode = DummyNode::createWithTexture(m_spiderSprite->getTexture(), 25);
        m_spiderBatchNode->retain();
        m_spiderBatchNode->addChild(m_spiderSprite);
        if (hasExisted && m_isSpider) m_mainLayer->addChild(m_spiderBatchNode, 2);
        m_spiderSprite->release();
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

        auto trailInfo = MoreIcons::TRAIL_INFO[trailFile];
        m_streakRelated1 = 14.0f;
        m_streakRelated2 = !trailInfo.tint;
        m_streakRelated3 = false;
        m_regularTrail->initWithFade(0.3f, 5.0f, 14.0f, { 255, 255, 255 }, CCTextureCache::get()->textureForKey(trailInfo.texture.c_str()));
        if (trailInfo.blend) m_regularTrail->setBlendFunc({ GL_SRC_ALPHA, GL_ONE });
    }
};
