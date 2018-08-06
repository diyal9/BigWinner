#ifndef __SpQuestionCardUI__
#define __SpQuestionCardUI__

#include <iostream>
#include "cocos2d.h"

#include "cocos-ext.h"

USING_NS_CC;
using namespace cocos2d::extension;

//const int ZERO = 5;

/*
 背包：题卡界面
 */
class SpQuestionCardUI : public CCLayerColor, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
    SpQuestionCardUI()
    {
        n_Tag = 0;
    }
	~SpQuestionCardUI(){};
//    static SpQuestionCardUI* create(Json::Value &data);
//	virtual bool init(Json::Value &data);
    
    CREATE_FUNC(SpQuestionCardUI);
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
    
private:
    CCTableView* tableView;
    CCSprite *pLayer2;
    //鼠标点击点的坐标
    CCPoint m_touchPoint;
    int n_Tag ;
    
};



#endif /* defined(__SpQuestionCardUI__) */

