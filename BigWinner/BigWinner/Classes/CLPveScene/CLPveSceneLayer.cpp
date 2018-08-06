//
//  CLPveSceneLayer.cpp
//  BigWinner
//
//  Created by Zhang Qiyun on 13-10-26.
//
//
// modify info
// 20131217 修改头像区域，使用封装的精灵

#include "CLPveSceneLayer.h"
#include "CLPlayerManager.h"
#include "CLSceneManager.h"
#include "CLChangeString.h"
#include "CLNetWorkManager.h"
#include "CLShelfsSceneLayer.h"
#include "JsonDataManager.h"
#include "CLPveGiveupLayer.h"
#include "CLTopicData.h"
#include "CLFightItem.h"
#include "CLMainSceneLayer.h"
#include "UserInfoSprite.h"
#include "CLPveSMangerLayer.h"
#include "CLAnimationManager.h"
#include "CLSoundManager.h"

#define ANIMATION_PROS   111

CLPveSceneLayer::CLPveSceneLayer()
{
    m_pSchedueTTF = NULL;
    m_pTopicLayer = NULL;
    m_nSchTime = 0;
    m_pShowGoodsBG = NULL;
    m_nEnterTime = 3; //倒计时时间
    m_nShelfNo = 0;
    m_pYinhuaIcon = NULL;
    m_nYinhuaNo = 0;
    m_pYinhuaMenu = NULL;
    m_bYinhuaMoveR = true;
    m_nYIndex = 0;
    m_pFunButtonBG = NULL;
    m_pSchBG = NULL;
    m_pResultLayer = NULL;
    m_bSMStyle = false;
    m_pSMNameBG = NULL;
    m_pSMForm = NULL;
    m_pSMMenu = NULL;
    m_pGiveUpMenu = NULL;
    m_pPorpMenu = NULL;
    m_bRoundUseProp = false;
    m_iBossShelfNo = 0;
}

CLPveSceneLayer::~CLPveSceneLayer()
{
    
}

CLPveSceneLayer* CLPveSceneLayer::create(bool bClickSM)
{
    CLPveSceneLayer* pLayer = new CLPveSceneLayer();
    if(pLayer && pLayer->init(bClickSM))
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


bool CLPveSceneLayer::init(bool bClickSM)
{
    if(!CLSceneLayer::init())
        return false;
    
    m_bSMStyle = bClickSM;
    CCSize size = CCDirector::sharedDirector()->getgSceneSize();
    
    //添加背景 //wangguolong 20131201
    int StoreNo = 0;
    std::string StoreBG_path;
    Json::Value* jv = CLSceneManager::sharedSceneManager()->GetJsonDataFromMsgID(8);
    
    if(jv)
    {
        StoreNo = (*jv)["storeNo"].asIntModestyle();
    }
    
    //暂时只有两个货架背景图片
//    if(StoreNo % 2 == 0){
//        StoreNo = 2;
//    }else{
//        StoreNo = 1;
//    }
    StoreNo = 3;
    //货架背景图片名称路径
    StoreBG_path = CCString::createWithFormat("Image/LoginLayer/StoreBG%d.png", StoreNo)->getCString();
    
    CCSprite* pBGSpr = CCSprite::create(StoreBG_path.c_str());
    pBGSpr->setPosition(ccp(size.width/2, size.height/2));
    this->addChild(pBGSpr, 0, 10086);
    
    
    //    //添加头像信息背景
    //    CCSprite* pRoleDataBG = CCSprite::create("Image/PublicUI/RoleDataBG.png");
    //    pRoleDataBG->setPosition(ccp(G_SF(160), G_SF(890)));
    //    pBGSpr->addChild(pRoleDataBG, 2);
    //
    //    //添加头像背景
    //    CCSprite* pPlayerHeadBG = CCSprite::create("Image/PublicUI/RoleHeadBG.png");
    //    pPlayerHeadBG->setPosition(ccp(G_SF(67), G_SF(65)));
    //    pRoleDataBG->addChild(pPlayerHeadBG, 1);
    //
    //
    //    //添加头像
    //    CCSprite* pPlayerHead = CCSprite::create("Image/PublicUI/RoleHeadIcon.png");
    //    pPlayerHead->setPosition(ccp(G_SF(67), G_SF(65)));
    //    pRoleDataBG->addChild(pPlayerHead, 2);
    //    //添加昵称背景
    //    CCSprite* pNamePanel = CCSprite::create("Image/PublicUI/RoleNameBG.png");
    //    pNamePanel->setPosition(ccp(G_SF(220), G_SF(94)));
    //    pRoleDataBG->addChild(pNamePanel, 1);
    //    //添加昵称信息
    //    CCLabelTTF* pNameTTF = CCLabelTTF::create(CLPlayerManager::shardPlayerManager()->getpUserInfo()->getsUsername().c_str(), "Arial", G_SF(24));
    //
    //    pNameTTF->setPosition(ccp(pNamePanel->getContentSize().width/2, pNamePanel->getContentSize().height/2));
    //    pNameTTF->setColor(ccc3(200, 100, 50));
    //    pNamePanel->addChild(pNameTTF, 1);
    
    UserInfoSprite *userInfo = UserInfoSprite::create();
    userInfo->setPosition(ccp(G_SF(170), G_SF(889)));
    pBGSpr->addChild(userInfo);
    
    //添加认输按钮
    m_pGiveUpMenu = CCMenu::create();
    m_pGiveUpMenu->setAnchorPoint(CCPointZero);
    m_pGiveUpMenu->setPosition(CCPointZero);
    m_pGiveUpMenu->setVisible(false);
    pBGSpr->addChild(m_pGiveUpMenu, 1);
    
    CCSprite* spr1 = CCSprite::create("Image/PveLayer/CP_GiveupIcon_N.png");
    CCSprite* spr2 = CCSprite::create("Image/PveLayer/CP_GiveupIcon_N.png");
    CCMenuItemSprite* pItem = CCMenuItemSprite::create(spr1, spr2, NULL, this, menu_selector(CLPveSceneLayer::onGiveupMenuPress));
    pItem->setPosition(ccp(G_SF(591), G_SF(907)));
    m_pGiveUpMenu->addChild(pItem, 1);
    
    m_pSchedueTTF = CCLabelBMFont::create("", "Data/strings/shuzu3.fnt", G_SF(22));
    m_pSchedueTTF->setPosition(ccp(G_SF(584), G_SF(728)));
    m_pSchedueTTF->setVisible(false);
    pBGSpr->addChild(m_pSchedueTTF, 3);
    
    if(!m_bSMStyle)   //如果是普通货架模式
    {
        //添加功能按钮背景
        //        m_pFunButtonBG = CCSprite::create("Image/PveLayer/CP_FunButtonBG.png");
        //        m_pFunButtonBG->setAnchorPoint(ccp(0.5, 0));
        //        m_pFunButtonBG->setPosition(ccp(pBGSpr->getContentSize().width/2, 0));
        //        pBGSpr->addChild(m_pFunButtonBG, 1);
        
        Json::Value* pJv = CLSceneManager::sharedSceneManager()->GetJsonDataFromMsgID(10);
        if(!pJv)
            return false;
        
        //中间做处理
        
        //......
        //货架图片名称路径 //wangguolong 20131201
        std::string GoodsShelfName_path;
        //货架类型序号
        int ClassNo = 0;
        
        m_nShelfNo = (*pJv)["shelfNo"].asIntModestyle();
        //货架类型序号
        ClassNo = JsonDataManager::shardJDManager()->m_Shelf[m_nShelfNo]->getnClassNo();
        
        //货架图片名称路径
        GoodsShelfName_path = CCString::createWithFormat("Image/ShelfLayer/GoodsShelf%d.png", ClassNo)->getCString();
        
        m_pShowGoodsBG = CCSprite::create(GoodsShelfName_path.c_str());
        m_pShowGoodsBG->setScale(1.4);
        m_pShowGoodsBG->setPosition(ccp(pBGSpr->getContentSize().width/2, pBGSpr->getContentSize().height/2 -G_SF(110)));
        pBGSpr->addChild(m_pShowGoodsBG, 10);
        
        char sSchTTF[20] = {0};
        sprintf(sSchTTF, "%s %dS", CLChangeString::sharedChangeString()->GetChangeString("StartPve").c_str(), m_nEnterTime);
        
        CCLabelTTF* pSchTimeTTF = CCLabelTTF::create(sSchTTF, "Arial", G_SF(48));
        //        pSchTimeTTF->setPosition(ccp(G_SF(274), G_SF(221)));
        pSchTimeTTF->setPosition(ccp(G_SF(200), G_SF(490)));
        pSchTimeTTF->setColor(ccBLACK);
        m_pShowGoodsBG->addChild(pSchTimeTTF, 2, 10010);
        
        //        m_nShelfNo = (*pJv)["shelfNo"].asIntModestyle();
        CLPlayerManager::shardPlayerManager()->getpUserInfo()->setnShelfNo(m_nShelfNo);
        for(int i=0; i < (*pJv)["goods"].size(); i++)   //在此显示添加的商品
        {
            //添加印花图片
            std::string stampSmall_path = CCString::createWithFormat("Image/GoodIcon/StampSmall%d.png", (*pJv)["goods"][i]["No"].asIntModestyle())->getCString();
            
            CCSprite* pGoodsSpr = CCSprite::create(stampSmall_path.c_str());
            pGoodsSpr->setPosition(ccp(G_SF(100)+i%3*G_SF(200), G_SF(420)-i/3*G_SF(200)));
            m_pShowGoodsBG->addChild(pGoodsSpr, 1);
            
            //印花流光效果
            std::string sFile;
            sFile = "stampLightingEffect";
        
            CCAnimation* animation = CLAnimationManager::sharedAnimationManager()->animationWithName(sFile.c_str(),0.041);
            CCSprite* magic = CLAnimationManager::sharedAnimationManager()->spriteWithAnimationName(sFile.c_str());
            magic->setScaleX(0.41);
            magic->setScaleY(0.41);
            magic->setPosition(ccp(pGoodsSpr->getContentSize().width*0.5, pGoodsSpr->getContentSize().height*0.5));
            pGoodsSpr->addChild(magic, 10);
            
            CCActionInterval* m_pAction = (CCActionInterval*)CCSequence::create(
                                                              CCAnimate::create(animation),
                                                              CCDelayTime::create(0.5),
                                                              NULL
                                                              );
            
            magic->runAction(CCRepeatForever::create(m_pAction));
            
        }
        
        this->schedule(schedule_selector(CLPveSceneLayer::FrameTime), 1.0);
        //音效(初始显示的时候为3S，代码在1S后调用，所以开始显示的时候就要有音效)
        CLSoundManager::sharedSoundManager()->PlayEffectSound("Sound/Sl_CountDown.wav");
        
        m_pYinhuaMenu = CCMenu::create();
        m_pYinhuaMenu->setAnchorPoint(CCPointZero);
        m_pYinhuaMenu->setPosition(CCPointZero);
        m_pYinhuaMenu->setTouchPriority(-1000);
        this->addChild(m_pYinhuaMenu, 2);
        
        CLSceneManager::sharedSceneManager()->RemoveJsonDataFromMsgID(10);
        
        m_pPorpMenu = CCMenu::create();
        m_pPorpMenu->setAnchorPoint(CCPointZero);
        m_pPorpMenu->setPosition(CCPointZero);
        m_pPorpMenu->setVisible(false);
        pBGSpr->addChild(m_pPorpMenu, 1);
        
        std::vector<playerCardJueseInfo*> vPropList = CLPlayerManager::shardPlayerManager()->getpUserInfo()->GetPveProps();
        for(int i=0; i<vPropList.size(); i++)
        {
            CLFightPropItem* pItem = CLFightPropItem::create(vPropList[i]->m_cardJueseInfo->m_artNo, this, callfuncO_selector(CLPveSceneLayer::onPropMenuPress));
            pItem->setAnchorPoint(CCPointZero);
            pItem->setPosition(ccp(G_SF(12)+i*G_SF(117), G_SF(14)));
            m_pPorpMenu->addChild(pItem, 0, i);
        }
        
    }
    else       //店长模式
    {
        Json::Value* pJv = CLSceneManager::sharedSceneManager()->GetJsonDataFromMsgID(10);
        if(!pJv)
            return false;
        m_iBossShelfNo = (*pJv)["shelfNo"].asIntModestyle();
        
        m_pSMNameBG = CCSprite::create("Image/PveLayer/CP_BossNameBG.png");
        m_pSMNameBG->setPosition(ccp(G_SF(320), G_SF(785)));
        pBGSpr->addChild(m_pSMNameBG, 1);
        
        CCLabelTTF* pLabel = CCLabelTTF::create(CLChangeString::sharedChangeString()->GetChangeString("ShopManager").c_str(), "Arial", G_SF(28));
        pLabel->setAnchorPoint(ccp(0, 0.5));
        pLabel->setPosition(ccp(G_SF(300), pLabel->getContentSize().height/2));
        
        m_pSMNameBG->addChild(pLabel, 1);
        
//        CCLabelTTF* pNameTTF = CCLabelTTF::create("沙龙巴斯", "Arial", G_SF(28));
//        pNameTTF->setAnchorPoint(ccp(0, 0.5));
//        pNameTTF->setPosition(ccp(G_SF(300), pLabel->getContentSize().height/2));
//        pNameTTF->setColor(ccRED);
//        m_pSMNameBG->addChild(pNameTTF, 1);
        
        //"Image/ShelfLayer/SL_SMIcon_N.png"
        //        m_pSMForm = CCSprite::create("Image/PveLayer/BossAvatar1.png");
        m_pSMForm = CCSprite::create("Image/ShelfLayer/SL_SMIcon_N.png");
        m_pSMForm->setPosition(ccp(G_SF(320), G_SF(462)));
        pBGSpr->addChild(m_pSMForm, 1);
        
        m_pSMMenu = CCMenu::create();
        m_pSMMenu->setAnchorPoint(CCPointZero);
        m_pSMMenu->setPosition(CCPointZero);
        pBGSpr->addChild(m_pSMMenu, 1);
        
        for(int i=0; i<3; i++)
        {
            if(i <2)
            {
                CCSprite* pSpr1 = CCSprite::create("Image/BackPack/C_composite.png");
                CCSprite* pSpr2 = CCSprite::create("Image/BackPack/C_composite.png");
                
                CCMenuItemSprite* pItem = CCMenuItemSprite::create(pSpr1, pSpr2, NULL, this, menu_selector(CLPveSceneLayer::onBossMenuClicked));
                pItem->setPosition(ccp(G_SF(180)+i*G_SF(290), G_SF(86)));
                m_pSMMenu->addChild(pItem, 0, i);
                
                CCLabelTTF* plaebl;
                if(i == 0)
                {
                    plaebl = CCLabelTTF::create(CLChangeString::sharedChangeString()->GetChangeString("GiveGift").c_str(), "Arial", G_SF(32));
                }
                else
                    plaebl = CCLabelTTF::create(CLChangeString::sharedChangeString()->GetChangeString("KickIt").c_str(), "Arial", G_SF(32));
                
                plaebl->setPosition(ccp(pItem->getContentSize().width/2, pItem->getContentSize().height/2));
                pItem->addChild(plaebl, 1);
            }
            else
            {
                
                CCSprite* pSpr1 = CCSprite::create("Image/PublicUI/Close.png");
                CCSprite* pSpr2 = CCSprite::create("Image/PublicUI/Close.png");
                CCMenuItemSprite* pItem = CCMenuItemSprite::create(pSpr1, pSpr2, NULL, this, menu_selector(CLPveSceneLayer::onBossMenuClicked));
                pItem->setPosition(ccp(G_SF(581),G_SF(920)));
                m_pSMMenu->addChild(pItem, 0, i);
            }
            
        }
        //m_pSchBG->setVisible(false);
        
    }
    
    m_nCTIndex = CLScene_PveLayer;
    return true;
}

void CLPveSceneLayer::onPropMenuPress(CCObject* pSender)
{
    CLFightPropItem* pItem = (CLFightPropItem*)pSender;
    if(pItem->getpPropInfo()->number == 0)
        return;
    
    Json::Value jv(objectValue);
    jv["propNo"] = pItem->getpPropInfo()->m_cardJueseInfo->m_artNo;
    CLNetWorkManager::shardNetWorkManager()->SendNetMessage(CMD_USEPROP, jv);
    CLSceneManager::sharedSceneManager()->SetWaitNetLayer(true);
    
    m_bRoundUseProp = true;
    SetPropMenuEnable(false);
}

void CLPveSceneLayer::SetPropMenuEnable(bool b)
{
    CCArray* pArray = m_pPorpMenu->getChildren();
    
    if (!pArray) { //add by yin
        return; //没有道具的时候，pArray ＝ null
    }
    
    if(b)   //开启
    {
        for(int i=0; i<pArray->count(); i++)
        {
            CLFightPropItem* pItem = (CLFightPropItem*)pArray->objectAtIndex(i);
            pItem->setEnabled(true);
        }
    }
    else    //关闭
    {
        for(int i=0; i<pArray->count(); i++)
        {
            CLFightPropItem* pItem = (CLFightPropItem*)pArray->objectAtIndex(i);
            pItem->setEnabled(false);
        }
    }
}

void CLPveSceneLayer::onGiveupMenuPress(CCObject* pSender)
{
    CLPveGiveupLayer* pGiveUpLayer = CLPveGiveupLayer::create();
    pGiveUpLayer->setAnchorPoint(CCPointZero);
    pGiveUpLayer->setPosition(CCPointZero);
    this->addChild(pGiveUpLayer, 5);
}

void CLPveSceneLayer::UpdateTopicLayer(Json::Value& jv)
{
    //音效
    CLSoundManager::sharedSoundManager()->PlayEffectSound("Sound/Sl_QuestionChange.wav");
    if(!m_bSMStyle)   //普通货架模式
    {
        if(!m_pTopicLayer)
        {
            m_pTopicLayer = CLPveTopicLayer::create(jv);
            m_pTopicLayer->setPosition(ccp(G_SF(320), G_SF(571)));
            CCSprite* pBG = (CCSprite*)this->getChildByTag(10086);
            pBG->addChild(m_pTopicLayer, 1);
            m_pSchedueTTF->setVisible(true);
            m_pGiveUpMenu->setVisible(true);
        }
        m_pTopicLayer->UpdateTopicData(jv);
        int nTime = jv["times"].asIntModestyle();
        StartSchTime(nTime);
        SetPropMenuEnable(true);
    }
    else              //店长模式
    {
        //以下东西需要删除
        //        m_pSMNameBG->removeFromParent();
        //        m_pSMNameBG = NULL;
        //        m_pSMMenu->removeFromParent();
        //        m_pSMMenu = NULL;
        m_pSMNameBG->setVisible(false);
        m_pSMMenu->setVisible(false);
        m_pSMForm->setScale(325.0/523);
        m_pSMForm->setPosition(ccp(G_SF(440), G_SF(742)));
        
        if(!m_pTopicLayer)
        {
            m_pTopicLayer = CLPveTopicLayer::create(jv);
            m_pTopicLayer->setPosition(ccp(G_SF(320), G_SF(446)));
            CCSprite* pBG = (CCSprite*)this->getChildByTag(10086);
            pBG->addChild(m_pTopicLayer, 1);
            m_pSchedueTTF->setPosition(ccp(G_SF(584), G_SF(605)));
            m_pSchedueTTF->setVisible(true);
            m_pGiveUpMenu->setVisible(true);
        }
        m_pTopicLayer->UpdateTopicData(jv);
        int nTime = jv["times"].asIntModestyle();
        StartSchTime(nTime);
    }
    
    
}

//继续挑战店长，返回到店长展示界面
void CLPveSceneLayer::ReStartToFightBoss()
{
    //设置店长相关的类容显示
    m_pSMNameBG->setVisible(true);
    m_pSMMenu->setVisible(true);
    m_pSMForm->setScale(1.0f);
    m_pSMForm->setVisible(true);
    m_pSMForm->setPosition(ccp(G_SF(320), G_SF(462)));
}

void CLPveSceneLayer::onAnswerMenuClicked(CCObject* pSender)
{
    CCMenuItemSprite* pItem = (CCMenuItemSprite*)pSender;
    EndSchTime(pItem->getTag()+1);
}

void CLPveSceneLayer::FrameTime(float ft)
{
    m_nEnterTime--;
    char sSchTTF[20] = {0};
    sprintf(sSchTTF, "%s %dS", CLChangeString::sharedChangeString()->GetChangeString("StartPve").c_str(), m_nEnterTime);
    CCLabelTTF* pLabel = (CCLabelTTF*)m_pShowGoodsBG->getChildByTag(10010);
    pLabel->setString(sSchTTF);
    
    //音效
    if(m_nEnterTime > 0)
    {
      CLSoundManager::sharedSoundManager()->PlayEffectSound("Sound/Sl_CountDown.wav");
    }
    
    if(m_nEnterTime <= 0)
    {
        m_pShowGoodsBG->removeFromParentAndCleanup(true);
        this->unschedule(schedule_selector(CLPveSceneLayer::FrameTime));
        BeginToPve();
    }
}

void CLPveSceneLayer::BeginToPve()
{
    Json::Value jv(objectValue);
    CLNetWorkManager::shardNetWorkManager()->SendNetMessage(CMD_ANSWER, jv);
    CLSceneManager::sharedSceneManager()->SetWaitNetLayer(true);
    
    if(m_pPorpMenu)
        m_pPorpMenu->setVisible(true);
}


void CLPveSceneLayer::StartSchTime(int nTime)
{
    //初始化（颜色设置为标准色，大小设置为1.0倍，值设置为服务器时间）
    m_pSchedueTTF->setScale(1.0f);
    m_pSchedueTTF->setColor(ccWHITE);
    char sTime[20] = {0};
    sprintf(sTime, "%d", nTime);
    m_pSchedueTTF->setString(sTime);
    
    m_nSchTime = nTime;
    this->schedule(schedule_selector(CLPveSceneLayer::onTimeFrame), 1.0);
}

void CLPveSceneLayer::onTimeFrame(float ft)
{
    m_nSchTime--;
    
    if(m_nSchTime <= 0)
    {
        EndSchTime();
    }
    
    char sTime[20] = {0};
    sprintf(sTime, "%d", m_nSchTime);
    m_pSchedueTTF->setString(sTime);
    
    
    if ((m_nSchTime <= 5) && (m_nSchTime > 0)) {
        //音效
        CLSoundManager::sharedSoundManager()->PlayEffectSound("Sound/Sl_CountDown.wav");
        
        m_pSchedueTTF->setColor(ccRED);
        
        CCActionInterval* m_pAction = CCSequence::create(CCScaleTo::create(0.2, 1.5f),
                                                         CCScaleTo::create(0.2, 1.0f),
                                                         NULL
                                                         );
        m_pSchedueTTF->runAction(m_pAction);
    }
}

void CLPveSceneLayer::EndSchTime(int nAnswer)
{
    this->unschedule(schedule_selector(CLPveSceneLayer::onTimeFrame));
    Json::Value jv(objectValue);
    if(nAnswer != -1)
        jv["submit"] = nAnswer;
    
    CLNetWorkManager::shardNetWorkManager()->SendNetMessage(CMD_ANSWER, jv);
    CLSceneManager::sharedSceneManager()->SetWaitNetLayer(true);
    if(m_pYinhuaIcon)
    {
        m_pYinhuaIcon->stopAllActions();
        m_pYinhuaIcon->removeFromParent();
        m_pYinhuaIcon = NULL;
    }
    if(m_pTopicLayer)
        m_pTopicLayer->ReSetAnswerState();
    
}


void CLPveSceneLayer::CreateyinhuaIcon(int nYinhuaNo)
{
    srand(time(NULL));
    m_nYinhuaNo = nYinhuaNo;
    if(m_pYinhuaIcon)
    {
        m_pYinhuaIcon->removeFromParent();
        m_pYinhuaIcon = NULL;
    }
    
    std::string stampSmall_path = CCString::createWithFormat("Image/GoodIcon/StampSmall%d.png", nYinhuaNo)->getCString();
    
    CCSprite* pSpr1 = CCSprite::create(stampSmall_path.c_str());
    m_pYinhuaIcon = CCMenuItemSprite::create(pSpr1, NULL, NULL, this, menu_selector(CLPveSceneLayer::onYinhuaIconPress));
    int nPosX = G_SF(rand()%640);
    if(nPosX < pSpr1->getContentSize().width/2)
        nPosX = pSpr1->getContentSize().width/2;
    else if(nPosX > G_SF(640) - pSpr1->getContentSize().width/2)
        nPosX = G_SF(640) - pSpr1->getContentSize().width/2;
    int nPosY = G_SF(1000);
    m_pYinhuaIcon->setPosition(ccp(nPosX, nPosY));
    //    m_pYinhuaIcon->setEnabled(false);
    m_pYinhuaMenu->addChild(m_pYinhuaIcon, 1);
    //    this->unschedule(schedule_selector(CLPveSceneLayer::onYinhuaFrame));
    //    this->schedule(schedule_selector(CLPveSceneLayer::onYinhuaFrame));
    
    //    int nDisY = -(rand()%480 + 240);
    
    //    m_pYinhuaIcon->runAction(CCSequence::create(CCEaseExponentialOut::create(CCMoveBy::create(1.5, ccp(0, G_SF(nDisY)))), CCCallFunc::create(this, callfunc_selector(CLPveSceneLayer::onYinhuaDownEnd)), NULL));
    randomAction();
}

//印花随机动作
void CLPveSceneLayer::randomAction()
{
    int rdNum = rand() % 5; //产生随机0～5的整数
    
    actionRandomGet(); //动作获得
    
    CCFiniteTimeAction *finite;
    
    //随机变速
    switch (rdNum) {
        case 1: //塞因缓冲  （头尾较为平缓、中间为加速度恒定的匀变速运动）
        {
            finite = CCEaseSineIn::create(m_pAction);
        }
            break;
        case 2: //弹性缓冲（效果：类似于弹了一下一根绷紧的橡皮筋）
        {
            finite = CCEaseElasticIn::create(m_pAction);
        }
            break;
        case 3: //跳跃缓冲（效果：可以理解为一个球从空中落地之后在地上弹跳的情况）
        {
            finite = CCEaseBounceOut::create(m_pAction);
        }
            break;
        case 4://回震缓冲（效果：跟跳跃缓冲差不多，不过回震可以看成类似完全弹性碰撞的结果）
        {
            finite = CCEaseBackIn::create(m_pAction);
        }
            break;
        case 5: //CCEaseBounceInOut
        {
            finite = CCEaseBounceInOut::create(m_pAction);
        }
            
            break;
            
        default: //指数缓冲加速 （效果：如同指数函数一样，在开头或者结尾或者中间突然获得指数型的加速度）
        {
            finite = CCEaseExponentialInOut::create(m_pAction);
        }
            break;
    }
    
    m_pYinhuaIcon->runAction(CCSequence::create(m_pAction, CCCallFunc::create(this, callfunc_selector(CLPveSceneLayer::onYinhuaDownEnd)), NULL));
    
}

//随即动作取得
void CLPveSceneLayer::actionRandomGet()
{
    int rdNum = 1 + rand() % 3; //随机动作种子取得（到（a + rand() % (b-a+1)））
    int rdPointNum = rand() % 5 + 1; //随机几个移动曲线点（曲线运动几个点，或贝塞尔动作使用）

    CCSize size = CCDirector::sharedDirector()->getgSceneSize();

    
    switch (rdNum)
    {
        case 1: //自然掉落动作
        {
            CCFiniteTimeAction *moveTo = CCMoveTo::create(1.5f, ccp(size.width*0.5, size.height*0.5));

            int nDisY = -(rand()%480 + 240);
            CCFiniteTimeAction *moveBy = CCMoveBy::create(0.5f, ccp(0, G_SF(nDisY)));
            m_pAction = CCSequence::create( moveBy,moveTo, NULL);

        }
            break;
        case 2: //掉落 ＋ 曲线移动
        {
            //曲线运动
            CCPointArray *array = CCPointArray::create(20);
            for (int j=0; j<rdPointNum; j++)
            {
                int nX = G_SF((rand()%100 + 10));  //(10, 100)
                int nY = G_SF((rand()%500 + 20));  //（20， 500）
                
                array->addControlPoint(ccp(nX, nY));
            }
            CCCatmullRomBy *catmuAction = CCCatmullRomBy::create(1.0, array);
            
            CCAction *delay = CCDelayTime::create(0.5); //停顿
            
            //掉落的值最好是在屏幕中间区域
            int nDisX = G_SF((rand()%200 + 10));   //(10, 200)
            int nDisY = G_SF((rand()%300 + 10));  //（10, 300）
            CCFiniteTimeAction *catAction;
            
            if (rdPointNum % 2 == 0)
            { //中间靠左
                catAction = CCMoveTo::create(0.2f, ccp(size.width - nDisX, size.height - nDisY));
                m_pAction = CCSequence::create(catAction, delay, catmuAction,NULL);

            }
            else
            { //中间靠右
//                catAction = CCJumpBy::create(2.0f, ccp(10, 20), G_SF(20),3);
                catAction = CCJumpTo::create(1.0f, ccp(nDisX, G_SF(100)), G_SF(20), rdPointNum);

                m_pAction = CCSequence::create(catAction, delay,catmuAction, NULL);
            }
            
        }
            break;
            
        default:
        {
            CCFiniteTimeAction *moveTo = CCMoveTo::create(0.5f, ccp(size.width*0.5, size.height*0.5));
            
            CCAction *delay = CCDelayTime::create(0.5); //停顿
            
            int nBezerX = G_SF((rand()%500 + 30));   //(30, 530)
            int nBezerY = G_SF((rand()%700 + 100));  //（100， 800）
            
            ccBezierConfig bezier;
            bezier.controlPoint_1 = ccp(0, size.height*0.5);
            bezier.controlPoint_2 = ccp(size.width*0.5, -size.width*0.5);
            bezier.endPosition = ccp(nBezerX, nBezerY);
            
            CCActionInterval*  bezierForward;
            if (rdPointNum % 2 == 0)
            { //直接飞到屏幕外，不让捡到
                bezierForward = CCBezierBy::create(0.2f, bezier);
                m_pAction = CCSequence::create( moveTo,bezierForward, NULL);
            }
            else
            {  //飞出，能飞回
                bezierForward = CCBezierBy::create(0.4f, bezier);
                CCActionInterval*  bezierBack = bezierForward->reverse();
                m_pAction = CCSequence::create( moveTo,bezierForward, delay,bezierBack, NULL);
            }

        }
            break;
    }
    
}


void CLPveSceneLayer::onYinhuaDownEnd()
{
    CCFadeOut* pOut = CCFadeOut::create(0.6);
    CCFadeIn* pIn = CCFadeIn::create(0.6);
    CCSequence* pSeqOI = CCSequence::create(pOut, pIn, NULL);
    CCRepeat* pRepeat = CCRepeat::create(pSeqOI, 2);
    
    m_pYinhuaIcon->runAction(CCSequence::create(pRepeat, CCFadeOut::create(0.6), CCCallFunc::create(this, callfunc_selector(CLPveSceneLayer::onDeleteYinhaIcon)), NULL));
    
    //    m_pYinhuaIcon->setEnabled(true);
}


void CLPveSceneLayer::onDeleteYinhaIcon()
{
    m_pYinhuaIcon->removeFromParent();
    m_pYinhuaIcon = NULL;
}

void CLPveSceneLayer::onYinhuaIconPress(CCObject* pSender)
{
    m_pYinhuaIcon->stopAllActions();
    m_pYinhuaIcon->setOpacity(255);
    
    Json::Value jv(Json::objectValue);
    jv["yinhua"] = CCString::createWithFormat("%d", m_nYinhuaNo)->getCString() ;
    CLNetWorkManager::shardNetWorkManager()->SendNetMessage(CMD_PICKUP, jv);
    
    CCPoint tPoint = ccp(G_SF(680), m_pYinhuaIcon->getPosition().y + G_SF(300));
    m_pYinhuaIcon->runAction(CCSequence::create(CCEaseSineOut::create(CCMoveTo::create(0.3, tPoint)), CCCallFunc::create(this, callfunc_selector(CLPveSceneLayer::onDeleteYinhaIcon)), NULL));
    
}


void CLPveSceneLayer::onYinhuaFrame(float ft)
{
    srand(time(NULL));
    m_nYIndex++;
    if(m_nYIndex >= 20)
    {
        m_bYinhuaMoveR = rand()%2;
        m_nYIndex = 0;
    }
    int nMovePixelX = rand()%15+20;
    int nMovePixelY = rand()%20+30;
    if(m_bYinhuaMoveR)   //说明向右飘
    {
        if(m_pYinhuaIcon->getPosition().x + m_pYinhuaIcon->getContentSize().width/2 >= G_SF(640))
        {
            m_pYinhuaIcon->setPositionX(m_pYinhuaIcon->getPosition().x - nMovePixelX);
            m_bYinhuaMoveR = false;
            m_nYIndex = 0;
        }
    }
    else
    {
        if(m_pYinhuaIcon->getPosition().x - m_pYinhuaIcon->getContentSize().width/2 <= 0)
        {
            m_pYinhuaIcon->setPositionX(m_pYinhuaIcon->getPosition().x + nMovePixelX);
            m_bYinhuaMoveR = true;
            m_nYIndex = 0;
        }
    }
    
    m_pYinhuaIcon->setPositionY(m_pYinhuaIcon->getPosition().y - nMovePixelY);
    
    if(m_pYinhuaIcon->getPosition().y + m_pYinhuaIcon->getContentSize().height/2 <= 0)
    {
        this->unschedule(schedule_selector(CLPveSceneLayer::onYinhuaFrame));
        m_pYinhuaIcon->removeFromParent();
        m_pYinhuaIcon = NULL;
        m_nYIndex = 0;
    }
    
}

void CLPveSceneLayer::ShowPveResultLayer(Json::Value& jv)
{
    if (m_pGiveUpMenu)
        m_pGiveUpMenu->setVisible(false);
    
    if (m_pSchedueTTF)
        m_pSchedueTTF->setVisible(false);
    
    if (m_pTopicLayer)
        m_pTopicLayer->removeFromParent();
    m_pTopicLayer = NULL;
    
    if(!m_bSMStyle)
    {
        this->unschedule(schedule_selector(CLPveSceneLayer::onTimeFrame));
        
        if(!m_pResultLayer)
        {
            m_pResultLayer = PassPointsSuccess::create(jv);
            m_pResultLayer->setAnchorPoint(CCPointZero);
            m_pResultLayer->setPosition(CCPointZero);
            this->addChild(m_pResultLayer, 1);
            //统一修改用户钱信息 add by yin 20131130
            int money = jv["jetton"].asIntModestyle();
            CLPlayerManager::shardPlayerManager()->getpUserInfo()->setMoneys(money);
            
            CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CLMainSceneLayer::UpdateMoney), NOTICE_UPDATEMONEY, NULL);
        }
        
        m_pPorpMenu->setVisible(false);
    }
    else
    {
        m_pSMForm->setVisible(false);
        this->unschedule(schedule_selector(CLPveSceneLayer::onTimeFrame));
        
        int nRight = jv["rights"].asIntModestyle();
        CLPveResultLayer* pLayer = NULL;
        if(nRight == 1)
        {
            pLayer = CLPveResultLayer::SuccessBoos(jv);
            int nStoreLv = jv["newlevel"].asIntModestyle();
            CLSceneManager::sharedSceneManager()->UpdateNewStoreLevel(nStoreLv);
            
            //wangguolong
            //            if(CLSceneManager::sharedSceneManager()->getnewlevel() == -2)
            if(CLSceneManager::sharedSceneManager()->getnewlevel() < 0)
            {
                CLSceneManager::sharedSceneManager()->setnewlevel(nStoreLv);
            }
            
        }
        else
        {
            pLayer = CLPveResultLayer::FailBoos(jv);
        }
        if(pLayer)
        {
            pLayer->setAnchorPoint(CCPointZero);
            pLayer->setPosition(CCPointZero);
            this->addChild(pLayer, 2);
        }
        
    }
    
}

//给店长送礼回调函数 add by yin 20131220
void CLPveSceneLayer::_GiftBossResultLayer(Json::Value& jv)
{
    CLPveResultLayer* pLayer = NULL;
    
    pLayer = CLPveResultLayer::SuccessBoos(jv);
    int nStoreLv = jv["newlevel"].asIntModestyle();
    CLSceneManager::sharedSceneManager()->UpdateNewStoreLevel(nStoreLv);
    
    //wangguolong
    //            if(CLSceneManager::sharedSceneManager()->getnewlevel() == -2)
    if(CLSceneManager::sharedSceneManager()->getnewlevel() < 0)
    {
        CLSceneManager::sharedSceneManager()->setnewlevel(nStoreLv);
    }
    
    if(pLayer)
    {
        pLayer->setAnchorPoint(CCPointZero);
        pLayer->setPosition(CCPointZero);
        this->addChild(pLayer, 2);
    }
    
    //统一修改用户钱信息 add by yin 20131221
    int money = jv["moneys"].asIntModestyle();
    CLPlayerManager::shardPlayerManager()->getpUserInfo()->setJetton(money);
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CLMainSceneLayer::UpdateMoney), NOTICE_UPDATEMONEY, NULL);
    
}

void CLPveSceneLayer::ReturnToShelfsLayer()
{
    Json::Value jv(Json::objectValue);
    jv["storeNo"] = CLPlayerManager::shardPlayerManager()->getpUserInfo()->getnStoreNo();
    CLNetWorkManager::shardNetWorkManager()->SendNetMessage(CMD_SHELFS, jv);
    
    CLSceneManager::sharedSceneManager()->setpCallTarget(this);
    CLSceneManager::sharedSceneManager()->setpCallFun(callfunc_selector(CLPveSceneLayer::AdvImageLoadEnd));
    
    CLSceneManager::sharedSceneManager()->getpLoadingLayer()->setVisible(true);
}

void CLPveSceneLayer::AdvImageLoadEnd()
{
    CLShelfsSceneLayer* pLayer = CLShelfsSceneLayer::create();
    pLayer->setAnchorPoint(CCPointZero);
    pLayer->setPosition(CCPointZero);
    CLSceneManager::sharedSceneManager()->ChangeScene(pLayer);
}

void CLPveSceneLayer::ContiueToPve()
{
    if(m_pResultLayer)
    {
        m_pResultLayer->removeFromParent();
        m_pResultLayer = NULL;
    }
    if(m_pPorpMenu)
        m_pPorpMenu->setVisible(true);
    EndSchTime();
}


void CLPveSceneLayer::onBossMenuClicked(CCObject* pSender)
{
    CCMenuItemSprite* pItem = (CCMenuItemSprite*)pSender;
    switch (pItem->getTag())
    {
        case 0:
        {
            //给他送礼
            CLPveSMangerLayer *sManager = CLPveSMangerLayer::create(m_iBossShelfNo, 2);
            sManager->setAnchorPoint(CCPointZero);
            sManager->setPosition(CCPointZero);
            this->addChild(sManager, 100, 100);
        }
            break;
        case 1:
        {
            BeginToPve();
        }
            break;
        case 2:
        {
            ReturnToShelfsLayer();
        }
            break;
        default:
            break;
    }
}

//播放动画
//proID 道具ID isShutDown 是否关闭  delayTime每帧延迟时间 shutTime 总关闭时间 repeatTimes  重复次数 0代表永远执行
void CLPveSceneLayer::animationWithPro(int proID, int isShutDown, float delayTime, float shutTime, int repeatTimes )
{
    //道具使用效果 音乐
    std::string musicStr;
    if(proID == 46)//买个表
    {
        CLSoundManager::sharedSoundManager()->PlayEffectSound("Sound/Sl_AddTimeCard.wav");
    }
    else if(proID == 47)//剪刀手
    {
        CLSoundManager::sharedSoundManager()->PlayEffectSound("Sound/Sl_Handssors.wav");
    }
    else//其它
    {
        
    }
    
    CCSize size = CCDirector::sharedDirector()->getgSceneSize();
    
    std::string sFile;
    sFile = CCString::createWithFormat("proAnimation%d", proID)->getCString();
    
    
    CCAnimation* animation = CLAnimationManager::sharedAnimationManager()->animationWithName(sFile.c_str(),delayTime);
    CCSprite* magic = CLAnimationManager::sharedAnimationManager()->spriteWithAnimationName(sFile.c_str());
    magic->setPosition(ccp(size.width/2, size.height/2));
    this->addChild(magic, 10, ANIMATION_PROS);
    
    if (repeatTimes)
        magic->runAction(CCRepeat::create(CCAnimate::create(animation), repeatTimes));
    else
        magic->runAction(CCRepeatForever::create(CCAnimate::create(animation)));
    
    
    //播放完成后关闭
    if (isShutDown)
    {
        this->scheduleOnce(schedule_selector(CLPveSceneLayer::animationCancle), shutTime);
    }
    

}
//取消动画
void CLPveSceneLayer::animationCancle(float t)
{
    this->removeChildByTag(ANIMATION_PROS, true);
}

void CLPveSceneLayer::UsePropSucces(Json::Value& jv)
{
    int pPropID = jv["prop"].asIntModestyle();
    CCArray* pArray = m_pPorpMenu->getChildren();
    CLFightPropItem* pUseItem = NULL;
    
    //gaoyuan 播放帧动画
    animationWithPro(pPropID,true, 0.3, 1.1, 1);
   
    
    //减少道具类型 gaoyuan
    for(int i=0; i<pArray->count(); i++)
    {
        CLFightPropItem* pItem = (CLFightPropItem*)pArray->objectAtIndex(i);
        if(pItem->getpPropInfo()->m_cardJueseInfo->m_artNo == pPropID)
        {
            pItem->getpPropInfo()->number--;
            if(pItem->getpPropInfo()->number <= 0)
            {
                pItem->removeFromParent();
                CLPlayerManager::shardPlayerManager()->getpUserInfo()->RemovePropFromID(pPropID);
            }
            else
            {
                pUseItem = pItem;
                pItem->getpNumLabel()->setString(CCString::createWithFormat("%d", pItem->getpPropInfo()->number)->getCString());
            }
            break;
        }
    }
    
    if(pUseItem)
        pUseItem->setnCoolRound(jv["cooling"].asIntModestyle());
    
    std::string sClass = jv["class"].asStringModeStyle();
    
    if(sClass.compare("D1") == 0)    //更新剩余时间
    {
        int nAddTime = jv["arg2"].asIntModestyle();
        m_nSchTime += nAddTime;
        char sTime[20] = {0};
        sprintf(sTime, "%d", m_nSchTime);
        m_pSchedueTTF->setString(sTime);
        m_pSchedueTTF->setScale(1.0f);
        m_pSchedueTTF->setColor(ccWHITE);
    }
    else if(sClass.compare("D2") == 0 )   //一个错误答案的选项
    {
        int nAnswerIndex = jv["arg1"].asIntModestyle();
        if(m_pTopicLayer)
            m_pTopicLayer->GiveAnanswerIndex(false, nAnswerIndex);
    }
    else if(sClass.compare("D3") == 0)   //增加血量
    {
        
    }
    else if(sClass.compare("D4") == 0)   //正确的答案
    {
        
    }
    else
    {
        
    }
    
}


