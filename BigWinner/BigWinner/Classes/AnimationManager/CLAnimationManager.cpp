//
//  CLAnimationManager.cpp
//  Yy_mobileGame3
//
//  Created by Jin Zhiqing on 13-7-1.
//
//

#include "CLAnimationManager.h"

static CLAnimationManager* g_pAnimationManager = NULL;

CLAnimationManager::CLAnimationManager()
{
}

CLAnimationManager::~CLAnimationManager()
{
    std::map<std::string, CCAnimation*>::iterator iter;
    for (iter = m_vAnimations.begin(); iter != m_vAnimations.end(); ++iter)
        CC_SAFE_DELETE(iter->second);
    
}

CLAnimationManager* CLAnimationManager::sharedAnimationManager()
{
    if (!g_pAnimationManager) {
        g_pAnimationManager = new CLAnimationManager();
    }
    
    return g_pAnimationManager;
}

void CLAnimationManager::init()
{
    //加载动画资源配置文件
    Json::Value pv;
    Json::Reader jsReader;
    std::string srcPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("Data/JsonData/Animation.json");
    
    CCString* pcstr = CCString::createWithContentsOfFile(srcPath.c_str());
    if(pcstr == NULL)
    {
        CCLOG("This file %s is not exist!!!", "Data/JsonData/Animation.json");
        return;
    }
    jsReader.parse(pcstr->getCString(), pv, false);
    Json::Value configData = pv["data"];
    int animSize = configData.size();
    
    for(int i=0; i < animSize; ++i)
    {
        m_vAnimConfig.insert(std::pair<std::string, Json::Value>(configData[i]["name"].asString(), configData[i]));
    }
}

CCAnimation* CLAnimationManager::animationWithName(const std::string& animName, float delay)
{
    CCAnimation* pAnimation = NULL;
    //查找已加载过的动画
    std::map<std::string, CCAnimation*>::iterator animIter = m_vAnimations.find(animName);
    if (animIter != m_vAnimations.end())
    {
        pAnimation = animIter->second;
    }
    else
    {
        //获取动画配置
        std::map<std::string, Json::Value>::iterator configIter = m_vAnimConfig.find(animName);
        if (configIter != m_vAnimConfig.end())
        {
            //加载动画资源
            CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
            cache->addSpriteFramesWithFile(("Image/Animation/" + animName + ".plist").c_str(),
                                           ("Image/Animation/" + animName + ".png").c_str());
            
            Json::Value animConfig = configIter->second;
            int frameCnt = animConfig["frameCnt"].asInt();
            
            CCArray* animFrames = CCArray::createWithCapacity(frameCnt);
            for (int i = 0; i < frameCnt; ++i)
            {
                const char* realAnimName = CCString::createWithFormat("%s_%d.png", animName.c_str(), i+1)->getCString();
                CCSpriteFrame* frame = cache->spriteFrameByName(realAnimName);
                animFrames->addObject(frame);
            }
            pAnimation = CCAnimation::createWithSpriteFrames(animFrames);
            pAnimation->retain();
            m_vAnimations.insert(std::pair<std::string, CCAnimation*>(animName, pAnimation));
        }
        else
        {
            CCLOG("%s animation config be not set", animName.c_str());
        }
    }
    
    //设置动画帧率
    if (pAnimation)
    {
        pAnimation->setDelayPerUnit(delay);
        return pAnimation;
    }
    
    return NULL;
}

CCSprite* CLAnimationManager::spriteWithAnimationName(const std::string &animName)
{
    //CCAnimation* anim = animationWithName(animName);
    //return spriteWithAnimation(anim);
    return CCSprite::createWithSpriteFrameName((animName + "_1.png").c_str());
}

CCSprite* CLAnimationManager::spriteWithAnimation(CCAnimation* anim)
{
    if (anim && anim->getFrames()->count() > 0)
    {
        CCSpriteFrame* frame = (CCSpriteFrame*) anim->getFrames()->objectAtIndex(0);
        return CCSprite::createWithSpriteFrame(frame);
    }
    
    return NULL;
}
