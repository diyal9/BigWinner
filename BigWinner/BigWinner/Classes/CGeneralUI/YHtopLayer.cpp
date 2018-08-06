//
//  YHtopLayer.cpp
//  BigWinner
//
//  Created by LunaBox03 on 13-11-3.
//
//

#include "YHtopLayer.h"

#include <LGameCommonDefine.h>
#include <Button.h>
#include <JsonDataManager.h>
#include "YHUILayer.h"
#include "CLSceneManager.h"
#include "CLSoundManager.h"

using namespace Json;

YHtopLayer* YHtopLayer::create()
{
    YHtopLayer *pRet = new YHtopLayer();
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

bool YHtopLayer::init()
{
    if(!CCLayerColor::initWithColor(ccc4(0, 0, 0, 0)))
    {
        return false;
    }
    
//    //数据解析
//    m_bkData = data;
    
//    CCLOG("data:%d", m_bkData["playerrole"].size());
    
    //初始化UI
    doBaseUI();
    doAction(this, true);

    return true;
}

//动作执行
void YHtopLayer::doAction(CCNode* r, bool moveIn)
{
    CCSize size = CCDirector::sharedDirector()->getgSceneSize();
    
    if (moveIn) //进场动作
    {
        //窗体开闭音乐
        CLSoundManager::sharedSoundManager()->PlayEffectSound("Sound/Sl_WInClose.wav");
        
        CCLayerColor *fadeLayer = CCLayerColor::create();

        CLSceneLayer* pMainLayer = (CLSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
        if(pMainLayer)
        {
            pMainLayer->addChild(fadeLayer,9,500);
        }
        
//        CCCallFunc * func = CCCallFunc::create(this, callfunc_selector(YHtopLayer::doActionTwo));
        
        CCFiniteTimeAction *actionOut = CCSequence::create(CCFadeTo::create(0.4f, 255*0.5), NULL); //透明度动画
        fadeLayer->runAction(actionOut);
        
        CCAction *actionMoveOut = CCEaseExponentialOut::create(CCMoveTo::create(0.4f, ccp(0, size.height))); //快到慢
        this->runAction(actionMoveOut);
    }
    
}


void YHtopLayer::doBaseUI()
{
    
    CCSize size = CCDirector::sharedDirector()->getgSceneSize();
    
    //印花
    CCSprite* yh = CCSprite::create("Image/BackPack/BKShop2.png");
    this->addChild(yh);
    yh->setPosition(ccp(size.width*0.5, size.height*0.43));
    
    PrizeShow* hyLay = PrizeShow::createPShow();
    this->addChild(hyLay,10);
    hyLay->setPosition(ccp(G_SF(-50), G_SF(0)));

}

void YHtopLayer::onEnter()
{
    CCLayerColor::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -10000, true);
}

void YHtopLayer::onExit()
{
    CCLayerColor::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}



bool YHtopLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if(this->isVisible())
        return true;
    else
        return false;
}

void YHtopLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void YHtopLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

