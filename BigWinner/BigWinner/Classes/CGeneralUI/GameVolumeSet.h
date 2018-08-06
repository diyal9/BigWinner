//
//  GameVolumeSet.h
//  BigWinner
//
//  Created by LunaBox on 13-12-16.
//
//

#ifndef __BigWinner__GameVolumeSet__
#define __BigWinner__GameVolumeSet__

#include <iostream>
#include "cocos2d.h"

#include "cocos-ext.h"

USING_NS_CC;
using namespace cocos2d::extension;

/*
 游戏音量音效设置
 */
class GameVolumeSet:public CCLayerColor
{
    
public:
	GameVolumeSet(){
        m_pGiveBG1 = NULL;
        m_slider = NULL;
        m_VolumeMusic = NULL;
        m_VolumeIcon01Music = NULL;
        m_slider2 = NULL;
        m_fMusic = 0;
        m_fSoundEffect = 0;
        m_NoVoice = NULL;
        m_NoVoice2 = NULL;
        
        m_VoiceSizeMusic = NULL;
        m_VoiceSizeMusic2 = NULL;
        m_VoiceSizeMusic3 = NULL;
        
        m_VoiceSizeYinxiao = NULL;
        m_VoiceSizeYinxiao2 = NULL;
        m_VoiceSizeYinxiao3 = NULL;
        
    };
	~GameVolumeSet(){};
    
    static GameVolumeSet* create();
	virtual bool init();
    
    //UI
    void doBaseUI();
    
    //取消
    void CanceBtn();
    
    //确定
    void AffirmBtn();
    
    //添加回调函数
    void changeValue(CCObject *sender, CCControlEvent controlEvent);
    void changeValue2(CCObject *sender, CCControlEvent controlEvent);
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    //背景音乐 //游戏音效 创建
    void createCCControlSlider(std::string name,float fscale);
    
    void doAction(CCNode* r, bool moveIn); //action执行 (节点，移动方式true为in)

    CC_SYNTHESIZE(CCSprite*, m_pGiveBG1, pGiveBG1) //白色背景图片
    CC_SYNTHESIZE(CCControlSlider*, m_slider, slider);
    CC_SYNTHESIZE(CCControlSlider*, m_slider2, slider2);
    CC_SYNTHESIZE(CCLabelTTF* , m_VolumeMusic, VolumeMusic);
    CC_SYNTHESIZE(CCSprite* , m_VolumeIcon01Music, VolumeIcon01Music);
    CC_SYNTHESIZE(CCSprite* , m_NoVoice, NoVoice);
    CC_SYNTHESIZE(CCSprite* , m_NoVoice2, NoVoice2);
    
    //背景音量大小的设置
    CC_SYNTHESIZE(CCSprite* , m_VoiceSizeMusic, VoiceSizeMusic);
    CC_SYNTHESIZE(CCSprite* , m_VoiceSizeMusic2, VoiceSizeMusic2);
    CC_SYNTHESIZE(CCSprite* , m_VoiceSizeMusic3, VoiceSizeMusic3);
    
    //背景音效大小的设置
    CC_SYNTHESIZE(CCSprite* , m_VoiceSizeYinxiao, VoiceSizYinxiao);
    CC_SYNTHESIZE(CCSprite* , m_VoiceSizeYinxiao2, VoiceSizYinxiao2);
    CC_SYNTHESIZE(CCSprite* , m_VoiceSizeYinxiao3, VoiceSizYinxiao3);
    
    //背景音乐值
    CC_SYNTHESIZE(int , m_fMusic, fMusic);
    //游戏音效值
    CC_SYNTHESIZE(int , m_fSoundEffect, fSoundEffect);
    
};

#endif /* defined(__BigWinner__GameVolumeSet__) */
