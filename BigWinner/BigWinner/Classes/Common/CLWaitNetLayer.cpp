//
//  CLWaitNetLayer.cpp
//  Yy_mobileGame3
//
//  Created by Zhang Qiyun on 13-8-16.
//
//

#include "CLWaitNetLayer.h"
#include "CLAnimationManager.h"
#include "CLChangeString.h"
#include "LGameCommonDefine.h"

CLWaitNetLayer::CLWaitNetLayer()
{
    
}

CLWaitNetLayer::~CLWaitNetLayer()
{
    
}


bool CLWaitNetLayer::init()
{
    CCSize size = CCDirector::sharedDirector()->getgSceneSize();
    if(!CCLayerColor::initWithColor(ccc4(0, 0, 0, 0), size.width, size.height))
        return false;
    
    CCAnimation* animation = CLAnimationManager::sharedAnimationManager()->animationWithName("loading");
    CCSprite* magic = CLAnimationManager::sharedAnimationManager()->spriteWithAnimationName("loading");
    magic->setPosition(ccp(size.width/2, size.height/2));
    this->addChild(magic, 1);
    magic->runAction(CCRepeatForever::create(CCAnimate::create(animation)));
    
    CCLabelTTF* pLabelTTF = CCLabelTTF::create(CLChangeString::sharedChangeString()->GetChangeString("Waiting").c_str(), "Arial", G_SF(30));
    pLabelTTF->setPosition(ccp(size.width/2+G_SF(12), size.height/2-G_SF(55)));
    this->addChild(pLabelTTF, 1);
    
    return true;
}

void CLWaitNetLayer::onEnter()
{
    CCLayerColor::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -100000000, true);
}

void CLWaitNetLayer::onExit()
{
    CCLayerColor::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}



bool CLWaitNetLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if(this->isVisible())
        return true;
    else
        return false;
}

void CLWaitNetLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void CLWaitNetLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
}


