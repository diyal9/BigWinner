//
//  BuyMoneyLayer.h
//  BigWinner
//
//  Created by LunaBox on 13-12-28.
//
//

#ifndef __BigWinner__BuyMoneyLayer__
#define __BigWinner__BuyMoneyLayer__

#include <iostream>
#include "cocos2d.h"
#include "JsonParser.h"
#include "cocos-ext.h"

USING_NS_CC;
using namespace cocos2d::extension;

/*
 背包：道具界面
 */

class BuyMoneyLayer : public CCLayerColor, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
    BuyMoneyLayer()
    {
        n_Tag = 0;
        //设置刷新tableview的时，正面是否隐藏
        backfag = true;
        //设置查看按钮是否可用
        btn = true;
        m_BuyData = NULL;
    };
    ~BuyMoneyLayer(){};
    
    static BuyMoneyLayer* create(Json::Value &data);
    
    virtual bool init(Json::Value &data);
    
    void getPipeInfo();
    //关闭按钮
    void menuCallBackButton();
    
    Json::Value m_BuyData;          //背包数据
    
	virtual void scrollViewDidScroll(CCScrollView* view){}
	virtual void scrollViewDidZoom(CCScrollView* view) {}
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
	virtual cocos2d::CCSize cellSizeForTable(CCTableView *table);
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	virtual unsigned int numberOfCellsInTableView(CCTableView *table);
	void createTableViewCell(CCNode *cell,CCTableView *table,int index);
	CCSize getTableSize();
    
    //查看道具信息
    void openView(CCNode* pSender);
    //从道具信息返回到道具界面
    void ClickView(CCNode* pSender,CCNode* pSender2,int tag);
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void doAction(CCNode* r, bool moveIn); //action执行 (节点，移动方式true为in)
    
private:
    CCTableView* tableView;
    CCSprite *pLayer2;
    
private:
    //鼠标点击点的坐标
    CCPoint m_touchPoint;
    
    //记录tag
    int n_Tag ;
    //记录信息卡牌的tag
    std::map<int, int> backMap;
    
    //设置刷新tableview的时，正面是否隐藏
    bool backfag ;
    //设置查看按钮是否可用
    bool btn;
    
};


#endif /* defined(__BigWinner__BuyMoneyLayer__) */
