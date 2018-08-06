//
//  CLMainSceneLayer.h
//  BigWinner
//
//  Created by ZhangQiyun on 13-10-14.
//
//

#ifndef __BigWinner__CLMainSceneLayer__
#define __BigWinner__CLMainSceneLayer__

#include "cocos2d.h"
#include "LGameCommonDefine.h"
#include "CLStoreShelfData.h"
#include "JsonParser.h"
USING_NS_CC;

class CLStorePanel : public CCMenuItemSprite
{
public:
    CLStorePanel();
    ~CLStorePanel();
    
    static CLStorePanel* create(CLStoreData* pData, CCNode *normalSprite, CCNode *selectedSprite, CCNode *disabledSprite, CCObject *target, SEL_MenuHandler selector);
    bool init(CLStoreData* pData, CCNode *normalSprite, CCNode *selectedSprite, CCNode *disabledSprite, CCObject *target, SEL_MenuHandler selector);
    
    CC_SYNTHESIZE(CLStoreData*, m_pStoreData, pStoreData);       //商店数据指针
    
    
};

class CLMainSceneLayer : public CLSceneLayer
{
public:
    CLMainSceneLayer();
    ~CLMainSceneLayer();
    CREATE_FUNC(CLMainSceneLayer);
    bool init();
    
    void UpdatePower(CCObject* pSender);
    void UpdateMoney(CCObject* pSender);
    void UpdateCard(CCObject* pSender);
    
    void onFunctionMenuPress(CCObject* pSender);  //下面6个功能按钮对应的处理接口
    void onAddMenuPress(CCObject* pSender);       //2个增加按钮的对接事件
    void onStoreMenuPress(CCObject* pSender);     //6个商店按钮的接口时间
    void onPvpMenuPress(CCObject* pSender);       //对战响应接口
    void CreateStoresMenu();       //创建商铺接口
    void AdvImageLoadEnd();                       //加载结束后的回调
    
    //wangguolong 新店铺解锁以后动作
    void Action(CLStorePanel* pSender);
    CC_SYNTHESIZE(CCActionInterval*, m_pAction, pAction);
    
    void SerPushAchiCreate(); //服务器推送成就提醒
    void UpdateNewAchi(); //更新可领取成就
    
    virtual void onExit();
    
    CC_SYNTHESIZE(CCSprite*, m_pPowerBar, pPowerBar);               //体力条
    CC_SYNTHESIZE(CCLabelBMFont*, m_pCPowerFont, pCPowerFont);      //当前体力文字
    CC_SYNTHESIZE(CCLabelBMFont*, m_pMPowerFont, pMPowerFont);      //最大体力文字
    CC_SYNTHESIZE(CCLabelBMFont*, m_pMoneyFont, pMoneyFont);        //钱数量文字
    CC_SYNTHESIZE(CCLabelBMFont*, m_pCardFont, pCardFont);          //信用卡数量文字
    CC_SYNTHESIZE(bool, m_bUpdateStores, bUpdateStores);            //是否已经更新过商店信息
    CC_SYNTHESIZE(CCMenu*, m_pStoreMenu, pStoreMenu);               //商铺按钮
    CC_SYNTHESIZE(int, m_nEnterSceneID, nEnterSceneID);             //需要进入的场景类型 1 是商铺 2是对战
    
    CC_SYNTHESIZE(CCSprite *, m_pAchiNumBg, pAchiNumBg);            //未领取成就显示背景（成就按钮）
    CC_SYNTHESIZE(CCLabelTTF *, m_pAchiTTF, pAchiTTF);              //未领取成就TTF
    
};

#endif /* defined(__BigWinner__CLMainSceneLayer__) */
