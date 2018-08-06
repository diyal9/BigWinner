//
//  CLPvpResultLayer.h
//  BigWinner
//
//  Created by YinYanlong on 13-11-27.
//
//

#ifndef __BigWinner__CLPvpResultLayer__
#define __BigWinner__CLPvpResultLayer__

#include <iostream>
#include "cocos2d.h"
#include "JsonParser.h"

USING_NS_CC;

class CLPvpSuccLayer : public cocos2d::CCLayerColor
{
public:
    CLPvpSuccLayer();
    ~CLPvpSuccLayer();
    
    static CLPvpSuccLayer* create(Json::Value& jv, std::string rivalName);
	virtual bool init(Json::Value& jv, std::string rivalName);
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
   
    CC_SYNTHESIZE(int, m_nYh1total, nYh1total); //拥有的银印花
    CC_SYNTHESIZE(int, m_nYh2total, nYh2total); //拥有的金印花
    
private:
    void ViewStampCall(); //查看印花按钮回调
    void continueChallengeCall(); //继续挑战按钮回调
    void onCloseMenuPress(CCObject* pSender); //返回大厅
    
    bool isSuccess; //对战成功
    
};

#endif /* defined(__BigWinner__CLPvpResultLayer__) */
