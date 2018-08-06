//
//  BillboardLayer.h
//  BigWinner
//
//  Created by LunaBox on 14-1-3.
//
//

#ifndef __BigWinner__BillboardLayer__
#define __BigWinner__BillboardLayer__

#include <iostream>
#include "cocos2d.h"

#include "cocos-ext.h"

USING_NS_CC;
using namespace cocos2d::extension;

/*
 背包：题卡界面
 */
class BillboardLayer : public CCLayerColor, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
    BillboardLayer()
    {
        m_pLayer = NULL;
        m_pBillboard= NULL;
        m_touchBPoint = CCPoint(CCPointZero);
        m_touchEPoint = CCPoint(CCPointZero);
        
    };
    ~BillboardLayer(){};
    CREATE_FUNC(BillboardLayer);
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
    
    void menuCallBackButton(CCNode* r);
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
private:
    //tableView
    CC_SYNTHESIZE(CCTableView*, m_tableView, tableView);
    //sprite
    CC_SYNTHESIZE(CCScale9Sprite*, m_pLayer, pLayer);
    
    CC_SYNTHESIZE(CCSprite* , m_pBillboard, pBillboard);
    
    CC_SYNTHESIZE(CCPoint , m_touchBPoint, touchBPoint);
    
    CC_SYNTHESIZE(CCPoint , m_touchEPoint, touchEPoint);
    
};

#endif /* defined(__BigWinner__BillboardLayer__) */
