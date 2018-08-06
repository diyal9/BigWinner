//
//  CLPvpingLayer.h
//  BigWinner
//
//  Created by Zhang Qiyun on 13-11-10.
//
//

#ifndef __BigWinner__CLPvpingLayer__
#define __BigWinner__CLPvpingLayer__

#include "cocos2d.h"
#include "JsonParser.h"
#include "CLPveTopicLayer.h"
#include "CLPvpTopicLayer.h"
USING_NS_CC;

class CLPvpingLayer : public CCLayer
{
public:
    CLPvpingLayer();
    ~CLPvpingLayer();
    
    static CLPvpingLayer* create(Json::Value& jv);
    bool init(Json::Value& jv);
    
    CC_SYNTHESIZE(int, m_nSelfCurLife, nSelfCurLife);  //自己的当前积分
    CC_SYNTHESIZE(int, m_nRivalCurLife, nRivalCurLife); //对手的当前积分
    
    void StartSchTime(int nTime);
    void onTimeFrame(float ft);
    void EndSchTime(int nAnswer = -1);
    
    void BeginToPvp();
    void UpdateTopicLayer(Json::Value& jv);
    void onGiveupMenuPress(CCObject* pSender);
    void onPropMenuPress(CCObject* pSender);
    
    void UpdateBlood(int nSelfLife, int nRivalLife);
    void CanUsePropCard(Json::Value& jv);
    
    void UsePropSucces(Json::Value& jv);    //使用道具成功
    void UseCardSucces(Json::Value& jv);    //使用卡牌成功
    void ShowRivalSelAnswer(Json::Value& jv); //显示对方选择的答案
    
    void setCardMenuEnable(bool b);
    void setPropMenuEnable(bool b);
    void EndPvping(Json::Value& jv);
    
    void StartSchUseCard();
    void EndSchUseCard();
    void SchUseCardFrame();
    
    void animationWithPro(int proID, int isShutDown,float delayTime , float shutTime , int repeatTimes);            //播放动画
    void animationCancle(float t);               //取消动画
    
    CC_SYNTHESIZE(CCSprite*, m_pSelfScoreSpr, pSelfScoreSpr);   //自己的积分条
    CC_SYNTHESIZE(CCSprite*, m_pRivalScoreSpr, pRivalScoreSpr);   //对手的积分条
    CC_SYNTHESIZE(CCLabelTTF*, m_pSelfScoreTTF, pSelfScoreTTF);   //自己的积分
    CC_SYNTHESIZE(CCLabelTTF*, m_pRivalScoreTTF, pRivalScoreTTF);   //对手的积分
    
    //到计时图标
    CC_SYNTHESIZE(CCLabelBMFont*, m_pSchedueTTF, pSchedueTTF);
    CC_SYNTHESIZE(CLPveTopicLayer*, m_pTopicLayer, pTopicLayer);
    CC_SYNTHESIZE(CLPvpTopicLayer*, m_pPTopicLayer, pPTopicLayer);  //对方答题时候的页面
    CC_SYNTHESIZE(int, m_nSchTime, nSchTime);
    CC_SYNTHESIZE(CCSprite*, m_pSchBG, pSchBG);
    
    CC_SYNTHESIZE(CCMenu*, m_pGiveUpMenu, pGiveUpMenu);         //认输按钮
    CC_SYNTHESIZE(CCMenu*, m_pPorpMenu, pPorpMenu);     //道具按钮
    CC_SYNTHESIZE(bool, m_bRoundUseProp, bRoundUseProp); //此回合有无使用道具
    CC_SYNTHESIZE(int, m_nRoundIndex, nRoundIndex);     //回合数
    CC_SYNTHESIZE(bool, m_bCanUsePropCard, bCanUsePropCard);  //可以开始使用道具
    
    CC_SYNTHESIZE(CCLabelTTF*, m_pUESelfTTF, pUESelfTTF);
    CC_SYNTHESIZE(CCLabelTTF*, m_pUERivalTTF, pUERivalTTF);
    CC_SYNTHESIZE(CCLabelTTF*, m_pSchUseProTTF, pSchUseProTTF);
    CC_SYNTHESIZE(int, m_nSchUseCardTime, nSchUseCardTime);
    CC_SYNTHESIZE(int, m_nTurns, nTurns);               //游戏所处哪个阶段
    CC_SYNTHESIZE(bool, m_bGiveTopic, bGiveTopic);      //轮流出题阶段是否我来出题
    
    std::string rivalName; //对手名称，result画面用
    
private:
    std::vector<CCSprite*> m_vSelfStarList;   // 自己的星星列表
    std::vector<CCSprite*> m_vRivalStarList;  // 对手的星星列表
};

#endif /* defined(__BigWinner__CLPvpingLayer__) */
