#ifndef __FightQUESTIONCARDUI__
#define __FightQUESTIONCARDUI__

#include <iostream>
#include "cocos2d.h"

#include "cocos-ext.h"

USING_NS_CC;
using namespace cocos2d::extension;

/*
 背包：题卡界面
 */
class FightQuestionCardUI : public CCLayerColor, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
    FightQuestionCardUI()
    {
        b_Useflg = true;
        b_Useflg2 = false;
        n_Tag = 0;
        fagR = true;
    };
    ~FightQuestionCardUI(){};
    CREATE_FUNC(FightQuestionCardUI);
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
    //鼠标点击点的坐标
    CCPoint m_touchPoint;
    bool b_Useflg;
    bool b_Useflg2;
    int n_Tag;

    //添加使用中信息
    std::map<int, int> useMap;
    
    //设置弹出层不可同时存在两个
    bool fagR;

};



#endif /* defined(__FightQUESTIONCARDUI__) */

