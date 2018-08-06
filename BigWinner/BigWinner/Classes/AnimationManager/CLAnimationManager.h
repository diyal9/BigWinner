//
//  CLAnimationManager.h
//  Yy_mobileGame3
//
//  Created by Jin Zhiqing on 13-7-1.
//
//

#ifndef __Yy_mobileGame3__CLAnimationManager__
#define __Yy_mobileGame3__CLAnimationManager__

#include "cocos2d.h"
#include "JsonParser.h"

USING_NS_CC;

class CLAnimationManager
{
public:
    CLAnimationManager();
    ~CLAnimationManager();
    static CLAnimationManager* sharedAnimationManager();
    
    void init();
    CCAnimation* animationWithName(const std::string& animName, float delay = 0.1f);      //获取动画
    
    CCSprite* spriteWithAnimationName(const std::string& animName);                       //获取精灵
    CCSprite* spriteWithAnimation(CCAnimation* anim);
    
private:
    std::map<std::string, CCAnimation*> m_vAnimations;
    std::map<std::string, Json::Value> m_vAnimConfig;
};

#endif /* defined(__Yy_mobileGame3__CLAnimationManager__) */
