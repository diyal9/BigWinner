//
//  CLFadeColorLayer.cpp
//  LolAllStars1_X
//
//  Created by Zhang Qiyun on 13-5-17.
//
//

#include "CLFadeColorLayer.h"

#define CL_FADETOUCHINDEX -1000000

CLFadeColorLayer::CLFadeColorLayer()
{
    
}

CLFadeColorLayer::~CLFadeColorLayer()
{
    
}


CLFadeColorLayer* CLFadeColorLayer::create(const ccColor4B& color, GLfloat width, GLfloat height)
{
    CLFadeColorLayer* pLayer = new CLFadeColorLayer();
    if(pLayer && pLayer->initWithColor(color, width, height))
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

void CLFadeColorLayer::onEnter()
{
    CCLayerColor::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, CL_FADETOUCHINDEX, true);
}

void CLFadeColorLayer::onExit()
{
    CCLayerColor::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool CLFadeColorLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}
void CLFadeColorLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CLFadeColorLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

