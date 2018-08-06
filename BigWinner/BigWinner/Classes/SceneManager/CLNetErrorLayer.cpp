//
//  CLNetErrorLayer.cpp
//  Yy_mobileGame3
//
//  Created by Zhang Qiyun on 13-8-28.
//
//

#include "CLNetErrorLayer.h"
#include "LGameCommonDefine.h"
#include "CLChangeString.h"
#include "CLSceneManager.h"

CLNetErrorLayer::CLNetErrorLayer()
{
    
}

CLNetErrorLayer::~CLNetErrorLayer()
{
    
}

CLNetErrorLayer* CLNetErrorLayer::create(const char* sText)
{
    CLNetErrorLayer* pLayer = new CLNetErrorLayer();
    if(pLayer && pLayer->init(sText))
    {
        pLayer->autorelease();
        return pLayer;
    }
    else
    {
        CC_SAFE_DELETE(pLayer);
        return NULL;
    }
}

bool CLNetErrorLayer::init(const char* sText)
{
    CCSize size = CCDirector::sharedDirector()->getgSceneSize();
    if(!CCLayerColor::initWithColor(ccc4(0, 0, 0, 100), size.width, size.height))
        return false;
    
    CCSprite* pErrorBG = CCSprite::create("Image/PublicUI/NetErrorBG.png");
    pErrorBG->setPosition(ccp(size.width/2, size.height/2));
    this->addChild(pErrorBG, 1);
    
    CCLabelTTF* pTextTTF = CCLabelTTF::create(CLChangeString::sharedChangeString()->GetChangeString(sText).c_str(), CLChangeString::sharedChangeString()->GetTTFName(2).c_str(), G_SF(28), CCSizeMake(G_SF(504), G_SF(200)), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    pTextTTF->setAnchorPoint(ccp(0.5, 0));
    pTextTTF->setPosition(ccp(pErrorBG->getContentSize().width/2, 0));
    pErrorBG->addChild(pTextTTF, 1);
    
    CCSprite* pErrorTTF = CCSprite::create("Image/PublicUI/NetErrorTTF.png");
    pErrorTTF->setPosition(ccp(pErrorBG->getContentSize().width/2, G_SF(240)));
    pErrorBG->addChild(pErrorTTF, 1);
    
    //关闭按钮
    CCMenu* pMenu = CCMenu::create();
    pMenu->setAnchorPoint(CCPointZero);
    pMenu->setPosition(CCPointZero);
    pMenu->setTouchPriority(-100000005);
    pErrorBG->addChild(pMenu, 1);
    
    CCSprite* pSpr1 = CCSprite::create("Image/PublicUI/ClickMenuClose_N.png");
    CCSprite* pSpr2 = CCSprite::create("Image/PublicUI/ClickMenuClose_S.png");
    CCMenuItemSprite* pItem = CCMenuItemSprite::create(pSpr1, pSpr2, NULL, this, menu_selector(CLNetErrorLayer::onCloseMenuClick));
    pItem->setAnchorPoint(ccp(1,1));
    pItem->setPosition(ccp(pErrorBG->getContentSize().width-G_SF(6), pErrorBG->getContentSize().height-G_SF(2)));
    pMenu->addChild(pItem, 1);
    
    pErrorBG->setScale(0.05);
    
    CCEaseSineIn* pSineIn = CCEaseSineIn::create(CCScaleTo::create(0.3, 1));
    CCMoveBy* pMove1 = CCMoveBy::create(0.05, ccp(G_SF(-5), G_SF(5)));
    CCMoveBy* pMove2 = CCMoveBy::create(0.05, ccp(G_SF(5), G_SF(-5)));
    
    CCSequence* seq = CCSequence::create(pSineIn, pMove1, pMove1->reverse(), pMove2, pMove2->reverse(), NULL);
    pErrorBG->runAction(seq);
    
    return true;
}

void CLNetErrorLayer::onCloseMenuClick(CCObject* pSender)
{
    CLSceneManager::sharedSceneManager()->SetNetErrorLayer(false);
}

void CLNetErrorLayer::onEnter()
{
    CCLayerColor::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -100000002, true);
}

void CLNetErrorLayer::onExit()
{
    CCLayerColor::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}



bool CLNetErrorLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if(this->isVisible())
        return true;
    else
        return false;
}

void CLNetErrorLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void CLNetErrorLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
}





