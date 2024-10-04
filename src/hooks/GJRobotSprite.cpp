#include "../MoreIcons.hpp"
#include "GJRobotSprite.hpp"

using namespace geode::prelude;

void MIRobotSprite::updateFrame(int frame) {
    GJRobotSprite::updateFrame(frame);

    auto gameLayer = GJBaseGameLayer::get();
    if (!gameLayer) return;

    if (!gameLayer->m_player1 || !gameLayer->m_player2) {
        useCustomSprite();
        return;
    }

    if (gameLayer->m_player1->m_robotSprite == this ||
        gameLayer->m_player2->m_robotSprite == this ||
        gameLayer->m_player1->m_spiderSprite == static_cast<GJRobotSprite*>(this) ||
        gameLayer->m_player2->m_spiderSprite == static_cast<GJRobotSprite*>(this)) useCustomSprite();
}

void MIRobotSprite::useCustom(std::string file) {
    auto spriteParts = m_paSprite->m_spriteParts;
    auto spriteFrameCache = CCSpriteFrameCache::get();
    for (int i = 0; i < spriteParts->count(); i++) {
        auto spritePart = static_cast<CCSpritePart*>(spriteParts->objectAtIndex(i));
        auto tag = spritePart->getTag();

        auto spriteFrame = fmt::format("{}_{:02}_001.png"_spr, file, tag);
        auto sprite2Frame = fmt::format("{}_{:02}_2_001.png"_spr, file, tag);
        auto spriteExtraFrame = fmt::format("{}_{:02}_extra_001.png"_spr, file, tag);
        auto spriteGlowFrame = fmt::format("{}_{:02}_glow_001.png"_spr, file, tag);

        spritePart->setDisplayFrame(spriteFrameCache->spriteFrameByName(spriteFrame.c_str()));
        if (auto secondSprite = static_cast<CCSprite*>(m_secondArray->objectAtIndex(i))) {
            secondSprite->setDisplayFrame(spriteFrameCache->spriteFrameByName(sprite2Frame.c_str()));
            secondSprite->setPosition(spritePart->getContentSize() / 2);
        }

        auto glowChild = static_cast<CCSprite*>(m_glowSprite->getChildren()->objectAtIndex(i));
        glowChild->setDisplayFrame(spriteFrameCache->spriteFrameByName(spriteGlowFrame.c_str()));

        if (spritePart == m_headSprite) {
            auto extraFrame = spriteFrameCache->spriteFrameByName(spriteExtraFrame.c_str());
            auto hasExtra = false;
            if (extraFrame && extraFrame->getFrameName() != "geode.texture-loader/fallback.png") {
                hasExtra = true;
                if (m_extraSprite) m_extraSprite->setDisplayFrame(extraFrame);
                else {
                    m_extraSprite = CCSprite::createWithSpriteFrame(extraFrame);
                    m_headSprite->addChild(m_extraSprite, 2);
                }
                m_extraSprite->setPosition(spritePart->getContentSize() / 2);
            }
            m_extraSprite->setVisible(hasExtra);
        }
    }
}

void MIRobotSprite::useCustomSprite() {
    if (m_iconType == IconType::Robot) useCustomRobot(Mod::get()->getSavedValue<std::string>("robot", ""));
    else if (m_iconType == IconType::Spider) useCustomSpider(Mod::get()->getSavedValue<std::string>("spider", ""));
}

void MIRobotSprite::useCustomRobot(std::string robotFile) {
    if (robotFile.empty() || !MoreIcons::hasRobot(robotFile)) return;
    setTexture(CCTextureCache::get()->textureForKey(MoreIcons::ROBOT_TEXTURES[robotFile].c_str()));
    useCustom(robotFile);
}

void MIRobotSprite::useCustomSpider(std::string spiderFile) {
    if (spiderFile.empty() || !MoreIcons::hasSpider(spiderFile)) return;
    setTexture(CCTextureCache::get()->textureForKey(MoreIcons::SPIDER_TEXTURES[spiderFile].c_str()));
    useCustom(spiderFile);
}
