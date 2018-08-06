//
//  CompoundSceneMain.h
//  DiyalStudy
//
//  Created by YinYanlong on 13-10-13.
//
//

#ifndef __DiyalStudy__CompoundSceneMain__
#define __DiyalStudy__CompoundSceneMain__

#include <iostream>
#include <map>

#include "cocos-ext.h"
#include "Button.h"
#include "RichLabel.h"
#include "JsonParser.h"

USING_NS_CC;


enum {
	TAG_UPCARD_BTN = 0,
    TAG_UPCARD_TTF,
    TAG_UPCARD_STAR,
    TAG_COMMOUND_BTN,
    TAG_CARD_SMALL,
    TAG_LAYER_SUCCESS,
};

//处理优先级
enum {
    tPri_TableView = -10001,
    tPri_Btn = -10002,
    tPri_SuccessLayer = -10003
};

//tableview背景图片
static const char sc_tableview_bg_Path[]       = "Image/BackPack/C_winbg3.png";

//选中题卡背景
static const char sc_cardListBg_bg_Path[]       = "Image/BackPack/C_winbg4.png";

//合成按钮
static const char sc_compoundBtn_Path[]       = "Image/BackPack/C_composite.png";

//大卡牌
static const char sc_card_bg_Path[]       = "Image/BackPack/CardBackground.png";

//星星
static const char sc_star_Path[]       = "Image/BackPack/C_card_star.png";

//小卡片
static const char sc_smallCard_Path[]   =   "Image/BackPack/card_small_temp.png";

//成功星星背景
static const char sc_succStar_Path[]   =   "Image/BackPack/card_compound_pft.png";

//继续按钮
static const char sc_continue_Path[]   =   "Image/BackPack/C_composite.png";

//系统字体
static const char sc_Font_MarkerFelt[]    = "Marker Felt";

class CompoundSceneMain : public cocos2d::CCLayerColor, public cocos2d::extension::CCTableViewDataSource, public cocos2d::extension::CCTableViewDelegate
{
public:
    CompoundSceneMain();
    ~CompoundSceneMain();
    
    virtual bool init();
    
    static cocos2d::CCScene* scene();
    
    void getPipeInfo();
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view){}
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {}
	virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
	virtual cocos2d::CCSize cellSizeForTable(cocos2d::extension::CCTableView *table);
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
	void createTableViewCell(CCNode *cell,cocos2d::extension::CCTableView *table,int index);
	CCSize getTableSize();
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void compoundCardSuccess(); //合成成功后的回调函数
    
	void menuCardBtnAddCall(CCNode *node);// 卡片按钮的回调函数
    
	void menuCardBtnDelCall(CCNode *node); // 卡片移除回调函数
    
    void _CardHeCallBack(Json::Value &data); //合成回调函数
    
    CREATE_FUNC(CompoundSceneMain);
    
public:
    cocos2d::extension::CCTableView* tableView;
    CCSprite* tableViewBg;
private:
    int cardWeight; //单个cell的宽度
    int cardHeight; //单个cell的高度
    
    int lineNum; //cell行数, 奇数的时候UI编辑用
    bool isOddNum; //奇数个题卡标识
    
    int hadAddCardNum;//当前已添加的卡片个数
    CC_SYNTHESIZE(CCSprite*, m_sprList, pSprList);//添加成功列表
    
    CCSprite* starLogicGet(int starNum, CCNode* cardNode); //星星处理
    
    //因为tableview中cell编辑没有规则，所以根据传输过来的数据的索引作为全过程的主Key来标记卡牌
    std::map<int, int> cardTNumMap;//记录当前卡片数量Map
    
    std::map<int, int> cardNoForTagMap;//记录当前卡牌No
    
    std::map<int, int> starLevelMap;//记录当前星级
    
    int tmpCardNum;//总共的卡牌个数
    
	void cardNumGet();// 卡片数取得
    
    bool updateQuantityLabel(CCNode *node, bool isAdd);//数量修改
    
    int xPosionGet(int cellNum);//列表中第N个卡片的坐标计算
    
    int mapKeyGetValue(std::map<int, int> map, int key);//取得Map中的存放的卡片数量
    
    void modifyMap(bool isAddNum, int key);//减去数据
    
//    void showMap();//测试用
    
    CCPoint m_tPoint; //触摸点
    
    bool touchSprite(CCSprite* sprite); //是否点击精灵
    
};

class CompoundSucLayer : public cocos2d::CCLayerColor
{
public:
    CompoundSucLayer(){};
    ~CompoundSucLayer(){
        
    };
    static CompoundSucLayer* create(int succCardNo, int succStarLevel);
    bool init(int succCardNo, int succStarLevel);
    void continueCallBack(CCNode *node);
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
};

#endif /* defined(__DiyalStudy__CompoundSceneMain__) */
