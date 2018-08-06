//
//  CLPvpTopicLayer.cpp
//  BigWinner
//
//  Created by Zhang Qiyun on 13-12-22.
//
//

#include "CLPvpTopicLayer.h"
#include "JsonDataManager.h"
#include "LGameCommonDefine.h"
#include "CLChangeString.h"

CLPvpTopicLayer::CLPvpTopicLayer()
{
    m_pAnswerMenu = NULL;
    m_nShowIndex = -1;
    m_nShowTimeIndex = 0;
}

CLPvpTopicLayer::~CLPvpTopicLayer()
{
    
}

CLPvpTopicLayer* CLPvpTopicLayer::create(Json::Value& value)
{
    CLPvpTopicLayer* pLayer = new CLPvpTopicLayer();
    if(pLayer&& pLayer->init(value))
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

bool CLPvpTopicLayer::init(Json::Value& value)
{
    if(!CCSprite::initWithFile("Image/PvpLayer/PP_TopicLayerBG.png"))
        return false;
    
    std::string sType = value["catalog"].asStringModeStyle();
    int nTopicId = value["query"].asIntModestyle();
    
    CLTopicData* pTopicData = JsonDataManager::shardJDManager()->GetTopicDataFromID(sType.c_str(), nTopicId);
    if(!pTopicData)
        return false;
    
    CCSprite* pTopicIndexBG = CCSprite::create("Image/PvpLayer/PP_TopicTypeLayer.png");
    pTopicIndexBG->setPosition(ccp(G_SF(74), G_SF(335)));
    this->addChild(pTopicIndexBG, 1);
    
    std::string sTypeName = CLChangeString::sharedChangeString()->GetChangeString(sType.c_str());
    
    CCLabelTTF* pTopicIndexTTF = CCLabelTTF::create(sTypeName.c_str(), "Arial", G_SF(32));
    pTopicIndexTTF->setPosition(ccp(pTopicIndexBG->getContentSize().width/2, pTopicIndexBG->getContentSize().height/2));
    pTopicIndexTTF->setColor(ccc3(0, 103, 195));
    pTopicIndexBG->addChild(pTopicIndexTTF, 1);
    
    //倒计时图标
    CCSprite* pSchIcon = CCSprite::create("Image/PveLayer/CP_SchTimeIcon.png");
    pSchIcon->setPosition(ccp(G_SF(526), G_SF(340)));
    this->addChild(pSchIcon, 1);
    
    for(int i=0; i<pTopicData->getnDifficulty(); i++)
    {
        CCSprite* pStar = CCSprite::create("Image/PublicUI/KingIcon.png");
        pStar->setPosition(ccp(G_SF(193)+i*G_SF(50), G_SF(332)));
        pStar->setVisible(false);
        this->addChild(pStar, 1);
    }
    
    CCLabelTTF* pTopicTextTTF = CCLabelTTF::create(pTopicData->getsTopicDesc().c_str(), "Arial", G_SF(28), CCSizeMake(G_SF(520), G_SF(230)), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
    pTopicTextTTF->setAnchorPoint(ccp(0.5, 0));
    pTopicTextTTF->setPosition(ccp(this->getContentSize().width/2, G_SF(33)));
    pTopicTextTTF->setColor(ccBLACK);
    this->addChild(pTopicTextTTF, 1);
    
    m_pAnswerMenu = CCMenu::create();
    m_pAnswerMenu->setAnchorPoint(CCPointZero);
    m_pAnswerMenu->setPosition(CCPointZero);
    this->addChild(m_pAnswerMenu, 1);
    
    for(int i=0; i<4; i++)
    {
        CCSprite* pSpr1 = CCSprite::create("Image/PvpLayer/PP_AnswerBG_N.png");
        CCSprite* pSpr2 = CCSprite::create("Image/PvpLayer/PP_AnswerBG_S.png");
        CCMenuItemSprite* pItem = CCMenuItemSprite::create(pSpr1, pSpr2, NULL);
        pItem->setAnchorPoint(ccp(0, 1));
        pItem->setPosition(ccp(0+i%2*G_SF(308), G_SF(-10)-i/2*G_SF(128)));
        m_pAnswerMenu->addChild(pItem, 0, i);
        
        CCLabelTTF* pAnswerTTF = CCLabelTTF::create((*(pTopicData->GetAnswerList()[i])).c_str(), "Arial", G_SF(28), CCSizeMake(G_SF(254), G_SF(104)), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
        pAnswerTTF->setColor(ccBLACK);
        pAnswerTTF->setPosition(ccp(G_SF(156), pItem->getContentSize().height/2));
        pItem->addChild(pAnswerTTF, 1);
        
        CCSprite* pSprIcon = CCSprite::create(CCString::createWithFormat("Image/PvpLayer/PP_AnswerIcon%d.png", i)->getCString());
        pSprIcon->setPosition(ccp(G_SF(10), G_SF(88)));
        pItem->addChild(pSprIcon, 1);
        pItem->setEnabled(false);
    }
    
    return true;

}

void CLPvpTopicLayer::StartShowAnswer(int nIndex)
{
    if(nIndex > 4 || nIndex <= 0)
        return;
    
    m_nShowIndex = nIndex-1;
    
    CCMenuItemSprite* pItem = (CCMenuItemSprite*)m_pAnswerMenu->getChildByTag(m_nShowIndex);
    pItem->selected();
    this->scheduleOnce(schedule_selector(CLPvpTopicLayer::onSchFrame), 0.1);
    
}

void CLPvpTopicLayer::onSchFrame(float ft)
{
    CCMenuItemSprite* pItem = (CCMenuItemSprite*)m_pAnswerMenu->getChildByTag(m_nShowIndex);
    pItem->unselected();
}

