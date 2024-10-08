#include "MoreIcons.hpp"

using namespace geode::prelude;

$on_mod(DataSaved) {
    MoreIcons::saveTrails();
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

void MoreIcons::naturalSort(std::vector<std::string>& vec) {
    std::sort(vec.begin(), vec.end(), [](const std::string& a, const std::string& b) {
        auto aIt = a.begin();
        auto bIt = b.begin();

        while (aIt != a.end() && bIt != b.end()) {
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

        return a.size() < b.size();
    });
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

void MoreIcons::loadIcons(const std::vector<std::filesystem::path>& packs, const std::string& suffix, IconType type) {
    int i = 0;
    for (auto& packPath : packs) {
        auto path = packPath / suffix;
        if (!std::filesystem::exists(path)) {
            if (i == 0) std::filesystem::create_directories(path);
            continue;
        }
        log::info("Loading {}s from {}", suffix, path.string());

        auto textureCache = CCTextureCache::get();
        auto textureQuality = CCDirector::get()->getLoadedTextureQuality();
        auto spriteFrameCache = CCSpriteFrameCache::get();
        for (auto& entry : naturalSort(path)) {
            if (!entry.is_regular_file() && !entry.is_directory()) continue;

            loadIcon(entry.path(), type);
        }

        i++;
    }
}

void MoreIcons::loadIcon(const std::filesystem::path& path, IconType type) {
    if (!std::filesystem::exists(path)) return;

    auto textureQuality = CCDirector::get()->getLoadedTextureQuality();
    if (std::filesystem::is_directory(path)) {
        auto name = path.stem().string();
        if (std::find(ALL.begin(), ALL.end(), name) != ALL.end()) {
            DUPLICATES.push_back(name);
            name += fmt::format("_{:02}", std::count(DUPLICATES.begin(), DUPLICATES.end(), name));
        }
        ALL.push_back(name);

        sharedPool().detach_task([path, textureQuality, name, type] {
            auto textureCache = CCTextureCache::get();
            auto spriteFrameCache = CCSpriteFrameCache::get();
            for (auto& subEntry : naturalSort(path)) {
                if (!subEntry.is_regular_file()) continue;

                auto subEntryPath = subEntry.path();
                if (subEntryPath.extension() != ".png") continue;

                auto pathFilename = subEntryPath.filename().string();
                auto fileQuality = kTextureQualityLow;
                if (pathFilename.find("-uhd.png") != std::string::npos) {
                    auto hdExists = std::filesystem::exists(string::replace(subEntryPath.string(), "-uhd.png", "-hd.png"));
                    if (hdExists || textureQuality != kTextureQualityHigh) {
                        if (!hdExists) log::warn("Ignoring too high quality PNG file: {}", path.filename() / pathFilename);
                        continue;
                    }
                    else fileQuality = kTextureQualityHigh;
                }
                else if (pathFilename.find("-hd.png") != std::string::npos) {
                    auto sdExists = std::filesystem::exists(string::replace(subEntryPath.string(), "-hd.png", ".png"));
                    if (sdExists || (textureQuality != kTextureQualityHigh && textureQuality != kTextureQualityMedium)) {
                        if (!sdExists) log::warn("Ignoring too high quality PNG file: {}", path.filename() / pathFilename);
                        continue;
                    }
                    else fileQuality = kTextureQualityMedium;
                }

                auto pngPath = subEntryPath.string();
                auto noGraphicPngPath = string::replace(string::replace(pngPath, "-uhd.png", ".png"), "-hd.png", ".png");
                auto possibleUHD = string::replace(noGraphicPngPath, ".png", "-uhd.png");
                auto possibleHD = string::replace(noGraphicPngPath, ".png", "-hd.png");
                auto usedTextureQuality = kTextureQualityLow;
                if (textureQuality == kTextureQualityHigh && (fileQuality == kTextureQualityHigh || std::filesystem::exists(possibleUHD))) {
                    pngPath = possibleUHD;
                    usedTextureQuality = kTextureQualityHigh;
                }
                else if (textureQuality == kTextureQualityMedium && (fileQuality == kTextureQualityMedium || std::filesystem::exists(possibleHD))) {
                    pngPath = possibleHD;
                    usedTextureQuality = kTextureQualityMedium;
                }

                auto image = new CCImage();
                if (image->initWithImageFileThreadSafe(pngPath.c_str())) {
                    std::lock_guard lock(IMAGE_MUTEX);
                    IMAGES.push_back({
                        .image = image,
                        .dict = nullptr,
                        .texturePath = pngPath,
                        .name = name,
                        .frameName = getFrameName(std::filesystem::path(noGraphicPngPath).filename().string(), name, type),
                        .type = type
                    });
                }
                else image->release();
            }
        });
    }
    else if (std::filesystem::is_regular_file(path)) {
        if (path.extension() != ".plist") return;

        auto name = string::replace(string::replace(path.filename().string(), "-uhd.plist", ""), "-hd.plist", "");
        if (std::find(ALL.begin(), ALL.end(), name) != ALL.end()) {
            DUPLICATES.push_back(name);
            name += fmt::format("_{:02}", std::count(DUPLICATES.begin(), DUPLICATES.end(), name));
        }
        ALL.push_back(name);

        sharedPool().detach_task([path, textureQuality, name, type] {
            auto pathFilename = path.filename().string();
            auto fileQuality = kTextureQualityLow;
            if (pathFilename.find("-uhd.plist") != std::string::npos) {
                auto hdExists = std::filesystem::exists(string::replace(path.string(), "-uhd.plist", "-hd.plist"));
                if (hdExists || textureQuality != kTextureQualityHigh) {
                    if (!hdExists) log::warn("Ignoring too high quality plist file: {}", pathFilename);
                    return;
                }
                else fileQuality = kTextureQualityHigh;
            }
            else if (pathFilename.find("-hd.plist") != std::string::npos) {
                auto sdExists = std::filesystem::exists(string::replace(path.string(), "-hd.plist", ".plist"));
                if (sdExists || (textureQuality != kTextureQualityHigh && textureQuality != kTextureQualityMedium)) {
                    if (!sdExists) log::warn("Ignoring too high quality plist file: {}", pathFilename);
                    return;
                }
                else fileQuality = kTextureQualityMedium;
            }

            auto plistPath = path.string();
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
            auto frames = new CCDictionary();
            for (auto [frameName, frame] : CCDictionaryExt<std::string, CCDictionary*>(static_cast<CCDictionary*>(dict->objectForKey("frames")))) {
                frames->setObject(frame, getFrameName(frameName, name, type));
            }
            dict->setObject(frames, "frames");
            auto metadata = static_cast<CCDictionary*>(dict->objectForKey("metadata"));
            auto fullTexturePath = (path.parent_path() / std::filesystem::path(metadata->valueForKey("textureFileName")->getCString()).filename()).string();
            if (!std::filesystem::exists(fullTexturePath)) {
                log::warn("Texture file not found: {}", fullTexturePath);
                return;
            }

            auto image = new CCImage();
            if (image->initWithImageFileThreadSafe(fullTexturePath.c_str())) {
                std::lock_guard lock(IMAGE_MUTEX);
                IMAGES.push_back({
                    .image = image,
                    .dict = dict,
                    .texturePath = fullTexturePath,
                    .name = name,
                    .frameName = "",
                    .type = type
                });
            }
            else {
                dict->release();
                image->release();
            }
        });
    }
}

void MoreIcons::loadTrails(const std::vector<std::filesystem::path>& packs) {
    int i = 0;
    for (auto& packPath : packs) {
        auto path = packPath / "trail";
        log::info("Loading trails from {}", path.string());
        if (!std::filesystem::exists(path)) {
            if (i == 0) std::filesystem::create_directories(path);
            return;
        }

        auto textureCache = CCTextureCache::get();
        for (auto& entry : naturalSort(path)) {
            if (!entry.is_regular_file()) continue;

            auto entryPath = entry.path();
            if (entryPath.extension() != ".png") continue;

            loadTrail(entryPath);
        }

        i++;
    }
}

void MoreIcons::loadTrail(const std::filesystem::path& path) {
    if (!std::filesystem::exists(path) || !std::filesystem::is_regular_file(path) || path.extension() != ".png") return;

    auto name = path.stem().string();
    if (std::find(TRAILS.begin(), TRAILS.end(), name) != TRAILS.end()) {
        TRAIL_DUPLICATES.push_back(name);
        name += fmt::format("_{:02}", std::count(TRAIL_DUPLICATES.begin(), TRAIL_DUPLICATES.end(), name));
    }

    sharedPool().detach_task([path, name] {
        auto jsonPath = std::filesystem::path(path).replace_extension(".json");
        matjson::Value json;
        if (!std::filesystem::exists(jsonPath)) json = matjson::Object { { "blend", false }, { "tint", false } };
        else {
            std::ifstream file(jsonPath);
            std::stringstream bufferStream;
            bufferStream << file.rdbuf();
            std::string error;
            auto tryJson = matjson::parse(bufferStream.str(), error);
            if (!error.empty()) {
                log::warn("Failed to parse JSON file {}: {}", jsonPath.filename().string(), error);
                json = matjson::Object { { "blend", false }, { "tint", false } };
            }
            else json = tryJson.value_or(matjson::Object { { "blend", false }, { "tint", false } });
        }

        auto fullTexturePath = path.string();
        auto image = new CCImage();
        if (image->initWithImageFileThreadSafe(fullTexturePath.c_str())) {
            std::lock_guard lock(IMAGE_MUTEX);
            IMAGES.push_back({
                .image = image,
                .dict = nullptr,
                .texturePath = fullTexturePath,
                .name = name,
                .frameName = "",
                .type = IconType::Special,
                .blend = json.contains("blend") && json["blend"].is_bool() ? json["blend"].as_bool() : false,
                .tint = json.contains("tint") && json["tint"].is_bool() ? json["tint"].as_bool() : false,
            });
        }
        else image->release();
    });
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
