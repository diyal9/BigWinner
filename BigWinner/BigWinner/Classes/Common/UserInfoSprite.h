//
//  UserInfoSprite.h
//  BigWinner
//
//  Created by YinYanlong on 13-12-17.
//
//

#ifndef __BigWinner__UserInfoSprite__
#define __BigWinner__UserInfoSprite__

#include <iostream>
#include "cocos2d.h"
#include "JsonParser.h"
USING_NS_CC;

//用户头像信息精灵
class UserInfoSprite : public CCSprite
{
public:
    UserInfoSprite();
    ~UserInfoSprite();
    
    static UserInfoSprite* create();
    
    bool init();
    
    void onHeadBtnPress();
    
};

//对战对手头像信息精灵
class RivalInfoSprite : public CCSprite
{
public:
    RivalInfoSprite();
    ~RivalInfoSprite();
    
    static RivalInfoSprite* create(Json::Value& jv);
    
    CC_SYNTHESIZE(CCLabelTTF*, m_pNameTTF, pNameTTF); //昵称
    
    bool init(Json::Value& jv);
    
};


#endif /* defined(__BigWinner__UserInfoSprite__) */
