class MoreIcons {
public:
    static inline std::vector<std::string> ICONS;
    static inline std::vector<std::string> SHIPS;
    static inline std::vector<std::string> BALLS;
    static inline std::vector<std::string> UFOS;
    static inline std::vector<std::string> WAVES;
    static inline std::vector<std::string> ROBOTS;
    static inline std::unordered_map<std::string, std::string> ROBOT_TEXTURES;
    static inline std::vector<std::string> SPIDERS;
    static inline std::unordered_map<std::string, std::string> SPIDER_TEXTURES;
    static inline std::vector<std::string> SWINGS;
    static inline std::vector<std::string> JETPACKS;

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

    static void clear() {
        ICONS.clear();
        SHIPS.clear();
        BALLS.clear();
        UFOS.clear();
        WAVES.clear();
        ROBOTS.clear();
        ROBOT_TEXTURES.clear();
        SPIDERS.clear();
        SPIDER_TEXTURES.clear();
        SWINGS.clear();
        JETPACKS.clear();
    }
    static void load() {
        auto configDir = geode::Mod::get()->getConfigDir();
        std::unordered_map<std::string, std::string> tempMap;
        loadIcons(configDir / "icon", ICONS, tempMap);
        loadIcons(configDir / "ship", SHIPS, tempMap);
        loadIcons(configDir / "ball", BALLS, tempMap);
        loadIcons(configDir / "ufo", UFOS, tempMap);
        loadIcons(configDir / "wave", WAVES, tempMap);
        loadIcons(configDir / "robot", ROBOTS, ROBOT_TEXTURES);
        loadIcons(configDir / "spider", SPIDERS, SPIDER_TEXTURES);
        loadIcons(configDir / "swing", SWINGS, tempMap);
        loadIcons(configDir / "jetpack", JETPACKS, tempMap);
    }
    static void loadIcons(const std::filesystem::path& path, std::vector<std::string>& list, std::unordered_map<std::string, std::string>& textures);
    static void changeSimplePlayer(SimplePlayer* player, IconType type) {
        changeSimplePlayer(player, geode::Mod::get()->getSavedValue<std::string>(savedForType(type), ""), type);
    }
    static void changeSimplePlayer(SimplePlayer*, const std::string&, IconType);
    static bool doesExist(cocos2d::CCSpriteFrame* frame) {
        return frame != nullptr && frame->getTag() != 105871529;
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
            default: {
                static std::vector<std::string> empty;
                return empty;
            }
        }
    }

    static std::string savedForType(IconType type) {
        switch (type) {
            case IconType::Cube:
                return "icon";
            case IconType::Ship:
                return "ship";
            case IconType::Ball:
                return "ball";
            case IconType::Ufo:
                return "ufo";
            case IconType::Wave:
                return "wave";
            case IconType::Robot:
                return "robot";
            case IconType::Spider:
                return "spider";
            case IconType::Swing:
                return "swing";
            case IconType::Jetpack:
                return "jetpack";
            default:
                return "";
        }
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
};
