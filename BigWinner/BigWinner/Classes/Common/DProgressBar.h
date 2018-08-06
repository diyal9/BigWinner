//
//  DProgressBar.h
//  DiyalStudy
//
//  Created by YinYanlong on 13-11-12.
//
//

#ifndef __DiyalStudy__DProgressBar__
#define __DiyalStudy__DProgressBar__

#include "CCControl.h"
#include "CCInvocation.h"
#include "CCControl.h"

USING_NS_CC;
USING_NS_CC_EXT;

/**
 *  进度条控件
 *
 *  //使用实例
 *  DProgressBar *processBar = DProgressBar::create("Images/Self/ProcessBlack.png", "Images/Self/ProcessRed.png");
 *  processBar->setPosition(ccp(350, 200));
 *  processBar->setMinimumValue(0); //设置最小值
 *  processBar->setMaximumValue(10); //设置最大值
 *  processBar->setValue(3); //设置进度值
 */

class DProgressBar: public CCLayer
{
    DProgressBar();
    virtual ~DProgressBar();
    
    CC_SYNTHESIZE_READONLY(float, m_value, Value); //当前进度值
    virtual void setValue(float val);
    
    CC_SYNTHESIZE_READONLY(float, m_minimumValue, MinimumValue); // 进度最小值 默认是0.0f
    virtual void setMinimumValue(float val);
    
    CC_SYNTHESIZE_READONLY(float, m_maximumValue, MaximumValue); // 进度最大值 默认1.0
    virtual void setMaximumValue(float val);
    
    CC_SYNTHESIZE_RETAIN(CCSprite*, m_progressSprite, ProgressSprite); //进度精灵
    CC_SYNTHESIZE_RETAIN(CCSprite*, m_backgroundSprite, BackgroundSprite);  //背景精灵
    
public:    
    static DProgressBar* create(const char* bgFile, const char* progressFile);
    
private:
    
    static DProgressBar* create(CCSprite * backgroundSprite, CCSprite* pogressSprite);
    
    virtual bool initWithSprites(CCSprite * backgroundSprite, CCSprite* progressSprite);
    
    virtual void layoutDraw();
    
};

#endif /* defined(__DiyalStudy__DProgressBar__) */
