//
//  CLFadeColorLayer.h
//  LolAllStars1_X
//
//  Created by Zhang Qiyun on 13-5-17.
//  该层主要是用于屏蔽场景任何触摸消息的颜色层，可以用来做淡入淡出切换层 ＋ 屏蔽触控
//

#ifndef __LolAllStars1_X__CLFadeColorLayer__
#define __LolAllStars1_X__CLFadeColorLayer__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;

class CLFadeColorLayer : public CCLayerColor
{
public:
    CLFadeColorLayer();
    ~CLFadeColorLayer();
    
    static CLFadeColorLayer* create(const ccColor4B& color, GLfloat width, GLfloat height);
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
};

#endif /* defined(__LolAllStars1_X__CLFadeColorLayer__) */
