//
//  YHUILayer.h
//  BigWinner
//
//  Created by LunaBox03 on 13-11-3.
//
//

#ifndef __BigWinner__YHUILayer__
#define __BigWinner__YHUILayer__

#include <iostream>
#include "cocos2d.h"

#include "cocos-ext.h"
#include "JsonParser.h"
#include "MLTextFieldTTFLayer.h"

USING_NS_CC;
using namespace cocos2d::extension;

/*
 印花：商品展柜
 */
class PrizeShow : public CCLayerColor, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
    static PrizeShow* createPShow();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool initShow();
    
    void getPipeInfo();
	virtual void scrollViewDidScroll(CCScrollView* view){}
	virtual void scrollViewDidZoom(CCScrollView* view) {}
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
	virtual cocos2d::CCSize cellSizeForTable(CCTableView *table);
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	virtual unsigned int numberOfCellsInTableView(CCTableView *table);
	void createTableViewCell(CCNode *cell,CCTableView *table,int index);
	CCSize getTableSize();
    
    //打开商品信息页面
    void openMessageUI(int YHNo,int tag);
    
    //查看按钮点下 ,取得No
    void getNo(CCNode* pSender);
    
    //打开兑换页面
    void openExchangeUI(CCNode* pSender);
    
    void menuCallBackButton(CCNode* r);
    
    void doAction(CCNode* r); //action执行 (节点，移动方式true为in)
    
public:
    Json::Value m_YHData;          //印花数据
    
    //纪录不同可兑换的商品的No和数量
    std::map<int, int> exchangMap;
    
private:
    CCTableView* tableView;
    CCSprite *pLayer2;
    //    int tag = 0;
    
};

/**************************************************
 * 印花详情
 ***************************************************/
class PrizeMessage : public cocos2d::CCLayerColor,public CCScrollViewDelegate
//, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
    PrizeMessage(){ Number = 0; }
    ~PrizeMessage(){};
    
    static PrizeMessage* create(Json::Value &data);
    
    //    CREATE_FUNC(PrizeMessage);
    //    static PrizeMessage* createMessage(Json::Value& jv);
	virtual bool init(Json::Value &data);
    
    void menuCallBackButton();
    
    virtual void scrollViewDidScroll(CCScrollView* view){}
	virtual void scrollViewDidZoom(CCScrollView* view){}
    
    void getMessageInfo(Json::Value &data);
    
    //返回主页面
    void backTOPrize();
    
    //打开兑换页面
    void openExchangeUI(CCNode* pSender);
    
    //跳转到主页面
    void JumpMainscene();
    
    //跳转到对战场景
    void JumpFightCene();
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
private:
    int Number;
    
};


/**************************************************
 * 兑换类
 ***************************************************/
enum EXCHANGE_TAG
{
    EXCHANGE_TAG_MAIN = 1,
    EXCHANGE_TAG_INPUT,
    EXCHANGE_TAG_CONFIRM
};

class GoodsExchangeMain : public cocos2d::CCLayerColor
{
public:
    GoodsExchangeMain(){
        maxStampNum = 0;
        oneCostGoldStamp = 0;
        oneCostSilverStampNum = 0;
        pCanExchangeNum = 0;
        pCostGoldStampNum = 0;
        pCostSilverStampNum = 0;
        isChecked = true;
        hadPhoneNo = false;
        m_pInputLabel = NULL;
        isTouchWrite = false;
    }
    ~GoodsExchangeMain(){};
    
    static GoodsExchangeMain* create(int goodsID, int canExchangeNum); //商品ID， 可兑换数量
    bool init(int goodsID, int canExchangeNum);
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
private:
    void defaultUI(); //默认固定UI
    
    void variableUI(); //可变动UI设置
    
    void menuCallBackButton(CCNode* r); //返回按钮回调
    
    CC_SYNTHESIZE(CCSprite *, m_pBasicBg, pBasicBg); //白板背景，兑换所有UI内容都放在上面
    
    void MoveLayerUp(CCNode*, void* value); //IME弹出后层的移动
    
    //记忆前面一个画面类型
    int frameFlgForNote; //（1：兑换画面（号码有） 2:兑换画面（号码无）3:更换号码发送手机画面 4:输入验证码画面 5:兑换成功）
    
private: //basic兑换画面相关
    
    bool hadPhoneNo; //能否取到用户手机号
    std::string pPhoneNo; //手机号码
    
    //服务器参数保存用
    int stampNo; //印花No
    int maxStampNum; //可兑换数量
    int oneCostGoldStamp; //单个商品消耗金印花数
    int oneCostSilverStampNum; //单个商品消耗银印花数
    
    //选择类容临时变量（作用于整个兑换场景）
    int pCanExchangeNum; //选择的可兑换数量
    int pCostGoldStampNum; //选择对应的金印花数量
    int pCostSilverStampNum; //选择对应的银印花数量
    
    void plusBtnCallback(CCNode* r);  //加按钮回调
    void reduceBtnCallback(CCNode* r);//减按钮回调
    
    CC_SYNTHESIZE(CCLabelTTF *, m_pCanExchange, pCanExchange); //可兑换数量
    CC_SYNTHESIZE(CCLabelTTF *, m_pCostGoldStamp, pCostGoldStamp);  //消耗金银花数量
    CC_SYNTHESIZE(CCLabelTTF *, m_pCostSilverStamp, pCostSilverStamp);  //消耗银银花数量
    
    CC_SYNTHESIZE(MLTextFieldTTFLayer *, m_pInputLabel, pInputLabel); //手机号码输入框（兑换无手机号码页面时输入框）
    
    void SendPhoneCallBack(CCNode* r);  //发送手机号回调（无手机号画面）
    
    void exchangeSure(CCNode* r);  //确认兑换回调
    
    //wangguolong
    //更新奖品展柜数据
    void setPrizeList(int No,int leavenum);
    //兑换商品个数
    CC_SYNTHESIZE(int, m_nStockNum, nStockNum);
    
    //判断是否数字
    bool isPhoneNum(std::string phone);
    
    void checkbox(CCNode* r); //复选按钮回调
    
    bool isChecked; //checkbox选中
    
    bool isTouchWrite;
    
private: //验证码输入相关
    
    CC_SYNTHESIZE(MLTextFieldTTFLayer *, m_pCodeInputLabel, pCodeInputLabel); //验证码输入框
    
    void backForwardCallBack(CCNode* r);  //返回回调
    
    void SendCodeConfirmCall(CCNode* r);  //确认验证码回调
    
    void CodeCheckUI(std::string verificationCode);  //验证码验证UI
    
private:  //更换手机号相关
    void changePhone(CCNode* r);  //更换号码回调
    void SendTmpPhoneCallBack(CCNode* r);  //发送临时手机回调（更该画面）
    CC_SYNTHESIZE(MLTextFieldTTFLayer *, m_pInputForChange, pInputForChange); //更换号码流程的验证手机号输入框
    void SendPhoneBack(CCNode* r);  //发送验证码回调
    
public: //服务器返回回调
    void _sendBackVerificationCode(std::string verificationCode, std::string phoneNo);//发送手机号后，服务器回馈后的回调
    
    void _sendCodeBack(std::string verificationCode, std::string phoneNo);//发送验证码后，服务器回馈后的回调
    
    void _sureExchangeSuccess(int No,int leavenum);

};

//兑换成功弹出层
class ExchangeSuccLayer : public cocos2d::CCLayerColor
{
public:
    ExchangeSuccLayer(){};
    ~ExchangeSuccLayer(){};
    
    static ExchangeSuccLayer* createLayer(std::string cardPath); //商品图片路径
    
	virtual bool init(std::string cardPath);
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
private:
    void createInitUI(); //初始化UI
    
    void okBackButton(CCNode* r); //确认按钮回调
    
};

#endif /* defined(__BigWinner__YHUILayer__) */
