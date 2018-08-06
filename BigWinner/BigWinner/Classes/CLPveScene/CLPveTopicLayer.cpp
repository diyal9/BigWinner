//
//  CLPveTopicLayer.cpp
//  BigWinner
//
//  Created by Zhang Qiyun on 13-10-26.
//
//

#include "CLPveTopicLayer.h"
#include "CLTopicData.h"
#include "JsonDataManager.h"
#include "LGameCommonDefine.h"
#include <algorithm>
#include "CLPveSceneLayer.h"
#include "CLChangeString.h"
#include "CLPvpingLayer.h"

CLPveTopicLayer::CLPveTopicLayer()
{
    m_bPveStyle = true;
    m_nTopicIndex = 0;
    m_pPauseMenu = NULL;
    m_pTopicIndexTTF = NULL;
    m_pTopicTextTTF = NULL;
    m_pFinger = NULL;
    m_pAnswerMenu = NULL;
}

CLPveTopicLayer::~CLPveTopicLayer()
{
    for(int i=0; i<4; i++)
    {
        delete m_vAnswerStructs[i];
    }
}

CLPveTopicLayer* CLPveTopicLayer::create(Json::Value& value, bool bPveStyle)
{
    CLPveTopicLayer* pLayer = new CLPveTopicLayer();
    if(pLayer && pLayer->init(value, bPveStyle))
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


bool CLPveTopicLayer::init(Json::Value &value, bool bPveStyle)
{
    m_bPveStyle = bPveStyle;
    if(!CCSprite::initWithFile("Image/PveLayer/CP_TopicLayerBG.png"))
        return false;
    
    std::string sType = value["catalog"].asStringModeStyle();
    int nTopicId = value["query"].asIntModestyle();
    
    CLTopicData* pTopicData = JsonDataManager::shardJDManager()->GetTopicDataFromID(sType.c_str(), nTopicId);
    if(!pTopicData)
        return false;
    
    CCSprite* pTopicIndexBG = CCSprite::create("Image/PveLayer/CP_TopicIndexBG.png");
    pTopicIndexBG->setPosition(ccp(G_SF(74), G_SF(335)));
    this->addChild(pTopicIndexBG, 1);
    
    if(m_bPveStyle)
    {
        m_pTopicIndexTTF = CCLabelTTF::create("Q.0", "Arial", G_SF(32));
        m_pTopicIndexTTF->setPosition(ccp(pTopicIndexBG->getContentSize().width/2, pTopicIndexBG->getContentSize().height/2));
        m_pTopicIndexTTF->setColor(ccc3(0, 103, 195));
        pTopicIndexBG->addChild(m_pTopicIndexTTF, 1);
    }
    else
    {
        std::string sTypeName = CLChangeString::sharedChangeString()->GetChangeString(sType.c_str());

        m_pTopicIndexTTF = CCLabelTTF::create(sTypeName.c_str(), "Arial", G_SF(32));
        m_pTopicIndexTTF->setPosition(ccp(pTopicIndexBG->getContentSize().width/2, pTopicIndexBG->getContentSize().height/2));
        m_pTopicIndexTTF->setColor(ccc3(0, 103, 195));
        pTopicIndexBG->addChild(m_pTopicIndexTTF, 1);
    }
    
    //倒计时图标
    CCSprite* pSchIcon = CCSprite::create("Image/PveLayer/CP_SchTimeIcon.png");
    pSchIcon->setPosition(ccp(G_SF(526), G_SF(340)));
    this->addChild(pSchIcon, 1);
    
    for(int i=0; i<5; i++)
    {
        CCSprite* pStar = CCSprite::create("Image/PublicUI/KingIcon.png");
        pStar->setPosition(ccp(G_SF(193)+i*G_SF(50), G_SF(332)));
        pStar->setVisible(false);
        this->addChild(pStar, 1);
        m_vToicStars.push_back(pStar);
    }
    
    m_pTopicTextTTF = CCLabelTTF::create("", "Arial", G_SF(28), CCSizeMake(G_SF(520), G_SF(230)), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
    m_pTopicTextTTF->setAnchorPoint(ccp(0.5, 0));
    m_pTopicTextTTF->setPosition(ccp(this->getContentSize().width/2, G_SF(33)));
    m_pTopicTextTTF->setColor(ccBLACK);
    this->addChild(m_pTopicTextTTF, 1);
    
    m_pAnswerMenu = CCMenu::create();
    m_pAnswerMenu->setAnchorPoint(CCPointZero);
    m_pAnswerMenu->setPosition(CCPointZero);
    this->addChild(m_pAnswerMenu, 1);
    
    for(int i=0; i<4; i++)
    {
        CCSprite* pSpr1 = CCSprite::create("Image/PveLayer/CP_AnswerBG_N.png");
        CCSprite* pSpr2 = CCSprite::create("Image/PveLayer/CP_AnswerBG_S.png");
        CCMenuItemSprite* pItem = CCMenuItemSprite::create(pSpr1, pSpr2, NULL, this, menu_selector(CLPveTopicLayer::onAnswerMenuClicked));
        pItem->setAnchorPoint(ccp(0, 1));
        pItem->setPosition(ccp(0+i%2*G_SF(308), G_SF(-10)-i/2*G_SF(128)));
        m_pAnswerMenu->addChild(pItem, 0, i);
        
        CCLabelTTF* pAnswerTTF = CCLabelTTF::create("", "Arial", G_SF(28), CCSizeMake(G_SF(254), G_SF(104)), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
        pAnswerTTF->setColor(ccBLACK);
        pAnswerTTF->setPosition(ccp(G_SF(156), pItem->getContentSize().height/2));
        pItem->addChild(pAnswerTTF, 1);
        m_vAnswerList.push_back(pAnswerTTF);
        
        
        CLAnswerStruct* pStruct = new CLAnswerStruct();
        m_vAnswerStructs.push_back(pStruct);
        
        CCSprite* pSprIcon = CCSprite::create(CCString::createWithFormat("Image/PveLayer/CP_AnswerIcon%d.png", i)->getCString());
        pSprIcon->setPosition(ccp(G_SF(10), G_SF(88)));
        pItem->addChild(pSprIcon, 1);
        
    }
    
    
    
    return true;
}


void CLPveTopicLayer::onPauseMenuCkicked(CCObject* pSender)
{
    CCMenuItemSprite* pItem = (CCMenuItemSprite*)pSender;
    switch (pItem->getTag())
    {
        case 0:
        {
            if(m_bPveStyle)
            {
                pItem->removeFromParentAndCleanup(true);
                CCSprite* spr1 = CCSprite::create("Image/PveLayer/CP_GoOn_N.png");
                CCSprite* spr2 = CCSprite::create("Image/PveLayer/CP_GoOn_S.png");
                
                CCMenuItemSprite* pItem = CCMenuItemSprite::create(spr1, spr2, NULL, this, menu_selector(CLPveTopicLayer::onPauseMenuCkicked));
                pItem->setPosition(ccp(G_SF(550), G_SF(635)));
                m_pPauseMenu->addChild(pItem, 0, 1);
                
                //事件
                
            }
            else
            {
                
            }

        }
            break;
        case 1:
        {
            if(m_bPveStyle)
            {
                pItem->removeFromParentAndCleanup(true);
                CCSprite* spr1 = CCSprite::create("Image/PveLayer/CP_Pause_N.png");
                CCSprite* spr2 = CCSprite::create("Image/PveLayer/CP_Pause_S.png");
                
                CCMenuItemSprite* pItem = CCMenuItemSprite::create(spr1, spr2, NULL, this, menu_selector(CLPveTopicLayer::onPauseMenuCkicked));
                pItem->setPosition(ccp(G_SF(550), G_SF(635)));
                m_pPauseMenu->addChild(pItem, 0, 0);
                
                //事件
            }
            else
            {
                
            }

        }
            break;
            
        default:
            break;
    }
}


void CLPveTopicLayer::onAnswerMenuClicked(CCObject* pSender)
{
//    bool bSMState;
//    CLPveSceneLayer* pPveLayer = (CLPveSceneLayer*)this->getParent()->getParent();
//    if(pPveLayer->getbSMStyle())
//        bSMState= true;
    
    
    CCMenuItemSprite* pItem = (CCMenuItemSprite*)pSender;

    if(pItem->getTag() >= 4)
        return;
    int nAnswer = m_vAnswerStructs[pItem->getTag()]->getnIndex();
    
    if(m_bPveStyle)
    {
        CLPveSceneLayer* pParNode = (CLPveSceneLayer*)this->getParent()->getParent();
        pParNode->EndSchTime(nAnswer+1);
    }
    else
    {
        CLPvpingLayer* pParNode = (CLPvpingLayer*)this->getParent();
        pParNode->EndSchTime(nAnswer+1);
        SetAnswersEnable(false);
    }


}

bool cmp(const CLAnswerStruct* p1, const CLAnswerStruct* p2)
{
    if(p1->getnRand() > p2->getnRand())
        return true;
    else
        return false;
}

void CLPveTopicLayer::UpdateTopicData(Json::Value& jv)
{
    if(!m_bPveStyle)
    {
        
        std::string sType = jv["catalog"].asStringModeStyle();
        int nTopicId = jv["query"].asIntModestyle();
        
        CLTopicData* pTopicData = JsonDataManager::shardJDManager()->GetTopicDataFromID(sType.c_str(), nTopicId);
        if(!pTopicData)
            return;
        
        std::string sTypeName = CLChangeString::sharedChangeString()->GetChangeString(sType.c_str());
        m_pTopicIndexTTF->setString(sTypeName.c_str());
        
        m_pTopicTextTTF->setString(pTopicData->getsTopicDesc().c_str());
        
        srand(time(NULL));
        for(int i=0; i<4; i++)
        {
            m_vAnswerStructs[i]->setnRand(rand()%100);
            m_vAnswerStructs[i]->setnIndex(i);
            m_vAnswerStructs[i]->setsAnswer(*(pTopicData->GetAnswerList()[i]));
        }
        
        for(int i=0; i<m_vToicStars.size(); i++)
        {
            if(i < pTopicData->getnDifficulty())
                m_vToicStars[i]->setVisible(true);
            else
                m_vToicStars[i]->setVisible(false);
        }
        
        sort(m_vAnswerStructs.begin(), m_vAnswerStructs.end(), cmp);  //是否开启排序错乱功能
        
        for (int i=0; i<4; i++)
        {
            m_vAnswerList[i]->setString(m_vAnswerStructs[i]->getsAnswer().c_str());
        }
        
        SetAnswersEnable(true);
    }
    else
    {
        bool bSMState = false;
        CLPveSceneLayer* pPveLayer = (CLPveSceneLayer*)this->getParent()->getParent();
        if(pPveLayer->getbSMStyle())
            bSMState= true;
        
        if(jv.isMember("yinhua") && !bSMState) //说明有印花 那么必须飘落印花(挑战Boss是没有印花掉落的)
        {
            CLPveSceneLayer* pParNode = (CLPveSceneLayer*)this->getParent()->getParent();
            pParNode->CreateyinhuaIcon(jv["yinhua"].asIntModestyle());
        }
        
        int nRound = jv["index"].asIntModestyle();
        
        std::string sType = jv["catalog"].asStringModeStyle();
        int nTopicId = jv["query"].asIntModestyle();
        
        CLTopicData* pTopicData = JsonDataManager::shardJDManager()->GetTopicDataFromID(sType.c_str(), nTopicId);
        if(!pTopicData)
            return;
        
        char sIndexTTF[20] = {0};
        if(bSMState)
            sprintf(sIndexTTF, "Q.%d/1", nRound);
        else
            sprintf(sIndexTTF, "Q.%d/10", nRound);
        
        if(nRound == 11)  //说明是 印花题
            m_pTopicIndexTTF->setString(CLChangeString::sharedChangeString()->GetChangeString("YinhuaTopic").c_str());
        else
            m_pTopicIndexTTF->setString(sIndexTTF);
        
        m_pTopicTextTTF->setString(pTopicData->getsTopicDesc().c_str());
        
        srand(time(NULL));
        for(int i=0; i<4; i++)
        {
            m_vAnswerStructs[i]->setnRand(rand()%100);
            m_vAnswerStructs[i]->setnIndex(i);
            m_vAnswerStructs[i]->setsAnswer(*(pTopicData->GetAnswerList()[i]));
        }
        
        sort(m_vAnswerStructs.begin(), m_vAnswerStructs.end(), cmp);  //是否开启排序错乱功能
        
        for (int i=0; i<4; i++)
        {
            m_vAnswerList[i]->setString(m_vAnswerStructs[i]->getsAnswer().c_str());
        }

    }
    
}

void CLPveTopicLayer::GiveAnanswerIndex(bool bRight, int nIndex)
{
    if(nIndex > 4)
        return;
    int nPosIndex = -1;
    for(int i=0; i< m_vAnswerStructs.size(); i++)
    {
        if(m_vAnswerStructs[i]->getnIndex() == nIndex-1)
        {
            nPosIndex = i;
            break;
        }
    }
    if(nPosIndex == -1)
        return;
    
    CCMenuItemSprite* pItem = (CCMenuItemSprite*)m_pAnswerMenu->getChildByTag(nPosIndex);
    
    CCPoint point = ccpAdd(pItem->getPosition(), ccp(G_SF(70), G_SF(-59)));
    
    if(bRight)   //如果是给出正确答案的话
    {
        if(!m_pFinger)
        {
            m_pFinger = CCSprite::create("Image/PveLayer/FingerEffect.png");
            m_pFinger->runAction(CCRepeatForever::create(CCSequence::create(CCFadeOut::create(0.3), CCFadeIn::create(0.3), NULL)));
            this->addChild(m_pFinger, 2);
        }
        m_pFinger->setPosition(point);
        m_pFinger->setVisible(true);
    }
    else
    {
        pItem->setVisible(false);
    }
}

void CLPveTopicLayer::SetAnswersEnable(bool b)
{
    CCArray* pArray = m_pAnswerMenu->getChildren();
    
    if(pArray)
    {
        for(int i=0; i<pArray->count(); i++)
        {
            CCMenuItemSprite* pItem = (CCMenuItemSprite*)pArray->objectAtIndex(i);
            if(!b)
                pItem->setEnabled(false);
            else
                pItem->setEnabled(true);
        }
    }

}

void CLPveTopicLayer::ReSetAnswerState()
{
    if(m_pFinger)
        m_pFinger->setVisible(false);
    CCArray* pArray = m_pAnswerMenu->getChildren();
    for(int i=0; i<pArray->count(); i++)
    {
        CCMenuItemSprite* pItem = (CCMenuItemSprite*)pArray->objectAtIndex(i);
        pItem->setVisible(true);
    }
    
}



