//
//  Achievements.h
//  BigWinner
//
//  Created by YinYanlong on 13-11-13.
//
//

#ifndef __BigWinner__Achievements__
#define __BigWinner__Achievements__

#include <iostream>
#include <cocos2d.h>
#include "cocos-ext.h"
#include "JsonParser.h"
#include "CLTopicData.h"

USING_NS_CC;

class CLAchievements: public cocos2d::CCLayerColor, public cocos2d::extension::CCTableViewDataSource, public cocos2d::extension::CCTableViewDelegate
{
    
public:
	CLAchievements();
	~CLAchievements();
    static CLAchievements* create(Json::Value &data);
	virtual bool init(Json::Value &data);
    void doBaseUI();
    void menuCallBackButton(CCNode* r);

private:
    
    Json::Value m_AchiData;          //数据
    
    CC_SYNTHESIZE(cocos2d::CCSprite*, m_pTabViewBgPic, pTabViewBgPic); //tableview底图
    
    CC_SYNTHESIZE(cocos2d::extension::CCTableView*, m_pTabView, pTabView); //tableview
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    //Table view infomations
    void getPipeInfo();
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view){}
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {}
	virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
	virtual cocos2d::CCSize cellSizeForTable(cocos2d::extension::CCTableView *table);
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
	void createTableViewCell(CCNode *cell,cocos2d::extension::CCTableView *table,int index);
	CCSize getTableSize();
    
    std::map<std::string, int> rightNumByClassMap; //记录每种题目答对多少

    std::vector<int> notGetMap; //记录没有领取的成就No
    
    bool hadNotGetted(std::vector<int> vect, int key); //判断是否可以领取（true： 没有领取）
    
    int mapKeyGetValue(std::map<std::string, int> map, std::string);//判断是否可以领取（true： 没有领取）
    
    void _getSuccessBack(Json::Value& jv); //兑换成功后的回调
    
    void doAction(CCNode* r, bool moveIn); //action执行 (节点，移动方式true为in)
    
    //action
    void doActionOne(int no);
    void doActionTwo();
    Json::Value m_getAfter; //领取后json
};



//成就显示cell类
class CLAchievementCell : public CCSprite
{
public:
    CLAchievementCell();
    ~CLAchievementCell();
    
    static CLAchievementCell* create(int nIndex, int achiNo, int nowAchiNum, bool aflg); //cellIndex，成就No，当前成就值， 是否兑奖
    bool init(int nIndex, int achiNo, int nowAchiNum, bool aflg);
    
    CC_SYNTHESIZE(AchievementBean*, m_pAchieveInfo, pAchieveInfo);
    
    void menuCallBackGet(CCNode* r);
};

//主场景成就达成Layer
class CLAchiReachLayer : public CCLayerColor
{
public:
    CLAchiReachLayer();
    ~CLAchiReachLayer();
    
    static CLAchiReachLayer* create();
    
    bool init();
    
    void UICreate();
    
    void closeMenuPress();    //关闭按钮回调

    
    virtual void onEnter();
    virtual void onExit();
    
    int iAcniNo;
    
    CC_SYNTHESIZE(CCSprite*, m_pWriteBg, pWriteBg);
    
    std::vector<int> m_vShowMsgVect; //显示消息列表
    int nowShowIndex; //显示成就索引

    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void onGetPress(CCObject* pSender);  //确认按钮回调
    void deleteFromParent(); //取消按钮回调
    
    void _getAchiSucc(Json::Value& jv);    //领取成功回调
    
    void doActionOne();
    void doActionTwo();
    Json::Value m_AData;
    
};


#endif /* defined(__BigWinner__Achievements__) */
