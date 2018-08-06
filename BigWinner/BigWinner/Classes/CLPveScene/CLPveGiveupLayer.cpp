//
//  CLPveGiveupLayer.cpp
//  BigWinner
//
//  Created by Zhang Qiyun on 13-11-27.
//
//

#include "CLPveGiveupLayer.h"
#include "LGameCommonDefine.h"
#include "CLChangeString.h"
#include "CLPveSceneLayer.h"

CLPveGiveupLayer::CLPveGiveupLayer()
{
    
}

CLPveGiveupLayer::~CLPveGiveupLayer()
{
    
}


bool CLPveGiveupLayer::init()
{
    CCSize size = CCDirector::sharedDirector()->getgSceneSize();
    if(!CCLayerColor::initWithColor(ccc4(0, 0, 0, 100), size.width, size.height))
        return false;
    
    CCSprite* pBgSpr = CCSprite::create("Image/PveLayer/CP_GiveupLayerBG.png");
    pBgSpr->setPosition(ccp(size.width/2, size.height/2));
    this->addChild(pBgSpr, 1);
    
    CCSprite* pTTFBG = CCSprite::create("Image/PveLayer/CP_GiveUpTTFBG.png");
    pTTFBG->setAnchorPoint(ccp(0, 1));
    pTTFBG->setPosition(ccp(G_SF(45), pBgSpr->getContentSize().height));
    pBgSpr->addChild(pTTFBG, 1);
    
    CCSprite* pGiveTTF = CCSprite::create("Image/PveLayer/CP_GiveUpTTF.png");
    pGiveTTF->setPosition(ccp(pTTFBG->getContentSize().width/2, G_SF(39)));
    pTTFBG->addChild(pGiveTTF);
    
    CCSprite* pGiveBG1 = CCSprite::create("Image/PveLayer/CP_GiveUpBG1.png");
    pGiveBG1->setPosition(ccp(pBgSpr->getContentSize().width/2, G_SF(240)));
    pBgSpr->addChild(pGiveBG1, 1);
    
    CCLabelTTF* pLabelTTF = CCLabelTTF::create(CLChangeString::sharedChangeString()->GetChangeString("GiveUpPve").c_str(), "Arial", G_SF(26), CCSizeMake(G_SF(534), G_SF(145)), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
    pLabelTTF->setPosition(ccp(pGiveBG1->getContentSize().width/2, pGiveBG1->getContentSize().height/2));
    pLabelTTF->setColor(ccc3(109, 60, 24));
    pGiveBG1->addChild(pLabelTTF, 1);
    
    CCMenu* pMenu = CCMenu::create();
    pMenu->setAnchorPoint(CCPointZero);
    pMenu->setPosition(CCPointZero);
    pMenu->setTouchPriority(-1000001);
    pBgSpr->addChild(pMenu, 1);
    
    CCSprite* pSpr1 = CCSprite::create("Image/PublicUI/CommonIcon_Ok.png");
    CCSprite* pSpr2 = CCSprite::create("Image/PublicUI/CommonIcon_Ok.png");
    CCMenuItemSprite* pItem = CCMenuItemSprite::create(pSpr1, pSpr2, NULL, this, menu_selector(CLPveGiveupLayer::onGiveupMenuPress));
    pItem->setPosition(ccp(G_SF(176), G_SF(83)));
    pMenu->addChild(pItem, 1, 0);
    CCLabelTTF* pLabel = CCLabelTTF::create(CLChangeString::sharedChangeString()->GetChangeString("GoOnAnwser").c_str(), "Arial", G_SF(40));
    pLabel->setPosition(ccp(pItem->getContentSize().width/2, pItem->getContentSize().height/2));
    pItem->addChild(pLabel, 1);
    
    pSpr1 = CCSprite::create("Image/PublicUI/CommonIcon_Cancel.png");
    pSpr2 = CCSprite::create("Image/PublicUI/CommonIcon_Cancel.png");
    pItem = CCMenuItemSprite::create(pSpr1, pSpr2, NULL, this, menu_selector(CLPveGiveupLayer::onGiveupMenuPress));
    pItem->setPosition(ccp(G_SF(440), G_SF(83)));
    pMenu->addChild(pItem, 1, 1);
    pLabel = CCLabelTTF::create(CLChangeString::sharedChangeString()->GetChangeString("GiveUpAnwser").c_str(), "Arial", G_SF(40));
    pLabel->setPosition(ccp(pItem->getContentSize().width/2, pItem->getContentSize().height/2));
    pItem->addChild(pLabel, 1);
    
    return true;
}

void CLPveGiveupLayer::onGiveupMenuPress(CCObject* pSender)
{
    CCMenuItemSprite* pItem = (CCMenuItemSprite*)pSender;
    switch (pItem->getTag())
    {
        case 0:
        {
            this->removeFromParent();
        }
            break;
        case 1:
        {
            CLPveSceneLayer* pParNode = (CLPveSceneLayer*)this->getParent();
            pParNode->ReturnToShelfsLayer();
        }
            break;
        default:
            break;
    }
}

void CLPveGiveupLayer::onEnter()
{
    CCLayerColor::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -1000000, true);
}

void CLPveGiveupLayer::onExit()
{
    CCLayerColor::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}



bool CLPveGiveupLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if(this->isVisible())
        return true;
    else
        return false;
}

void CLPveGiveupLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void CLPveGiveupLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
}


