#include <BS_thread_pool.hpp>
#include "api/MoreIconsAPI.hpp"

struct TexturePack {
    std::string name;
    std::string id;
};

struct TrailInfo {
    std::string texture;
    TexturePack pack;
    bool blend;
    bool tint;
};

struct ImageData {
    cocos2d::CCImage* image;
    cocos2d::CCDictionary* dict;
    std::string texturePath;
    std::string name;
    std::string frameName;
    TexturePack pack;
    IconType type;
    int index;
    bool blend;
    bool tint;
};

enum class LogType {
    Info,
    Warn,
    Error
};

struct LogData {
    std::string message;
    LogType type;
};

// https://github.com/GlobedGD/globed2/blob/v1.6.2/src/util/cocos.cpp#L44
namespace {
    template <typename TC>
    using priv_method_t = void(TC::*)(cocos2d::CCDictionary*, cocos2d::CCTexture2D*);

    template <typename TC, priv_method_t<TC> func>
    struct priv_caller {
        friend void _addSpriteFramesWithDictionary(cocos2d::CCDictionary* dict, cocos2d::CCTexture2D* texture) {
            (cocos2d::CCSpriteFrameCache::get()->*func)(dict, texture);
        }
    };

    template struct priv_caller<cocos2d::CCSpriteFrameCache, &cocos2d::CCSpriteFrameCache::addSpriteFramesWithDictionary>;

    void _addSpriteFramesWithDictionary(cocos2d::CCDictionary*, cocos2d::CCTexture2D*);
}

class MoreIcons {
public:
    static inline std::unordered_map<std::string, TexturePack> ICON_INFO;
    static inline std::unordered_map<std::string, TexturePack> SHIP_INFO;
    static inline std::unordered_map<std::string, TexturePack> BALL_INFO;
    static inline std::unordered_map<std::string, TexturePack> UFO_INFO;
    static inline std::unordered_map<std::string, TexturePack> WAVE_INFO;
    static inline std::unordered_map<std::string, TexturePack> ROBOT_INFO;
    static inline std::unordered_map<std::string, TexturePack> SPIDER_INFO;
    static inline std::unordered_map<std::string, TexturePack> SWING_INFO;
    static inline std::unordered_map<std::string, TexturePack> JETPACK_INFO;
    static inline std::unordered_map<std::string, TrailInfo> TRAIL_INFO;
    static inline std::vector<ImageData> IMAGES;
    static inline std::mutex IMAGE_MUTEX;
    static inline std::vector<LogData> LOGS;
    static inline std::mutex LOG_MUTEX;
    static inline LogType HIGHEST_SEVERITY = LogType::Info;

    static BS::thread_pool& sharedPool() {
        static BS::thread_pool _sharedPool(std::thread::hardware_concurrency() - 2);
        return _sharedPool;
    }

    static void clear() {
        MoreIconsAPI::ICONS.clear();
        ICON_INFO.clear();
        MoreIconsAPI::SHIPS.clear();
        SHIP_INFO.clear();
        MoreIconsAPI::BALLS.clear();
        BALL_INFO.clear();
        MoreIconsAPI::UFOS.clear();
        UFO_INFO.clear();
        MoreIconsAPI::WAVES.clear();
        WAVE_INFO.clear();
        MoreIconsAPI::ROBOTS.clear();
        ROBOT_INFO.clear();
        MoreIconsAPI::SPIDERS.clear();
        SPIDER_INFO.clear();
        MoreIconsAPI::SWINGS.clear();
        SWING_INFO.clear();
        MoreIconsAPI::JETPACKS.clear();
        JETPACK_INFO.clear();
        MoreIconsAPI::TRAILS.clear();
        saveTrails();
        TRAIL_INFO.clear();
        removeSaved();
        LOGS.clear();
        HIGHEST_SEVERITY = LogType::Info;
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
        geode::Mod::get()->setSavedValue("icons", MoreIconsAPI::ICONS);
        geode::Mod::get()->setSavedValue("ships", MoreIconsAPI::SHIPS);
        geode::Mod::get()->setSavedValue("balls", MoreIconsAPI::BALLS);
        geode::Mod::get()->setSavedValue("ufos", MoreIconsAPI::UFOS);
        geode::Mod::get()->setSavedValue("waves", MoreIconsAPI::WAVES);
        geode::Mod::get()->setSavedValue("robots", MoreIconsAPI::ROBOTS);
        geode::Mod::get()->setSavedValue("spiders", MoreIconsAPI::SPIDERS);
        geode::Mod::get()->setSavedValue("swings", MoreIconsAPI::SWINGS);
        geode::Mod::get()->setSavedValue("jetpacks", MoreIconsAPI::JETPACKS);
        geode::Mod::get()->setSavedValue("trails", MoreIconsAPI::TRAILS);
    }

    static void load(LoadingLayer*);

    static std::vector<std::filesystem::directory_entry> naturalSort(const std::filesystem::path& path);

    static void naturalSort(std::vector<std::string>& vec);

    static std::vector<std::filesystem::path> getTexturePacks();

    static std::string replaceEnd(const std::string& str, const std::string& end, const std::string& replace, bool check = false) {
        if (!check || str.ends_with(end)) return str.substr(0, str.size() - end.size()) + replace;
        return str;
    }

    static void loadIcons(
        const std::vector<std::filesystem::path>& packs, const std::string& suffix, IconType type
    );

    static void loadIcon(const std::filesystem::path& path, const TexturePack& pack, IconType type);

    static void loadTrails(const std::vector<std::filesystem::path>& packs);

    static void loadTrail(const std::filesystem::path& path, const TexturePack& pack);

    static void saveTrails() {
        for (auto& [trail, info] : TRAIL_INFO) {
            std::fstream file(std::filesystem::path(info.texture).replace_extension(".json"), std::ios::out);
            file << matjson::Value(matjson::Object {
                { "blend", info.blend },
                { "tint", info.tint },
            }).dump();
            file.close();
        }
    }

    static void changeSimplePlayer(SimplePlayer* player, IconType type) {
        MoreIconsAPI::updateSimplePlayer(player, geode::Mod::get()->getSavedValue<std::string>(savedForType(type), ""), type);
    }

    static void changeSimplePlayer(SimplePlayer* player, IconType type, bool dual) {
        MoreIconsAPI::updateSimplePlayer(player, geode::Mod::get()->getSavedValue<std::string>(MoreIconsAPI::savedForType(type, dual), ""), type);
    }

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

    static std::unordered_map<std::string, TexturePack>& infoForType(IconType type) {
        switch (type) {
            case IconType::Cube:
                return ICON_INFO;
            case IconType::Ship:
                return SHIP_INFO;
            case IconType::Ball:
                return BALL_INFO;
            case IconType::Ufo:
                return UFO_INFO;
            case IconType::Wave:
                return WAVE_INFO;
            case IconType::Robot:
                return ROBOT_INFO;
            case IconType::Spider:
                return SPIDER_INFO;
            case IconType::Swing:
                return SWING_INFO;
            case IconType::Jetpack:
                return JETPACK_INFO;
            default: {
                static std::unordered_map<std::string, TexturePack> empty;
                return empty;
            }
        }
    }

    static std::string savedForType(IconType type) {
        auto sdi = geode::Loader::get()->getLoadedMod("weebify.separate_dual_icons");
        return MoreIconsAPI::savedForType(type, sdi && sdi->getSavedValue("2pselected", false));
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

    static SimplePlayer* findPlayer(cocos2d::CCNode* node) {
        if (!node) return nullptr;
        return geode::cocos::findFirstChildRecursive<SimplePlayer>(node, [](auto) { return true; });
    }

    static std::vector<std::string> getPage(IconType type, int page) {
        auto& vec = MoreIconsAPI::vectorForType(type);
        auto customPage = page - (GameManager::get()->countForType(type) + 35) / 36;
        if (vec.size() <= customPage * 36) return {};
        if (customPage < 0) return {};
        return std::vector<std::string>(vec.begin() + customPage * 36, vec.begin() + std::min((int)vec.size(), (customPage + 1) * 36));
    }

    static int countForType(IconType type) {
        return MoreIconsAPI::vectorForType(type).size() + ((GameManager::get()->countForType(type) + 35) / 36 * 36);
    }

    static int wrapPage(IconType type, int page) {
        auto pages = (countForType(type) + 35) / 36;
        return pages > 0 ? page < 0 ? pages - 1 : page >= pages ? 0 : page : 0;
    }

    static int findIconPage(IconType type) {
        auto active = (GameManager::get()->activeIconForType(type) - 1) / 36;
        auto& vec = MoreIconsAPI::vectorForType(type);
        auto savedType = savedForType(type);
        if (savedType.empty()) return active;
        auto it = std::find(vec.begin(), vec.end(), geode::Mod::get()->getSavedValue<std::string>(savedType, ""));
        return it == vec.end() ? active : ((GameManager::get()->countForType(type) + 35) / 36) + (it - vec.begin()) / 36;
    }

    static bool isNormalPage(int page, IconType type) {
        auto count = GameManager::get()->countForType(type);
        return page * 36 < count;
    }

    static void showInfoPopup(bool folderButton = false) {
        geode::createQuickPopup(
            "More Icons",
            fmt::format(std::locale(""),
                "<cg>Icons</c>: {:L}\n"
                "<cp>Ships</c>: {:L}\n"
                "<cr>Balls</c>: {:L}\n"
                "<co>UFOs</c>: {:L}\n"
                "<cj>Waves</c>: {:L}\n"
                "Robots: {:L}\n"
                "<ca>Spiders</c>: {:L}\n"
                "<cy>Swings</c>: {:L}\n"
                "<cd>Jetpacks</c>: {:L}\n"
                "<cb>Trails</c>: {:L}",
                MoreIconsAPI::ICONS.size(),
                MoreIconsAPI::SHIPS.size(),
                MoreIconsAPI::BALLS.size(),
                MoreIconsAPI::UFOS.size(),
                MoreIconsAPI::WAVES.size(),
                MoreIconsAPI::ROBOTS.size(),
                MoreIconsAPI::SPIDERS.size(),
                MoreIconsAPI::SWINGS.size(),
                MoreIconsAPI::JETPACKS.size(),
                MoreIconsAPI::TRAILS.size()
            ),
            "OK",
            folderButton ? "Folder" : nullptr,
            300.0f,
            [folderButton](auto, bool btn2) {
                if (folderButton && btn2) geode::utils::file::openFolder(geode::Mod::get()->getConfigDir());
            }
        );
    }
};
