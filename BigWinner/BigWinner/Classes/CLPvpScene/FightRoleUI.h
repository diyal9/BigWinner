#ifndef __test__FightRoleUI__
#define __test__FightRoleUI__

#include <iostream>
#include "cocos2d.h"

#include "cocos-ext.h"

USING_NS_CC;
using namespace cocos2d::extension;

// 触摸误差
const int TOUCH_DELTA = 5;

/*
 背包：角色界面
 */
class FightRoleUI : public CCLayerColor, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
    FightRoleUI()
    {
        //记录tag
        n_Tag = 0;
        //设置刷新tableview的时，正面是否隐藏
        backfag = true;
        //设置弹出层不可同时存在两个
        fagR = true;
        //wangguolong
        b_Useflg = true;
        b_Useflg2 = false;
        //设置查看按钮是否可用
        btn = true;
    };
    ~FightRoleUI(){};
    CREATE_FUNC(FightRoleUI);
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
//
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
//    //悬浮提示层
//    void tiShiView();
    
    //去掉悬浮层
    void removeUI();

private:
    CCTableView* tableView;
    CCSprite *pLayer2;
    
    //记录tag
    int n_Tag;
    
    //添加使用中信息
    std::map<int, int> useingMap;
    //记录信息卡牌的tag
    std::map<int, int> backMap;
    
    //设置刷新tableview的时，正面是否隐藏
    bool backfag;
    //设置弹出层不可同时存在两个
    bool fagR;
    
    //wangguolong
    bool b_Useflg;
    bool b_Useflg2;
    
    std::map<int, int> useMap;

    
private:

    // 记录触摸起始点的位置
    CCPoint m_touchPoint;
    
    //设置查看按钮是否可用
    bool btn;
    
};


#endif /* defined(__test__FightRoleUI__) */
