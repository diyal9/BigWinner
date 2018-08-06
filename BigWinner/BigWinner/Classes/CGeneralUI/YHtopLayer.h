//
//  YHtopLayer.h
//  BigWinner
//
//  Created by LunaBox03 on 13-11-3.
//
//

#ifndef __BigWinner__YHtopLayer__
#define __BigWinner__YHtopLayer__

#include <iostream>
#include <cocos2d.h>

#include "cocos-ext.h"
#include "JsonParser.h"

USING_NS_CC;

//enum YH_TYPE
//{
//    YH_PRIZE = 1, //奖品展柜
//    YH_MESSAGE,//奖品信息
//    YH_DUIHUAN1,//兑换
//    YH_DUIHUAN2,//兑换
//    YH_DUIHUAN3,//兑换
//    YH_SUCCESS//兑换成功
//    
//};
//
//enum YH_TAG
//{
//    YH_PRIZE_TAG = 10, //奖品展柜
//    YH_MESSAGE_TAG,//奖品信息
//    YH_DUIHUAN1_TAG,//兑换
//    YH_DUIHUAN2_TAG,//兑换
//    YH_DUIHUAN3_TAG,//兑换
//    YH_SUCCESS_TAG,//兑换成功
//    YH_GUANBI_TAG,//关闭
//    YH_GENERAL_TAG//默认
//};

class YHtopLayer:public CCLayerColor
{
    
public:
	YHtopLayer()
//    :m_indesSprite(NULL),m_nlayerTpye(SP_TIKA)
    {
    }
	~YHtopLayer(){};
//    static YHtopLayer* create(Json::Value &data);
//	virtual bool init(Json::Value &data);

    static YHtopLayer* create();
	virtual bool init();
    
    void doBaseUI();
//    void menuCallBackButton(CCNode* r);
//    void setIndexPosition();

    
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void doAction(CCNode* r, bool moveIn); //action执行 (节点，移动方式true为in)
    
    
public:
//    int YHCellTag = 0;//保存选择cell
    CC_SYNTHESIZE(int, m_nYHCellTag, nYHCellTag);
    CC_SYNTHESIZE(int, m_nStockOne, nStockOne);
    
protected:
    int m_nlayerTpye;              //当前选择类型
    
    CCSprite* m_indesSprite;      //当前高亮位置精灵
    
};


#endif /* defined(__BigWinner__YHtopLayer__) */
