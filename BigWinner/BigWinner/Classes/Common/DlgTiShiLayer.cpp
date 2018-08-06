//
//  TiShiLayer.cpp
//  BigWinner
//
//  Created by LunaBox on 13-11-3.
//
//

#include "DlgTiShiLayer.h"
#include "LGameCommonDefine.h"
#include "CLSceneManager.h"


using namespace cocos2d;
using namespace cocos2d::extension;
TiShiLayer* TiShiLayer::create(std::string Str)
{
    TiShiLayer *pRet = new TiShiLayer();
	if (pRet && pRet->init(Str))
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

bool TiShiLayer::init(std::string Str)
{
    if(!CCLayerColor::initWithColor(ccc4(0, 0, 0, 225*0.5)))
    {
        return false;
    }

    CLSceneManager::sharedSceneManager()->getpParentNode()->addChild(this, DLG_GENERAL_ZORDER);
    CCSize size = CCDirector::sharedDirector()->getgSceneSize();
    
//    this->setContentSize( ccp(G_SF(640), G_SF(80)));
    this->setContentSize( ccp(size.width, size.height));
//    this->setPosition(ccp(0, size.height*0.5));
    this->setPosition(ccp(0, 0));
    

    CCScale9Sprite* tishiBg = CCScale9Sprite::create("Image/PublicUI/Tishi_BG.png");
    tishiBg->setPosition(ccp(this->getContentSize().width*0.5, this->getContentSize().height*0.5));
    tishiBg->setPreferredSize(CCSizeMake(G_SF(640), G_SF(80)));
    this->addChild(tishiBg);
    
    CCLabelTTF *ButtonStr = CCLabelTTF::create(Str.c_str(), "Arial",  G_SF(30));
    ButtonStr->setColor(ccc3(50, 50, 50));
    ButtonStr->setPosition(ccp(G_SF(320), this->getContentSize().height * 0.5));
    this->addChild(ButtonStr);
    
//    //提示信息停留0.5秒
//    CCDelayTime *waiting=CCDelayTime::create(0.5f);
//    
//    //提示信息移动
//    CCMoveTo *pMoveTo = CCMoveTo::create(1, ccp(this->getPositionX(),this->getPositionY()+40));
//    
//    CCDelayTime *waiting2=CCDelayTime::create(0.2f);
//    
//    //提示信息先停留然后移动
//    CCActionInterval* pAction = (CCActionInterval*)CCSequence::create(waiting,pMoveTo,waiting2,NULL);
//    
//    //执行动画效果，
//    tishiBg->runAction(pAction);
    
    //显示几秒 以后把它remove掉
    this->scheduleOnce(schedule_selector(TiShiLayer::removeUI), 1.0);
    
    return true;
}

void TiShiLayer::removeUI()
{
    //初始化时 几秒以后remove掉
    this->removeFromParent();

}