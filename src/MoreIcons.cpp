#include "MoreIcons.hpp"
#include "hooks/GJRobotSprite.hpp"

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

void MoreIcons::loadIcons(const std::filesystem::path& path, std::vector<std::string>& list, std::unordered_map<std::string, std::string>& textures) {
    log::info("Loading {}s", path.filename().string());
    if (!std::filesystem::exists(path)) {
        std::filesystem::create_directories(path);
        return;
    }

    auto textureCache = CCTextureCache::get();
    for (auto& entry : naturalSort(path)) {
        if (!entry.is_regular_file()) continue;

        auto path = entry.path();
        if (path.extension() != ".plist") continue;

        auto textureQuality = CCDirector::get()->getLoadedTextureQuality();
        auto pathFilename = path.filename().string();
        auto fileQuality = kTextureQualityLow;
        if (pathFilename.find("-uhd.plist") != std::string::npos) {
            auto hdExists = std::filesystem::exists(string::replace(path.string(), "-uhd.plist", "-hd.plist"));
            if (hdExists || textureQuality != kTextureQualityHigh) {
                if (!hdExists) log::warn("Ignoring too high quality plist file: {}", pathFilename);
                continue;
            }
            else {
                fileQuality = kTextureQualityHigh;
                log::info("Loading UHD plist file: {}", pathFilename);
            }
        }
        else if (pathFilename.find("-hd.plist") != std::string::npos) {
            auto sdExists = std::filesystem::exists(string::replace(path.string(), "-hd.plist", ".plist"));
            if (sdExists || (textureQuality != kTextureQualityHigh && textureQuality != kTextureQualityMedium)) {
                if (!sdExists) log::warn("Ignoring too high quality plist file: {}", pathFilename);
                continue;
            }
            else {
                fileQuality = kTextureQualityMedium;
                log::info("Loading HD plist file: {}", pathFilename);
            }
        }
        else log::info("Loading plist file: {}", pathFilename);

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
        auto frames = CCDictionary::create();
        for (auto [frameName, frame] : CCDictionaryExt<std::string, CCDictionary*>(static_cast<CCDictionary*>(dict->objectForKey("frames")))) {
            frames->setObject(frame, ""_spr + frameName);
        }
        dict->setObject(frames, "frames");
        auto metadata = static_cast<CCDictionary*>(dict->objectForKey("metadata"));
        auto fullTexturePath = (path.parent_path() / std::filesystem::path(metadata->valueForKey("textureFileName")->getCString()).filename()).string();
        if (!std::filesystem::exists(fullTexturePath)) {
            log::warn("Texture file not found: {}", fullTexturePath);
            continue;
        }
        _addSpriteFramesWithDictionary(dict, textureCache->addImage(fullTexturePath.c_str(), false));
        dict->release();
        auto name = std::filesystem::path(noGraphicPlistPath).stem().string();
        list.push_back(name);
        textures.emplace(name, fullTexturePath);
    }

    Mod::get()->setSavedValue(path.filename().string() + "s", list);
}

void MoreIcons::changeSimplePlayer(SimplePlayer* player, const std::string& file, IconType iconType) {
    if (!player) return;

    if (iconType == IconType::Robot) {
        MIRobotSprite::useCustomRobot(player->m_robotSprite, file);
        return;
    }
    else if (iconType == IconType::Spider) {
        MIRobotSprite::useCustomSpider(player->m_spiderSprite, file);
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
