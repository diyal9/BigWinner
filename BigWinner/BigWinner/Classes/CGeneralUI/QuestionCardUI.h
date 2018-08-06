#ifndef __QUESTIONCARDUI__
#define __QUESTIONCARDUI__

#include <iostream>
#include "cocos2d.h"

#include "cocos-ext.h"

USING_NS_CC;
using namespace cocos2d::extension;

/*
 背包：题卡界面
 */
class QuestionCardUI : public CCLayerColor, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
    QuestionCardUI()
    {
        m_layerColor = NULL;
    };
    ~QuestionCardUI(){};
    CREATE_FUNC(QuestionCardUI);
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
    
    
    void ToShop();
private:
    //tableView
    CC_SYNTHESIZE(CCTableView*, m_tableView, tableView);
    //sprite
    CC_SYNTHESIZE(CCSprite*, m_pLayer, pLayer);
    //数量为0时的弹出层
    CC_SYNTHESIZE(CCLayerColor*, m_layerColor, layerColor);

};



#endif /* defined(__QUESTIONCARDUI__) */

