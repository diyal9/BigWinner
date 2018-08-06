//
//  SystemInformationLayer.cpp
//  BigWinner
//
//  Created by LunaBox on 13-11-27.
//
//

#include "SystemInformationLayer.h"
#include "LGameCommonDefine.h"
#include "CLChangeString.h"

SystemInformationLayer::SystemInformationLayer(void)
{
}


SystemInformationLayer::~SystemInformationLayer(void)
{
}

bool SystemInformationLayer::init()
{
	bool bRet=false;
	do
	{
		CC_BREAK_IF(!CCLayer::init());
        
		//水平滚动
        textbase=CCSprite::create("Image/BackPack/018X.png");
		textbase->setPosition(ccp(G_SF(140),G_SF(50)));
		this->addChild(textbase);
        
        m_STTFStr =CLChangeString::sharedChangeString()->GetChangeString("SystemInformation").c_str();

		text2=CCLabelTTF::create(m_STTFStr.c_str(),"Arial",G_SF(25));
        text2->setPosition(ccp(G_SF(1300), G_SF(23)));
        text2->setPosition(ccp(G_SF(m_STTFStr.length()*25/3)*0.5+ textbase->getContentSize().width*0.5 , G_SF(23)));
		text2->setColor(ccc3(220,100,0));
		textbase->addChild(text2);
        
		this->scheduleUpdate();
        
		bRet=true;
	} while (0);
	return bRet;
}

void SystemInformationLayer::update(float delta)
{
	CCSize visibleSize=CCDirector::sharedDirector()->getVisibleSize();
    
    CCSize size1 = CCDirector::sharedDirector()->getWinSize();
    
    int newX=text2->getPositionX()-G_SF(2)*640/size1.width;
    
//    CCLog("newX: %i",newX);
    if (newX<=-text2->getContentSize().width/2)
    {
        newX=visibleSize.width+text2->getContentSize().width/2;
    }
    
    text2->setPositionX(newX);
    
    
}