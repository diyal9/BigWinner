#ifndef __test__SpPropsUI__
#define __test__SpPropsUI__

#include <iostream>
#include "cocos2d.h"

#include "cocos-ext.h"

USING_NS_CC;
using namespace cocos2d::extension;

/*
 背包：道具界面
 */

class SpPropsUI : public CCLayerColor, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
    SpPropsUI()
    {
        //记录tag
        n_Tag = 0;
        //设置刷新tableview的时，正面是否隐藏
        backfag = true;
        //设置查看按钮是否可用
        btn = true;
    };
    ~SpPropsUI(){};
    CREATE_FUNC(SpPropsUI);
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
    //记录tag
    int n_Tag;

    //记录信息卡牌的tag
    std::map<int, int> backMap;
    //设置刷新tableview的时，正面是否隐藏
    bool backfag;
    //设置查看按钮是否可用
    bool btn;

};

#endif /* defined(__test__SpPropsUI__) */
