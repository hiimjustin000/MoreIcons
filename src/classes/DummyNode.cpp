#include "DummyNode.hpp"

using namespace geode::prelude;

void DummyNode::recursiveBlend(CCNode* node, ccBlendFunc blendFunc) {
    if (node) {
        if (auto blendNode = typeinfo_cast<CCBlendProtocol*>(node)) {
            blendNode->setBlendFunc(blendFunc);
        }
        for (auto child : CCArrayExt<CCNode*>(node->getChildren())) {
            if (auto blendNode = typeinfo_cast<CCBlendProtocol*>(child)) {
                recursiveBlend(child, blendFunc);
            }
        }
    }
}
