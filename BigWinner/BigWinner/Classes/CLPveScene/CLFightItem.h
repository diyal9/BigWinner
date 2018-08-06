//
//  CLFightItem.h
//  BigWinner
//
//  Created by Zhang Qiyun on 13-11-28.
//
//

#ifndef __BigWinner__CLFightItem__
#define __BigWinner__CLFightItem__

#include "cocos2d.h"
#include "CLTopicData.h"
USING_NS_CC;

class CLFightPropItem : public CCMenuItemSprite
{
public:
    CLFightPropItem();
    ~CLFightPropItem();
    
    static CLFightPropItem* create(int nPropId, CCObject* pTarget, SEL_CallFuncO pCallFun, bool bCard = false);
    
    bool init(int nPropId, CCObject* pTarget, SEL_CallFuncO pCallFun, bool bCard = false);
    
    virtual void setOpacity(GLubyte opacity);
    virtual void setEnabled(bool bEnabled);
    
    CC_SYNTHESIZE(playerCardJueseInfo*, m_pPropInfo, pPropInfo);
    CC_SYNTHESIZE(CCObject*, m_pCallTarget, pCallTarget);     //回调用的加载对象
    CC_SYNTHESIZE(SEL_CallFuncO, m_pCallFun, pCallFun);        //回调用的加载函数
    CC_SYNTHESIZE(CCLabelTTF*, m_pNumLabel, pNumLabel);
    CC_SYNTHESIZE(int, m_nCoolRound, nCoolRound);             //冷却回合数
    CC_SYNTHESIZE(bool, m_bTopicCard, bTopicCard);            //是否是题卡
    
};

#endif /* defined(__BigWinner__CLFightItem__) */
