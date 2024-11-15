#include "ButtonHooker.hpp"

using namespace geode::prelude;

ButtonHooker* ButtonHooker::create(CCMenuItem* button, CCObject* listener, SEL_MenuHandler selector) {
    auto hooker = new ButtonHooker();
    hooker->m_listener = button->m_pListener;
    hooker->m_newListener = listener;
    hooker->m_selector = button->m_pfnSelector;
    hooker->m_newSelector = selector;
    hooker->autorelease();
    button->setTarget(hooker, menu_selector(ButtonHooker::onClick));
    button->setUserObject("hooker"_spr, hooker);
    return hooker;
}

void ButtonHooker::onClick(CCObject* sender) {
    (m_listener->*m_selector)(sender);
    (m_newListener->*m_newSelector)(sender);
}
