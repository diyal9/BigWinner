//
//  CLPveResultLayer.h
//  BigWinner
//
//  Created by Zhang Qiyun on 13-10-26.
//
//

#ifndef __BigWinner__CLPveResultLayer__
#define __BigWinner__CLPveResultLayer__

#include <iostream>
#include "cocos2d.h"
#include "JsonParser.h"

USING_NS_CC;

//闯关胜利类
class PassPointsSuccess : public cocos2d::CCLayerGradient
{
public:
    PassPointsSuccess();
    ~PassPointsSuccess();
    
	virtual bool init(Json::Value& jv);
    
	static PassPointsSuccess* create(Json::Value& jv);
    void onCloseMenuPress(CCObject* pSender);
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
private:
    
    int m_nStarLevel; //胜利星级
    int m_nCorrentNum; //正确个数
    int m_nMoneyNum; //金额
    int m_nShoppingCardNum; //购物卡数
    int m_ntampID; //印花ID
    std::vector<int> m_vTopicCards; //题卡数
    
    std::map<int, int> m_vTopCardMap; //答题得到题卡
    
    void serverInfoGet(Json::Value& jv); //服务器数据取得
    void ViewStampCall(); //查看印花按钮回调
    void continueChallengeCall(); //继续挑战按钮回调
    
    int nMeYH1; //银银花
    int nMeYH2; //金银花
    
};

/*
 创建BOOS弹出层
 */
class CLPveResultLayer : public CCNode,public CCTargetedTouchDelegate
//public CCTargetedTouchDelegate

{
public:
    CLPveResultLayer(){ intTag = 0; }
    ~CLPveResultLayer(){};
    
    
    //BOOS胜利弹出层
    static CLPveResultLayer* SuccessBoos(Json::Value& jv);
    bool initSuccess(Json::Value& jv);
    
    //失败弹出层
    static CLPveResultLayer* FailBoos(Json::Value& jv);
    bool initFail(Json::Value& jv);
    
    //remove
    void removeCallback();
    
    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    void onEnter();
	void onExit();
    
    void onCloseMenuPress(CCObject* pSender);
    void returnToMain();     //返回主场景
    
    void back();//返回到闯关
    
    void actionPatic(); //烟花粒子效果

private:
    int intTag;
    //public:
    //    Json::Value m_Data;          //此货架答题结束:
    
    void continueCallBack();           //继续挑战店长
    
};

#endif /* defined(__BigWinner__CLPveResultLayer__) */

