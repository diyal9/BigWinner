//
//  CLStampExamineLayer.h
//  BigWinner
//
//  Created by LunaBox on 13-12-22.
//
//

#ifndef __BigWinner__CLStampExamineLayer__
#define __BigWinner__CLStampExamineLayer__

#include <iostream>
#include <cocos2d.h>

#include "cocos-ext.h"
#include "JsonParser.h"
using namespace cocos2d::extension;

USING_NS_CC;

/**************************************************
 * 印花详情
 ***************************************************/
class CLStampExamineLayer : public cocos2d::CCLayerColor,public CCScrollViewDelegate
//, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
    CLStampExamineLayer(){ Number = 0;}
    ~CLStampExamineLayer(){};
    
    static CLStampExamineLayer* create(int nMeyh1,int nMeyh2);
    
	virtual bool init(Json::Value &data,int nMeyh1,int nMeyh2 );
    
    void menuCallBackButton();
    
    virtual void scrollViewDidScroll(CCScrollView* view){}
	virtual void scrollViewDidZoom(CCScrollView* view){}
    
    void getMessageInfo(Json::Value &data,int nMeyh1,int nMeyh2);
    
    //返回主页面
    void closeBtn();
    
    //跳转到主页面
    void JumpMainscene();
    
    //跳转到对战场景
    void JumpFightCene();
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    
private:
    int Number;
    

    
};


#endif /* defined(__BigWinner__CLStampExamineLayer__) */



