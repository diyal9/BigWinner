//
//  CLPveGiveupLayer.h
//  BigWinner
//
//  Created by Zhang Qiyun on 13-11-27.
//
//

#ifndef __BigWinner__CLPveGiveupLayer__
#define __BigWinner__CLPveGiveupLayer__

#include "cocos2d.h"
USING_NS_CC;

class CLPveGiveupLayer : public CCLayerColor
{
public:
    CLPveGiveupLayer();
    ~CLPveGiveupLayer();
    
    CREATE_FUNC(CLPveGiveupLayer);
    
    virtual bool init();
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void onGiveupMenuPress(CCObject* pSender);
    
};

#endif /* defined(__BigWinner__CLPveGiveupLayer__) */
