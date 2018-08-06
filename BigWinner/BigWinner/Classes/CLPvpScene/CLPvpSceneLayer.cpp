//
//  CLPvpSceneLayer.cpp
//  BigWinner
//
//  Created by ZhangQiyun on 13-11-6.
//
//

#include "CLPvpSceneLayer.h"
#include "CLPvpReady.h"
#include "CLPvpingLayer.h"
#include "CLSceneManager.h"
#include "CLSoundManager.h"

CLPvpSceneLayer::CLPvpSceneLayer()
{
    m_nLayerMode = PVPLAYER_NULL;
    m_pShowNode = NULL;
    m_bSubReady = true;
    m_pFadeLayer = NULL;
}

CLPvpSceneLayer::~CLPvpSceneLayer()
{
    
}


bool CLPvpSceneLayer::init()
{
    if(!CCLayer::init())
        return false;
    
    CCSize size = CCDirector::sharedDirector()->getgSceneSize();
    
    //添加背景
//    CCSprite* pBGSpr = CCSprite::create("Image/LoginLayer/ML_BG.png");
    //对战背景图片添加 wangguolong
    CCSprite* pBGSpr = CCSprite::create("Image/PvpLayer/PVP_BG.png");
    pBGSpr->setPosition(ccp(size.width/2, size.height/2));
    this->addChild(pBGSpr, 0, 10086);
    
    // 添加实际场景
    CLPvpReady* pLayer = CLPvpReady::create();
    pLayer->setAnchorPoint(CCPointZero);
    pLayer->setPosition(CCPointZero);
    pBGSpr->addChild(pLayer, 1);
    
    m_pShowNode = pLayer;
    m_nCTIndex = CLScene_PvpLayer;
    
    return true;
}



void CLPvpSceneLayer::ChangeSubLayer()
{
    CCSize size = CCDirector::sharedDirector()->getgSceneSize();
    m_pFadeLayer = CLFadeColorLayer::create(ccc4(0, 0, 0, 0), size.width, size.height);
    m_pFadeLayer->setAnchorPoint(CCPointZero);
    m_pFadeLayer->setPosition(CCPointZero);
    
    this->addChild(m_pFadeLayer, 10);
    
    CCFadeIn* pFadeIn = CCFadeIn::create(0.3);
    CCCallFunc* pCall = CCCallFunc::create(this, callfunc_selector(CLPvpSceneLayer::ChangeSceneCall));
    CCSequence* pSeq = (CCSequence*)CCSequence::create(pFadeIn, pCall, NULL);
    m_pFadeLayer->runAction(pSeq);
}

void CLPvpSceneLayer::ChangeSceneCall()
{
    if(m_bSubReady)  //说明要切换到 战斗界面了
    {
        m_pShowNode->removeFromParent();
        m_pShowNode = NULL;
        m_bSubReady = false;
        
        Json::Value* pV = CLSceneManager::sharedSceneManager()->GetJsonDataFromMsgID(CMD_PK1_PAIR);
        CLPvpingLayer* pLayer = CLPvpingLayer::create(*pV);
        pLayer->setAnchorPoint(CCPointZero);
        pLayer->setPosition(CCPointZero);
        
        CCNode* pNode = this->getChildByTag(10086);
        pNode->addChild(pLayer, 1);
        
        m_pShowNode = pLayer;
        CLSceneManager::sharedSceneManager()->RemoveJsonDataFromMsgID(CMD_PK1_PAIR);
    }
    else
    {
        m_pShowNode->removeFromParent();
        m_pShowNode = NULL;
        m_bSubReady = true;
        
        CLPvpReady* pLayer = CLPvpReady::create();
        pLayer->setAnchorPoint(CCPointZero);
        pLayer->setPosition(CCPointZero);
        
        CCNode* pNode = this->getChildByTag(10086);
        pNode->addChild(pLayer, 1);
        m_pShowNode = pLayer;
    }
    
    CCFadeOut* pFadeOut = CCFadeOut::create(0.3);
    CCCallFunc* pCallEnd = CCCallFunc::create(this, callfunc_selector(CLPvpSceneLayer::ChangeSceneCallEnd));
    CCSequence* pSeq = (CCSequence*)CCSequence::create(pFadeOut, pCallEnd, NULL);
    m_pFadeLayer->runAction(pSeq);
}

void CLPvpSceneLayer::ChangeSceneCallEnd()
{
    this->removeChild(m_pFadeLayer, true);
    m_pFadeLayer = NULL;
}

