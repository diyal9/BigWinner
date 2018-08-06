//
//  BackPack.cpp
//  BigWinner
//
//  Created by LunaBox03 on 13-10-19.
//
//

#include "CLPvpReady.h"
#include <LGameCommonDefine.h>
#include <Button.h>
#include "FightBackPack.h"
#include "CLSceneManager.h"
#include "CLPlayerManager.h"

#include "JsonDataManager.h"
#include "CLNetWorkManager.h"
#include "FightExplain.h"
#include "UserInfoSprite.h"
#include "CLSoundManager.h"

using namespace Json;


CLPvpReady::CLPvpReady():m_indesSprite(NULL),m_nlayerTpye(-1)
{
    m_pCenterNode = NULL;
    m_pWaitingLayer = NULL;
    m_pBluePitchNode = NULL;
    m_ntag = 0;
}

CLPvpReady::~CLPvpReady()
{
    
}

CLPvpReady* CLPvpReady::create()
{
    CLPvpReady *pRet = new CLPvpReady();
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool CLPvpReady::init()
{
    if(!CCLayerColor::initWithColor(ccc4(0, 0, 0, 0)))
    {
        return false;
    }
    //初始化UI
    doBaseUI();
    
    //发送内容 获取备战设置
    Json::Value data;
    data["msgid"].append("47"); //指令码
    
    CLNetWorkManager::shardNetWorkManager()->SendNetMessage(CMD_PK1_GETEQUIP, data);
    CLSceneManager::sharedSceneManager()->SetWaitNetLayer(true);
    
    return true;
}

void CLPvpReady::doBaseUI()
{
    
    CCSize size = CCDirector::sharedDirector()->getgSceneSize();
    
//    //add by gaoyuan
//    CCSprite* pPlayerHeadBG = CCSprite::create("Image/LoginLayer/ML_Head_BG.png");
//    pPlayerHeadBG->setPosition(ccp(G_SF(150), G_SF(890)));
//    this->addChild(pPlayerHeadBG, 1);
//    
//    //ML_HeadSbg
//    CCSprite* pPlayerHeadSmallBG = CCSprite::create("Image/PublicUI/RoleHeadBG.png");
//    pPlayerHeadSmallBG->setPosition(ccp(G_SF(71), G_SF(889)));
//    this->addChild(pPlayerHeadSmallBG, 1);
//    
//    //添加头像
//    CCSprite* pPlayerHead = CCSprite::create("Image/LoginLayer/ML_RoleHead.png");
//    pPlayerHead->setPosition(ccp(G_SF(71), G_SF(889)));
//    this->addChild(pPlayerHead, 2);
//    
//    //添加头像信息区
//    CCSprite* pNamePanel = CCSprite::create("Image/LoginLayer/ML_NameBG.png");
//    pNamePanel->setPosition(ccp(G_SF(225), G_SF(924)));
//    this->addChild(pNamePanel, 1);
//    
//    //添加昵称信息
//    CCLabelTTF* pNameTTF = CCLabelTTF::create(CLPlayerManager::shardPlayerManager()->getpUserInfo()->getsUsername().c_str(), "Arial", G_SF(24));
//    pNameTTF->setColor(ccc3(200, 100, 50));
//    pNameTTF->setPosition(ccp(pNamePanel->getContentSize().width/2, pNamePanel->getContentSize().height/2));
//    pNamePanel->addChild(pNameTTF, 1);
//    
//    //添加2个勋章图标
//    CCSprite* pGmedalSpr = CCSprite::create("Image/LoginLayer/ML_GmedalIcon.png");
//    pGmedalSpr->setPosition(ccp(G_SF(152), G_SF(874)));
//    this->addChild(pGmedalSpr, 1);
//    
//    CCSprite* pSmedalSpr = CCSprite::create("Image/LoginLayer/ML_SmedalIcon.png");
//    pSmedalSpr->setPosition(ccp(G_SF(210), G_SF(874)));
//    this->addChild(pSmedalSpr, 1);
    
    //添加头像
    UserInfoSprite *userInfo = UserInfoSprite::create();
    userInfo->setPosition(ccp(G_SF(170), G_SF(889)));
    this->addChild(userInfo);
    
    //团战报名
    Button *hecheng = Button::createBtn("Image/PvpLayer/CP_PvpFunIcon2.png");
    hecheng->setPosition( ccp(G_SF(90), hecheng->getContentSize().height*0.5) );
//    hecheng->setOnClickCallbackNode(callfuncN_selector(CLPvpReady::menuCallBackButton), this);
    hecheng->setHandlerPriority(-10001);
    this->addChild(hecheng,2,PVP_TUANZHAN_TAG);

    //排行榜
    Button *juese = Button::createBtn("Image/PvpLayer/CP_PvpFunIcon4.png");
    juese->setPosition( ccp(G_SF(242), juese->getContentSize().height*0.5) );
    juese->setOnClickCallbackNode(callfuncN_selector(CLPvpReady::menuCallBackButton), this);
    juese->setHandlerPriority(-10001);
    this->addChild(juese,2,PVP_PAIHANG_TAG);

    //对战说明
    Button *tika = Button::createBtn("Image/PvpLayer/CP_PvpFunIcon6.png");
    tika->setPosition( ccp(G_SF(394), tika->getContentSize().height*0.5) );
    tika->setOnClickCallbackNode(callfuncN_selector(CLPvpReady::menuCallBackButton), this);
    tika->setHandlerPriority(-10001);
    this->addChild(tika,2,PVP_HELP_TAG);

    //备战设置
    Button *daoju = Button::createBtn("Image/PvpLayer/CP_PvpFunIcon8.png");
    daoju->setPosition( ccp(G_SF(546), daoju->getContentSize().height*0.5) );
    daoju->setOnClickCallbackNode(callfuncN_selector(CLPvpReady::menuCallBackButton), this);
    daoju->setHandlerPriority(-10001);
    this->addChild(daoju,2,PVP_SETTING_TAG);
    //modify by yin end
    
    //第一次默认加载ui
    this->scheduleOnce(schedule_selector(CLPvpReady::addBaseUI), 0.2);
    
}

void CLPvpReady::addBaseUI()
{
    
    //加载默认UI
    StampFightSelected *stamp = StampFightSelected::create();
    this->addChild(stamp, 1);
    stamp->setAnchorPoint(CCPointZero);
    stamp->setPosition(CCPointZero);
    m_pCenterNode = stamp;
    
    //刷新位置
    setIndexPosition(-1);
    
}

void CLPvpReady::ReturnRoomSelLayer()
{
    if(m_pCenterNode)
    {
        m_pCenterNode->removeFromParent();
        m_pCenterNode = NULL;
    }
    
    //wangguolong
    if(m_pBluePitchNode){
        m_pBluePitchNode->removeFromParent();
        m_pBluePitchNode = NULL;
    }
    
    StampFightSelected *stamp = StampFightSelected::create();
    stamp->setAnchorPoint(CCPointZero);
    stamp->setPosition(CCPointZero);
    this->addChild(stamp,7);
    
    m_pCenterNode = stamp;
    
    setIndexPosition(PVP_DEFAULT);
}


void CLPvpReady::menuCallBackButton(CCNode* r)
{
    if(m_nlayerTpye == r->getTag())
        return;
    
    if(m_pCenterNode)
    {
        m_pCenterNode->removeFromParent();
        m_pCenterNode = NULL;
    }
    
    switch (r->getTag())
    {
            
//        case PVP_DEFAULT:   //默认 ， 选择 房间列表
//        {
//            CCLOG("房间");
//            StampFightSelected *stamp = StampFightSelected::create();
//            stamp->setPosition(ccp(G_SF(0), G_SF(-40)));
//            this->addChild(stamp,10);
//            
//            m_pCenterNode = stamp;
//            
//        }
//            break;
//            
        case PVP_TUANZHAN:   //团战 层
        {
            if(this->getntag()!= 0)
            {
                this->getChildByTag(this->getntag())->removeFromParentAndCleanup(true);
            }
            
            this->setntag(PVP_TUANZHAN_TAG2);
            
            //团战报名
            Button *hecheng = Button::createBtn("Image/PvpLayer/CP_PvpFunIcon1.png");
            hecheng->setPosition( ccp(G_SF(90), hecheng->getContentSize().height*0.5) );
            hecheng->setOnClickCallbackNode(callfuncN_selector(CLPvpReady::menuCallBackButton), this);
            hecheng->setHandlerPriority(-10001);
            this->addChild(hecheng,2,PVP_TUANZHAN_TAG2);
            
            CCLOG("团战");
            m_pCenterNode = NULL;
        }
            break;
            
        case PVP_PAIHANG:   //排行层
        case PVP_PAIHANG_TAG2:
        {
            
            if(m_pBluePitchNode){
                m_pBluePitchNode->removeFromParent();
            }
            
            //排行榜
            Button *juese = Button::createBtn("Image/PvpLayer/CP_PvpFunIcon3.png");
            juese->setPosition( ccp(G_SF(242), juese->getContentSize().height*0.5) );
            juese->setOnClickCallbackNode(callfuncN_selector(CLPvpReady::menuCallBackButton), this);
            juese->setHandlerPriority(-10001);
            this->addChild(juese,2,PVP_PAIHANG_TAG2);
            
            m_pBluePitchNode = juese;
            CCLOG("排行");

            Json::Value value;
            CLNetWorkManager::shardNetWorkManager()->SendNetMessage(CMD_PK1_RANKINGLIST, value);
            CLSceneManager::sharedSceneManager()->SetWaitNetLayer(true);
            
//            RankingList* pLayer = RankingList::create();
//            pLayer->setAnchorPoint(CCPointZero);
//            pLayer->setPosition(CCPointZero);
//            this->addChild(pLayer, 10);
//            
//            m_pCenterNode = pLayer;
             break;
        }
           
            
        case PVP_DESC:    //对战说明层
        case PVP_HELP_TAG2:
        {
            
            if(m_pBluePitchNode){
                m_pBluePitchNode->removeFromParent();
            }
            
            //对战说明
            Button *tika = Button::createBtn("Image/PvpLayer/CP_PvpFunIcon5.png");
            tika->setPosition( ccp(G_SF(394), tika->getContentSize().height*0.5) );
            tika->setOnClickCallbackNode(callfuncN_selector(CLPvpReady::menuCallBackButton), this);
            tika->setHandlerPriority(-10001);
            this->addChild(tika,2,PVP_HELP_TAG2);

            m_pBluePitchNode = tika;
            CCLOG("对战说明");
            m_pCenterNode = NULL;
            FightExplainLayer* FExplain = FightExplainLayer::create();
            this->addChild(FExplain, 10, PVP_GENERAL_TAG);
            FExplain->setPosition(ccp(G_SF(0), G_SF(0)));

            m_pCenterNode = FExplain;
            
        }
            break;
            
        case PVP_SETTING:  //备战准备层
        case PVP_SETTING_TAG2:
        {
            
            if(m_pBluePitchNode){
                m_pBluePitchNode->removeFromParent();
            }
            
            //备战设置
            Button *daoju = Button::createBtn("Image/PvpLayer/CP_PvpFunIcon7.png");
            daoju->setPosition( ccp(G_SF(546), daoju->getContentSize().height*0.5) );
            daoju->setOnClickCallbackNode(callfuncN_selector(CLPvpReady::menuCallBackButton), this);
            daoju->setHandlerPriority(-10001);
            this->addChild(daoju,2,PVP_SETTING_TAG2);
            
            m_pBluePitchNode = daoju;
            CCLOG("对战设置");
        
            //发送内容 获取备战道具信息
            Json::Value data2;
            data2["msgid"].append("48"); //指令码
            
            CLNetWorkManager::shardNetWorkManager()->SendNetMessage(48, data2);
            CLSceneManager::sharedSceneManager()->SetWaitNetLayer(true);

            FightBackPackLayer *rankList = FightBackPackLayer::create();
            this->addChild(rankList, 10, PVP_GENERAL_TAG);
            rankList->setPosition(ccp(G_SF(0), G_SF(0)));
            
            m_pCenterNode = rankList;
        }
            break;
        default:
            break;
    }
    
    setIndexPosition(r->getTag());
}

void CLPvpReady::setIndexPosition(int nIndex)
{
//    switch (nIndex)
//    {
//        case PVP_DEFAULT:
//        {
//            m_indesSprite->setVisible(false);
//        }
//            break;
//        case PVP_TUANZHAN:
//        {
//            m_indesSprite->setVisible(true);
//            m_indesSprite->setPosition(ccp(G_SF(88), G_SF(50)));
//        }
//            break;
//        case PVP_PAIHANG:
//        {
//            m_indesSprite->setVisible(true);
//            m_indesSprite->setPosition(ccp(G_SF(228), G_SF(50)));
//        }
//            break;
//        case PVP_DESC:
//        {
//            m_indesSprite->setVisible(true);
//            m_indesSprite->setPosition(ccp(G_SF(375), G_SF(50)));
//        }
//            break;
//        case PVP_SETTING:
//        {
//            m_indesSprite->setVisible(true);
//            m_indesSprite->setPosition(ccp(G_SF(525), G_SF(50)));
//        }
//            break;
//            
//        default:
//            break;
//    }
//    m_nlayerTpye = nIndex;
    
}


void CLPvpReady::onEnter()
{
    CCLayerColor::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -10000, true);
}

void CLPvpReady::onExit()
{
    CCLayerColor::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}



bool CLPvpReady::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if(this->isVisible())
        return true;
    else
        return false;
}

void CLPvpReady::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void CLPvpReady::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void CLPvpReady::ShowWaitRivalLayer(bool b)
{
    if(b)
    {
        if(!m_pWaitingLayer)
        {
            m_pWaitingLayer = CLPvpWaitRivalLayer::create();
            m_pWaitingLayer->setAnchorPoint(CCPointZero);
            m_pWaitingLayer->setPosition(CCPointZero);
            this->addChild(m_pWaitingLayer, 10);
        }
        m_pWaitingLayer->setVisible(true);
    }
    else
    {
        if(m_pWaitingLayer)
        {
            m_pWaitingLayer->removeFromParent();
            m_pWaitingLayer = NULL;
        }
    }
}

void CLPvpReady::WaitRivalSuccess(Json::Value& jv)
{
    if(m_pWaitingLayer)
    {
        //窗体开闭音乐
        CLSoundManager::sharedSoundManager()->PlayEffectSound("Sound/Sl_MatchSuccess.wav");
        m_pWaitingLayer->WaitRivalSuccess(jv);
    }
}



