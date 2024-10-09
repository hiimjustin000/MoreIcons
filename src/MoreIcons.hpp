struct TrailInfo {
    std::string texture;
    bool blend;
    bool tint;
};

class MoreIcons {
public:
    static inline std::vector<std::string> ICONS;
    static inline std::vector<std::string> SHIPS;
    static inline std::vector<std::string> BALLS;
    static inline std::vector<std::string> UFOS;
    static inline std::vector<std::string> WAVES;
    static inline std::vector<std::string> ROBOTS;
    static inline std::vector<std::string> SPIDERS;
    static inline std::vector<std::string> SWINGS;
    static inline std::vector<std::string> JETPACKS;
    static inline std::vector<std::string> TRAILS;
    static inline std::unordered_map<std::string, TrailInfo> TRAIL_INFO;
    static inline LoadingLayer* LOADING_LAYER = nullptr;

    static bool hasIcon(const std::string& name) {
        return !ICONS.empty() && std::find(ICONS.begin(), ICONS.end(), name) != ICONS.end();
    }

    static bool hasShip(const std::string& name) {
        return !SHIPS.empty() && std::find(SHIPS.begin(), SHIPS.end(), name) != SHIPS.end();
    }

    static bool hasBall(const std::string& name) {
        return !BALLS.empty() && std::find(BALLS.begin(), BALLS.end(), name) != BALLS.end();
    }

    static bool hasUfo(const std::string& name) {
        return !UFOS.empty() && std::find(UFOS.begin(), UFOS.end(), name) != UFOS.end();
    }

    static bool hasWave(const std::string& name) {
        return !WAVES.empty() && std::find(WAVES.begin(), WAVES.end(), name) != WAVES.end();
    }

    static bool hasRobot(const std::string& name) {
        return !ROBOTS.empty() && std::find(ROBOTS.begin(), ROBOTS.end(), name) != ROBOTS.end();
    }

    static bool hasSpider(const std::string& name) {
        return !SPIDERS.empty() && std::find(SPIDERS.begin(), SPIDERS.end(), name) != SPIDERS.end();
    }

    static bool hasSwing(const std::string& name) {
        return !SWINGS.empty() && std::find(SWINGS.begin(), SWINGS.end(), name) != SWINGS.end();
    }

    static bool hasJetpack(const std::string& name) {
        return !JETPACKS.empty() && std::find(JETPACKS.begin(), JETPACKS.end(), name) != JETPACKS.end();
    }

    static bool hasTrail(const std::string& name) {
        return !TRAILS.empty() && std::find(TRAILS.begin(), TRAILS.end(), name) != TRAILS.end();
    }

    static void clear() {
        ICONS.clear();
        SHIPS.clear();
        BALLS.clear();
        UFOS.clear();
        WAVES.clear();
        ROBOTS.clear();
        SPIDERS.clear();
        SWINGS.clear();
        JETPACKS.clear();
        TRAILS.clear();
        saveTrails();
        TRAIL_INFO.clear();
        removeSaved();
    }

    static void removeSaved() {
        geode::Mod::get()->setSavedValue<std::vector<std::string>>("icons", {});
        geode::Mod::get()->setSavedValue<std::vector<std::string>>("ships", {});
        geode::Mod::get()->setSavedValue<std::vector<std::string>>("balls", {});
        geode::Mod::get()->setSavedValue<std::vector<std::string>>("ufos", {});
        geode::Mod::get()->setSavedValue<std::vector<std::string>>("waves", {});
        geode::Mod::get()->setSavedValue<std::vector<std::string>>("robots", {});
        geode::Mod::get()->setSavedValue<std::vector<std::string>>("spiders", {});
        geode::Mod::get()->setSavedValue<std::vector<std::string>>("swings", {});
        geode::Mod::get()->setSavedValue<std::vector<std::string>>("jetpacks", {});
        geode::Mod::get()->setSavedValue<std::vector<std::string>>("trails", {});
    }

    static void restoreSaved() {
        geode::Mod::get()->setSavedValue("icons", ICONS);
        geode::Mod::get()->setSavedValue("ships", SHIPS);
        geode::Mod::get()->setSavedValue("balls", BALLS);
        geode::Mod::get()->setSavedValue("ufos", UFOS);
        geode::Mod::get()->setSavedValue("waves", WAVES);
        geode::Mod::get()->setSavedValue("robots", ROBOTS);
        geode::Mod::get()->setSavedValue("spiders", SPIDERS);
        geode::Mod::get()->setSavedValue("swings", SWINGS);
        geode::Mod::get()->setSavedValue("jetpacks", JETPACKS);
        geode::Mod::get()->setSavedValue("trails", TRAILS);
    }

    static void load() {
        auto packs = getTexturePacks();
        auto packSize = packs.size();
        std::vector<std::string> duplicates;
        loadIcons(packs, "icon", ICONS, duplicates, IconType::Cube);
        loadIcons(packs, "ship", SHIPS, duplicates, IconType::Ship);
        loadIcons(packs, "ball", BALLS, duplicates, IconType::Ball);
        loadIcons(packs, "ufo", UFOS, duplicates, IconType::Ufo);
        loadIcons(packs, "wave", WAVES, duplicates, IconType::Wave);
        loadIcons(packs, "robot", ROBOTS, duplicates, IconType::Robot);
        loadIcons(packs, "spider", SPIDERS, duplicates, IconType::Spider);
        loadIcons(packs, "swing", SWINGS, duplicates, IconType::Swing);
        loadIcons(packs, "jetpack", JETPACKS, duplicates, IconType::Jetpack);
        duplicates.clear();
        loadTrails(packs, duplicates);
        duplicates.clear();
        if (LOADING_LAYER) {
            if (auto smallLabel2 = static_cast<cocos2d::CCLabelBMFont*>(LOADING_LAYER->getChildByID("geode-small-label-2"))) smallLabel2->setString("");
        }
        restoreSaved();
    }

    static std::vector<std::filesystem::directory_entry> naturalSort(const std::filesystem::path& path);

    static std::vector<std::filesystem::path> getTexturePacks();

    static void loadIcons(
        const std::vector<std::filesystem::path>& packs, const std::string& suffix,
        std::vector<std::string>& list, std::vector<std::string>& duplicates, IconType type
    );

    static void loadIcon(const std::filesystem::path& path, std::vector<std::string>& list, std::vector<std::string>& duplicates, IconType type);

    static void loadTrails(const std::vector<std::filesystem::path>& packs, std::vector<std::string>& duplicates);

    static void loadTrail(const std::filesystem::path& path, std::vector<std::string>& duplicates);

    static void saveTrails() {
        for (auto& [trail, info] : MoreIcons::TRAIL_INFO) {
            std::fstream file(std::filesystem::path(info.texture).replace_extension(".json"), std::ios::out);
            file << matjson::Value(matjson::Object{
                { "blend", info.blend },
                { "tint", info.tint },
            }).dump();
            file.close();
        }
    }

    static void changeSimplePlayer(SimplePlayer* player, IconType type) {
        changeSimplePlayer(player, geode::Mod::get()->getSavedValue<std::string>(savedForType(type), ""), type);
    }

    static void changeSimplePlayer(SimplePlayer* player, IconType type, bool dual) {
        changeSimplePlayer(player, geode::Mod::get()->getSavedValue<std::string>(savedForType(type, dual), ""), type);
    }

    static void changeSimplePlayer(SimplePlayer*, const std::string&, IconType);

    static bool doesExist(cocos2d::CCSpriteFrame* frame) {
        return frame != nullptr && frame->getTag() != 105871529;
    }

    static std::string getDual(const std::string& name, bool dual) {
        return geode::Loader::get()->isModLoaded("weebify.separate_dual_icons") && dual ? name + "-dual" : name;
    }

    static void swapDual(const std::string& name) {
        auto dualName = name + "-dual";
        auto normalIcon = geode::Mod::get()->getSavedValue<std::string>(name, "");
        auto dualIcon = geode::Mod::get()->getSavedValue<std::string>(dualName, "");
        geode::Mod::get()->setSavedValue(name, dualIcon);
        geode::Mod::get()->setSavedValue(dualName, normalIcon);
    }

    static std::vector<std::string>& vectorForType(IconType type) {
        switch (type) {
            case IconType::Cube:
                return ICONS;
            case IconType::Ship:
                return SHIPS;
            case IconType::Ball:
                return BALLS;
            case IconType::Ufo:
                return UFOS;
            case IconType::Wave:
                return WAVES;
            case IconType::Robot:
                return ROBOTS;
            case IconType::Spider:
                return SPIDERS;
            case IconType::Swing:
                return SWINGS;
            case IconType::Jetpack:
                return JETPACKS;
            case IconType::Special:
                return TRAILS;
            default: {
                static std::vector<std::string> empty;
                return empty;
            }
        }
    }

    static std::string savedForType(IconType type) {
        auto sdi = geode::Loader::get()->getLoadedMod("weebify.separate_dual_icons");
        return savedForType(type, sdi && sdi->getSavedValue("2pselected", false));
    }

    static std::string savedForType(IconType type, bool dual) {
        std::string prefix;
        switch (type) {
            case IconType::Cube: prefix = "icon"; break;
            case IconType::Ship: prefix = "ship"; break;
            case IconType::Ball: prefix = "ball"; break;
            case IconType::Ufo: prefix = "ufo"; break;
            case IconType::Wave: prefix = "wave"; break;
            case IconType::Robot: prefix = "robot"; break;
            case IconType::Spider: prefix = "spider"; break;
            case IconType::Swing: prefix = "swing"; break;
            case IconType::Jetpack: prefix = "jetpack"; break;
            case IconType::Special: prefix = "trail"; break;
            default: prefix = ""; break;
        }

        return getDual(prefix, dual);
    }

    static std::string getFrameName(const std::string& name, const std::string prefix, IconType type) {
        if (type != IconType::Robot && type != IconType::Spider) {
            if (name.ends_with("_2_001.png")) return fmt::format("{}_2_001.png"_spr, prefix);
            else if (name.ends_with("_3_001.png")) return fmt::format("{}_3_001.png"_spr, prefix);
            else if (name.ends_with("_extra_001.png")) return fmt::format("{}_extra_001.png"_spr, prefix);
            else if (name.ends_with("_glow_001.png")) return fmt::format("{}_glow_001.png"_spr, prefix);
            else if (name.ends_with("_001.png")) return fmt::format("{}_001.png"_spr, prefix);
        }
        else for (int i = 1; i < 5; i++) {
            if (name.ends_with(fmt::format("_{:02}_2_001.png", i))) return fmt::format("{}_{:02}_2_001.png"_spr, prefix, i);
            else if (i == 1 && name.ends_with(fmt::format("_{:02}_extra_001.png", i))) return fmt::format("{}_{:02}_extra_001.png"_spr, prefix, i);
            else if (name.ends_with(fmt::format("_{:02}_glow_001.png", i))) return fmt::format("{}_{:02}_glow_001.png"_spr, prefix, i);
            else if (name.ends_with(fmt::format("_{:02}_001.png", i))) return fmt::format("{}_{:02}_001.png"_spr, prefix, i);
        }

        return name;
    }

    static std::vector<std::string> getPage(IconType type, int page) {
        auto& vec = vectorForType(type);
        if (vec.size() <= page * 36) return {};
        return std::vector<std::string>(vec.begin() + page * 36, vec.begin() + std::min((int)vec.size(), (page + 1) * 36));
    }

    static int wrapPage(IconType type, int page) {
        auto pages = (vectorForType(type).size() + 35) / 36;
        return pages > 0 ? page < 0 ? pages - 1 : page >= pages ? 0 : page : 0;
    }

    static int findIconPage(IconType type) {
        auto& vec = vectorForType(type);
        auto it = std::find(vec.begin(), vec.end(), geode::Mod::get()->getSavedValue<std::string>(savedForType(type), ""));
        return it == vec.end() ? 0 : (it - vec.begin()) / 36;
    }

    static void useCustomRobot(GJRobotSprite* robot, const std::string& robotFile) {
        if (!robot || robotFile.empty() || !MoreIcons::hasRobot(robotFile)) return;
        robot->setBatchNode(nullptr);
        useCustomSprite(robot, robotFile);
    }

    static void useCustomSpider(GJSpiderSprite* spider, const std::string& spiderFile) {
        if (!spider || spiderFile.empty() || !MoreIcons::hasSpider(spiderFile)) return;
        spider->setBatchNode(nullptr);
        useCustomSprite(spider, spiderFile);
    }

    static void useCustomSprite(GJRobotSprite* robot, const std::string& file);
};
