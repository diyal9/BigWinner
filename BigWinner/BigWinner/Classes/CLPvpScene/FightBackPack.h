//
//  BackPack.h
//  BigWinner
//
//  Created by LunaBox03 on 13-10-19.
//
//

#ifndef __BigWinner__FightBackPack__
#define __BigWinner__FightBackPack__

#include <iostream>
#include <cocos2d.h>
#include "JsonParser.h"

USING_NS_CC;

enum FIGHT_TYPE
{
    FI_HECHENG = 1,
    FI_JUESE,
    FI_TIKA,
    FI_DAOJU
};

enum FIGHT_TAG
{
    FI_JUESE_TAG= 10,
    FI_TIKA_TAG,
    FI_DAOJU_TAG,
    FI_GUANBI_TAG,
    FI_GENERAL_TAG,
    FI_JUESE_TAG2,
    FI_TIKA_TAG2,
    FI_DAOJU_TAG2
};

class FightBackPackLayer:public CCLayerColor
{
    
public:
	FightBackPackLayer():m_indesSprite(NULL),m_nlayerTpye(FI_TIKA)
    {
        m_ntag = 0;
    }
	~FightBackPackLayer(){};
    static FightBackPackLayer* create();
	virtual bool init();
    void doBaseUI();
    void menuCallBackButton(CCNode* r);
    void setIndexPosition();
    void addBaseUI();
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
protected:
    int m_nlayerTpye;              //当前选择类型
    
    CCSprite* m_indesSprite;      //当前高亮位置精灵
    
    CC_SYNTHESIZE(int, m_ntag, ntag);//记录当前选中
    
};

#endif /* defined(__BigWinner__FightBackPack__) */
