class DummyNode : public cocos2d::CCSpriteBatchNode {
public:
    static DummyNode* createWithTexture(cocos2d::CCTexture2D* texture, unsigned int capacity) {
        auto ret = new DummyNode();
        ret->initWithTexture(texture, capacity);
        ret->autorelease();
        return ret;
    }

    void draw() override {
        CCNode::draw();
    }

    void reorderChild(CCNode* child, int zOrder) override {
        CCNode::reorderChild(child, zOrder);
    }

    void removeChild(CCNode* child, bool cleanup) override {
        CCNode::removeChild(child, cleanup);
    }

    void removeAllChildrenWithCleanup(bool cleanup) override {
        CCNode::removeAllChildrenWithCleanup(cleanup);
    }

    void addChild(CCNode* child, int zOrder, int tag) override {
        CCNode::addChild(child, zOrder, tag);
    }

    void sortAllChildren() override {
        CCNode::sortAllChildren();
    }

    void visit() override {
        CCNode::visit();
    }
};

class MoreIconsAPI {
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

    static bool doesExist(cocos2d::CCSpriteFrame* frame) {
        return frame && frame->getTag() != 105871529;
    }

    static std::vector<std::string>& vectorForType(IconType type);

    static std::string savedForType(IconType type, bool dual);

    static std::string activeForType(IconType type, bool dual) {
        auto savedType = savedForType(type, dual);
        return !savedType.empty() ? geode::Mod::get()->getSavedValue<std::string>(savedType, "") : "";
    }

    static bool hasIcon(const std::string& icon, IconType type) {
        auto& vec = vectorForType(type);
        return !vec.empty() && !icon.empty() && std::find(vec.begin(), vec.end(), icon) != vec.end();
    }

    static void updateSimplePlayer(SimplePlayer* player, const std::string& icon, IconType type);

    static void updateRobotSprite(GJRobotSprite* sprite, const std::string& icon) {
        if (!sprite || icon.empty()) return;

        updateRobotSprite(sprite, icon, sprite->m_iconType);
    }

    static void updateRobotSprite(GJRobotSprite* sprite, const std::string& icon, IconType type);

    static void updatePlayerObject(PlayerObject* object, const std::string& icon) {
        if (!object || icon.empty()) return;

        if (object->m_isShip) updatePlayerObject(object, icon, object->m_isPlatformer ? IconType::Jetpack : IconType::Ship);
        else if (object->m_isBall) updatePlayerObject(object, icon, IconType::Ball);
        else if (object->m_isBird) updatePlayerObject(object, icon, IconType::Ufo);
        else if (object->m_isDart) updatePlayerObject(object, icon, IconType::Wave);
        else if (object->m_isRobot) updatePlayerObject(object, icon, IconType::Robot);
        else if (object->m_isSpider) updatePlayerObject(object, icon, IconType::Spider);
        else if (object->m_isSwing) updatePlayerObject(object, icon, IconType::Swing);
        else updatePlayerObject(object, icon, IconType::Cube);
    }

    static void updatePlayerObject(PlayerObject* object, const std::string& icon, IconType type);
};