//
//  CLPvpWaitRivalLayer.cpp
//  BigWinner
//
//  Created by Zhang Qiyun on 13-11-13.
//
//

#include "CLPvpWaitRivalLayer.h"
#include "CLAnimationManager.h"
#include "CLChangeString.h"
#include "LGameCommonDefine.h"
#include "UserInfoSprite.h"
#include "CLPvpSceneLayer.h"

CLPvpWaitRivalLayer::CLPvpWaitRivalLayer()
{
    m_pLabelTTF = NULL;
    m_magic = NULL;
}

CLPvpWaitRivalLayer::~CLPvpWaitRivalLayer()
{
    
}


bool CLPvpWaitRivalLayer::init()
{
    CCSize size = CCDirector::sharedDirector()->getgSceneSize();
    if(!CCLayerColor::initWithColor(ccc4(0, 0, 0, 0), size.width, size.height))
        return false;
    
    CCSprite* pBGSpr = CCSprite::create("Image/PvpLayer/CP_WaitRivalBG.png");
    pBGSpr->setAnchorPoint(CCPointZero);
    pBGSpr->setPosition(CCPointZero);
    this->addChild(pBGSpr, 2, 10086);
    
    CCAnimation* animation = CLAnimationManager::sharedAnimationManager()->animationWithName("matching", 0.2);
    m_magic = CLAnimationManager::sharedAnimationManager()->spriteWithAnimationName("matching");
    m_magic->setPosition(ccp(size.width/2, size.height/2));
    pBGSpr->addChild(m_magic, 1);
    m_magic->runAction(CCRepeatForever::create(CCAnimate::create(animation)));
    
    m_pLabelTTF = CLStrokeLabelTTF::create(CLChangeString::sharedChangeString()->GetChangeString("WaitingRival").c_str(), "Arial", G_SF(28), G_SF(1.5));
    m_pLabelTTF->setPosition(ccp(size.width/2, G_SF(195)));
    pBGSpr->addChild(m_pLabelTTF, 1);
    
    return true;
}

void CLPvpWaitRivalLayer::onEnter()
{
    CCLayerColor::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -10000000, true);
}

void CLPvpWaitRivalLayer::onExit()
{
    CCLayerColor::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}



bool CLPvpWaitRivalLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if(this->isVisible())
        return true;
    else
        return false;
}

void CLPvpWaitRivalLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void CLPvpWaitRivalLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void CLPvpWaitRivalLayer::WaitRivalSuccess(Json::Value& jv)
{
    m_magic->stopAllActions();
    m_magic->removeFromParent();
    m_magic = NULL;
    
    m_pLabelTTF->removeFromParent();
    m_pLabelTTF = NULL;
    
    CCNode* pNode = this->getChildByTag(10086);
    
    //添加自己的头像信息
    UserInfoSprite* pUserInfo = UserInfoSprite::create();
    pUserInfo->setPosition(ccp(G_SF(-320), G_SF(837)));
    pUserInfo->setScale(608.0/296);
    pNode->addChild(pUserInfo, 1);
    
    pUserInfo->runAction(CCMoveBy::create(0.3, ccp(G_SF(640), 0)));
    
    RivalInfoSprite* pRivalInfo = RivalInfoSprite::create(jv);
    pRivalInfo->setPosition(ccp(G_SF(960), G_SF(134)));
    pRivalInfo->setScale(608.0/296);
    pNode->addChild(pRivalInfo, 1);
    
    pRivalInfo->runAction(CCSequence::create(CCMoveBy::create(0.3, ccp(G_SF(-640), 0)), CCDelayTime::create(0.3), CCCallFunc::create(this, callfunc_selector(CLPvpWaitRivalLayer::onInfoPanelMoved)), NULL));
    
}

void CLPvpWaitRivalLayer::onInfoPanelMoved()
{
    CCNode* pNode = this->getChildByTag(10086);
    
    CCSprite* pVsSpr = CCSprite::create("Image/PvpLayer/CP_VsIcon.png");
    pVsSpr->setPosition(ccp(G_SF(320), G_SF(480)));
    pVsSpr->setScale(640.0/79);
    pNode->addChild(pVsSpr, 1);
    
    pVsSpr->runAction(CCSequence::create(CCScaleTo::create(0.3, 1.0), CCDelayTime::create(0.5), CCCallFunc::create(this, callfunc_selector(CLPvpWaitRivalLayer::BeginToPvping)), NULL));
}

void CLPvpWaitRivalLayer::BeginToPvping()
{
    CLPvpSceneLayer* pParNode = (CLPvpSceneLayer*)this->getParent()->getParent()->getParent();
    pParNode->ChangeSubLayer();
}




