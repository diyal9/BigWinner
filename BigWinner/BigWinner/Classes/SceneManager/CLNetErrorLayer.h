//
//  CLNetErrorLayer.h
//  Yy_mobileGame3
//
//  Created by Zhang Qiyun on 13-8-28.
//
//

#ifndef __Yy_mobileGame3__CLNetErrorLayer__
#define __Yy_mobileGame3__CLNetErrorLayer__

#include "cocos2d.h"
USING_NS_CC;

class CLNetErrorLayer : public CCLayerColor
{
public:
    CLNetErrorLayer();
    ~CLNetErrorLayer();
    void onCloseMenuClick(CCObject* pSender);
    
    static CLNetErrorLayer* create(const char* sText);

    bool init(const char* sText);
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
};


#endif /* defined(__Yy_mobileGame3__CLNetErrorLayer__) */
