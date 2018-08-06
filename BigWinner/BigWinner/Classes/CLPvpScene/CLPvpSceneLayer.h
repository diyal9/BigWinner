//
//  CLPvpSceneLayer.h
//  BigWinner
//
//  Created by ZhangQiyun on 13-11-6.
//
//

#ifndef __BigWinner__CLPvpSceneLayer__
#define __BigWinner__CLPvpSceneLayer__

#include "cocos2d.h"
#include "LGameCommonDefine.h"
#include "CLFadeColorLayer.h"
USING_NS_CC;

enum PVPLAYER_MODE
{
    PVPLAYER_NULL = 0,
    PVPLAYER_FIRST,
    PVPLAYER_SECOND
};

class CLPvpSceneLayer : public CLSceneLayer
{
public:
    CLPvpSceneLayer();
    ~CLPvpSceneLayer();
    
    CREATE_FUNC(CLPvpSceneLayer);
    bool init();
    
    void ChangeSubLayer();
    void ChangeSceneCall();
    void ChangeSceneCallEnd();
    
    CC_SYNTHESIZE(PVPLAYER_MODE, m_nLayerMode, nLayerMode);   // pvp 模式 进入的是备战界面 还是战斗界面
    CC_SYNTHESIZE(CCNode*, m_pShowNode, pShowNode);           // 显示子界面
    CC_SYNTHESIZE(bool, m_bSubReady, bSubReady);              // 是否是战斗准备界面
    CC_SYNTHESIZE(CLFadeColorLayer*, m_pFadeLayer, pFadeLayer);     //用来过渡的阴影层
};

#endif /* defined(__BigWinner__CLPvpSceneLayer__) */
