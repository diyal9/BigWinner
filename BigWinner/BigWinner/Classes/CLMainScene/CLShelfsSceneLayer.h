//
//  CLShelfsLayer.h
//  BigWinner
//
//  Created by ZhangQiyun on 13-10-21.
//
//

#ifndef __BigWinner__CLShelfsLayer__
#define __BigWinner__CLShelfsLayer__

#include "cocos2d.h"
#include "LGameCommonDefine.h"
#include "CLStoreShelfData.h"
#include "CCScale9Sprite.h"

using namespace cocos2d::extension;
USING_NS_CC;

//每一个货架的显示类
class CLShelfPanel : public CCLayer
{
public:
    CLShelfPanel();
    ~CLShelfPanel();
    
    static CLShelfPanel* create(Json::Value& jv, CCNode *normalSprite, CCNode *selectedSprite, CCNode *disabledSprite, CCObject *target, SEL_MenuHandler selector);
    bool init(Json::Value& jv, CCNode *normalSprite, CCNode *selectedSprite, CCNode *disabledSprite, CCObject *target, SEL_MenuHandler selector);
    
    CC_SYNTHESIZE(CLShelfData*, m_pShelfData, pShelfData);       //货架数据指针
    CC_SYNTHESIZE(CCMenuItemSprite*, m_pItem, pItem);

};


//进入商铺后的货架显示场景层
class CLShelfsSceneLayer : public CLSceneLayer
{
public:
    CLShelfsSceneLayer();
    ~CLShelfsSceneLayer();
    
    CREATE_FUNC(CLShelfsSceneLayer);
    
    void UpdateMoney(CCObject* pSender);
    void UpdateCard(CCObject* pSender);
    void onAddMenuPress(CCObject* pSender);       //2个增加按钮的对接事件
    void onShopManagerPress(CCObject* pSender);   //店长按钮点击事件
    void onShelfMenuPress(CCObject* pSender);     //6个货架按钮的接口事件
    void onExitMenuPress(CCObject* pSender);      //退出按钮的接口事件
    void AdvImageLoadEnd();                       //加载结束后的回调
    
    bool init();
    virtual void onExit();
    
    CC_SYNTHESIZE(CCLabelBMFont*, m_pMoneyFont, pMoneyFont);        //钱数量文字
    CC_SYNTHESIZE(CCLabelBMFont*, m_pCardFont, pCardFont);          //信用卡数量文字
    CC_SYNTHESIZE(CCMenu*, m_pShelfsMenu, pShelfsMenu);             //货架总按钮
    CC_SYNTHESIZE(int, m_nAllStars, nAllStars);                     //在此商店获得的星星数量
    CC_SYNTHESIZE(CCMenu*, m_pSMMenu, pSMMenu);                     //店长按钮
    CC_SYNTHESIZE(bool, m_bJihuo, bJihuo);                          //店长是否激活状态
    CC_SYNTHESIZE(int, m_nStoreNo, nStoreNo);                       //所属商铺号
    CC_SYNTHESIZE(bool, m_bClickSM, bClickSM);                      //是否点击的是店长
    CC_SYNTHESIZE(CLScene_TypeIndex, m_nGSceneIndex, nGSceneIndex); // 跳到哪个场景
    
    
    void AddShopManStr();//添加店长话语
    void removeShopManStr();//删除店长话语
    
private:
    CC_SYNTHESIZE(int, m_nCoolTime, nCoolTime);        //店长冷却时间
    CC_SYNTHESIZE(int, m_iSmNo, iSmNo);                //店长货架No
    void CoolTimeSch(float coolTime);                  //倒计时函数
    void StartSchTime(int nTime);                      //开始倒计时
    void EndSchTime();                                 //结束倒计时
    CC_SYNTHESIZE(CCLabelTTF*, m_pShopManSchTTF, pShopManSchTTF);
    CC_SYNTHESIZE(CCLabelTTF*, m_pShopManStrTTF, pShopManStrTTF);//店长说话
    CC_SYNTHESIZE(CCScale9Sprite*, m_Scale9Sprite, Scale9Sprite);//店长说话背景
    
    void _CanceSmCool();                              //取消店长
    
};

#endif /* defined(__BigWinner__CLShelfsLayer__) */
