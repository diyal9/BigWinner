//
//  CLPvpWaitRivalLayer.h
//  BigWinner
//
//  Created by Zhang Qiyun on 13-11-13.
//
//

#ifndef __BigWinner__CLPvpWaitRivalLayer__
#define __BigWinner__CLPvpWaitRivalLayer__

#include "cocos2d.h"
#include "CLStrokeLabelTTF.h"
#include "JsonParser.h"
USING_NS_CC;

class CLPvpWaitRivalLayer : public CCLayerColor
{
public:
    CLPvpWaitRivalLayer();
    ~CLPvpWaitRivalLayer();
    
    CREATE_FUNC(CLPvpWaitRivalLayer);
    
    virtual bool init();
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    CC_SYNTHESIZE(CLStrokeLabelTTF*, m_pLabelTTF, pLabelTTF);
    CC_SYNTHESIZE(CCSprite*, m_magic, magic);
    
    void WaitRivalSuccess(Json::Value& jv);
    void onInfoPanelMoved();
    void BeginToPvping();
    
};

#endif /* defined(__BigWinner__CLPvpWaitRivalLayer__) */
