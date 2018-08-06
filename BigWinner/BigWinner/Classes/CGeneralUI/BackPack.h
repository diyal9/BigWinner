//
//  BackPack.h
//  BigWinner
//
//  Created by LunaBox03 on 13-10-19.
//
//

#ifndef __BigWinner__BackPack__
#define __BigWinner__BackPack__

#include <iostream>
#include <cocos2d.h>
#include "JsonParser.h"

USING_NS_CC;

enum BACKPACK_TYPE
{
    BK_HECHENG = 1,
    BK_JUESE,
    BK_TIKA,
    BK_DAOJU
};

enum BACKPACK_TAG
{
    BK_HECHENG_TAG = 10,
    BK_JUESE_TAG,
    BK_TIKA_TAG,
    BK_DAOJU_TAG,
    BK_GUANBI_TAG,
    BK_GENERAL_TAG
    
    ,BK_HECHENG_TAG2,
    BK_JUESE_TAG2,
    BK_TIKA_TAG2,
    BK_DAOJU_TAG2

};

class BackPackLayer:public CCLayerColor
{
    
public:
	BackPackLayer():m_indesSprite(NULL),m_nlayerTpye(BK_TIKA)
    {
        m_ntag =0;
    }
	~BackPackLayer(){};
    static BackPackLayer* create(int index, Json::Value &data);
	virtual bool init(int index, Json::Value &data);
    void doBaseUI();
    void menuCallBackButton(CCNode* r);
    void setIndexPosition();
    void addBaseUI();
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
public:
    Json::Value m_bkData;          //背包数据
    CC_SYNTHESIZE(int, m_ntag, ntag);//记录当前选中
protected:
    int m_nlayerTpye;              //当前选择类型
    
    CCSprite* m_indesSprite;      //当前高亮位置精灵
    
public:  //（仅背包内部用，demo版本）
    
    std::vector<int> m_pCardList;   //背包专用卡牌信息（按照服务器端发送的信息顺序）
    
    void insertData(int No); //插入数据
    
    void cardInfoSet(Json::Value &value); //每次将Json的卡牌数据存在里面
    
    std::map<int, int> m_pCardMap;//记录当前题卡数量Map
    
    void cardNumModify(int cardNo, int nowNum); //根据卡No修改Map中数量
    
    int getNumByNo(int No); //通过No从Map中查找数量
    
    void doAction(CCNode* r, bool moveIn); //action执行 (节点，移动方式true为in)

};

#endif /* defined(__BigWinner__BackPack__) */
