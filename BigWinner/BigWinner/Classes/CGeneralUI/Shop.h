//
//  Shop.h
//  BigWinner
//
//  Created by LunaBox03 on 13-11-01.
//
//

#ifndef __BigWinner__Shop__
#define __BigWinner__Shop__

#include <iostream>
#include <cocos2d.h>

#include "cocos-ext.h"
#include "JsonParser.h"

USING_NS_CC;

enum SHOP_TYPE
{
    SP_TIKA = 1,
    SP_HUOBI,
    SP_JUESE,
    SP_DAOJU

};

enum SHOP_TAG
{
    SP_TIKA_TAG = 10, //题卡
    SP_HUOBI_TAG,//货币
    SP_JUESE_TAG,//角色
    SP_DAOJU_TAG,//道具
    SP_GUANBI_TAG,//关闭
    SP_GENERAL_TAG//默认
    
    ,SP_TIKA_TAG2 , //题卡
    SP_HUOBI_TAG2,//货币
    SP_JUESE_TAG2,//角色
    SP_DAOJU_TAG2,//道具
};


#define SHOPBUYPALYER_TAG 1000000    //商店购买层tag，调用购买弹出层的时候都必须使用这个
#define BASICTOUCHPRO -10005    //商店购买层tag，调用购买弹出层的时候都必须使用这个

#define SCROLLITEMTAG 2  
#define SCROLLITEMUPTAG 3  //上面部分TAG
#define SCROLLITEMCENTAG 4  //中间部分TAG
#define SCROLLITEMDOWNTAG 5  //下面部分TAG


class ShopLayer:public CCLayerColor
{
    
public:
	ShopLayer():m_indesSprite(NULL),m_nlayerTpye(SP_TIKA)
    {
        m_ntag =0;
    }
	~ShopLayer(){};
    static ShopLayer* create(int index, Json::Value &data);
	virtual bool init(int index, Json::Value &data);
    void doBaseUI();
    void menuCallBackButton(CCNode* r);
    void setIndexPosition();
    void addBaseUI();
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    Json::Value m_bkData;          //背包数据
    
    //    int tmpMenuFlg = 0;    //记录当前选中的是那一个菜单（网络回调使用）
    void  _netCallBack();  //网络回调
    
    //记录购买了那张人物卡
    std::map<int, int> BuyRoleNoMap;
    CC_SYNTHESIZE(int, m_nRoleNo, nRoleNo);//记录当前选中
    
    void doAction(CCNode* r, bool moveIn); //action执行 (节点，移动方式true为in)
    
private:
    CC_SYNTHESIZE(int, m_ntag, ntag);//记录当前选中
    
protected:
    int m_nlayerTpye;              //当前选择类型
    
    CCSprite* m_indesSprite;      //当前高亮位置精灵
    
};



//购买弹出层
class ShopBuyLayer : public cocos2d::CCLayerColor
{
public:
    ShopBuyLayer(){
    pCardId = 0;
    pStarLevel = 0;
    pCostType = 0;
    pUnitPrice = 0;
    pCanChangeNum = true;
    buyCardNum = 0;
    scollNumMax = 0;
    userCurrentMoney = 0;
    director = true;
    }
    ~ShopBuyLayer(){};
    
    static ShopBuyLayer* createLayer(int cardId,
                                     int costType, //购买类型
                                     int unitPrice, //单价
                                     bool canChangeNum //是否可以选择购买个数
                                     ,int buyMaxNum //购买最大数量
    ,std::string shop
                                     );
    
    
	virtual bool init(int cardId,
                       int costType, //购买类型
                       int unitPrice, //单价
                       bool canChangeNum //是否可以选择购买个数
                      ,int buyMaxNum //购买最大数量
    ,std::string shop
                       );
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    void createInitUI(); //初始化UI

    CC_SYNTHESIZE(CCLabelTTF *, m_pScollNumSprc, pScollNumSprc);  //数字选择控件中间
    CC_SYNTHESIZE(CCLabelTTF *, m_pSumCostLabel, pSumCostLabel);  //总花费
    
private:
    int pCardId; //卡牌Id
    std::string pCardPath; //商品图片路径
    int pStarLevel; //商品星级
    std::string pBuyTypePath; //购买类型图片路径
    int pCostType; //购买类型
    int pUnitPrice; //单价
    bool pCanChangeNum; //是否可以改变购买数量
    int buyCardNum; //购买数量
  
    void canceBtnCall(); //取消回调
    void confirmBtnCall(); //确认回调
    
    CC_SYNTHESIZE(std::string, m_sCardName_path, sCardName_path);//20131126
    
    
    //
    float touchPointPos; //触摸起始点
    float moveDistance; //移动距离
    CCRect labelRect; //滑动合理范围
    int scollNumMax; //滑动数字的最大值
    bool director; //滑动方向
    
    int userCurrentMoney;
    
    void plusBtnCall(); //加回调
    void subBtnCall(); //减回调
    
    void doBuyAction(CCNode* r, bool moveIn); //action执行
    
};

class BuySucLayer : public cocos2d::CCLayerColor
{
public:
    BuySucLayer(){};
    ~BuySucLayer(){
        
    };
    static BuySucLayer* create(int succCardNo, int succStarLevel);
    bool init(int succCardNo, int succStarLevel);
    void continueCallBack(CCNode *node);
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
};

/*
 购买的商品展示
 */
class ShowGoodsLayer : public cocos2d::CCLayerColor
{
public:
    ShowGoodsLayer(){};
    ~ShowGoodsLayer(){
        
    };
    static ShowGoodsLayer* create(int No);
    bool init(int No);
    
    void CallBack();//关闭
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
};

#endif /* defined(__BigWinner__Shop__) */
