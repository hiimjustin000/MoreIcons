#include "MoreIcons.hpp"

using namespace geode::prelude;

$on_mod(Loaded) {
    MoreIcons::load();
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

std::vector<std::filesystem::directory_entry> MoreIcons::naturalSort(const std::filesystem::path& path) {
    std::vector<std::filesystem::directory_entry> entries;
    for (auto& entry : std::filesystem::directory_iterator(path)) {
        entries.push_back(entry);
    }
    std::sort(entries.begin(), entries.end(), [](const std::filesystem::directory_entry& a, const std::filesystem::directory_entry& b) {
        auto aStr = a.path().filename().string();
        auto bStr = b.path().filename().string();

        auto aIt = aStr.begin();
        auto bIt = bStr.begin();

        while (aIt != aStr.end() && bIt != bStr.end()) {
            if (std::isdigit(*aIt) && std::isdigit(*bIt)) {
                std::string aNum, bNum;
                while (std::isdigit(*aIt)) aNum += *aIt++;
                while (std::isdigit(*bIt)) bNum += *bIt++;
                if (aNum != bNum) return std::stoi(aNum) < std::stoi(bNum);
            }
            else {
                auto aLower = std::tolower(*aIt);
                auto bLower = std::tolower(*bIt);
                if (aLower != bLower) return aLower < bLower;
                aIt++;
                bIt++;
            }
        }

        return aStr.size() < bStr.size();
    });
    return entries;
}

// The cooler https://github.com/Alphalaneous/HappyTextures/blob/1.5.0/src/Utils.h#L60
std::vector<std::filesystem::path> MoreIcons::getTexturePacks() {
    std::vector<std::filesystem::path> packs;
    packs.push_back(Mod::get()->getConfigDir());
    if (auto textureLoader = Loader::get()->getLoadedMod("geode.texture-loader")) {
        auto searchPaths = CCFileUtils::get()->getSearchPaths();
        auto texturePacks = textureLoader->getConfigDir() / "packs";
        auto unzippedPacks = textureLoader->getSaveDir() / "unzipped";
        auto modID = Mod::get()->getID();
        for (auto searchPath : searchPaths) {
            auto path = std::filesystem::path(searchPath);
            auto parentPath = std::filesystem::path(searchPath);
            while (parentPath.has_parent_path()) {
                if (parentPath == texturePacks || parentPath == unzippedPacks && std::find(packs.begin(), packs.end(), path) == packs.end()) {
                    auto configPath = path / "config" / modID;
                    if (std::filesystem::exists(configPath)) packs.push_back(configPath);
                    break;
                }
                if (parentPath == std::filesystem::current_path().root_path()) break;
                parentPath = parentPath.parent_path();
            }
        }
    }
    return packs;
}

void MoreIcons::loadIcons(
    const std::filesystem::path& path, std::vector<std::string>& list, std::vector<std::string>& duplicates,
    std::unordered_map<std::string, std::string>& textures, IconType type, bool create
) {
    auto folder = path.filename().string();
    log::info("Loading {}s from {}", folder, path.string());
    if (LOADING_LAYER)
        static_cast<CCLabelBMFont*>(LOADING_LAYER->getChildByID("geode-small-label-2"))->setString(fmt::format("More Icons: Loading {}s", folder).c_str());
    if (!std::filesystem::exists(path)) {
        if (create) std::filesystem::create_directories(path);
        return;
    }

    auto textureCache = CCTextureCache::get();
    auto textureQuality = CCDirector::get()->getLoadedTextureQuality();
    for (auto& entry : naturalSort(path)) {
        if (!entry.is_regular_file()) continue;

        auto entryPath = entry.path();
        if (entryPath.extension() != ".plist") continue;

        auto pathFilename = entryPath.filename().string();
        auto fileQuality = kTextureQualityLow;
        if (pathFilename.find("-uhd.plist") != std::string::npos) {
            auto hdExists = std::filesystem::exists(string::replace(entryPath.string(), "-uhd.plist", "-hd.plist"));
            if (hdExists || textureQuality != kTextureQualityHigh) {
                if (!hdExists) log::warn("Ignoring too high quality plist file: {}", pathFilename);
                continue;
            }
            else fileQuality = kTextureQualityHigh;
        }
        else if (pathFilename.find("-hd.plist") != std::string::npos) {
            auto sdExists = std::filesystem::exists(string::replace(entryPath.string(), "-hd.plist", ".plist"));
            if (sdExists || (textureQuality != kTextureQualityHigh && textureQuality != kTextureQualityMedium)) {
                if (!sdExists) log::warn("Ignoring too high quality plist file: {}", pathFilename);
                continue;
            }
            else fileQuality = kTextureQualityMedium;
        }

        auto plistPath = entryPath.string();
        auto noGraphicPlistPath = string::replace(string::replace(plistPath, "-uhd.plist", ".plist"), "-hd.plist", ".plist");
        auto possibleUHD = string::replace(noGraphicPlistPath, ".plist", "-uhd.plist");
        auto possibleHD = string::replace(noGraphicPlistPath, ".plist", "-hd.plist");
        auto usedTextureQuality = kTextureQualityLow;
        if (textureQuality == kTextureQualityHigh && (fileQuality == kTextureQualityHigh || std::filesystem::exists(possibleUHD))) {
            plistPath = possibleUHD;
            usedTextureQuality = kTextureQualityHigh;
        }
        else if (textureQuality == kTextureQualityMedium && (fileQuality == kTextureQualityMedium || std::filesystem::exists(possibleHD))) {
            plistPath = possibleHD;
            usedTextureQuality = kTextureQualityMedium;
        }

        auto dict = CCDictionary::createWithContentsOfFileThreadSafe(plistPath.c_str());
        auto frames = CCDictionary::create();
        auto name = std::filesystem::path(noGraphicPlistPath).stem().string();
        if (std::find(list.begin(), list.end(), name) != list.end()) {
            duplicates.push_back(name);
            name += fmt::format("_{:02}", std::count(duplicates.begin(), duplicates.end(), name));
        }
        for (auto [frameName, frame] : CCDictionaryExt<std::string, CCDictionary*>(static_cast<CCDictionary*>(dict->objectForKey("frames")))) {
            frames->setObject(frame, getFrameName(frameName, name, type));
        }
        dict->setObject(frames, "frames");
        auto metadata = static_cast<CCDictionary*>(dict->objectForKey("metadata"));
        auto fullTexturePath = (entryPath.parent_path() / std::filesystem::path(metadata->valueForKey("textureFileName")->getCString()).filename()).string();
        if (!std::filesystem::exists(fullTexturePath)) {
            log::warn("Texture file not found: {}", fullTexturePath);
            continue;
        }
        _addSpriteFramesWithDictionary(dict, textureCache->addImage(fullTexturePath.c_str(), false));
        dict->release();
        list.push_back(name);
        textures.emplace(name, fullTexturePath);
    }

    Mod::get()->setSavedValue(path.filename().string() + "s", list);
}

void MoreIcons::loadTrails(const std::filesystem::path& path, std::vector<std::string>& duplicates, bool create) {
    auto folder = path.filename().string();
    log::info("Loading trails from {}", path.string());
    if (LOADING_LAYER)
        static_cast<CCLabelBMFont*>(LOADING_LAYER->getChildByID("geode-small-label-2"))->setString("More Icons: Loading Trails");
    if (!std::filesystem::exists(path)) {
        if (create) std::filesystem::create_directories(path);
        return;
    }

    auto textureCache = CCTextureCache::get();
    for (auto& entry : naturalSort(path)) {
        if (!entry.is_regular_file()) continue;

        auto entryPath = entry.path();
        if (entryPath.extension() != ".png") continue;

        auto name = entryPath.stem().string();
        if (std::find(TRAILS.begin(), TRAILS.end(), name) != TRAILS.end()) {
            duplicates.push_back(name);
            name += fmt::format("_{:02}", std::count(duplicates.begin(), duplicates.end(), name));
        }

        auto fullTexturePath = entryPath.string();
        textureCache->addImage(fullTexturePath.c_str(), false);
        TRAILS.push_back(name);
        TRAIL_TEXTURES.emplace(name, fullTexturePath);
    }
}

void MoreIcons::changeSimplePlayer(SimplePlayer* player, const std::string& file, IconType iconType) {
    if (!player) return;

    if (iconType == IconType::Robot) {
        useCustomRobot(player->m_robotSprite, file);
        return;
    }
    else if (iconType == IconType::Spider) {
        useCustomSpider(player->m_spiderSprite, file);
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

void MoreIcons::useCustomSprite(GJRobotSprite* robot, const std::string& file) {
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
