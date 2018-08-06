#ifndef __test__FightPropsUI__
#define __test__FightPropsUI__

#include <iostream>
#include "cocos2d.h"

#include "cocos-ext.h"

USING_NS_CC;
using namespace cocos2d::extension;

/*
 背包：道具界面
 */

class FightPropsUI : public CCLayerColor, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
    FightPropsUI(){
        b_Useflg = true;
        b_Useflg2 = false;
        n_Tag = 0;
        backfag = true;
        fagR = true;
        btn = true;
    };
    ~FightPropsUI(){
    
    };
    CREATE_FUNC(FightPropsUI);
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    void getPipeInfo();
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
    
    //去掉悬浮层
    void removeUI();
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
private:
    CCTableView* tableView;
    CCSprite *pLayer2;
    
private:
    //鼠标点击点的坐标
    CCPoint m_touchPoint;
    bool b_Useflg ;
    bool b_Useflg2;
    //记录tag
    int n_Tag;
    
    //添加使用中信息
    std::map<int, int> useMap;
    //记录信息卡牌的tag
    std::map<int, int> backMap;
    
    //设置刷新tableview的时，正面是否隐藏
    bool backfag ;
    //设置弹出层不可同时存在两个
    bool fagR;
    
    //设置查看按钮是否可用
    bool btn;
};

#endif /* defined(__test__PropsUI__) */
