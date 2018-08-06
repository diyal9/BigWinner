//
//  Glgs_cocosAppDelegate.cpp
//  Glgs_cocos
//
//  Created by ZhangQiyun on 13-9-23.
//  Copyright __MyCompanyName__ 2013年. All rights reserved.
//

#include "AppDelegate.h"

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "HelloWorldScene.h"

USING_NS_CC;
using namespace CocosDenshion;

AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());
    
    // turn on display FPS
    pDirector->setDisplayStats(false);
    
    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);
    
    //    std::vector<std::string> vPath;
    //    vPath.push_back("Data/JsonData/");
    //    CCFileUtils::sharedFileUtils()->setSearchResolutionsOrder(vPath);
    
    float res_width = 640.0;
    
    CCSize screenSize = CCEGLView::sharedOpenGLView()->getFrameSize();
    float dw = screenSize.width;
    float dh = screenSize.height;
    float scale = 1.0;
    
    /*＊ 被＊号注释掉的新的适配策略 这样其实主游戏区的适配UI布局是不变的，这样可以适配所有的机器类型 包括ipone ipad android 等
     新的适配策略是底图用一张大的背景， 然后再底图上加一层 裁剪层作为游戏区， 所有的触摸显示都在这个裁剪区内，裁剪区外的们触摸要屏蔽掉！切换场景
     等都在这个裁剪层上进行操作，等于是 游戏一旦启动，裁剪层作为低层是不会变了*/
    
    float dw1;    //游戏设计尺寸
    float dh1;
    if(dh/dw >= 1.5)     // 如果高宽比大于1.5 那么就按最小的宽来进行设计尺寸 上下留底图背景
    {
        dh = dw*960.0/640.0;
        dw1 = screenSize.width;
        dh1 = screenSize.height;
        scale = res_width/dw;
    }
    else                // 如果高宽比小于1.5 那么就按最小的高度来进行设计尺寸 左右留底图背景
    {
        dw = dh*640.0/960.0;
        dw1 = screenSize.width;
        dh1 = screenSize.height;
        scale = res_width/dw;
    }
    pDirector->setgSceneSize(CCSizeMake(dw, dh));
    CCEGLView::sharedOpenGLView()->setDesignResolutionSize(dw1, dh1, kResolutionNoBorder);  // 所有屏幕设计都按屏幕原始尺寸来 不过缩放是按 对比 640x960的标准分辨率设计模式来
    
    
    
    //    if (screenSize.width <= 480)
    //    {
    //        scale = res_width / screenSize.width;
    //    }
    //    else if (screenSize.width < 640)
    //    {
    //        //把高度介于480和640之间的屏幕，虚拟成480，这种机型目前已知的分辨率高度只有540
    //        //例如 900*540 == 800*480
    //        //    960*540 == 854*480;
    //        scale = res_width / 480;
    //
    //        dw = 480;
    //        dh = (int)(screenSize.height * 480 / screenSize.width);
    //    }
    //    else
    //    {
    //        scale = res_width / dw;
    //
    //        if (screenSize.width == 768*2)
    //        {
    //            dh = screenSize.height / 2;
    //            dw = 768;
    //        }
    //        else
    //        {
    //            if (screenSize.height >= 1136)
    //            {
    //                //超过了极限屏幕之后，需要缩放
    //
    //                dw = 640;
    //                dh = (int)(screenSize.height * 640 / screenSize.width);
    //            }
    //
    //            if (dh > 1136)
    //            {
    //                dh = 1136;
    //            }
    //        }
    //    }
    //
    //    //如果屏幕宽度不是偶数，+1
    //    if ((int)dh % 2)
    //    {
    //        dh = (int)dh + 1;
    //    }
    //
    //
    //    CCEGLView::sharedOpenGLView()->setDesignResolutionSize(dw, dh, kResolutionNoBorder);
    
    pDirector->setContentScaleFactor(scale);
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    cocos2d::CCLog("游戏启动");
    cocos2d::CCLog("原始尺寸: %d x %d", (int)screenSize.width, (int)screenSize.height);
    cocos2d::CCLog("设计尺寸: %d x %d", (int)size.width, (int)size.height);
    cocos2d::CCLog("资源缩放: %0.3f", scale);
    
    
    // create a scene. it's an autorelease object
    CCScene *pScene = HelloWorld::scene();
    
    // run
    pDirector->runWithScene(pScene);
    
    return true;

}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->pauseAllEffects();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->resumeAllEffects();
}
