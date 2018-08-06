//
//  CLPveTopicLayer.h
//  BigWinner
//
//  Created by Zhang Qiyun on 13-10-26.
//
//

#ifndef __BigWinner__CLPveTopicLayer__
#define __BigWinner__CLPveTopicLayer__

#include "cocos2d.h"
#include "JsonParser.h"
USING_NS_CC;

class CLAnswerStruct
{
public:
    CLAnswerStruct(){m_nIndex=-1; m_nRand = 0;}
    ~CLAnswerStruct(){};
    CC_SYNTHESIZE(int, m_nIndex, nIndex);
    CC_SYNTHESIZE(std::string, m_sAnswer, sAnswer);
    CC_SYNTHESIZE(int, m_nRand, nRand);
};

class CLPveTopicLayer : public CCSprite
{
public:
    CLPveTopicLayer();
    ~CLPveTopicLayer();
    
    static CLPveTopicLayer* create(Json::Value& value, bool bPveStyle = true);
    bool init(Json::Value& value, bool bPveStyle = true);
    void onPauseMenuCkicked(CCObject* pSender);
    void onAnswerMenuClicked(CCObject* pSender);
    
    void UpdateTopicData(Json::Value& jv);
    void GiveAnanswerIndex(bool bRight, int nIndex);
    void ReSetAnswerState();
    void SetAnswersEnable(bool b);
    
    CC_SYNTHESIZE(CCMenu*, m_pPauseMenu, pPauseMenu);  //暂停按钮
    CC_SYNTHESIZE(CCLabelTTF*, m_pTopicIndexTTF, pTopicIndexTTF);  //第几道题目字样
    CC_SYNTHESIZE(CCLabelTTF*, m_pTopicTextTTF, pTopicTextTTF);  //题目字样
    CC_SYNTHESIZE(int, m_nTopicIndex, nTopicIndex);     //第几道题目
    CC_SYNTHESIZE(bool, m_bPveStyle, bPveStyle);        //是否pve模式 默认为true
    CC_SYNTHESIZE(CCSprite*, m_pFinger, pFinger);
    CC_SYNTHESIZE(CCMenu*, m_pAnswerMenu, pAnswerMenu);
    
private:
    std::vector<CCSprite*> m_vToicStars;
    std::vector<CCLabelTTF*> m_vAnswerList;
    std::vector<CLAnswerStruct*> m_vAnswerStructs;
    
};

#endif /* defined(__BigWinner__CLPveTopicLayer__) */
