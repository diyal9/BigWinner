//
//  CLPveSMangerLayer.cpp
//  BigWinner
//
//  Created by diyal yin on 13-12-19.
//
//

#include "CLPveSMangerLayer.h"
#include "LGameCommonDefine.h"
#include "CLChangeString.h"
#include "CLPveSceneLayer.h"
#include "CLSceneManager.h"
#include "CLNetWorkManager.h"
#include "CLPlayerManager.h"
#include "DlgTiShiLayer.h"

CLPveSMangerLayer::CLPveSMangerLayer()
{
//    m_iShelfNo = 0;
//    m_iStatusNo = 0;
}

CLPveSMangerLayer::~CLPveSMangerLayer()
{
    
}

CLPveSMangerLayer* CLPveSMangerLayer::create(int iShelfNo, int iStatusNo)
{
    CLPveSMangerLayer* pItem = new CLPveSMangerLayer();
    if(pItem && pItem->init(iShelfNo, iStatusNo))
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

bool CLPveSMangerLayer::init(int iShelfNo, int iStatusNo)
{
    CCSize size = CCDirector::sharedDirector()->getgSceneSize();
    if(!CCLayerColor::initWithColor(ccc4(0, 0, 0, 100), size.width, size.height))
        return false;
    
    m_iShelfNo = iShelfNo;
    m_iStatusNo = iStatusNo;
    
    CCSprite* pBgSpr = CCSprite::create("Image/PveLayer/CP_GiveupLayerBG.png");
    pBgSpr->setPosition(ccp(size.width/2, size.height/2));
    this->addChild(pBgSpr, 1);
    
    CCSprite* pTTFBG = CCSprite::create("Image/PveLayer/CP_GiveUpTTFBG.png");
    pTTFBG->setAnchorPoint(ccp(0, 1));
    pTTFBG->setPosition(ccp(G_SF(45), pBgSpr->getContentSize().height));
    pBgSpr->addChild(pTTFBG, 1);
    
    CCSprite* pGiveTTF = CCSprite::create("Image/PveLayer/FastProcess.png");
    pGiveTTF->setPosition(ccp(pTTFBG->getContentSize().width/2, G_SF(39)));
    pTTFBG->addChild(pGiveTTF);
    
    CCSprite* pGiveBG1 = CCSprite::create("Image/PveLayer/CP_GiveUpBG1.png");
    pGiveBG1->setPosition(ccp(pBgSpr->getContentSize().width/2, G_SF(240)));
    pBgSpr->addChild(pGiveBG1, 1);
    
    CCLabelTTF* pLabelTTF  = CCLabelTTF::create("", "Arial", G_SF(26), CCSizeMake(G_SF(534), G_SF(145)), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);

    if (m_iStatusNo == 1 || m_iStatusNo == 3) //消除冷却
    {
        pLabelTTF->setString(CLChangeString::sharedChangeString()->GetChangeString("PresentConfirm1").c_str());
    }
    else if (m_iStatusNo == 2)
    {
        pLabelTTF->setString(CLChangeString::sharedChangeString()->GetChangeString("PresentSM").c_str());
    }
    pLabelTTF->setPosition(ccp(pGiveBG1->getContentSize().width/2, pGiveBG1->getContentSize().height/2));
    pLabelTTF->setColor(ccc3(109, 60, 24));
    pGiveBG1->addChild(pLabelTTF, 1);
    
    CCMenu* pMenu = CCMenu::create();
    pMenu->setAnchorPoint(CCPointZero);
    pMenu->setPosition(CCPointZero);
    pMenu->setTouchPriority(-1000001);
    pBgSpr->addChild(pMenu, 1);
    
    //确定
    CCSprite* pSpr1 = CCSprite::create("Image/PublicUI/CommonIcon_Ok.png");
    CCSprite* pSpr2 = CCSprite::create("Image/PublicUI/CommonIcon_Ok.png");
    CCMenuItemSprite* pItem = CCMenuItemSprite::create(pSpr1, pSpr2, NULL, this, menu_selector(CLPveSMangerLayer::onOKMenuPress));
    pItem->setPosition(ccp(G_SF(440), G_SF(83)));
    pMenu->addChild(pItem, 1, 0);
    CCLabelTTF* pLabel = CCLabelTTF::create(CLChangeString::sharedChangeString()->GetChangeString("OkBtn").c_str(), "Arial", G_SF(40));
    pLabel->setPosition(ccp(pItem->getContentSize().width/2, pItem->getContentSize().height/2));
    pItem->addChild(pLabel, 1);
    
    //取消
    pSpr1 = CCSprite::create("Image/PublicUI/CommonIcon_Cancel.png");
    pSpr2 = CCSprite::create("Image/PublicUI/CommonIcon_Cancel.png");
    pItem = CCMenuItemSprite::create(pSpr1, pSpr2, NULL, this, menu_selector(CLPveSMangerLayer::onCanceMenuPress));
    pItem->setPosition(ccp(G_SF(176), G_SF(83)));
    pMenu->addChild(pItem, 1, 1);
    pLabel = CCLabelTTF::create(CLChangeString::sharedChangeString()->GetChangeString("CancleBtn").c_str(), "Arial", G_SF(40));
    pLabel->setPosition(ccp(pItem->getContentSize().width/2, pItem->getContentSize().height/2));
    pItem->addChild(pLabel, 1);
    
    return true;
}

//确认按钮回调
void CLPveSMangerLayer::onOKMenuPress(CCObject* pSender)
{
    int money = CLPlayerManager::shardPlayerManager()->getpUserInfo()->getJetton(); //用户卡数量
    if (money < 10)
    {
        TiShiLayer::create(CLChangeString::sharedChangeString()->GetChangeString("GiftPoor"));
        return;
    }
    
    //发送数据
    Json::Value jv(Json::objectValue);
    jv["shelfno"] = m_iShelfNo;
    
    switch (m_iStatusNo)
    {
        case 1: //冷却取消
        {
            CLNetWorkManager::shardNetWorkManager()->SendNetMessage(CMD_REMOVETIME, jv);
            this->removeFromParent();
        }
            break;
        case 2: //给店长送礼
        {
            CLNetWorkManager::shardNetWorkManager()->SendNetMessage(CMD_DIRECTPASS, jv);
            this->removeFromParent();
        }
            break;
        case 3: //挑战店长失败，继续挑战后，冷却取消
        {
            CLNetWorkManager::shardNetWorkManager()->SendNetMessage(CMD_REMOVETIME, jv);
            this->removeFromParent();
        }
            break;
        default:
            break;
    }
}

//取消按钮回调
void CLPveSMangerLayer::onCanceMenuPress(CCObject* pSender)
{

    if (m_iStatusNo == 3) //挑战失败，取消回到商铺
    {
       CLPveSceneLayer* pParNode = (CLPveSceneLayer*)this->getParent();
       pParNode->ReturnToShelfsLayer();
    }
    else
    {
       this->removeFromParent();
    }
    
}

void CLPveSMangerLayer::onEnter()
{
    CCLayerColor::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -1000000, true);
}

void CLPveSMangerLayer::onExit()
{
    CCLayerColor::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}



bool CLPveSMangerLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if(this->isVisible())
        return true;
    else
        return false;
}

void CLPveSMangerLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void CLPveSMangerLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
}


