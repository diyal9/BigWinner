//
//  BackPack.h
//  BigWinner
//
//  Created by LunaBox03 on 13-10-19.
//
//

#ifndef __BigWinner__CLPvpReady__
#define __BigWinner__CLPvpReady__

#include <iostream>
#include <cocos2d.h>
#include "JsonParser.h"
#include "StampFightSelected.h"
#include "CLPvpWaitRivalLayer.h"

USING_NS_CC;

enum CLPvpReady_TYPE
{
    PVP_DEFAULT = -1,    //默认 什么都不选
    PVP_TUANZHAN = 10,   //团战按钮
    PVP_PAIHANG,         //排行按钮
    PVP_DESC,            //对战说明
    PVP_SETTING          //对战设置
};

enum CLPvpReady_TAG
{
    PVP_TUANZHAN_TAG = 10,
    PVP_PAIHANG_TAG,
    PVP_HELP_TAG,
    PVP_SETTING_TAG,
    PVP_GUANBI_TAG,
    PVP_GENERAL_TAG
    ,PVP_TUANZHAN_TAG2 = 100,
    PVP_PAIHANG_TAG2,
    PVP_HELP_TAG2,
    PVP_SETTING_TAG2
};

class CLPvpReady:public CCLayerColor
{
    
public:
	CLPvpReady();
	~CLPvpReady();
    static CLPvpReady* create();
	virtual bool init();
    void doBaseUI();
    void menuCallBackButton(CCNode* r);
    void setIndexPosition(int nIndex);
    void addBaseUI();
    
    void ReturnRoomSelLayer();  //选择回到房间选择层
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void ShowWaitRivalLayer(bool b); // 是否生成匹配界面
    void WaitRivalSuccess(Json::Value& jv);
    
    CC_SYNTHESIZE(CCNode*, m_pCenterNode, pCenterNode);   //中央显示区模块
    CC_SYNTHESIZE(CLPvpWaitRivalLayer*, m_pWaitingLayer, pWaitingLayer);
    CC_SYNTHESIZE(CCNode*, m_pBluePitchNode, pGreenPitchNode);   //下方蓝色按钮
    
    CC_SYNTHESIZE(int, m_ntag, ntag);//记录当前选中
    
protected:
    int m_nlayerTpye;              //当前选择类型
    
    CCSprite* m_indesSprite;      //当前高亮位置精灵
    
};

#endif /* defined(__BigWinner__BackPack__) */
