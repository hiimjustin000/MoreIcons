#include "MoreIcons.hpp"
#include "hooks/GJRobotSprite.hpp"

using namespace geode::prelude;

$on_mod(Loaded) {
    auto configDir = Mod::get()->getConfigDir();
    std::unordered_map<std::string, std::string> tempMap;
    MoreIcons::loadIcons(configDir / "icon", MoreIcons::ICONS, tempMap);
    MoreIcons::loadIcons(configDir / "ship", MoreIcons::SHIPS, tempMap);
    MoreIcons::loadIcons(configDir / "ball", MoreIcons::BALLS, tempMap);
    MoreIcons::loadIcons(configDir / "ufo", MoreIcons::UFOS, tempMap);
    MoreIcons::loadIcons(configDir / "wave", MoreIcons::WAVES, tempMap);
    MoreIcons::loadIcons(configDir / "robot", MoreIcons::ROBOTS, MoreIcons::ROBOT_TEXTURES);
    MoreIcons::loadIcons(configDir / "spider", MoreIcons::SPIDERS, MoreIcons::SPIDER_TEXTURES);
    MoreIcons::loadIcons(configDir / "swing", MoreIcons::SWINGS, tempMap);
    MoreIcons::loadIcons(configDir / "jetpack", MoreIcons::JETPACKS, tempMap);
}

// https://github.com/GlobedGD/globed2/blob/v1.6.2/src/util/cocos.cpp#L44
namespace {
    template <typename TC>
    using priv_method_t = void(TC::*)(CCDictionary*, CCTexture2D*);

    template <typename TC, priv_method_t<TC> func>
    struct priv_caller {
        friend void _addSpriteFramesWithDictionary(CCDictionary* p1, CCTexture2D* p2) {
            auto* obj = CCSpriteFrameCache::sharedSpriteFrameCache();
            (obj->*func)(p1, p2);
        }
    };

    template struct priv_caller<CCSpriteFrameCache, &CCSpriteFrameCache::addSpriteFramesWithDictionary>;

    void _addSpriteFramesWithDictionary(CCDictionary* p1, CCTexture2D* p2);
}

void MoreIcons::loadIcons(const std::filesystem::path& path, std::vector<std::string>& list, std::unordered_map<std::string, std::string>& textures) {
    log::info("Loading {}s", path.filename().string());
    if (!std::filesystem::exists(path)) {
        std::filesystem::create_directories(path);
        return;
    }

    auto textureCache = CCTextureCache::get();
    for (auto& entry : std::filesystem::directory_iterator(path)) {
        auto path = entry.path();
        if (path.extension() != ".plist") continue;

        auto pathFilename = path.filename().string();
        if (pathFilename.find("-uhd.plist") != std::string::npos) {
            log::warn("Ignoring UHD plist file: {}", pathFilename);
            continue;
        }
        else if (pathFilename.find("-hd.plist") != std::string::npos) {
            log::warn("Ignoring HD plist file: {}", pathFilename);
            continue;
        }
        else log::info("Loading plist file: {}", pathFilename);

        auto plistPath = path.string();
        auto textureQuality = CCDirector::get()->getLoadedTextureQuality();
        auto possibleUHD = string::replace(plistPath, ".plist", "-uhd.plist");
        auto possibleHD = string::replace(plistPath, ".plist", "-hd.plist");
        auto usedTextureQuality = TextureQuality::kTextureQualityLow;
        if (textureQuality == kTextureQualityHigh && std::filesystem::exists(possibleUHD)) {
            plistPath = possibleUHD;
            usedTextureQuality = kTextureQualityHigh;
        }
        else if (textureQuality == kTextureQualityMedium && std::filesystem::exists(possibleHD)) {
            plistPath = possibleHD;
            usedTextureQuality = kTextureQualityMedium;
        }

        auto dict = CCDictionary::createWithContentsOfFileThreadSafe(plistPath.c_str());
        auto frames = CCDictionary::create();
        for (auto [frameName, frame] : CCDictionaryExt<std::string, CCDictionary*>(static_cast<CCDictionary*>(dict->objectForKey("frames")))) {
            frames->setObject(frame, ""_spr + frameName);
        }
        dict->setObject(frames, "frames");
        auto metadata = static_cast<CCDictionary*>(dict->objectForKey("metadata"));
        auto fullTexturePath = (path.parent_path() / metadata->valueForKey("textureFileName")->getCString()).string();
        if (!std::filesystem::exists(fullTexturePath)) {
            log::warn("Texture file not found: {}", fullTexturePath);
            continue;
        }
        _addSpriteFramesWithDictionary(dict, textureCache->addImage(fullTexturePath.c_str(), false));
        dict->release();
        auto name = path.stem().string();
        list.push_back(name);
        textures.emplace(name, fullTexturePath);
    }
}

void MoreIcons::changeSimplePlayer(SimplePlayer* player, const std::string& file, IconType iconType) {
    if (iconType == IconType::Robot) {
        static_cast<MIRobotSprite*>(player->m_robotSprite)->useCustomRobot(file);
        return;
    }
    else if (iconType == IconType::Spider) {
        static_cast<MIRobotSprite*>(static_cast<GJRobotSprite*>(player->m_spiderSprite))->useCustomSpider(file);
        return;
    }

    if (file.empty()) return;
    switch (iconType) {
        case IconType::Cube:
            if (!MoreIcons::hasIcon(file)) return;
            break;
        case IconType::Ship:
            if (!MoreIcons::hasShip(file)) return;
            break;
        case IconType::Ball:
            if (!MoreIcons::hasBall(file)) return;
            break;
        case IconType::Ufo:
            if (!MoreIcons::hasUfo(file)) return;
            break;
        case IconType::Wave:
            if (!MoreIcons::hasWave(file)) return;
            break;
        case IconType::Swing:
            if (!MoreIcons::hasSwing(file)) return;
            break;
        case IconType::Jetpack:;
            if (!MoreIcons::hasJetpack(file)) return;
            break;
        default:
            return;
    }

    auto sfc = CCSpriteFrameCache::get();
    auto icon = fmt::format("{}_001.png"_spr, file);
    auto icon2 = fmt::format("{}_2_001.png"_spr, file);
    auto icon3 = fmt::format("{}_3_001.png"_spr, file);
    auto iconExtra = fmt::format("{}_extra_001.png"_spr, file);
    auto iconGlow = fmt::format("{}_glow_001.png"_spr, file);

    player->m_firstLayer->setDisplayFrame(sfc->spriteFrameByName(icon.c_str()));
    player->m_secondLayer->setDisplayFrame(sfc->spriteFrameByName(icon2.c_str()));
    auto firstCenter = player->m_firstLayer->getContentSize() / 2;
    player->m_secondLayer->setPosition(firstCenter);
    player->m_outlineSprite->setDisplayFrame(sfc->spriteFrameByName(iconGlow.c_str()));
    player->m_outlineSprite->setPosition(firstCenter);
    if (iconType == IconType::Ufo) {
        player->m_birdDome->setDisplayFrame(sfc->spriteFrameByName(icon3.c_str()));
        player->m_birdDome->setPosition(firstCenter);
    }
    auto extraFrame = sfc->spriteFrameByName(iconExtra.c_str());
    auto extraVisible = doesExist(extraFrame);
    player->m_detailSprite->setVisible(extraVisible);
    if (extraVisible) {
        player->m_detailSprite->setDisplayFrame(extraFrame);
        player->m_detailSprite->setPosition(firstCenter);
    }
}
