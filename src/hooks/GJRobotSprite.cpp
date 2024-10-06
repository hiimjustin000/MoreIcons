#include "../MoreIcons.hpp"
#include "GJRobotSprite.hpp"

using namespace geode::prelude;

void MIRobotSprite::useCustom(GJRobotSprite* robot, std::string file) {
    auto spriteParts = robot->m_paSprite->m_spriteParts;
    auto spriteFrameCache = CCSpriteFrameCache::get();
    for (int i = 0; i < spriteParts->count(); i++) {
        auto spritePart = static_cast<CCSpritePart*>(spriteParts->objectAtIndex(i));
        auto tag = spritePart->getTag();

        auto spriteFrame = fmt::format("{}_{:02}_001.png"_spr, file, tag);
        auto sprite2Frame = fmt::format("{}_{:02}_2_001.png"_spr, file, tag);
        auto spriteExtraFrame = fmt::format("{}_{:02}_extra_001.png"_spr, file, tag);
        auto spriteGlowFrame = fmt::format("{}_{:02}_glow_001.png"_spr, file, tag);

        spritePart->setBatchNode(nullptr);
        spritePart->setDisplayFrame(spriteFrameCache->spriteFrameByName(spriteFrame.c_str()));
        if (auto secondSprite = static_cast<CCSprite*>(robot->m_secondArray->objectAtIndex(i))) {
            secondSprite->setBatchNode(nullptr);
            secondSprite->setDisplayFrame(spriteFrameCache->spriteFrameByName(sprite2Frame.c_str()));
            secondSprite->setPosition(spritePart->getContentSize() / 2);
        }

        auto glowChild = static_cast<CCSprite*>(robot->m_glowSprite->getChildren()->objectAtIndex(i));
        glowChild->setBatchNode(nullptr);
        glowChild->setDisplayFrame(spriteFrameCache->spriteFrameByName(spriteGlowFrame.c_str()));

        if (spritePart == robot->m_headSprite) {
            auto extraFrame = spriteFrameCache->spriteFrameByName(spriteExtraFrame.c_str());
            auto hasExtra = MoreIcons::doesExist(extraFrame);
            if (hasExtra) {
                if (robot->m_extraSprite) {
                    robot->m_extraSprite->setBatchNode(nullptr);
                    robot->m_extraSprite->setDisplayFrame(extraFrame);
                }
                else {
                    robot->m_extraSprite = CCSprite::createWithSpriteFrame(extraFrame);
                    robot->m_headSprite->addChild(robot->m_extraSprite, 2);
                }
                robot->m_extraSprite->setPosition(spritePart->getContentSize() / 2);
            }
            robot->m_extraSprite->setVisible(hasExtra);
        }
    }
}

void MIRobotSprite::useCustomSprite(GJRobotSprite* robot, bool dual) {
    if (robot->m_iconType == IconType::Robot) useCustomRobot(robot, Mod::get()->getSavedValue<std::string>(MoreIcons::getDual("robot", dual), ""));
    else if (robot->m_iconType == IconType::Spider) useCustomSpider(robot, Mod::get()->getSavedValue<std::string>(MoreIcons::getDual("spider", dual), ""));
}

void MIRobotSprite::useCustomRobot(GJRobotSprite* robot, std::string robotFile) {
    if (robotFile.empty() || !MoreIcons::hasRobot(robotFile)) return;
    robot->setBatchNode(nullptr);
    robot->setTexture(CCTextureCache::get()->textureForKey(MoreIcons::ROBOT_TEXTURES[robotFile].c_str()));
    useCustom(robot, robotFile);
}

void MIRobotSprite::useCustomSpider(GJRobotSprite* spider, std::string spiderFile) {
    if (spiderFile.empty() || !MoreIcons::hasSpider(spiderFile)) return;
    spider->setBatchNode(nullptr);
    spider->setTexture(CCTextureCache::get()->textureForKey(MoreIcons::SPIDER_TEXTURES[spiderFile].c_str()));
    useCustom(spider, spiderFile);
}
