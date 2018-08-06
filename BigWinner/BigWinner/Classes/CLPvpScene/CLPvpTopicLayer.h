//
//  CLPvpTopicLayer.h
//  BigWinner
//
//  Created by Zhang Qiyun on 13-12-22.
//
//

#ifndef __BigWinner__CLPvpTopicLayer__
#define __BigWinner__CLPvpTopicLayer__

#include "cocos2d.h"
#include "JsonParser.h"

USING_NS_CC;

class CLPvpTopicLayer : public CCSprite
{
public:
    CLPvpTopicLayer();
    ~CLPvpTopicLayer();
    
    static CLPvpTopicLayer* create(Json::Value& value);
    bool init(Json::Value& value);
    void StartShowAnswer(int nIndex);
    void onSchFrame(float ft);
    
    CC_SYNTHESIZE(CCMenu*, m_pAnswerMenu, pAnswerMenu);
    CC_SYNTHESIZE(int, m_nShowIndex, nShowIndex);
    CC_SYNTHESIZE(int, m_nShowTimeIndex, nShowTimeIndex);  //显示答案时间第几针
    
    
};


#endif /* defined(__BigWinner__CLPvpTopicLayer__) */
