class ButtonHooker : public cocos2d::CCObject {
private:
    cocos2d::CCObject* m_listener;
    cocos2d::CCObject* m_newListener;
    cocos2d::SEL_MenuHandler m_selector;
    cocos2d::SEL_MenuHandler m_newSelector;
public:
    static ButtonHooker* create(cocos2d::CCMenuItem* button, cocos2d::CCObject* listener, cocos2d::SEL_MenuHandler selector);

    void onClick(cocos2d::CCObject* sender);
};
