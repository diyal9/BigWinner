//
//  CLFightItem.cpp
//  BigWinner
//
//  Created by Zhang Qiyun on 13-11-28.
//
//

#include "CLFightItem.h"
#include "CLPlayerManager.h"
#include "LGameCommonDefine.h"

CLFightPropItem::CLFightPropItem()
{
    m_pPropInfo = NULL;
    m_pCallTarget = NULL;
    m_pCallFun = NULL;
    m_pNumLabel = NULL;
    m_nCoolRound = 0;
    m_bTopicCard = false;
}

CLFightPropItem::~CLFightPropItem()
{
    
}

CLFightPropItem* CLFightPropItem::create(int nPropId, CCObject* pTarget, SEL_CallFuncO pCallFun, bool bCard)
{
    CLFightPropItem* pItem = new CLFightPropItem();
    if(pItem && pItem->init(nPropId, pTarget, pCallFun, bCard))
    {
        pItem->autorelease();
        return pItem;
    }
    else
    {
        CC_SAFE_DELETE(pItem);
        return NULL;
    }
}

bool CLFightPropItem::init(int nPropId, CCObject* pTarget, SEL_CallFuncO pCallFun, bool bCard)
{
    m_pCallTarget = pTarget;
    m_pCallFun = pCallFun;
    m_bTopicCard = bCard;
    
    m_pPropInfo = CLPlayerManager::shardPlayerManager()->getpUserInfo()->GetPropFromId(nPropId);
    if(!m_pPropInfo || !m_pPropInfo->m_cardJueseInfo)
        return false;
    
    CCSprite* pspr1 = CCSprite::create("Image/PublicUI/PropItemBG.png");
    if(!initWithNormalSprite(pspr1, NULL, NULL, m_pCallTarget, m_pCallFun))
        return false;
    
    int ArtIconId = m_pPropInfo->m_cardJueseInfo->m_artIconId;
    
    std::string sFile;
    if(!bCard)
        sFile = CCString::createWithFormat("Image/BackPack/Props%d.png", ArtIconId)->getCString();
    else
        sFile = CCString::createWithFormat("Image/BackPack/CardIcon%d.png", ArtIconId)->getCString();
    CCSprite* pIcon = CCSprite::create(sFile.c_str());
    pIcon->setScale(0.5);
    pIcon->setPosition(ccp(this->getContentSize().width/2, this->getContentSize().height/2));
    this->addChild(pIcon, 1);
    
    m_pNumLabel = CCLabelTTF::create(CCString::createWithFormat("%d", m_pPropInfo->number)->getCString(), "Verdana_Bold", G_SF(24));
    m_pNumLabel->setAnchorPoint(CCPointZero);
    m_pNumLabel->setColor(ccBLACK);
    m_pNumLabel->setPosition(ccp(G_SF(10), G_SF(10)));
    this->addChild(m_pNumLabel, 2);
    
    return true;
}


void CLFightPropItem::setOpacity(GLubyte opacity)
{
    CCNodeRGBA::setOpacity(opacity);
    
    CCArray* pArray = this->getChildren();
    for(int i=0; i<pArray->count(); i++)
    {
        CCNodeRGBA* pNode = (CCNodeRGBA*)pArray->objectAtIndex(i);
        pNode->setOpacity(opacity);
    }
}

void CLFightPropItem::setEnabled(bool bEnabled)
{
    if(bEnabled)
    {
        m_nCoolRound--;
        if(m_nCoolRound < 0)
            m_nCoolRound = 0;
        if(m_nCoolRound == 0 && m_pPropInfo->number != 0)
        {
            CCMenuItemSprite::setEnabled(true);
            setOpacity(255);
        }
        else
        {
            CCMenuItemSprite::setEnabled(false);
            setOpacity(80);
        }
        
    }
    else
    {
        CCMenuItemSprite::setEnabled(false);
        setOpacity(80);
    }
}




