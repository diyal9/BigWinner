//
//  CLStrokeLabelTTF.h
//  LolAllStars1_X
//
//  Created by Zhang Qiyun on 13-5-21.
//  带描边字体类
//

#ifndef __LolAllStars1_X__CLStrokeLabelTTF__
#define __LolAllStars1_X__CLStrokeLabelTTF__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;

// 描边字体类
class CLStrokeLabelTTF : public CCSprite
{
public:
    CLStrokeLabelTTF();
    ~CLStrokeLabelTTF();
    
    static CLStrokeLabelTTF* create(const char* str, const char* sFontName, float fontSize, float fStrokeSize, const ccColor3B& fontColor = ccWHITE, const ccColor3B& strokeColor = ccBLACK);  // 1.字符串 2.字体名字 3.字体大小 4.描边大小(一般1到2个像素) 5.字体颜色 6.描边颜色(默认黑色)
    
    bool init(const char* str, const char* sFontName, float fontSize, float fStrokeSize, const ccColor3B& fontColor = ccWHITE, const ccColor3B& strokeColor = ccBLACK);
    
    
};


// 投射字体类
class CLProjectLabelTTF : public CCSprite
{
public:
    CLProjectLabelTTF();
    ~CLProjectLabelTTF();
    
    static CLProjectLabelTTF* create(const char* str, const char* sFontName, float fontSize, float fStrokeSize, const ccColor3B& fontColor = ccWHITE, const ccColor3B& strokeColor = ccBLACK);  // 1.字符串 2.字体名字 3.字体大小 4.投射距离大小(一般1到2个像素) 5.原字体颜色 6.投射字体颜色(默认黑色)
    
    bool init(const char* str, const char* sFontName, float fontSize, float fStrokeSize, const ccColor3B& fontColor = ccWHITE, const ccColor3B& strokeColor = ccBLACK);
    
    
};

#endif /* defined(__LolAllStars1_X__CLStrokeLabelTTF__) */
