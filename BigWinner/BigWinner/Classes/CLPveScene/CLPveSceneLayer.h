//
//  CLPveSceneLayer.h
//  BigWinner
//
//  Created by Zhang Qiyun on 13-10-26.
//
//

#ifndef __BigWinner__CLPveSceneLayer__
#define __BigWinner__CLPveSceneLayer__

#include "cocos2d.h"
#include "LGameCommonDefine.h"
#include "JsonParser.h"
#include "CLPveTopicLayer.h"
#include "CLPveResultLayer.h"
USING_NS_CC;

class CLPveSceneLayer : public CLSceneLayer
{
public:
    CLPveSceneLayer();
    ~CLPveSceneLayer();
    
    static CLPveSceneLayer* create(bool bClickSM = false);
    bool init(bool bClickSM = false);
    
    void StartSchTime(int nTime);
    void onTimeFrame(float ft);
    void EndSchTime(int nAnswer = -1);
    
    void FrameTime(float ft);
    
    void BeginToPve();
    void UpdateTopicLayer(Json::Value& jv);
    void CreateyinhuaIcon(int nYinhuaNo);
    void onYinhuaIconPress(CCObject* pSender);
    void onYinhuaFrame(float ft);
    void ShowPveResultLayer(Json::Value& jv);  //显示结果
    void ReturnToShelfsLayer();
    void AdvImageLoadEnd();
    void ContiueToPve();
    void onBossMenuClicked(CCObject* pSender);
    void onAnswerMenuClicked(CCObject* pSender);
    void onYinhuaDownEnd();
    void onDeleteYinhaIcon();
    void onGiveupMenuPress(CCObject* pSender);
    void ReStartToFightBoss();
    void onPropMenuPress(CCObject* pSender);
    void SetPropMenuEnable(bool b);
    
    void randomAction();  //随机动画
    void actionRandomGet();
    CC_SYNTHESIZE(CCActionInterval*, m_pAction, pAction); //动作指针
    
    void UsePropSucces(Json::Value& jv);    //使用道具成功
    
    void  _GiftBossResultLayer(Json::Value& jv);         //给店长送礼成功回调函数
    
    void animationWithPro(int proID, int isShutDown,float delayTime , float shutTime , int repeatTimes);            //播放动画
    void animationCancle(float t);               //取消动画
    
    //到计时图标
    CC_SYNTHESIZE(CCLabelBMFont*, m_pSchedueTTF, pSchedueTTF);
    CC_SYNTHESIZE(CLPveTopicLayer*, m_pTopicLayer, pTopicLayer);
    CC_SYNTHESIZE(int, m_nSchTime, nSchTime);
    CC_SYNTHESIZE(int, m_nEnterTime, nEnterTime);
    CC_SYNTHESIZE(int, m_nShelfNo, nShelfNo);
    
    CC_SYNTHESIZE(CCSprite*, m_pShowGoodsBG, pShowGoodsBG);
    CC_SYNTHESIZE(CCMenuItemSprite*, m_pYinhuaIcon, pYinhuaIcon);
    CC_SYNTHESIZE(int, m_nYinhuaNo, nYinhuaNo);
    CC_SYNTHESIZE(CCMenu*, m_pYinhuaMenu, pYinhuaMenu);
    CC_SYNTHESIZE(bool, m_bYinhuaMoveR, bYinhuaMoveR);
    CC_SYNTHESIZE(int, m_nYIndex, nYIndex);
    CC_SYNTHESIZE(CCSprite*, m_pSchBG, pSchBG);
    CC_SYNTHESIZE(CCSprite*, m_pFunButtonBG, pFunButtonBG);
    CC_SYNTHESIZE(PassPointsSuccess*, m_pResultLayer, pResultLayer);
    CC_SYNTHESIZE(bool, m_bSMStyle, bSMStyle);
    CC_SYNTHESIZE(CCSprite*, m_pSMNameBG, pSMNameBG);   //店长名字
    CC_SYNTHESIZE(int, m_iBossShelfNo, iBossShelfNo);           //店长货架No
    CC_SYNTHESIZE(CCSprite*, m_pSMForm, pSMForm);       //店长形象
    CC_SYNTHESIZE(CCMenu*, m_pSMMenu, pSMMenu);         //店长 2个按钮
    CC_SYNTHESIZE(CCMenu*, m_pGiveUpMenu, pGiveUpMenu);         //认输按钮
    CC_SYNTHESIZE(CCMenu*, m_pPorpMenu, pPorpMenu);     //道具按钮
    CC_SYNTHESIZE(bool, m_bRoundUseProp, bRoundUseProp); //此回合有无使用道具
    
};

#endif /* defined(__BigWinner__CLPveSceneLayer__) */
