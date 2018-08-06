//
//  FightExplain.h
//  BigWinner
//
//  Created by LunaBox on 13-11-23.
//
//

#ifndef __BigWinner__FightExplain__
#define __BigWinner__FightExplain__

#include <iostream>
#include <cocos2d.h>
#include "JsonParser.h"

USING_NS_CC;

class FightExplainLayer:public CCLayerColor
{
    
public:
	FightExplainLayer()
    {
    }
	~FightExplainLayer(){};
    static FightExplainLayer* create();
	virtual bool init();
    
    //加载UI
    void doBaseUI();
    //返回按钮
    void menuCallBackButton(CCNode* r);

    
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
};

#endif /* defined(__BigWinner__FightExplain__) */
