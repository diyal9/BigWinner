//
//  CLPvpResultLayer.cpp
//  BigWinner
//
//  Created by YinYanlong on 13-11-27.
//
//

#include "CLPvpResultLayer.h"
#include "Button.h"
#include "LGameCommonDefine.h"
#include "JsonParser.h"
#include "CLChangeString.h"
#include "JsonDataManager.h"
#include "CLPvpReady.h"
#include "CLPvpSceneLayer.h"
#include "CLSceneManager.h"
#include "CLAnimationManager.h"
#include "CLStampExamineLayer.h"
#include "CLNetWorkManager.h"
#include "CLSoundManager.h"

USING_NS_CC;

CLPvpSuccLayer::CLPvpSuccLayer()
{
    isSuccess = true;
    m_nYh1total = 0; //拥有的银印花
    m_nYh2total = 0; //拥有的金印花
}

CLPvpSuccLayer::~CLPvpSuccLayer()
{
    
}

CLPvpSuccLayer* CLPvpSuccLayer::create(Json::Value& jv, std::string rivalName)
{
    CLPvpSuccLayer* pLayer = new CLPvpSuccLayer();
    if(pLayer && pLayer->init(jv, rivalName))
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

bool CLPvpSuccLayer::init(Json::Value& jv, std::string rivalName)
{
    
    if ( !CCLayerColor::initWithColor(ccc4(0,0,0,250)) )
    {
        return false;
    }
    
    //获得屏幕大小
	CCSize size = CCDirector::sharedDirector()->getgSceneSize();
    
    int iScore = jv["score"].asIntModestyle();
    int iScoreAdd = jv["addScore"].asIntModestyle();
    int iStampID = jv["gdsno"].asIntModestyle();
    
    m_nYh1total = jv["yh1total"].asIntModestyle();//当前拥有的银印花
    m_nYh2total = jv["yh2total"].asIntModestyle();//当前拥有的金印花
    
    
    if (jv["yh"].asIntModestyle() > 0 || jv["yh2"].asIntModestyle() > 0)
    {
        isSuccess = true; //失败
    }
    else
    {
        isSuccess = false;//成功
    }
    
    //    isSuccess = false; //test
    
    
    CCSprite* bg;
    if (isSuccess) {
        
        //成功音效
        CLSoundManager::sharedSoundManager()->PlayEffectSound("Sound/Sl_PvpWin.wav");
        
        bg = CCSprite::create("Image/PveLayer/C_PveWinBg.png");
        bg->setPosition(ccp(size.width * 0.5, size.height * 0.5));
        this->addChild(bg, 1);
    
//        CCSprite *sSpr_Star = CCSprite::create("Image/BackPack/card_compound_pft.png");  //背景星
//        sSpr_Star->setPosition(ccp(size.width * 0.5, G_SF(690)));
//        bg->addChild(sSpr_Star);
        
        //星星播放效果
        std::string sFile;
        sFile = "flicker";
        
        CCAnimation* animation = CLAnimationManager::sharedAnimationManager()->animationWithName(sFile.c_str(),0.041);
        CCSprite* sSpr_Star = CLAnimationManager::sharedAnimationManager()->spriteWithAnimationName(sFile.c_str());
        sSpr_Star->setPosition(ccp(this->getContentSize().width*0.5,G_SF(1000)));
        this->addChild(sSpr_Star,2);
        
        CCActionInterval* m_pAction2 = (CCActionInterval*)CCSequence::create(
                                                                             CCAnimate::create(animation),
                                                                             NULL
                                                                             );
        
        sSpr_Star->runAction(CCRepeatForever::create(m_pAction2));
        
        CCSprite *winSpr = CCSprite::create("Image/PveLayer/CP_WinICon.png");
        winSpr->setPosition( ccp(sSpr_Star->getContentSize().width * 0.5, sSpr_Star->getContentSize().height * 0.6));
        sSpr_Star->addChild(winSpr);
        
        sSpr_Star->setAnchorPoint(ccp(0.5, 0));
        
        //胜利文字动画
        CCActionInterval* m_pAction = (CCActionInterval*)CCSequence::create(
                                                                            CCDelayTime::create(0.5),
                                                                            CCMoveTo::create(0.2, ccp(G_SF(320), G_SF(500))),
                                                                            CCScaleBy::create(0.1, 1.1,0.9),
                                                                            CCMoveTo::create(0.1, ccp(G_SF(320), G_SF(504))),
                                                                            CCScaleTo::create(0.1, 1.0,1.0),
                                                                            CCMoveTo::create(0.1, ccp(G_SF(320), G_SF(500))),
                                                                            NULL
                                                                            );
        sSpr_Star->runAction(m_pAction);
        
        //小背景框添加
        CCSprite *getInfoBg = CCSprite::create("Image/PveLayer/C_BgWinLab.png");
        getInfoBg->setPosition( ccp(size.width * 0.5, size.height * 0.33));
        bg->addChild(getInfoBg, 10);
        
        //当前总印花
        //        std::string c_ =  CLChangeString::sharedChangeString()->GetChangeString("RightAnsNum");
        //        m_nCorrentNum = 0;
        //        c_corrStr += CCString::createWithFormat("%d", m_nCorrentNum)->getCString();
        //
        //        CCLabelTTF *corrStr = CCLabelTTF::create(c_corrStr.c_str(), "Arial", G_SF(36));
        //        corrStr->setPosition(ccp(size.width * 0.5, G_SF(596)));
        //        this->addChild(corrStr);
        
        //积分
        CCSprite *sSprInteger = CCSprite::create("Image/PvpLayer/CP_IntegerStr.png");
        sSprInteger->setPosition(ccp(G_SF(275), G_SF(320)));
        getInfoBg->addChild(sSprInteger);
        
        std::string c_integralStr = "+";
        c_integralStr += CCString::createWithFormat("%d", iScoreAdd)->getCString();
        CCLabelTTF *integral = CCLabelTTF::create(c_integralStr.c_str(), "Arial", G_SF(26));
        integral->setPosition(ccp(sSprInteger->getPositionX() + G_SF(80), sSprInteger->getPositionY()));
        getInfoBg->addChild(integral);
        
        //恭喜Label
        std::string c_gxiStr =  CLChangeString::sharedChangeString()->GetChangeString("PvpSuccInfo") + rivalName;
        CCLabelTTF *congrStr = CCLabelTTF::create(c_gxiStr.c_str(), "Arial", G_SF(28), ccp(G_SF(450), G_SF(80)), kCCTextAlignmentCenter);
        congrStr->setPosition(ccp(getInfoBg->getContentSize().width * 0.5, G_SF(220)));
        getInfoBg->addChild(congrStr);
        
        
        std::string stampName = JsonDataManager::shardJDManager()->m_yinHua[iStampID]->m_Name;
        
        //战利品
        if (jv.isMember("yh2") && (jv["yh2"].asIntModestyle() == 1)) //获得金印花
        {
            //商品图片路径
            std::string stampSmall_path = CCString::createWithFormat("Image/GoodIcon/StampSmall%d.png", jv["gdsno"].asIntModestyle())->getCString();
            
            //印花
            CCSprite *goodsSpr = CCSprite::create(stampSmall_path.c_str());
            goodsSpr->setPosition( ccp(size.width * 0.5, G_SF(140)));
            getInfoBg->addChild(goodsSpr);
            
            std::string c_stamp = stampName + CLChangeString::sharedChangeString()->GetChangeString("GoldStr") + "x1";
            CCLabelTTF *goodsNumSpr = CCLabelTTF::create(c_stamp.c_str(), "Arial", 20);
            //            goodsNumSpr->setPosition(ccp(goodsSpr->getPositionX(), goodsSpr->getPositionY() - goodsSpr->getContentSize().height * 0.5 - G_SF(20)));
            //            getInfoBg->addChild(goodsNumSpr);
            goodsNumSpr->setPosition(ccp(goodsSpr->getContentSize().width*0.5, -25));
            goodsSpr->addChild(goodsNumSpr);
            
            //印花效果
            CCActionInterval* m_pAction = (CCActionInterval*)CCSequence::create(
                                                                                CCHide::create(),
                                                                                CCScaleTo::create(1.0, 2.5,2.5),
                                                                                CCDelayTime::create(0.2),
                                                                                CCShow::create(),
                                                                                CCScaleTo::create(0.2, 1.0,1.0),
                                                                                CCFadeTo::create(0.2, 255),
                                                                                
                                                                                NULL
                                                                                );
            goodsSpr->runAction(m_pAction);
            
        }
        else
        {
            //商品图片路径
            std::string stampSmall_path = CCString::createWithFormat("Image/GoodIcon/StampSmall%d.png", jv["gdsno"].asIntModestyle())->getCString();
            
            //没有抢到金印花，则显示银印花
            CCSprite *sivleSpr = CCSprite::create(stampSmall_path.c_str());
            sivleSpr->setPosition( ccp(size.width * 0.5, G_SF(140)));
            getInfoBg->addChild(sivleSpr);
            
            std::string c_stamp = stampName + CLChangeString::sharedChangeString()->GetChangeString("SilverStr") + "x1";
            CCLabelTTF *goodsNumSpr = CCLabelTTF::create(c_stamp.c_str(), "Arial", 20);
            goodsNumSpr->setPosition(ccp(sivleSpr->getPositionX(), sivleSpr->getPositionY() - sivleSpr->getContentSize().height * 0.5 - G_SF(20)));
            getInfoBg->addChild(goodsNumSpr);
        }
        
        //查看印花按钮
        Button *viewStampbtn = Button::createBtn("Image/PublicUI/Btn_gray.png");
        viewStampbtn->setPosition(ccp(G_SF(170), G_SF(60)));
        viewStampbtn->setOnClickCallback(menu_selector(CLPvpSuccLayer::ViewStampCall),this);
        viewStampbtn->setHandlerPriority(-10010);
        bg->addChild(viewStampbtn,1,3);
        
        CCLabelTTF* lBtnTTF = CCLabelTTF::create(CLChangeString::sharedChangeString()->GetChangeString("ViewStamp").c_str(), "Arial", G_SF(30), CCSizeMake(G_SF(200), G_SF(40)), kCCTextAlignmentCenter);
        viewStampbtn->addChild(lBtnTTF, 10);
        
        //返回大厅
        Button *returnRoomBtn = Button::createBtn("Image/BackPack/C_composite.png");
        returnRoomBtn->setPosition(ccp(viewStampbtn->getPositionX() + viewStampbtn->getContentSize().width +G_SF(20), viewStampbtn->getPositionY()));
        returnRoomBtn->setOnClickCallback(menu_selector(CLPvpSuccLayer::onCloseMenuPress),this);
        returnRoomBtn->setHandlerPriority(-10010);
        bg->addChild(returnRoomBtn,1,3);
        
        CCLabelTTF* lBtnConTTF = CCLabelTTF::create(CLChangeString::sharedChangeString()->GetChangeString("ReturnToHome").c_str(), "Arial", G_SF(30), CCSizeMake(G_SF(200), G_SF(40)), kCCTextAlignmentCenter);
        returnRoomBtn->addChild(lBtnConTTF, 10);
        
        Json::Value data;
        data["msgid"].append("87"); //指令码
        CCLog("印花商品No : %i",jv["gdsno"].asIntModestyle());
        data["No"] = jv["gdsno"].asIntModestyle();  //点击查看印花商品No
        
        CLNetWorkManager::shardNetWorkManager()->SendNetMessage(CMD_PRIZEINFO, data);
        
        CLSceneManager::sharedSceneManager()->SetWaitNetLayer(true);
    }
    else
    {
        //失败音乐
        CLSoundManager::sharedSoundManager()->PlayEffectSound("Sound/Sl_PvpFail.wav");
        
        bg = CCSprite::create("Image/PveLayer/C_PveFailBg.png");
        bg->setPosition(ccp(size.width * 0.5, size.height * 0.5));
        this->addChild(bg, 1);
        
        CCSprite *sSpr_Star = CCSprite::create("Image/PveLayer/C_FailedBg.png");
        sSpr_Star->setPosition(ccp(size.width * 0.5, G_SF(690)));
        bg->addChild(sSpr_Star);
        
        CCSprite *winSpr = CCSprite::create("Image/PveLayer/C_FailedLab.png");
        winSpr->setPosition( ccp(sSpr_Star->getContentSize().width * 0.5, sSpr_Star->getContentSize().height * 0.6));
        sSpr_Star->addChild(winSpr);
        
        sSpr_Star->setAnchorPoint(ccp(0.5, 0));
        
        //失败文字动画
        CCActionInterval* m_pAction = (CCActionInterval*)CCSequence::create(
                                                                            CCDelayTime::create(0.5),
                                                                            CCMoveTo::create(0.2, ccp(G_SF(320), G_SF(600))),
                                                                            CCScaleBy::create(0.1, 1.1,0.9),
                                                                            CCMoveTo::create(0.1, ccp(G_SF(320), G_SF(604))),
                                                                            CCScaleTo::create(0.1, 1.0,1.0),
                                                                            CCMoveTo::create(0.1, ccp(G_SF(320), G_SF(600))),
                                                                            NULL
                                                                            );
        sSpr_Star->runAction(m_pAction);
        
        //小背景框添加
        CCSprite *getInfoBg = CCSprite::create("Image/PveLayer/C_BgWinLab.png");
        getInfoBg->setPosition( ccp(size.width * 0.5, size.height * 0.33));
        bg->addChild(getInfoBg, 10);
        
        //重新来过label
        CCLabelTTF* labelInfo = CCLabelTTF::create(CLChangeString::sharedChangeString()->GetChangeString("FailInfoLabel").c_str(), "Arial", G_SF(28), CCSizeMake(G_SF(550), G_SF(80)), kCCTextAlignmentCenter);
        labelInfo->setPosition(ccp(getInfoBg->getContentSize().width * 0.5, getInfoBg->getContentSize().height * 0.5));
        getInfoBg->addChild(labelInfo, 10);
        
        //返回大厅
        Button *returnRoomBtn = Button::createBtn("Image/BackPack/C_composite.png");
        returnRoomBtn->setPosition(ccp(size.width * 0.5, G_SF(60)));
        returnRoomBtn->setOnClickCallback(menu_selector(CLPvpSuccLayer::onCloseMenuPress),this);
        returnRoomBtn->setHandlerPriority(-10010);
        bg->addChild(returnRoomBtn,1,3);
        
        CCLabelTTF* lBtnConTTF = CCLabelTTF::create(CLChangeString::sharedChangeString()->GetChangeString("ReturnToHome").c_str(), "Arial", G_SF(30), CCSizeMake(G_SF(200), G_SF(40)), kCCTextAlignmentCenter);
        returnRoomBtn->addChild(lBtnConTTF, 10);
        
    }
    
	return true;
}

//返回大厅回调
void CLPvpSuccLayer::onCloseMenuPress(CCObject* pSender)
{
    CLPvpSceneLayer* pParNode = (CLPvpSceneLayer*)getParent()->getParent()->getParent();
    pParNode->ChangeSubLayer();
}

//查看印花回调
void CLPvpSuccLayer::ViewStampCall()
{
    CCLog("view");
    
    int nMeYH1 = m_nYh1total;//当前拥有的银印花
    int nMeYH2 = m_nYh2total;//当前拥有的金印花
    
    CLStampExamineLayer *CLStampExamineLayer = CLStampExamineLayer::create(nMeYH1,nMeYH2);
    CLStampExamineLayer->setPosition(0, 0);
    this->addChild(CLStampExamineLayer,11);
}

void CLPvpSuccLayer::continueChallengeCall()
{

}

void CLPvpSuccLayer::onEnter()
{
    CCLayerColor::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -10008, true);
}

void CLPvpSuccLayer::onExit()
{
    CCLayerColor::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool CLPvpSuccLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if(this->isVisible())
        return true;
    else
        return false;
}