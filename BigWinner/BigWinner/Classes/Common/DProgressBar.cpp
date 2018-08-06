//
//  DProgressBar.cpp
//  DiyalStudy
//
//  Created by YinYanlong on 13-11-12.
//
//

#include "DProgressBar.h"
#include "CCControlSlider.h"
#include "support/CCPointExtension.h"
#include "touch_dispatcher/CCTouch.h"
#include "CCDirector.h"

DProgressBar::DProgressBar()
: m_value(0.0f)
, m_minimumValue(0.0f)
, m_maximumValue(0.0f)
, m_progressSprite(NULL)
, m_backgroundSprite(NULL)
{
    
}

DProgressBar::~DProgressBar()
{
    CC_SAFE_RELEASE(m_progressSprite);
    CC_SAFE_RELEASE(m_backgroundSprite);
}

DProgressBar* DProgressBar::create(const char* bgFile, const char* progressFile)
{
    CCSprite *backgroundSprite      = CCSprite::create(bgFile);
    
    CCSprite *progressSprite        = CCSprite::create(progressFile);
    
    return DProgressBar::create(backgroundSprite, progressSprite);
}

DProgressBar* DProgressBar::create(CCSprite * backgroundSprite, CCSprite* pogressSprite)
{
    DProgressBar *pRet = new DProgressBar();
    pRet->initWithSprites(backgroundSprite, pogressSprite);
    pRet->autorelease();
    return pRet;
}

bool DProgressBar::initWithSprites(CCSprite * backgroundSprite, CCSprite* progressSprite)
{
//    if (CCControl::init())
//    {
        CCAssert(backgroundSprite,  "Background sprite must be not nil");
        CCAssert(progressSprite,    "Progress sprite must be not nil");
        
        ignoreAnchorPointForPosition(false);
        
        this->setBackgroundSprite(backgroundSprite);
        this->setProgressSprite(progressSprite);
        
        // 设置大小
        CCRect maxRect   = CCControlUtils::CCRectUnion(backgroundSprite->boundingBox(), progressSprite->boundingBox());
        
        setContentSize(CCSizeMake(maxRect.size.width, maxRect.size.height));
        
        //添加进度背景
        m_backgroundSprite->setAnchorPoint(ccp(0.5f, 0.5f));
        m_backgroundSprite->setPosition(ccp(this->getContentSize().width / 2, this->getContentSize().height / 2));
        addChild(m_backgroundSprite);
        
        //添加进度精灵
        m_progressSprite->setAnchorPoint(ccp(0.0f, 0.5f));
        m_progressSprite->setPosition(ccp(0.0f, m_backgroundSprite->getContentSize().height / 2));
        m_backgroundSprite->addChild(m_progressSprite);
        
        //初始值
        m_minimumValue                   = 0.0f;
        m_maximumValue                   = 1.0f;
        
        
        return true;
//    }
//    else
//    {
//        return false;
//    }
}

void DProgressBar::setValue(float value)
{
    // set new value with sentinel
    if (value < m_minimumValue)
    {
        value = m_minimumValue;
    }
    
    if (value > m_maximumValue)
    {
        value = m_maximumValue;
    }
    
    m_value = value;
    
    layoutDraw();
}

void DProgressBar::setMinimumValue(float minimumValue)
{
    m_minimumValue=minimumValue;
    if (m_minimumValue >= m_maximumValue)
    {
        m_maximumValue   = m_minimumValue + 1.0f;
    }
    setValue(m_value);
}

void DProgressBar::setMaximumValue(float maximumValue)
{
    m_maximumValue=maximumValue;
    if (m_maximumValue <= m_minimumValue)
    {
        m_minimumValue   = m_maximumValue - 1.0f;
    }
    setValue(m_value);
}



void DProgressBar::layoutDraw()
{
    if (NULL == m_backgroundSprite || NULL == m_progressSprite)
    {
        return;
    }
     
    float percent               = (m_value - m_minimumValue) / (m_maximumValue - m_minimumValue); //进度百分比（实际值／最大可用进度值）
    
    CCPoint pos                 = m_progressSprite->getPosition(); //获得进度条的位置
    pos.x                       = percent * m_backgroundSprite->getContentSize().width; //x轴在背景图条的长度
    
    // 拉升比例
    CCRect textureRect          = m_progressSprite->getTextureRect();
    textureRect                 = CCRectMake(textureRect.origin.x, textureRect.origin.y, pos.x, textureRect.size.height);
    
    m_progressSprite->setTextureRect(textureRect, m_progressSprite->isTextureRectRotated(), textureRect.size);
}
