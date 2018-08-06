//
//  CLWaitNetLayer.h
//  Yy_mobileGame3
//
//  Created by Zhang Qiyun on 13-8-16.
//
//

#ifndef __Yy_mobileGame3__CLWaitNetLayer__
#define __Yy_mobileGame3__CLWaitNetLayer__

#include "cocos2d.h"
USING_NS_CC;

class CLWaitNetLayer : public CCLayerColor
{
public:
    CLWaitNetLayer();
    ~CLWaitNetLayer();
    
    CREATE_FUNC(CLWaitNetLayer);
    
    virtual bool init();
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
};

#endif /* defined(__Yy_mobileGame3__CLWaitNetLayer__) */
