//
//  CLLoadingLayer.h
//  Yy_mobileGame3
//
//  Created by Zhang Qiyun on 13-6-26.
//
//

#ifndef __Yy_mobileGame3__CLLoadingLayer__
#define __Yy_mobileGame3__CLLoadingLayer__

#include "cocos2d.h"
USING_NS_CC;

class CLLoadingLayer : public CCLayerColor
{
public:
    CLLoadingLayer();
    ~CLLoadingLayer();
    
    static CLLoadingLayer* create(float fwidth, float fheight);
    
    bool init(float fwidth, float fheight);
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void setVisible(bool visible);
    
    void QQMoveFrame();
    void onDoorCloseEnd();
    void onDoorOpenEnd();
    
    CC_SYNTHESIZE(int, m_nCurIndex, nCurIndex);
    CC_SYNTHESIZE(CCSprite*, m_pLeftDoor, pLeftDoor);
    CC_SYNTHESIZE(CCSprite*, m_pRightDoor, pRightDoor);
    CC_SYNTHESIZE(CCLabelTTF*, m_pLoadTTF, pLoadTTF);
    
private:
    std::vector<CCSprite*> m_vQQList;
};

#endif /* defined(__Yy_mobileGame3__CLLoadingLayer__) */
