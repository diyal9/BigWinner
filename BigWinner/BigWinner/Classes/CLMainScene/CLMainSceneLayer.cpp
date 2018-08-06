//
//  CLMainSceneLayer.cpp
//  BigWinner
//
//  Created by ZhangQiyun on 13-10-14.
//
//
// modify info
// 20131217 修改头像区域，使用封装的精灵

#include "CLMainSceneLayer.h"
#include "CLPlayerManager.h"
#include "LGameCommonDefine.h"
#include "CLNetWorkManager.h"
#include "CLSceneManager.h"
#include "CLShelfsSceneLayer.h"
#include "YHtopLayer.h"
#include "CLPvpSceneLayer.h"
#include "DlgTiShiLayer.h"
#include "CLChangeString.h"
#include <BackPack.h>
#include "DlgLayerBtnB.h"
#include "GameVolumeSet.h"
#include "UserInfoSprite.h"
#include "Achievements.h"
#include "CLSoundManager.h"
#include "BillboardLayer.h"


CLStorePanel::CLStorePanel()
{
    m_pStoreData = NULL;
}

CLStorePanel::~CLStorePanel()
{
}

CLStorePanel* CLStorePanel::create(CLStoreData* pData, CCNode *normalSprite, CCNode *selectedSprite, CCNode *disabledSprite, CCObject *target, SEL_MenuHandler selector)
{
    CLStorePanel* pLayer = new CLStorePanel();
    if(pLayer && pLayer->init(pData, normalSprite, selectedSprite, disabledSprite, target, selector))
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


bool CLStorePanel::init(CLStoreData* pData, CCNode *normalSprite, CCNode *selectedSprite, CCNode *disabledSprite, CCObject *target, SEL_MenuHandler selector)
{
    if(!initWithNormalSprite(normalSprite, selectedSprite, disabledSprite, target, selector))
        return false;
    m_pStoreData = pData;
    
    //这里可以根据数据里面的信息做这种操作
    
    
    
    return true;
}

CLMainSceneLayer::CLMainSceneLayer()
{
    m_pPowerBar = NULL;
    m_pCPowerFont = NULL;
    m_pMPowerFont = NULL;
    m_pMoneyFont = NULL;
    m_pCardFont = NULL;
    m_bUpdateStores = false;
    m_pStoreMenu = NULL;
    m_nEnterSceneID = 0;
    
    m_pAction = NULL;//wangguolong
    m_pAchiNumBg = NULL;
    m_pAchiTTF = NULL;
}

CLMainSceneLayer::~CLMainSceneLayer()
{
    
}

bool CLMainSceneLayer::init()
{
    if(!CLSceneLayer::init())
        return false;
    
    CCSize size = CCDirector::sharedDirector()->getgSceneSize();
    
    //添加背景
    CCSprite* pBGSpr = CCSprite::create("Image/LoginLayer/ML_BG.png");
    pBGSpr->setPosition(ccp(size.width/2, size.height/2));
    this->addChild(pBGSpr, 0, 10086);

    //五六商店覆盖用
    CCSprite* bg = CCSprite::create("Image/LoginLayer/ML_bg1.png");
    bg->setPosition(ccp(size.width/2, G_SF(530)));
    pBGSpr->addChild(bg, 10);
    
    //三四商店覆盖用
    CCSprite* bg2 = CCSprite::create("Image/LoginLayer/ML_bg2.png");
    bg2->setPosition(ccp(size.width/2, G_SF(372)));
    pBGSpr->addChild(bg2,10);
    
    //对战大厦覆盖用
    CCSprite* bg3 = CCSprite::create("Image/LoginLayer/ML_bg3.png");
    bg3->setPosition(ccp(G_SF(328), G_SF(590)));
    pBGSpr->addChild(bg3, 10);
    
    //左路灯1
    CCSprite* LeftStreetLamp1 = CCSprite::create("Image/LoginLayer/LeftStreetLamp.png");
    LeftStreetLamp1->setPosition(ccp(G_SF(180), G_SF(0)));
    LeftStreetLamp1->setScale(1.3);
    pBGSpr->addChild(LeftStreetLamp1, 10);
    
//    //左路灯2
//    CCSprite* LeftStreetLamp2 = CCSprite::create("Image/LoginLayer/LeftStreetLamp.png");
//    LeftStreetLamp2->setPosition(ccp(G_SF(270), G_SF(200)));
//    pBGSpr->addChild(LeftStreetLamp2, 10);
    
    //左路灯3
    CCSprite* LeftStreetLamp3 = CCSprite::create("Image/LoginLayer/LeftStreetLamp.png");
    LeftStreetLamp3->setPosition(ccp(G_SF(335), G_SF(280)));
    LeftStreetLamp3->setScale(0.6);
    pBGSpr->addChild(LeftStreetLamp3, 10);

    //左路灯4
    CCSprite* LeftStreetLamp4 = CCSprite::create("Image/LoginLayer/LeftStreetLamp.png");
    LeftStreetLamp4->setScale(0.5);
    LeftStreetLamp4->setPosition(ccp(G_SF(325), G_SF(420)));
    pBGSpr->addChild(LeftStreetLamp4, 10);
    
    //右边路灯1
    CCSprite* RightStreetLamp1 = CCSprite::create("Image/LoginLayer/RightStreetLamp.png");
    RightStreetLamp1->setPosition(ccp(G_SF(403), G_SF(125)));
    pBGSpr->addChild(RightStreetLamp1, 10);
    
    //右边路灯2
    CCSprite* RightStreetLamp2 = CCSprite::create("Image/LoginLayer/RightStreetLamp.png");
    RightStreetLamp2->setPosition(ccp(G_SF(425), G_SF(385)));
    RightStreetLamp2->setScale(0.7);
    pBGSpr->addChild(RightStreetLamp2, 10);

    //右边路灯3
    CCSprite* RightStreetLamp3 = CCSprite::create("Image/LoginLayer/RightStreetLamp.png");
    RightStreetLamp3->setPosition(ccp(G_SF(330), G_SF(461)));
    RightStreetLamp3->setScale(0.5);
    pBGSpr->addChild(RightStreetLamp3, 10);

    //右边路灯4
    CCSprite* RightStreetLamp4 = CCSprite::create("Image/LoginLayer/RightStreetLamp.png");
    RightStreetLamp4->setPosition(ccp(G_SF(260), G_SF(485)));
    RightStreetLamp4->setScale(0.4);
    pBGSpr->addChild(RightStreetLamp4, 10);
    
    //人物一
    CCSprite* Person1 = CCSprite::create("Image/LoginLayer/Person1.png");
    Person1->setPosition(ccp(G_SF(407), G_SF(396)));
    Person1->setScale(0.8);
    pBGSpr->addChild(Person1, 10);
    
    //人物二
    CCSprite* Person2 = CCSprite::create("Image/LoginLayer/Person2.png");
    Person2->setPosition(ccp(G_SF(348), G_SF(360)));
    Person2->setScale(0.8);
    pBGSpr->addChild(Person2, 9);
    
    //人物三
    CCSprite* Person3 = CCSprite::create("Image/LoginLayer/Person3.png");
    Person3->setPosition(ccp(G_SF(437), G_SF(100)));
    pBGSpr->addChild(Person3, 10);
    
    //人物四
    CCSprite* Person4 = CCSprite::create("Image/LoginLayer/Person4.png");
    Person4->setPosition(ccp(G_SF(468), G_SF(40)));
    pBGSpr->addChild(Person4, 10);
    
    //头像信息区域Z坐标为最上层
    CCLayer *topInfoLayer = CCLayerColor::create();
    
    this->addChild(topInfoLayer, 1000, 1000);
    
    //添加头像
    UserInfoSprite *userInfo = UserInfoSprite::create();
    userInfo->setPosition(ccp(G_SF(170), G_SF(889)));
    topInfoLayer->addChild(userInfo);
    
    //    //添加体力条
    //    CCTexture2D* pTexture = CCTextureCache::sharedTextureCache()->addImage("Image/LoginLayer/ML_PpowerBar.png");
    //    float fPre = (float)CLPlayerManager::shardPlayerManager()->getpUserInfo()->getnCurPower()/CLPlayerManager::shardPlayerManager()->getpUserInfo()->getnMaxPower();
    //    m_pPowerBar = CCSprite::createWithTexture(pTexture, CCRectMake(0, 0, pTexture->getContentSize().width*fPre, pTexture->getContentSize().height));
    //    m_pPowerBar->setAnchorPoint(CCPointZero);
    //    m_pPowerBar->setPosition(ccp(G_SF(20),G_SF(32)));
    //    pRolePanel->addChild(m_pPowerBar, 1);
    
    //添加监听事件
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CLMainSceneLayer::UpdatePower), NOTICE_UPDATEPOWER, NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CLMainSceneLayer::UpdateMoney), NOTICE_UPDATEMONEY, NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CLMainSceneLayer::UpdateCard), NOTICE_UPDATECARD, NULL);
    
    //    //体力文字
    //    m_pCPowerFont = CCLabelBMFont::create(CCString::createWithFormat("%d", CLPlayerManager::shardPlayerManager()->getpUserInfo()->getnCurPower())->getCString(), "Data/strings/shuzu2.fnt");
    //    m_pCPowerFont->setAnchorPoint(ccp(1, 0));
    //    m_pCPowerFont->setPosition(ccp(G_SF(62), G_SF(40)));
    //    pRolePanel->addChild(m_pCPowerFont, 3);
    //
    //    m_pMPowerFont = CCLabelBMFont::create(CCString::createWithFormat("/%d", CLPlayerManager::shardPlayerManager()->getpUserInfo()->getnMaxPower())->getCString(), "Data/strings/shuzu1.fnt");
    //    m_pMPowerFont->setAnchorPoint(ccp(0, 0));
    //    m_pMPowerFont->setPosition(ccp(G_SF(65), G_SF(40)));
    //    pRolePanel->addChild(m_pMPowerFont, 3);
    
    //添加钞票条
    CCSprite* pMoneyBG = CCSprite::create("Image/LoginLayer/ML_MoneyBG.png");
    pMoneyBG->setPosition(ccp(G_SF(520), G_SF(916)));
    //    pBGSpr->addChild(pMoneyBG, 1);
    topInfoLayer->addChild(pMoneyBG, 1);
    
    m_pMoneyFont = CCLabelBMFont::create(CCString::createWithFormat("%d", CLPlayerManager::shardPlayerManager()->getpUserInfo()->getMoneys())->getCString(), "Data/strings/shuzu3.fnt", G_SF(20));
    m_pMoneyFont->setPosition(ccp(pMoneyBG->getContentSize().width/2, pMoneyBG->getContentSize().height/2));
    pMoneyBG->addChild(m_pMoneyFont, 1);
    
    CCSprite* pMoneyIcon = CCSprite::create("Image/LoginLayer/ML_MoneyIcon.png");
    pMoneyIcon->setPosition(ccp(G_SF(-25), G_SF(34)));
    pMoneyBG->addChild(pMoneyIcon, 1);
    
    //添加购物卡条
    CCSprite* pCardBG = CCSprite::create("Image/LoginLayer/ML_MoneyBG.png");
    pCardBG->setPosition(ccp(G_SF(520), G_SF(854)));
    //    pBGSpr->addChild(pCardBG, 1);
    topInfoLayer->addChild(pCardBG, 1);
    
    CCSprite* pCardIcon = CCSprite::create("Image/LoginLayer/ML_CardIcon.png");
    pCardIcon->setPosition(ccp(G_SF(-25), G_SF(32)));
    pCardBG->addChild(pCardIcon, 1);
    
    m_pCardFont = CCLabelBMFont::create(CCString::createWithFormat("%d", CLPlayerManager::shardPlayerManager()->getpUserInfo()->getJetton())->getCString(), "Data/strings/shuzu3.fnt");
    m_pCardFont->setPosition(ccp(pCardBG->getContentSize().width/2, pCardBG->getContentSize().height/2));
    pCardBG->addChild(m_pCardFont, 1);
    
    //增加二个添加按钮
    CCMenu* pAddMenu = CCMenu::create();
    pAddMenu->setAnchorPoint(CCPointZero);
    pAddMenu->setPosition(CCPointZero);
    //    pBGSpr->addChild(pAddMenu, 3);
    pAddMenu->setTouchPriority(-100000);
    topInfoLayer->addChild(pAddMenu, 3);
    
    for(int i=0; i<2; i++)
    {
        CCSprite* pSprNor = CCSprite::create("Image/LoginLayer/ML_AddIcon_N.png");
        CCSprite* pSprSel = CCSprite::create("Image/LoginLayer/ML_AddIcon_S.png");
        CCMenuItemSprite* pItem = CCMenuItemSprite::create(pSprNor, pSprSel, NULL, this, callfuncO_selector(CLMainSceneLayer::onAddMenuPress));
        pItem->setPosition(ccp(G_SF(600), G_SF(917)-i*G_SF(61)));
        pAddMenu->addChild(pItem, 0, i);
    }

    //添加分支模块功能按钮
    CCMenu* pMenu = CCMenu::create();
    pMenu->setAnchorPoint(CCPointZero);
    pMenu->setPosition(CCPointZero);
    pMenu->setTouchPriority(-1000);
    pBGSpr->addChild(pMenu, 11);
    
    for(int i=0; i < 6; i++)
    {
        CCMenuItemSprite* pItem = NULL;
        CCSprite* pNor = NULL;
        CCSprite* pSel = NULL;
        
        switch (i)
        {
                //设置
            case 0:
            {
                pNor = CCSprite::create("Image/LoginLayer/ML_SettingIcon_N.png");
                pSel = CCSprite::create("Image/LoginLayer/ML_SettingIcon_S.png");
                pItem = CCMenuItemSprite::create(pNor, pSel, NULL, this, callfuncO_selector(CLMainSceneLayer::onFunctionMenuPress));
                pItem->setPosition(ccp(G_SF(62),G_SF(327)));
            }
                break;
                
                //成就
            case 1:
            {
                pNor = CCSprite::create("Image/LoginLayer/ML_AchieveIcon_N.png");
                pSel = CCSprite::create("Image/LoginLayer/ML_AchieveIcon_S.png");
                pItem = CCMenuItemSprite::create(pNor, pSel, NULL, this, callfuncO_selector(CLMainSceneLayer::onFunctionMenuPress));
                pItem->setPosition(ccp(G_SF(580),G_SF(327)));
                
                //未领取成就提醒
                int achiNo = CLSceneManager::sharedSceneManager()->GetUngetAchiNum();
                m_pAchiNumBg = CCSprite::create("Image/PublicUI/AchiNum_Bg.png");
                m_pAchiNumBg->setPosition(ccp(m_pAchiNumBg->getContentSize().width * 0.5,pNor->getContentSize().height - m_pAchiNumBg->getContentSize().height * 0.5));
                pItem->addChild(m_pAchiNumBg, 10);
                
                m_pAchiTTF = CCLabelTTF::create(CCString::createWithFormat("%d", achiNo)->getCString(), "Data/strings/shuzu3.fnt", G_SF(18));
                m_pAchiTTF->setPosition(ccp(m_pAchiNumBg->getContentSize().width * 0.5, m_pAchiNumBg->getContentSize().height * 0.5));
                m_pAchiNumBg->addChild(m_pAchiTTF, 1, 100);
                
                if (achiNo <= 0)
                {
                    m_pAchiNumBg->setVisible(false); //如果未领取成就小于等于0，则设置隐藏
                }
            }
                break;
                //奖品橱窗
            case 2:
            {
                pNor = CCSprite::create("Image/LoginLayer/ML_AwardIcon_N.png");
                pSel = CCSprite::create("Image/LoginLayer/ML_AwardIcon_S.png");
                pItem = CCMenuItemSprite::create(pNor, pSel, NULL, this, callfuncO_selector(CLMainSceneLayer::onFunctionMenuPress));
                pItem->setPosition(ccp(G_SF(62),G_SF(224)));
            }
                break;
                //消息
            case 3:
            {
                pNor = CCSprite::create("Image/LoginLayer/ML_MessageIcon_N.png");
                pSel = CCSprite::create("Image/LoginLayer/ML_MessageIcon_S.png");
                pItem = CCMenuItemSprite::create(pNor, pSel, NULL, this, callfuncO_selector(CLMainSceneLayer::onFunctionMenuPress));
                pItem->setPosition(ccp(G_SF(580),G_SF(224)));
            }
                break;
                //商店
            case 4:
            {
                pNor = CCSprite::create("Image/LoginLayer/ML_ShopIcon_N.png");
                pSel = CCSprite::create("Image/LoginLayer/ML_ShopIcon_S.png");
                pItem = CCMenuItemSprite::create(pNor, pSel, NULL, this, callfuncO_selector(CLMainSceneLayer::onFunctionMenuPress));
                pItem->setPosition(ccp(G_SF(90),G_SF(96)));
            }
                break;
                //背包
            case 5:
            {
                pNor = CCSprite::create("Image/LoginLayer/ML_BagIcon_N.png");
                pSel = CCSprite::create("Image/LoginLayer/ML_BagIcon_S.png");
                pItem = CCMenuItemSprite::create(pNor, pSel, NULL, this, callfuncO_selector(CLMainSceneLayer::onFunctionMenuPress));
                pItem->setPosition(ccp(G_SF(553),G_SF(96)));
            }
                break;
                
            default:
                break;
        }
        
        pMenu->addChild(pItem, 11, i);
    }
    
    CCMenu* pPvpMenu = CCMenu::create();
    pPvpMenu->setAnchorPoint(CCPointZero);
    pPvpMenu->setPosition(CCPointZero);
    pBGSpr->addChild(pPvpMenu, 0);
    
    CCSprite* p1 = CCSprite::create("Image/LoginLayer/ML_StoreBut6_N.png");
    CCSprite* pS = CCSprite::create("Image/LoginLayer/ML_StoreBut6_S.png");
    CCMenuItemSprite* pItem = CCMenuItemSprite::create(p1, pS, NULL, this, callfuncO_selector(CLMainSceneLayer::onPvpMenuPress));
    pItem->setPosition(ccp(G_SF(327),G_SF(633)));
    pPvpMenu->addChild(pItem, 0, 0);
    
    
    //根据单例网络json列表库有无添加商铺按钮
    int nStoreNum = CLSceneManager::sharedSceneManager()->GetStoreDataNum();
    if(nStoreNum && !m_pStoreMenu)   //说明需要创建
    {
        CreateStoresMenu();
    }
    
    this->scheduleOnce(schedule_selector(CLMainSceneLayer::SerPushAchiCreate), 1.0); //如果闯关对战由成就推送，则弹出通知
    
    
    setnCTIndex(CLScene_MainScene);
    
    if(CLSceneManager::sharedSceneManager()->getnGgaoFlg() == 0)
    {
        //添加公告页面
        BillboardLayer * pBillboardLayer = BillboardLayer::create();
        this->addChild(pBillboardLayer,1200,12000);
    }

    
    return true;
}

void CLMainSceneLayer::CreateStoresMenu()
{
    m_pStoreMenu = CCMenu::create();
    m_pStoreMenu->setAnchorPoint(CCPointZero);
    m_pStoreMenu->setPosition(CCPointZero);
    
    CCNode* pParNode = this->getChildByTag(10086);
    pParNode->addChild(m_pStoreMenu, 0);
    int nMax = CLSceneManager::sharedSceneManager()->GetStoreDataNum();
    if(nMax == 0)
        return;
    for(int i=0; i < nMax; i++)
    {
        CLStorePanel* pItem = NULL;
        CCSprite* pNor = NULL;
        CCSprite* pSel = NULL;
        CLStoreData* pData = CLSceneManager::sharedSceneManager()->GetStoreDataFromList(i);
        
        switch (i)
        {
            case 0:
            {
                pNor = CCSprite::create("Image/LoginLayer/ML_StoreBut0_N.png");
                pSel = CCSprite::create("Image/LoginLayer/ML_StoreBut0_S.png");
                pItem = CLStorePanel::create(pData, pNor, pSel, NULL, this, callfuncO_selector(CLMainSceneLayer::onStoreMenuPress));
                
                //新解锁了该商铺
                if(CLSceneManager::sharedSceneManager()->GetStoreDataFromList(1)->getnOneLevel() != 0)
                {
                    Action(pItem);
                    
                }
                pItem->setAnchorPoint(ccp(0.5, 0));
                
                //设置锚点以后的坐标
                pItem->setPosition(ccp(G_SF(208),G_SF(222) - pNor->getContentSize().height*0.5));
            }
                break;
            case 1:
            {
                pNor = CCSprite::create("Image/LoginLayer/ML_StoreBut1_N.png");
                pNor->setAnchorPoint(ccp(0.5, 0));
                pSel = CCSprite::create("Image/LoginLayer/ML_StoreBut1_S.png");
                pItem = CLStorePanel::create(pData, pNor, pSel, NULL, this, callfuncO_selector(CLMainSceneLayer::onStoreMenuPress));
                
                //新解锁了该商铺
                if(CLSceneManager::sharedSceneManager()->getnNolock() == 1 && CLSceneManager::sharedSceneManager()->getnewlevel() > -1)
                {
                    Action(pItem);
                    
                }
                pItem->setAnchorPoint(ccp(0.5, 0));
                
                //设置锚点以后的坐标
                pItem->setPosition(ccp(G_SF(494),G_SF(129)));
            }
                break;
            case 2:
            {
                
                
                pNor = CCSprite::create("Image/LoginLayer/ML_StoreBut2_N.png");
                pSel = CCSprite::create("Image/LoginLayer/ML_StoreBut2_S.png");
                pItem = CLStorePanel::create(pData, pNor, pSel, NULL, this, callfuncO_selector(CLMainSceneLayer::onStoreMenuPress));
                
                //新解锁了该商铺
                if(CLSceneManager::sharedSceneManager()->getnNolock() == 2 && CLSceneManager::sharedSceneManager()->getnewlevel() > -1)
                {
                    Action(pItem);
                    
                }
                pItem->setAnchorPoint(ccp(0.5, 0));
                
                //设置锚点以后的坐标
                pItem->setPosition(ccp(G_SF(151),G_SF(397) - pNor->getContentSize().height*0.5));
                
            }
                break;
            case 3:
            {
                pNor = CCSprite::create("Image/LoginLayer/ML_StoreBut3_N.png");
                pSel = CCSprite::create("Image/LoginLayer/ML_StoreBut3_S.png");
                pItem = CLStorePanel::create(pData, pNor, pSel, NULL, this, callfuncO_selector(CLMainSceneLayer::onStoreMenuPress));
                
                //新解锁了该商铺
                if(CLSceneManager::sharedSceneManager()->getnNolock() == 3 && CLSceneManager::sharedSceneManager()->getnewlevel() > -1)
                {
                    Action(pItem);
                    
                }
                pItem->setAnchorPoint(ccp(0.5, 0));
                
                //设置锚点以后的坐标
                pItem->setPosition(ccp(G_SF(484),G_SF(395) - pNor->getContentSize().height*0.5));
                
            }
                break;
            case 4:
            {
                pNor = CCSprite::create("Image/LoginLayer/ML_StoreBut4_N.png");
                pSel = CCSprite::create("Image/LoginLayer/ML_StoreBut4_S.png");
                pItem = CLStorePanel::create(pData, pNor, pSel, NULL, this, callfuncO_selector(CLMainSceneLayer::onStoreMenuPress));
                //新解锁了该商铺
                if(CLSceneManager::sharedSceneManager()->getnNolock() == 4 && CLSceneManager::sharedSceneManager()->getnewlevel() > -1)
                {
                    Action(pItem);
                    
                }
                pItem->setAnchorPoint(ccp(0.5, 0));
                
                //设置锚点以后的坐标
                pItem->setPosition(ccp(G_SF(140),G_SF(564) - pNor->getContentSize().height*0.5));
                
            }
                break;
            case 5:
            {
                pNor = CCSprite::create("Image/LoginLayer/ML_StoreBut5_N.png");
                pSel = CCSprite::create("Image/LoginLayer/ML_StoreBut5_S.png");
                pItem = CLStorePanel::create(pData, pNor, pSel, NULL, this, callfuncO_selector(CLMainSceneLayer::onStoreMenuPress));
                
                //新解锁了该商铺
                if(CLSceneManager::sharedSceneManager()->getnNolock() == 5 && CLSceneManager::sharedSceneManager()->getnewlevel() > -1)
                {
                    Action(pItem);
                    
                }
                pItem->setAnchorPoint(ccp(0.5, 0));
                
                //设置锚点以后的坐标
                pItem->setPosition(ccp(G_SF(497),G_SF(560) - pNor->getContentSize().height*0.5));
            }
                break;
                
                
            default:
                break;
        }
        
        m_pStoreMenu->addChild(pItem, 1, i);
    }
    
}

void CLMainSceneLayer::Action(CLStorePanel* pSender)
{
    m_pAction = (CCActionInterval*)CCSequence::create(
                                                      CCDelayTime::create(3.0),
                                                      CCScaleBy::create(0.2, 1.1,0.9),
                                                      CCScaleTo::create(0.2, 1.0,1.0),
                                                      CCScaleBy::create(0.2, 0.8,1.1),
                                                      CCScaleTo::create(0.2, 1.0,1.0),
                                                      CCScaleBy::create(0.2, 1.1,0.9),
                                                      CCScaleTo::create(0.2, 1.0,1.0),
                                                      CCDelayTime::create(1.5),
                                                      NULL
                                                      );

    pSender->runAction(CCRepeatForever::create(m_pAction));

}

void CLMainSceneLayer::onStoreMenuPress(CCObject* pSender)
{
    //商铺按下音效
    CLSoundManager::sharedSoundManager()->PlayEffectSound("Sound/Sl_ComeInShop.wav");
    
    CLStorePanel* pItem = (CLStorePanel*)pSender;
    
    //点击新解锁的店铺以后 停止Action

        stopAction(m_pAction);
        m_pAction = NULL;
        
        CLSceneManager::sharedSceneManager()->setnewlevel(-1);
    
    switch (pItem->getTag())
    {
        case 0:
        {
            if(pItem->getpStoreData()->getbLocked())
                break;
            //            Json::Value jv(Json::objectValue);
            //            jv["storeNo"] = pItem->getpStoreData()->getnNomber();
            //            CLNetWorkManager::shardNetWorkManager()->SendNetMessage(CMD_SHELFS, jv);
            //
            //            CLSceneManager::sharedSceneManager()->setpCallTarget(this);
            //            CLSceneManager::sharedSceneManager()->setpCallFun(callfunc_selector(CLMainSceneLayer::AdvImageLoadEnd));
            //
            //            CLSceneManager::sharedSceneManager()->getpLoadingLayer()->setVisible(true);
            
        }
            break;
        case 1:
        {
            if(pItem->getpStoreData()->getbLocked())
                break;
        }
            break;
        case 2:
        {
            if(pItem->getpStoreData()->getbLocked())
                break;
        }
            break;
        case 3:
        {
            if(pItem->getpStoreData()->getbLocked())
                break;
        }
            break;
        case 4:
        {
            if(pItem->getpStoreData()->getbLocked())
                break;
        }
            break;
        case 5:
        {
            if(pItem->getpStoreData()->getbLocked())
                break;
        }
            break;
            
        default:
            break;
    }
    
    //add by yin
    if (!pItem->getpStoreData()->getbLocked())
    { //货架解锁情况下发送请求
        Json::Value jv(Json::objectValue);
        jv["storeNo"] = pItem->getpStoreData()->getnNomber();
        CLNetWorkManager::shardNetWorkManager()->SendNetMessage(CMD_SHELFS, jv);
        
        CLSceneManager::sharedSceneManager()->setpCallTarget(this);
        CLSceneManager::sharedSceneManager()->setpCallFun(callfunc_selector(CLMainSceneLayer::AdvImageLoadEnd));
        
        CLSceneManager::sharedSceneManager()->getpLoadingLayer()->setVisible(true);
    }
    else
    {
        
        //未解锁处理
        TiShiLayer::create(CLChangeString::sharedChangeString()->GetChangeString("LockMsg"));
        return;
        
    }
    
    //进入第一个商铺点击以后 动作停止
    CLSceneManager::sharedSceneManager()->GetStoreDataFromList(1)->setnOneLevel(0);
    
    m_nEnterSceneID = 1;
}


void CLMainSceneLayer::onFunctionMenuPress(CCObject* pSender)
{
    CCMenuItemSprite* pItem = (CCMenuItemSprite*)pSender;
    CCLOG("tag:%d", pItem->getTag());
    
    CCSize size = CCDirector::sharedDirector()->getgSceneSize();
    
    switch (pItem->getTag())
    {
            
        case 0:
        {
            
            CLSceneLayer* pMainLayer = (CLSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
            if(pMainLayer->getnCTIndex() != CLScene_MainScene)
            {
                CCLOG("Error: The cur scene is not the main scene!!!");
                break;
            }
            
            GameVolumeSet *VolumeSet = GameVolumeSet::create();
            VolumeSet->setPosition(ccp(0, -size.height));
            pMainLayer->addChild(VolumeSet,10, VOLUMESET);
            break;
        }
            
        case 1:
        {
            //20131101 add by yin
            //成就按钮按下，发送请求
            Json::Value value;
            CLNetWorkManager::shardNetWorkManager()->SendNetMessage(CMD_PLYACHIEVE, value);
            CLSceneManager::sharedSceneManager()->SetWaitNetLayer(true);
            
            break;
        }
            
        case 2:
        {
            
            //            //20131104 add by wang
            //            //印花按钮按下，发送请求
            //            Json::Value value;
            //            CLNetWorkManager::shardNetWorkManager()->SendNetMessage(CMD_PRIZESHOW, value);
            //            CLSceneManager::sharedSceneManager()->SetWaitNetLayer(true);
            //奖品橱窗数据存在的情况
            CCSize size = CCDirector::sharedDirector()->getgSceneSize();
            if(CLSceneManager::sharedSceneManager()->GetPrizeNum() > 0){
                YHtopLayer *yh = YHtopLayer::create();
                //                yh->setPosition(ccp(0, 0));
                yh->setPosition(ccp(0, size.height * (-1)));
                CLSceneLayer* pMainLayer = (CLSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
                if(pMainLayer->getnCTIndex() != CLScene_MainScene)
                {
                    CCLOG("Error: The cur scene is not the main scene!!!");
                    break;
                }
                pMainLayer->addChild(yh,10, STAMPTAG);
            }else
            {
                //奖品橱窗数据不存在的情况下
                Json::Value value;
                CLNetWorkManager::shardNetWorkManager()->SendNetMessage(CMD_PRIZESHOW, value);
                CLSceneManager::sharedSceneManager()->SetWaitNetLayer(true);
            }
            
            break;
        }
            
        case 3:
        {
//            //个人信息页面 tets用 以后删掉。wang
//            PersonalInfoLayer *PI = PersonalInfoLayer::create();
//            CLSceneLayer* pMainLayer = (CLSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
//            if(pMainLayer->getnCTIndex() != CLScene_MainScene)
//            {
//                CCLOG("Error: The cur scene is not the main scene!!!");
//                break;
//            }
//            pMainLayer->addChild(PI,1001,1001);
            
            break;
        }
            
        case 4:
        {
            //20131101 add by yin
            //商城按钮按下，发送请求
            Json::Value value;
            CLNetWorkManager::shardNetWorkManager()->SendNetMessage(CMD_SHOPSHOW, value);
            CLSceneManager::sharedSceneManager()->SetWaitNetLayer(true);
            
            break;
        }
            
            //背包
        case 5:
        {
            Json::Value value;
            CLNetWorkManager::shardNetWorkManager()->SendNetMessage(CMD_BACKPACK, value);
            CLSceneManager::sharedSceneManager()->SetWaitNetLayer(true);
            
            break;
        }
            
        default:
            break;
    }
}

void CLMainSceneLayer::onAddMenuPress(CCObject* pSender)
{
    CLSceneLayer* pMainLayer = (CLSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
    
    CCMenuItemSprite* pItem = (CCMenuItemSprite*)pSender;
    switch (pItem->getTag())
    {
        case 0:
        {
            if(pMainLayer->getChildByTag(RMBBUYCARD)){
                pMainLayer->getChildByTag(RMBBUYCARD)->removeFromParentAndCleanup(true);
                if(pMainLayer->getChildByTag(500))
                {
                    pMainLayer->getChildByTag(500)->removeFromParentAndCleanup(true);
                }
            }
            Json::Value value;
            CLNetWorkManager::shardNetWorkManager()->SendNetMessage(CMD_SHOPSHOW, value);
            CLSceneManager::sharedSceneManager()->SetWaitNetLayer(true);
        }
            break;
        case 1:
        {
            if(pMainLayer->getChildByTag(SHOPTAG)){
                pMainLayer->getChildByTag(SHOPTAG)->removeFromParentAndCleanup(true);
                if(pMainLayer->getChildByTag(500))
                {
                    pMainLayer->getChildByTag(500)->removeFromParentAndCleanup(true);
                }
                
            }
            //CMD_RMBBUYCARD 109 请求RMB购买购物卡信息
            Json::Value value;
            CLNetWorkManager::shardNetWorkManager()->SendNetMessage(CMD_RMBBUYCARD, value);
            CLSceneManager::sharedSceneManager()->SetWaitNetLayer(true);
            
        }
            break;
            
        default:
            break;
    }
}

void CLMainSceneLayer::UpdatePower(CCObject* pSender)
{
    
}

void CLMainSceneLayer::UpdateMoney(CCObject* pSender)
{
    CLSoundManager::sharedSoundManager()->PlayEffectSound("Sound/Sl_GetGoods.wav");
    m_pMoneyFont->setString(CCString::createWithFormat("%d", CLPlayerManager::shardPlayerManager()->getpUserInfo()->getMoneys())->getCString());
}

void CLMainSceneLayer::UpdateCard(CCObject* pSender)
{
    CLSoundManager::sharedSoundManager()->PlayEffectSound("Sound/Sl_GetGoods.wav");
    m_pCardFont->setString(CCString::createWithFormat("%d", CLPlayerManager::shardPlayerManager()->getpUserInfo()->getJetton())->getCString());
}

void CLMainSceneLayer::onExit()
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, NOTICE_UPDATEPOWER);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, NOTICE_UPDATEMONEY);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, NOTICE_UPDATECARD);
    
    CCLayer::onExit();
}

void CLMainSceneLayer::onPvpMenuPress(CCObject* pSender)
{
    CLSceneManager::sharedSceneManager()->setpCallTarget(this);
    CLSceneManager::sharedSceneManager()->setpCallFun(callfunc_selector(CLMainSceneLayer::AdvImageLoadEnd));
    CLSceneManager::sharedSceneManager()->getpLoadingLayer()->setVisible(true);
    m_nEnterSceneID = 2;
    
    //    if(CLSceneManager::sharedSceneManager()->GetPvpRoomNum() == 0) //如果房间列表为空的话 那么需要向服务器请求
    //    {
    //        Json::Value value;
    //        CLNetWorkManager::shardNetWorkManager()->SendNetMessage(CMD_PK1_ROOMS, value);
    //
    //    }
    //    else
    //    {
    //        CLSceneManager::sharedSceneManager()->BeginLoadAdvImages(CMD_PK1_ROOMS_REPLY);
    //    }
    
    Json::Value value;
    CLNetWorkManager::shardNetWorkManager()->SendNetMessage(CMD_PK1_ROOMS, value);
}

void CLMainSceneLayer::AdvImageLoadEnd()
{
    if(m_nEnterSceneID ==1)     //闯关
    {
        CLShelfsSceneLayer* pLayer = CLShelfsSceneLayer::create();
        pLayer->setAnchorPoint(CCPointZero);
        pLayer->setPosition(CCPointZero);
        CLSceneManager::sharedSceneManager()->ChangeScene(pLayer);
    }
    else if(m_nEnterSceneID == 2)   //对战
    {
        CLPvpSceneLayer* pLayer = CLPvpSceneLayer::create();
        pLayer->setAnchorPoint(CCPointZero);
        pLayer->setPosition(CCPointZero);
        CLSceneManager::sharedSceneManager()->ChangeScene(pLayer);
    }
    
}

//闯关或对战成就主动推送
void CLMainSceneLayer::SerPushAchiCreate()
{
    if (CLSceneManager::sharedSceneManager()->GetIfHadMsgShow())
    {
        CLAchiReachLayer *clAchi = CLAchiReachLayer::create();
        clAchi->setPosition(CCPointZero);
        this->addChild(clAchi, 10,ACHIEVEMENTS + 1);
    }

}

//修改成就按钮显示数字
void CLMainSceneLayer::UpdateNewAchi()
{
    int achiNo = CLSceneManager::sharedSceneManager()->GetUngetAchiNum();
    m_pAchiTTF->setString(CCString::createWithFormat("%d", achiNo)->getCString());
    if (achiNo <= 0)
    {
        m_pAchiNumBg->setVisible(false);
    }
}


