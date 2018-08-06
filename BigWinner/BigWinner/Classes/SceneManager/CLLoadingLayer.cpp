//
//  CLLoadingLayer.cpp
//  Yy_mobileGame3
//
//  Created by Zhang Qiyun on 13-6-26.
//
//

#include "CLLoadingLayer.h"
#include "LGameCommonDefine.h"
#include "CLChangeString.h"

CLLoadingLayer::CLLoadingLayer()
{
    m_nCurIndex = 3;
    m_pLeftDoor = NULL;
    m_pRightDoor = NULL;
    m_pLoadTTF = NULL;
}

CLLoadingLayer::~CLLoadingLayer()
{
    
}

CLLoadingLayer* CLLoadingLayer::create(float fwidth, float fheight)
{
    CLLoadingLayer* pLayer = new CLLoadingLayer();
    if(pLayer && pLayer->init(fwidth, fheight))
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

bool CLLoadingLayer::init(float fwidth, float fheight)
{
    if(!CCLayerColor::initWithColor(ccc4(0, 0, 0, 0), fwidth, fheight))
    {
        return false;
    }
    
//    CCSprite* pLogo = CCSprite::create("Image/PublicUI/GameLogo.png");
//    pLogo->setPosition(ccp(this->getContentSize().width*0.5, this->getContentSize().height*0.5));
//    this->addChild(pLogo, 1);
    
    m_pLeftDoor = CCSprite::create("Image/PublicUI/LoadingDoor.png");
    m_pLeftDoor->setAnchorPoint(ccp(1, 0.5));
    m_pLeftDoor->setPosition(ccp(0, fheight/2));
    this->addChild(m_pLeftDoor, 1);
    
    m_pRightDoor = CCSprite::create("Image/PublicUI/LoadingDoor.png");
    m_pRightDoor->setFlipX(true);
    m_pRightDoor->setAnchorPoint(ccp(0, 0.5));
    m_pRightDoor->setPosition(ccp(fwidth, fheight/2));
    this->addChild(m_pRightDoor, 1);
    
    for(int i=0; i<7; i++)
    {
        CCSprite* pSpr = NULL;
        if(i < 4)
        {
            char sFile[50] = {0};
            sprintf(sFile, "Image/PublicUI/LoadingIcon%d.png", i);
            pSpr = CCSprite::create(sFile);
        }
        else
            pSpr = CCSprite::create("Image/PublicUI/LoadingIcon0.png");
        pSpr->setPosition(ccp(G_SF(180)+i*G_SF(40), G_SF(396)));
        this->addChild(pSpr, 1);
        m_vQQList.push_back(pSpr);
    }
    
    m_pLoadTTF = CCLabelTTF::create("Loading. . .", CLChangeString::sharedChangeString()->GetTTFName(2).c_str(), G_SF(30));
    m_pLoadTTF->setColor(ccc3(251, 224, 133));
    m_pLoadTTF->setPosition(ccp(G_SF(320), G_SF(340)));
    this->addChild(m_pLoadTTF, 1);
    
    this->schedule(schedule_selector(CLLoadingLayer::QQMoveFrame), 0.12 );
    return true;
    
}



void CLLoadingLayer::onEnter()
{
    CCLayer::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -100000000, true);
}

void CLLoadingLayer::onExit()
{
    CCLayer::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}



bool CLLoadingLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if(this->isVisible())
        return true;
    else
        return false;
}

void CLLoadingLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void CLLoadingLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
}


void CLLoadingLayer::QQMoveFrame()
{
    m_nCurIndex++;
    if(m_nCurIndex > 6)
        m_nCurIndex = 0;
    
    for(int i=0; i < 4; i++)
    {
        int nIndex = m_nCurIndex-i;
        if(nIndex < 0)
            nIndex += 7;
        CCSprite* pSpr = m_vQQList[nIndex];
        
        char sFile[50] = {0};
        sprintf(sFile, "Image/PublicUI/LoadingIcon%d.png", 3-i);
        CCTexture2D* pTex = CCTextureCache::sharedTextureCache()->addImage(sFile);
        pSpr->initWithTexture(pTex);
    }
}


void CLLoadingLayer::setVisible(bool visible)
{
    if(visible)
    {
        CCNode::setVisible(visible);
        
        m_pLeftDoor->stopAllActions();
        m_pRightDoor->stopAllActions();
        m_pLoadTTF->setVisible(false);
        for(int i=0; i<m_vQQList.size(); i++)
        {
            m_vQQList[i]->setVisible(false);
        }
        
        m_pLeftDoor->setPosition(ccp(0, this->getContentSize().height/2));
        m_pRightDoor->setPosition(ccp(this->getContentSize().width, this->getContentSize().height/2));
        
        m_pLeftDoor->runAction(CCMoveBy::create(0.3, ccp(G_SF(320), 0)));
        m_pRightDoor->runAction(CCSequence::create(CCMoveBy::create(0.3, ccp(G_SF(-320), 0)), CCCallFunc::create(this, callfunc_selector(CLLoadingLayer::onDoorCloseEnd)), NULL));
        
    }
    else
    {
        m_pLeftDoor->stopAllActions();
        m_pRightDoor->stopAllActions();
        
        m_pLoadTTF->setVisible(false);
        for(int i=0; i<m_vQQList.size(); i++)
        {
            m_vQQList[i]->setVisible(false);
        }
        
        m_pLeftDoor->setPosition(ccp(G_SF(320), this->getContentSize().height/2));
        m_pRightDoor->setPosition(ccp(G_SF(320), this->getContentSize().height/2));
        
        m_pLeftDoor->runAction(CCMoveBy::create(0.3, ccp(G_SF(-320), 0)));
        m_pRightDoor->runAction(CCSequence::create(CCMoveBy::create(0.3, ccp(G_SF(320), 0)), CCCallFunc::create(this, callfunc_selector(CLLoadingLayer::onDoorOpenEnd)), NULL));
    }
}


void CLLoadingLayer::onDoorCloseEnd()
{
    m_pLoadTTF->setVisible(true);
    for(int i=0; i<m_vQQList.size(); i++)
    {
        m_vQQList[i]->setVisible(true);
    }
}

void CLLoadingLayer::onDoorOpenEnd()
{
    CCNode::setVisible(false);
}



