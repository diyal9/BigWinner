//
//  CLTopLayerScene.cpp
//  Yy_mobileGame3
//
//  Created by Zhang Qiyun on 13-6-20.
//
//

#include "CLTopLayerScene.h"
#include "LGameCommonDefine.h"
#include "CLChangeString.h"
#include "CLSceneManager.h"
#include "CLLoadImageManager.h"
#include "CLAnimationManager.h"
#include "JsonDataManager.h"
#include "MLTextFieldTTFLayer.h"
#include "CLNetWorkManager.h"
#include "JsonParser.h"
#include "CLRealGameLayer.h"
#include "CLMainSceneLayer.h"

using namespace Json;

CLTopLayerScene::CLTopLayerScene()
{
    m_bHaveClicked = false;
    m_bHasAccountRecord = false;
}

CLTopLayerScene::~CLTopLayerScene()
{
    
}

bool CLTopLayerScene::init()
{
    if(!CCLayer::init())
        return false;
    
    CCSize size = CCDirector::sharedDirector()->getgSceneSize();    //新适配用
    this->setContentSize(size);
    
    CCSprite* pBG = CCSprite::create("Image/TopLayer/TopLayerBG.png");
    pBG->setAnchorPoint(ccp(0.5,0.5));
    pBG->setPosition(ccp(size.width/2, size.height/2));
    this->addChild(pBG, 0);
    
//    CCSprite* pLogo = CCSprite::create("Image/TopLayer/CompanyLogo.png");
//    pLogo->setPosition(ccp(size.width/2, G_SF(552)));
//    pBG->addChild(pLogo, 0);
    
//    CCSprite* pTopBar = CCSprite::create("Image/TopLayer/TopBarSpr.png");
//    pTopBar->setAnchorPoint(ccp(0, 1));
//    pTopBar->setPosition(ccp(0, size.height));
//    pBG->addChild(pTopBar, 0);
//    
//    CCSprite* pBottomBar = CCSprite::create("Image/TopLayer/TopBarSpr.png");
//    pBottomBar->setFlipY(true);
//    pBottomBar->setAnchorPoint(CCPointZero);
//    pBottomBar->setPosition(CCPointZero);
//    pBG->addChild(pBottomBar, 0);
    
    std::string str = CLChangeString::sharedChangeString()->GetChangeString("CurVersion") + JsonDataManager::shardJDManager()->getsCurVersion();
    CCLabelTTF* pLabel = CCLabelTTF::create(str.c_str(), CLChangeString::sharedChangeString()->GetTTFName(2).c_str(), G_SF(30), CCSizeMake(G_SF(300), G_SF(35)), kCCTextAlignmentCenter);
    pLabel->setPosition(ccp(size.width/2, G_SF(300)));
    this->addChild(pLabel, 1);
    pLabel->setColor(ccc3(70, 27, 0));
    
//    pLabel = CCLabelTTF::create(CLChangeString::sharedChangeString()->GetChangeString("ClickToStart").c_str(), CLChangeString::sharedChangeString()->GetTTFName(2).c_str(), G_SF(30) );
//    pLabel->setPosition(ccp(size.width/2, size.height*0.2));
//    pBG->addChild(pLabel, 1);
//    pLabel->setColor(ccc3(70, 27, 0));
    CCSprite *pStart = CCSprite::create("Image/TopLayer/StartGame.png");
    pStart->setPosition(ccp(size.width/2, size.height*0.15));
    pBG->addChild(pStart, 1);
    
    CCActionInterval*  action1 = CCBlink::create(2, 3);
    pStart->runAction(CCRepeatForever::create(action1));
    
    
    CCFadeIn* pFadeIn   = CCFadeIn::create(1.0);
    CCFadeOut* pFadeOut = CCFadeOut::create(1.0);
    CCSequence* pSeq = (CCSequence*)CCSequence::create(pFadeIn, pFadeOut, NULL);
    pLabel->runAction(CCRepeatForever::create(pSeq));
    
    setnCTIndex(CLScene_TopLayer);
    
    //获取移动设备网络类型 接口 测试
//    NetWorkType nType = GetNetwrokType();
//    CCLOG("Cur Network Type is %d", nType);
    // end
    
    //end
    
    return true;
}

void CLTopLayerScene::onEnter()
{
    CCLayer::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -1, true);
}

void CLTopLayerScene::onExit()
{
    CCLayer::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool CLTopLayerScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if(m_bHaveClicked)
        return false;
    if(!CLNetWorkManager::shardNetWorkManager()->Connect(JsonDataManager::shardJDManager()->getsLoginUrl().c_str(), JsonDataManager::shardJDManager()->getnLoginPort()))
    {
        
        //此处需要一个屏蔽后面操作的Dielog 点击后重新连接； 暂时先打印一个错误的信息1314
        
        CCLOG("Connect Login Server is error");
    }
    m_bHaveClicked = true;
    
    CLRealGameLayer* pParNode = (CLRealGameLayer*)CLSceneManager::sharedSceneManager()->getpParentNode();
    pParNode->StartLoadingNetWork();
    
    //心跳
    pParNode->StartJumpNetWork();
    
    CLSceneManager::sharedSceneManager()->setpCallTarget(this);
    CLSceneManager::sharedSceneManager()->setpCallFun(callfunc_selector(CLTopLayerScene::AdvImageLoadEnd));
    
    CLSceneManager::sharedSceneManager()->getpLoadingLayer()->setVisible(true);
    
    //依据帐号信息连接登陆服务器
    CLNetWorkManager::shardNetWorkManager()->LogonReq();
    
    return true;
}

void CLTopLayerScene::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void CLTopLayerScene::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void CLTopLayerScene::AdvImageLoadEnd()              //异步加载图片完成后的回调 一般都是开始切换场景
{
    CLMainSceneLayer* pLayer = CLMainSceneLayer::create();
    pLayer->setAnchorPoint(CCPointZero);
    pLayer->setPosition(CCPointZero);
    CLSceneManager::sharedSceneManager()->ChangeScene(pLayer);
}






