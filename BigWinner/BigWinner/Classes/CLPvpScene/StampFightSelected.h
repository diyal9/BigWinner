//
//  StampFightSelected.h
//  DiyalStudy
//
//  Created by YinYanlong on 13-10-27.
//
//

#ifndef __DiyalStudy__StampFightSelected__
#define __DiyalStudy__StampFightSelected__

#include <iostream>
#include "JsonParser.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "CLTopicData.h"

USING_NS_CC;

enum {
    TAG_CELL_BG = 0,
};

/**************************************************************************************
 * 对战房间cell创建类
 **************************************************************************************/
class CLPvpRoomShowCell : public CCSprite
{
public:
    CLPvpRoomShowCell();
    ~CLPvpRoomShowCell();
    
    static CLPvpRoomShowCell* create(int nIndex);
    bool init(int nIndex);
    
    CC_SYNTHESIZE(PvpRoomInfo*, m_pPvpRoomInfo, pPvpRoomInfo); //房间模板
    
private:
    void canSelectCellCreate(int index); //能对战文字内容创建
    void canNotSelectCellCreate(int index); //尚未发放文字内容创建
    
};


/**************************************************************************************
 * 对战房间创建类
 **************************************************************************************/
class StampFightSelected : public cocos2d::CCLayerColor, public cocos2d::extension::CCTableViewDataSource, public cocos2d::extension::CCTableViewDelegate
{
public:
	StampFightSelected();
	virtual ~StampFightSelected();
    
	virtual bool init();
    
    void getPipeInfo();
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view){}
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {}
	virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
	virtual cocos2d::CCSize cellSizeForTable(cocos2d::extension::CCTableView *table);
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
	void createTableViewCell(CCNode *cell,cocos2d::extension::CCTableView *table,int index);
	CCSize getTableSize();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    void menuCallBackButton(CCNode* r); //关闭按钮回调
    void AdvImageLoadEnd();
    
	CREATE_FUNC(StampFightSelected);
    
private:
    
    CC_SYNTHESIZE(cocos2d::CCSprite*, m_pTabViewBgPic, pTabViewBgPic); //tableview底图
    
    CC_SYNTHESIZE(cocos2d::extension::CCTableView*, m_pTabView, pTabView); //tableview
    
    void createUI();
    
    int stampCellheight;

};


/**************************************************************************************
 * 排行榜
 **************************************************************************************/
class RankingList : public cocos2d::CCLayerColor, public cocos2d::extension::CCTableViewDataSource, public cocos2d::extension::CCTableViewDelegate
{
public:
	RankingList();
	virtual ~RankingList();
    
    static RankingList* create(Json::Value &data);
    bool init(Json::Value &data);
    
    void getPipeInfo();
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view){}
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {}
	virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
	virtual cocos2d::CCSize cellSizeForTable(cocos2d::extension::CCTableView *table);
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
	void createTableViewCell(CCLayerColor *cell,cocos2d::extension::CCTableView *table,int index);
	CCSize getTableSize();

private:
    
    CC_SYNTHESIZE(cocos2d::CCSprite*, m_pTabViewBgPic, pTabViewBgPic); //tableview底图
    
    CC_SYNTHESIZE(cocos2d::extension::CCTableView*, m_pTabView, pTabView); //tableview
    
    Json::Value m_RankData; //数据
    
    void createUI();
    
    void selfInfoListUI(); //自排名UI
    
    void titleInfoUI(); //titleUI编辑
    
    int selfInfoSum; //根据自己排名情况，判断取后几条信息
    
    int cellheight; //每个cell的高度
    
    void closeCallBackBtn(CCNode* r); //关闭按钮回调
    
};

#endif /* defined(__DiyalStudy__StampFightSelected__) */
