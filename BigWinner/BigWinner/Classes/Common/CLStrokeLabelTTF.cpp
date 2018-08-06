//
//  CLStrokeLabelTTF.cpp
//  LolAllStars1_X
//
//  Created by Zhang Qiyun on 13-5-21.
//
//

#include "CLStrokeLabelTTF.h"

CLStrokeLabelTTF::CLStrokeLabelTTF()
{
    
}

CLStrokeLabelTTF::~CLStrokeLabelTTF()
{
    
}

CLStrokeLabelTTF* CLStrokeLabelTTF::create(const char* str, const char* sFontName, float fontSize, float fStrokeSize, const ccColor3B& fontColor, const ccColor3B& strokeColor)  // 1.字符串 2.字体名字 3.字体大小 4.描边大小(一般1到2个像素) 5.字体颜色 6.描边颜色(默认黑色)
{
    CLStrokeLabelTTF* pLabelTTF = new CLStrokeLabelTTF();
    if(pLabelTTF && pLabelTTF->init(str, sFontName, fontSize, fStrokeSize,fontColor,strokeColor))
    {
        pLabelTTF->autorelease();
        return pLabelTTF;
    }
    else
    {
        CC_SAFE_DELETE(pLabelTTF);
        return NULL;
    }
}


bool CLStrokeLabelTTF::init(const char* str, const char* sFontName, float fontSize, float fStrokeSize, const ccColor3B& fontColor, const ccColor3B& strokeColor)
{
    CCLabelTTF* pText = CCLabelTTF::create(str, sFontName, fontSize);
    pText->setAnchorPoint(ccp(0.5, 0.5));
    pText->setPosition(ccp(pText->getContentSize().width/2, pText->getContentSize().height/2));
    
    CCSize textureSize = pText->getContentSize();
    textureSize.width += 2*fStrokeSize;
    textureSize.height += 2*fStrokeSize;
    
    CCRenderTexture *rt = CCRenderTexture::create(textureSize.width, textureSize.height);
    pText->setColor(strokeColor);
    ccBlendFunc originalBlend = pText->getBlendFunc();
    ccBlendFunc func = { GL_SRC_ALPHA, GL_ONE};
    pText->setBlendFunc(func);
    
    rt->begin();
    for(int i = 0; i < 360; i += 30)
    {
        float r = CC_DEGREES_TO_RADIANS(i);
        pText->setPosition(ccp(textureSize.width * 0.5f + sin(r) * fStrokeSize, textureSize.height * 0.5f + cos(r) * fStrokeSize));
        pText->visit();
    }
    pText->setColor(fontColor);
    pText->setBlendFunc(originalBlend);
    pText->setPosition(ccp(textureSize.width * 0.5f, textureSize.height * 0.5f));
    pText->visit();
    rt->end();
    
    CCTexture2D *texture = rt->getSprite()->getTexture();
    texture->setAntiAliasTexParameters();
    if(!this->initWithTexture(texture))
        return false;
    this->setFlipY(true);
    
    return true;
    
}







CLProjectLabelTTF::CLProjectLabelTTF()
{
    
}

CLProjectLabelTTF::~CLProjectLabelTTF()
{
    
}


CLProjectLabelTTF* CLProjectLabelTTF::create(const char* str, const char* sFontName, float fontSize, float fStrokeSize, const ccColor3B& fontColor, const ccColor3B& strokeColor) // 1.字符串 2.字体名字 3.字体大小 4.投射距离大小(一般1到2个像素) 5.原字体颜色 6.投射字体颜色(默认黑色)
{
    CLProjectLabelTTF* pLabel = new CLProjectLabelTTF();
    if(pLabel && pLabel->init(str, sFontName, fontSize, fStrokeSize,fontColor,strokeColor))
    {
        pLabel->autorelease();
        return pLabel;
    }
    else
    {
        CC_SAFE_DELETE(pLabel);
        return NULL;
    }
}


bool CLProjectLabelTTF::init(const char* str, const char* sFontName, float fontSize, float fStrokeSize, const ccColor3B& fontColor, const ccColor3B& strokeColor)
{
    CCLabelTTF* pText = CCLabelTTF::create(str, sFontName, fontSize);
    pText->setAnchorPoint(ccp(0.5, 0.5));
    pText->setPosition(ccp(pText->getContentSize().width/2, pText->getContentSize().height/2));
    
    CCSize textureSize = pText->getContentSize();
    textureSize.width += fStrokeSize;
    textureSize.height += fStrokeSize;

    CCRenderTexture *rt = CCRenderTexture::create(textureSize.width, textureSize.height);
    pText->setColor(strokeColor);
    
    rt->begin();
    float r = CC_DEGREES_TO_RADIANS(135);
    pText->setPosition(ccp(textureSize.width * 0.5f + sin(r) * fStrokeSize, textureSize.height * 0.5f + cos(r) * fStrokeSize));
    pText->visit();
    pText->setColor(fontColor);
    pText->setPosition(ccp(textureSize.width * 0.5f, textureSize.height * 0.5f));
    pText->visit();
    rt->end();
    
    CCTexture2D *texture = rt->getSprite()->getTexture();
    texture->setAntiAliasTexParameters();
    if(!this->initWithTexture(texture))
        return false;
    this->setFlipY(true);
    
    return true;
}

