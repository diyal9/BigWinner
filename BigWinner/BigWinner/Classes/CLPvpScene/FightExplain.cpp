//
//  FightExplain.cpp
//  BigWinner
//
//  Created by LunaBox on 13-11-23.
//
//

#include "FightExplain.h"
#include <LGameCommonDefine.h>
#include <Button.h>
#include "CLMainSceneLayer.h"
#include "CLSceneManager.h"
#include "CLPvpReady.h"
#include "CLPvpSceneLayer.h"
#include "CLChangeString.h"


FightExplainLayer* FightExplainLayer::create()
{
    FightExplainLayer *pRet = new FightExplainLayer();
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

bool FightExplainLayer::init()
{
    if(!CCLayerColor::initWithColor(ccc4(0, 0, 0, 0)))
    {
        return false;
    }
    
    //初始化UI
    doBaseUI();
    
    return true;
}

void FightExplainLayer::doBaseUI()
{
    
    CCSize size = CCDirector::sharedDirector()->getgSceneSize();
    
    CCSprite *layer = CCSprite::create("Image/BackPack/StampBackground1.png");
    layer->setPosition(ccp(size.width*0.5, size.height*0.49));
    this->addChild(layer);
    
    //bg
    CCSprite* shuoming = CCSprite::create("Image/PvpLayer/CP_PvpExplainBG.png");
    shuoming->setPosition(ccp(layer->getContentSize().width*0.5, layer->getContentSize().height*0.5 -G_SF(20)));
    layer->addChild(shuoming);
    
    //备战说明
    CCSprite* PrepareExplain = CCSprite::create("Image/PublicUI/TitleBackground.png");
    PrepareExplain->setPosition(ccp(G_SF(90), G_SF(660)));
    layer->addChild(PrepareExplain);
    
    //备战说明文字
    CCSprite *PrepareExplainStr = CCSprite::create("Image/PvpLayer/PrepareExplain.png");
    PrepareExplainStr->setPosition(ccp(G_SF(80), G_SF(25)));
    PrepareExplain->addChild(PrepareExplainStr,1);
    
    //备战说明
    CCLabelTTF *Explain = CCLabelTTF::create(CLChangeString::sharedChangeString()->GetChangeString("PvpExplain").c_str(), "Arial", G_SF(18), CCSizeMake(shuoming->getContentSize().width-G_SF(40),G_SF(545)), kCCTextAlignmentLeft);
    Explain->setColor(ccc3(220,100,0));
    Explain->setPosition(ccp(shuoming->getContentSize().width*0.5+ G_SF(10), shuoming->getContentSize().height*0.5));
    shuoming->addChild(Explain);
    
    //关闭
    Button *shutdown = Button::createBtn("Image/PublicUI/Back.png");
    shutdown->setPosition(ccp(G_SF(540), G_SF(630)));
    shutdown->setOnClickCallbackNode(callfuncN_selector(FightExplainLayer::menuCallBackButton), this);
    shutdown->setHandlerPriority(-10001);
    shuoming->addChild(shutdown);
    
}

void FightExplainLayer::menuCallBackButton(CCNode* r)
{
    CLPvpReady* pParNode = (CLPvpReady*)getParent();
    pParNode->ReturnRoomSelLayer();
}

void FightExplainLayer::onEnter()
{
    CCLayerColor::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -10000, true);
}

void FightExplainLayer::onExit()
{
    CCLayerColor::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);

}



bool FightExplainLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if(this->isVisible())
        return true;
    else
        return false;
}

void FightExplainLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void FightExplainLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
