//
//  BackPack.cpp
//  BigWinner
//
//  Created by LunaBox03 on 13-10-19.
//
//

#include "FightBackPack.h"

#include <LGameCommonDefine.h>
#include <Button.h>
#include "FightRoleUI.h"
#include "FightQuestionCardUI.h"
#include "FightPropsUI.h"
#include "CLMainSceneLayer.h"
#include "CLSceneManager.h"
#include "CLPvpReady.h"


FightBackPackLayer* FightBackPackLayer::create()
{
    FightBackPackLayer *pRet = new FightBackPackLayer();
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

bool FightBackPackLayer::init()
{
    if(!CCLayerColor::initWithColor(ccc4(0, 0, 0, 0)))
    {
        return false;
    }
    
    //初始化UI
    doBaseUI();
    
    return true;
}

void FightBackPackLayer::doBaseUI()
{
    
    CCSize size = CCDirector::sharedDirector()->getgSceneSize();
    
    //bg
    CCSprite* bg = CCSprite::create("Image/PvpLayer/CP_BgBG.png");
    this->addChild(bg);
    bg->setPosition(ccp(size.width*0.5, size.height*0.49));
    
    //备战设置
    CCSprite *bzShezhi = CCSprite::create("Image/PublicUI/TitleBackground.png");
    bzShezhi->setPosition(ccp(G_SF(80), G_SF(600)));
    bg->addChild(bzShezhi);
    
    //备战设置文字
    CCSprite *bzwenzi = CCSprite::create("Image/BackPack/FightSet.png");
    bzwenzi->setPosition(ccp(G_SF(80), G_SF(25)));
    bzShezhi->addChild(bzwenzi,1);
    
    //关闭
    Button *shutdown = Button::createBtn("Image/PublicUI/Close.png");
    shutdown->setPosition(ccp(G_SF(550), G_SF(600)));
    shutdown->setOnClickCallbackNode(callfuncN_selector(FightBackPackLayer::menuCallBackButton), this);
    shutdown->setHandlerPriority(-10001);
    bg->addChild(shutdown,0,FI_GUANBI_TAG);
    
    //角色
    Button *juese = Button::createBtn("Image/BackPack/Titlebtn.png");
    juese->setPosition( ccp(G_SF(70), G_SF(465)) );
    juese->setOnClickCallbackNode(callfuncN_selector(FightBackPackLayer::menuCallBackButton), this);
    juese->setHandlerPriority(-10001);
    bg->addChild(juese,0,FI_JUESE_TAG);
    
    //角色文字
    CCSprite *jswenzi = CCSprite::create("Image/BackPack/Role.png");
    jswenzi->setPosition(ccp(G_SF(0), G_SF(0)));
    juese->addChild(jswenzi,1);

    //提卡
    Button *tika = Button::createBtn("Image/BackPack/Titlebtn.png");
    tika->setPosition( ccp(G_SF(70), G_SF(295)) );
    tika->setOnClickCallbackNode(callfuncN_selector(FightBackPackLayer::menuCallBackButton), this);
    tika->setHandlerPriority(-10001);
    bg->addChild(tika,0,FI_TIKA_TAG);
    
    //题卡文字
    CCSprite *tkwenzi = CCSprite::create("Image/BackPack/card.png");
    tkwenzi->setPosition(ccp(G_SF(0), G_SF(0)));
    tika->addChild(tkwenzi,1);
    
    //道具
    Button *daoju = Button::createBtn("Image/BackPack/Titlebtn.png");
    daoju->setPosition( ccp(G_SF(70), G_SF(125)) );
    daoju->setOnClickCallbackNode(callfuncN_selector(FightBackPackLayer::menuCallBackButton), this);
    daoju->setHandlerPriority(-10001);
    bg->addChild(daoju,0,FI_DAOJU_TAG);
    
    //道具文字
    CCSprite *djwenzi = CCSprite::create("Image/BackPack/Props.png");
    djwenzi->setPosition(ccp(G_SF(0), G_SF(0)));
    daoju->addChild(djwenzi,1);

    //第一次默认加载ui
    this->scheduleOnce(schedule_selector(FightBackPackLayer::addBaseUI), 0.2);
    
}

void FightBackPackLayer::addBaseUI()
{

    //加载默认  角色卡
    FightRoleUI* RoleLay = FightRoleUI::create();
    this->addChild(RoleLay,10, FI_GENERAL_TAG);
    RoleLay->setPosition(ccp(G_SF(-10), G_SF(0)));
    m_nlayerTpye = FI_JUESE;
    
    if(this->getntag()!= 0)
    {
        this->getChildByTag(this->getntag())->removeFromParentAndCleanup(true);
    }
    this->setntag(FI_JUESE_TAG2);
    
    //角色
    Button *juese = Button::createBtn("Image/BackPack/Titlebtn2.png");
    juese->setPosition( ccp(G_SF(80), G_SF(615)) );
    juese->setOnClickCallbackNode(callfuncN_selector(FightBackPackLayer::menuCallBackButton), this);
    juese->setHandlerPriority(-10001);
    this->addChild(juese,0,FI_JUESE_TAG2);
    
    //角色文字
    CCSprite *jswenzi = CCSprite::create("Image/BackPack/Role.png");
    jswenzi->setPosition(ccp(G_SF(0), G_SF(0)));
    jswenzi->setScaleX(1.1);
    juese->addChild(jswenzi,1);
    
    //刷新位置
    setIndexPosition();

}

void FightBackPackLayer::menuCallBackButton(CCNode* r)
{
    
    CCNode* node = this->getChildByTag(FI_GENERAL_TAG);
    if (node)
    {
        node->removeAllChildrenWithCleanup(true);
        node->removeFromParentAndCleanup(true);
        m_nlayerTpye = 0;
        
    }
   
    switch (r->getTag())
    {
    
       //wangguolong
        case FI_JUESE_TAG:
        case FI_JUESE_TAG2:
        {
            if(this->getntag()!= 0)
            {
                this->getChildByTag(this->getntag())->removeFromParentAndCleanup(true);
            }
            
            this->setntag(FI_JUESE_TAG2);
            //角色选中
            Button *juese = Button::createBtn("Image/BackPack/Titlebtn2.png");
            juese->setPosition( ccp(G_SF(80), G_SF(615)) );
            juese->setOnClickCallbackNode(callfuncN_selector(FightBackPackLayer::menuCallBackButton), this);
            juese->setHandlerPriority(-10001);
            this->addChild(juese,0,FI_JUESE_TAG2);
            
            //角色文字选中
            CCSprite *jswenzi = CCSprite::create("Image/BackPack/Role.png");
            jswenzi->setScaleX(1.1);
            jswenzi->setPosition(ccp(G_SF(0), G_SF(0)));
            juese->addChild(jswenzi,1);
            
             CCLOG("角色");
            if (m_nlayerTpye == FI_JUESE)
                return;
            
            FightRoleUI* RoleLay = FightRoleUI::create();
            this->addChild(RoleLay,10, FI_GENERAL_TAG);
            RoleLay->setPosition(ccp(G_SF(-10), G_SF(0)));
            
            m_nlayerTpye = FI_JUESE;
        }
            break;
        case FI_TIKA_TAG:
        case FI_TIKA_TAG2:
        {
            if(this->getntag()!= 0)
            {
                this->getChildByTag(this->getntag())->removeFromParentAndCleanup(true);
            }
            this->setntag(FI_TIKA_TAG2);
            //提卡选中
            Button *tika = Button::createBtn("Image/BackPack/Titlebtn2.png");
            tika->setPosition( ccp(G_SF(80), G_SF(445)) );
            tika->setOnClickCallbackNode(callfuncN_selector(FightBackPackLayer::menuCallBackButton), this);
            tika->setHandlerPriority(-10001);
            this->addChild(tika,0,FI_TIKA_TAG2);
            
            //题卡文字选中
            CCSprite *tkwenzi = CCSprite::create("Image/BackPack/card.png");
            tkwenzi->setScaleX(1.1);
            tkwenzi->setPosition(ccp(G_SF(0), G_SF(0)));
            tika->addChild(tkwenzi,1);

             CCLOG("提卡");
            if (m_nlayerTpye == FI_TIKA)
                return;
            
            FightQuestionCardUI* QuestionCardLay = FightQuestionCardUI::create();
            this->addChild(QuestionCardLay,10, FI_GENERAL_TAG);
            QuestionCardLay->setPosition(ccp(G_SF(-10), G_SF(0)));
            
            m_nlayerTpye = FI_TIKA;
        }
            break;
        case FI_DAOJU_TAG:
        case FI_DAOJU_TAG2:
        {
            if(this->getntag()!= 0)
            {
                this->getChildByTag(this->getntag())->removeFromParentAndCleanup(true);
            }
            
            this->setntag(FI_DAOJU_TAG2);
            //道具选中
            Button *daoju = Button::createBtn("Image/BackPack/Titlebtn2.png");
            daoju->setPosition( ccp(G_SF(80), G_SF(275)) );
            daoju->setOnClickCallbackNode(callfuncN_selector(FightBackPackLayer::menuCallBackButton), this);
            daoju->setHandlerPriority(-10001);
            this->addChild(daoju,0,FI_DAOJU_TAG2);
            
            //道具文字选中
            CCSprite *djwenzi = CCSprite::create("Image/BackPack/Props.png");
            djwenzi->setScaleX(1.1);
            djwenzi->setPosition(ccp(G_SF(0), G_SF(0)));
            daoju->addChild(djwenzi,1);
            
             CCLOG("道具");
            if (m_nlayerTpye == FI_DAOJU)
                return;
            
            FightPropsUI* PropsLay = FightPropsUI::create();
            this->addChild(PropsLay,10, FI_GENERAL_TAG);
            PropsLay->setPosition(ccp(G_SF(-10), G_SF(0)));
            
            m_nlayerTpye = FI_DAOJU;
        }
            break;
            
        case FI_GUANBI_TAG:
        {
            CLPvpReady* pParNode = (CLPvpReady*)getParent();
            pParNode->ReturnRoomSelLayer();
            return;
        }
        default:
            break;
    }
    
    setIndexPosition();
}

void FightBackPackLayer::setIndexPosition()
{
//    m_indesSprite->setVisible(true);
    
//    switch (m_nlayerTpye)
//    {
////            //wangguolong
//        case FI_JUESE:
//        {
//            m_indesSprite->setPosition(ccp(m_indesSprite->getParent()->getContentSize().width*0.5, G_SF(370)));
//        }
//            break;
//        case FI_TIKA:
//        {
//            m_indesSprite->setPosition(ccp(m_indesSprite->getParent()->getContentSize().width*0.5, G_SF(250)));
//        }
//            break;
//        case FI_DAOJU:
//        {
//          m_indesSprite->setPosition(ccp(m_indesSprite->getParent()->getContentSize().width*0.5, G_SF(120)));
//        }
//            break;
//            
//        default:
//            break;
//    }

}


void FightBackPackLayer::onEnter()
{
    CCLayerColor::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -10000, true);
}

void FightBackPackLayer::onExit()
{
    CCLayerColor::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    
    //获取选择的道具 发送给服务器选择的道具信息
    CLSceneManager::sharedSceneManager()->getStatusMessage();
}

bool FightBackPackLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if(this->isVisible())
        return true;
    else
        return false;
}

void FightBackPackLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void FightBackPackLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
