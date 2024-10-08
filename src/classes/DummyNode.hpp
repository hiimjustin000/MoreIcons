// Inspired by Alphalaneous
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
        if (auto blendNode = geode::cast::typeinfo_cast<cocos2d::CCBlendProtocol*>(child)) {
            recursiveBlend(child, blendNode->getBlendFunc());
        }
        CCNode::addChild(child, zOrder, tag);
    }

    void sortAllChildren() override {
        CCNode::sortAllChildren();
    }

    void setBlendFunc(cocos2d::ccBlendFunc blendFunc) override {
        CCSpriteBatchNode::setBlendFunc(blendFunc);
        for (auto child : geode::cocos::CCArrayExt<CCNode*>(getChildren())) {
            if (auto blendNode = geode::cast::typeinfo_cast<cocos2d::CCBlendProtocol*>(child)) {
                blendNode->setBlendFunc(blendFunc);
            }
        }
    }

    void visit() override {
        CCNode::visit();
    }

    void recursiveBlend(CCNode* node, cocos2d::ccBlendFunc blendFunc);
};
