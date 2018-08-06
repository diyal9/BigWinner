//
//  CLPveSMangerLayer.h
//  BigWinner
//
//  Created by diyal yin on 13-12-19.
//
//

#ifndef __BigWinner__CLPveSMangerLayer__
#define __BigWinner__CLPveSMangerLayer__

#include "cocos2d.h"
USING_NS_CC;

class CLPveSMangerLayer : public CCLayerColor
{
public:
    CLPveSMangerLayer();
    ~CLPveSMangerLayer();
    
//    CREATE_FUNC(CLPveSMangerLayer);
//    
//    virtual bool init();
    
    static CLPveSMangerLayer* create(int iShelfNo, int iStatusNo);
    
    bool init(int iShelfNo, int iStatusNo);
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void onOKMenuPress(CCObject* pSender);  //确认按钮回调
    void onCanceMenuPress(CCObject* pSender); //取消按钮回调

    
private:
    CC_SYNTHESIZE(int, m_iShelfNo, iShelfNo);//当前店长No
    
    CC_SYNTHESIZE(int, m_iStatusNo, iStatusNo);//内容确认 （1: 冷却消除对话框， 2:店长送礼对话框 3:失败询问对话框）

};


#endif /* defined(__BigWinner__CLPveSMangerLayer__) */
