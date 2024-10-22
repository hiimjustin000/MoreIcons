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
    std::sort(vec.begin(), vec.end(), [](const std::string& aStr, const std::string& bStr) {
        auto a = aStr.substr(aStr.find_first_of(':') + 1);
        auto b = bStr.substr(bStr.find_first_of(':') + 1);
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

void MoreIcons::load(LoadingLayer* layer) {
    auto packs = getTexturePacks();
    { loadIcons(packs, "icon", IconType::Cube); }
    { loadIcons(packs, "ship", IconType::Ship); }
    { loadIcons(packs, "ball", IconType::Ball); }
    { loadIcons(packs, "ufo", IconType::Ufo); }
    { loadIcons(packs, "wave", IconType::Wave); }
    { loadIcons(packs, "robot", IconType::Robot); }
    { loadIcons(packs, "spider", IconType::Spider); }
    { loadIcons(packs, "swing", IconType::Swing); }
    { loadIcons(packs, "jetpack", IconType::Jetpack); }
    { loadTrails(packs); }

    sharedPool().wait();

    {
        std::lock_guard lock(IMAGE_MUTEX);
        auto textureCache = CCTextureCache::get();
        auto spriteFrameCache = CCSpriteFrameCache::get();
        for (auto& image : IMAGES) {
            auto texture = new CCTexture2D();
            if (texture->initWithImage(image.image)) {
                textureCache->m_pTextures->setObject(texture, image.texturePath);
                if (!image.dict && !image.frameName.empty())
                    spriteFrameCache->addSpriteFrame(
                        CCSpriteFrame::createWithTexture(texture, { { 0, 0 }, texture->getContentSize() }),
                        image.frameName.c_str()
                    );
                else if (image.dict) {
                    _addSpriteFramesWithDictionary(image.dict, texture);
                    CC_SAFE_RELEASE(image.dict);
                }
                if (image.index == 0) {
                    vectorForType(image.type).push_back(image.name);
                    if (image.type != IconType::Special) infoForType(image.type)[image.name] = image.pack;
                }
                if (image.type == IconType::Special) {
                    TRAIL_INFO[image.name] = {
                        .texture = image.texturePath,
                        .pack = image.pack,
                        .blend = image.blend,
                        .tint = image.tint
                    };
                }
            }

            texture->release();
            CC_SAFE_RELEASE(image.image);
        }

        IMAGES.clear();
        restoreSaved();
        naturalSort(ICONS);
        naturalSort(SHIPS);
        naturalSort(BALLS);
        naturalSort(UFOS);
        naturalSort(WAVES);
        naturalSort(ROBOTS);
        naturalSort(SPIDERS);
        naturalSort(SWINGS);
        naturalSort(JETPACKS);
        naturalSort(TRAILS);

        if (auto smallLabel2 = static_cast<CCLabelBMFont*>(layer->getChildByID("geode-small-label-2")))
            smallLabel2->setString("");
    }
}

void MoreIcons::loadIcons(const std::vector<std::filesystem::path>& packs, const std::string& suffix, IconType type) {
    int i = 0;
    for (auto& packPath : packs) { 
        std::string packName;
        std::string packID;
        if (i != 0) {
            auto rootPackPath = packPath.parent_path().parent_path();
            auto packJSON = rootPackPath / "pack.json";
            if (std::filesystem::exists(packJSON)) {
                std::ifstream file(packJSON);
                std::stringstream bufferStream;
                bufferStream << file.rdbuf();
                std::string error;
                auto tryJson = matjson::parse(bufferStream.str(), error);
                auto packFileName = rootPackPath.filename().string();
                if (!error.empty()) {
                    packName = packFileName;
                    packID = packFileName;
                }
                auto json = tryJson.value_or(matjson::Object());
                if (json.contains("name") && json["name"].is_string()) packName = json["name"].as_string();
                else packName = packFileName;
                if (json.contains("id") && json["id"].is_string()) packID = json["id"].as_string();
                else packID = packFileName;
            }
        }
        else {
            packName = "More Icons";
            packID = "";
        }

        auto path = packPath / suffix;
        if (!std::filesystem::exists(path)) {
            if (i == 0) std::filesystem::create_directories(path);
            continue;
        }
        log::info("Loading {}s from {}", suffix, path.string());

        for (auto& entry : naturalSort(path)) {
            if (!entry.is_regular_file() && !entry.is_directory()) continue;

            loadIcon(entry.path(), {
                .name = packName,
                .id = packID
            }, type);
        }

        i++;
    }
}

void MoreIcons::loadIcon(const std::filesystem::path& path, const TexturePack& pack, IconType type) {
    if (!std::filesystem::exists(path)) return;

    auto textureQuality = CCDirector::get()->getLoadedTextureQuality();
    if (std::filesystem::is_directory(path)) {
        sharedPool().detach_task([path, pack, textureQuality, type] {
            auto name = (!pack.id.empty() ? pack.id + ":" : "") + path.stem().string();
            auto textureCache = CCTextureCache::get();
            auto spriteFrameCache = CCSpriteFrameCache::get();
            int i = 0;
            for (auto& subEntry : naturalSort(path)) {
                if (!subEntry.is_regular_file()) continue;

                auto subEntryPath = subEntry.path();
                if (subEntryPath.extension() != ".png") continue;

                auto pathFilename = subEntryPath.filename().string();
                auto fileQuality = kTextureQualityLow;
                if (pathFilename.ends_with("-uhd.png")) {
                    if (textureQuality != kTextureQualityHigh) {
                        auto logMessage = fmt::format("{}: Ignoring high-quality PNG file for {} texture quality", subEntryPath.string(),
                            textureQuality == kTextureQualityMedium ? "medium" : "low");
                        log::warn("{}", logMessage);
                        {
                            std::lock_guard lock(LOG_MUTEX);
                            LOGS.push_back({ .message = logMessage, .type = LogType::Info });
                        }
                        continue;
                    }

                    fileQuality = kTextureQualityHigh;
                }
                else if (pathFilename.ends_with("-hd.png")) {
                    if (textureQuality != kTextureQualityHigh && textureQuality != kTextureQualityMedium) {
                        auto logMessage = fmt::format("{}: Ignoring medium-quality PNG file for low texture quality", subEntryPath.string());
                        log::warn("{}", logMessage);
                        {
                            std::lock_guard lock(LOG_MUTEX);
                            LOGS.push_back({ .message = logMessage, .type = LogType::Info });
                        }
                        continue;
                    }

                    if (
                        std::filesystem::exists(replaceEnd(subEntryPath.string(), "-hd.png", "-uhd.png")) &&
                        textureQuality == kTextureQualityHigh
                    ) continue;
                    else fileQuality = kTextureQualityMedium;
                }
                else {
                    if (
                        std::filesystem::exists(replaceEnd(subEntryPath.string(), ".png", "-uhd.png")) &&
                        textureQuality == kTextureQualityHigh
                    ) continue;
                    else if (
                        std::filesystem::exists(replaceEnd(subEntryPath.string(), ".png", "-hd.png")) &&
                        (textureQuality == kTextureQualityMedium || textureQuality == kTextureQualityHigh)
                    ) continue;
                    else fileQuality = kTextureQualityLow;
                }

                auto pngPath = subEntryPath.string();
                std::string noGraphicPngPath;
                if (fileQuality == kTextureQualityHigh) noGraphicPngPath = replaceEnd(pngPath, "-uhd.png", ".png");
                else if (fileQuality == kTextureQualityMedium) noGraphicPngPath = replaceEnd(pngPath, "-hd.png", ".png");
                else noGraphicPngPath = pngPath;
                auto image = new CCImage();
                if (image->initWithImageFileThreadSafe(pngPath.c_str())) {
                    std::lock_guard lock(IMAGE_MUTEX);
                    IMAGES.push_back({
                        .image = image,
                        .dict = nullptr,
                        .texturePath = pngPath,
                        .name = name,
                        .frameName = getFrameName(std::filesystem::path(noGraphicPngPath).filename().string(), name, type),
                        .pack = pack,
                        .type = type,
                        .index = i
                    });

                    i++;
                }
                else image->release();
            }
        });
    }
    else if (std::filesystem::is_regular_file(path)) {
        if (path.extension() != ".plist") return;

        sharedPool().detach_task([path, pack, textureQuality, type] {
            auto pathFilename = path.filename().string();
            auto fileQuality = kTextureQualityLow;
            if (pathFilename.ends_with("-uhd.plist")) {
                if (textureQuality != kTextureQualityHigh) {
                    auto logMessage = fmt::format("{}: Ignoring high-quality PLIST file for {} texture quality", path.string(),
                        textureQuality == kTextureQualityMedium ? "medium" : "low");
                    log::warn("{}", logMessage);
                    {
                        std::lock_guard lock(LOG_MUTEX);
                        LOGS.push_back({ .message = logMessage, .type = LogType::Info });
                    }
                    return;
                }

                fileQuality = kTextureQualityHigh;
            }
            else if (pathFilename.ends_with("-hd.plist")) {
                if (textureQuality != kTextureQualityHigh && textureQuality != kTextureQualityMedium) {
                    auto logMessage = fmt::format("{}: Ignoring medium-quality PLIST file for low texture quality", path.string());
                    log::warn("{}", logMessage);
                    {
                        std::lock_guard lock(LOG_MUTEX);
                        LOGS.push_back({ .message = logMessage, .type = LogType::Info });
                    }
                    return;
                }

                if (
                    std::filesystem::exists(replaceEnd(path.string(), "-hd.plist", "-uhd.plist")) &&
                    textureQuality == kTextureQualityHigh
                ) return;
                else fileQuality = kTextureQualityMedium;
            }
            else {
                if (
                    std::filesystem::exists(replaceEnd(path.string(), ".plist", "-uhd.plist")) &&
                    textureQuality == kTextureQualityHigh
                ) return;
                else if (
                    std::filesystem::exists(replaceEnd(path.string(), ".plist", "-hd.plist")) &&
                    (textureQuality == kTextureQualityMedium || textureQuality == kTextureQualityHigh)
                ) return;
                else fileQuality = kTextureQualityLow;
            }

            auto plistPath = path.string();
            std::string noGraphicPlistPath;
            if (fileQuality == kTextureQualityHigh) noGraphicPlistPath = replaceEnd(plistPath, "-uhd.plist", ".plist");
            else if (fileQuality == kTextureQualityMedium) noGraphicPlistPath = replaceEnd(plistPath, "-hd.plist", ".plist");
            else noGraphicPlistPath = plistPath;
            auto name = (!pack.id.empty() ? pack.id + ":" : "") + std::filesystem::path(noGraphicPlistPath).stem().string();

            auto dict = CCDictionary::createWithContentsOfFileThreadSafe(plistPath.c_str());
            auto frames = new CCDictionary();
            for (auto [frameName, frame] : CCDictionaryExt<std::string, CCDictionary*>(static_cast<CCDictionary*>(dict->objectForKey("frames")))) {
                frames->setObject(frame, getFrameName(frameName, name, type));
            }
            dict->setObject(frames, "frames");
            auto metadata = static_cast<CCDictionary*>(dict->objectForKey("metadata"));
            auto texturePath = std::filesystem::path(metadata->valueForKey("textureFileName")->getCString()).filename().string();
            auto fullTexturePath = (path.parent_path() / texturePath).string();
            if (!std::filesystem::exists(fullTexturePath)) {
                auto fallbackTexturePath = std::filesystem::path(plistPath).replace_extension(".png").string();
                if (!std::filesystem::exists((fallbackTexturePath))) {
                    auto logMessage = fmt::format("{}: Texture file {} not found, no fallback", path.string(), texturePath); 
                    log::warn("{}", logMessage);
                    {
                        std::lock_guard lock(LOG_MUTEX);
                        LOGS.push_back({ .message = logMessage, .type = LogType::Error });
                        if (HIGHEST_SEVERITY < LogType::Error) HIGHEST_SEVERITY = LogType::Error;
                    }
                }
                else fullTexturePath = fallbackTexturePath;
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
                    .pack = pack,
                    .type = type,
                    .index = 0
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
        std::string packName;
        std::string packID;
        if (i != 0) {
            auto rootPackPath = packPath.parent_path().parent_path();
            auto packJSON = rootPackPath / "pack.json";
            if (std::filesystem::exists(packJSON)) {
                std::ifstream file(packJSON);
                std::stringstream bufferStream;
                bufferStream << file.rdbuf();
                std::string error;
                auto tryJson = matjson::parse(bufferStream.str(), error);
                auto packFileName = rootPackPath.filename().string();
                if (!error.empty()) {
                    packName = packFileName;
                    packID = packFileName;
                }
                auto json = tryJson.value_or(matjson::Object());
                if (json.contains("name") && json["name"].is_string()) packName = json["name"].as_string();
                else packName = packFileName;
                if (json.contains("id") && json["id"].is_string()) packID = json["id"].as_string();
                else packID = packFileName;
            }
        }
        else {
            packName = "More Icons";
            packID = "";
        }

        auto path = packPath / "trail";
        log::info("Loading trails from {}", path.string());
        if (!std::filesystem::exists(path)) {
            if (i == 0) std::filesystem::create_directories(path);
            return;
        }

        for (auto& entry : naturalSort(path)) {
            if (!entry.is_regular_file()) continue;

            auto entryPath = entry.path();
            if (entryPath.extension() != ".png") continue;

            loadTrail(entryPath, {
                .name = packName,
                .id = packID
            });
        }

        i++;
    }
}

void MoreIcons::loadTrail(const std::filesystem::path& path, const TexturePack& pack) {
    if (!std::filesystem::exists(path) || !std::filesystem::is_regular_file(path) || path.extension() != ".png") return;

    sharedPool().detach_task([path, pack] {
        auto name = (!pack.id.empty() ? pack.id + ":" : "") + path.stem().string();
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
                auto logMessage = fmt::format("{}: Failed to parse JSON file ({})", path.string(), error);
                log::warn("{}", logMessage);
                {
                    std::lock_guard lock(LOG_MUTEX);
                    LOGS.push_back({ .message = logMessage, .type = LogType::Warn });
                    if (HIGHEST_SEVERITY < LogType::Warn) HIGHEST_SEVERITY = LogType::Warn;
                }
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
                .pack = pack,
                .type = IconType::Special,
                .index = 0,
                .blend = json.contains("blend") && json["blend"].is_bool() ? json["blend"].as_bool() : false,
                .tint = json.contains("tint") && json["tint"].is_bool() ? json["tint"].as_bool() : false,
            });
        }
        else image->release();
    });
}
