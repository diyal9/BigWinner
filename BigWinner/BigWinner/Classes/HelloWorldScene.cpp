#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "JsonDataManager.h"
#include "CLSoundManager.h"
#include "CLRealGameLayer.h"
#include "CLTopLayerScene.h"
#include "CLSceneManager.h"
#include "CLLoadingLayer.h"
#include "CLAnimationManager.h"
#include "CLNetWorkManager.h"
#include "CLMainSceneLayer.h"
#include "DlgLayerBtnB.h"
#include "CLChangeString.h"
#include "Button.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CLSceneManager::sharedSceneManager()->setnGgaoFlg(0);
    
    // 初始化json 对象管理器
    JsonDataManager::shardJDManager()->init();
    // 初始化动画管理器
    CLAnimationManager::sharedAnimationManager()->init();
    
    //初始化声音
    CLSoundManager::sharedSoundManager()->init();

    CLSoundManager::sharedSoundManager()->PlayBackGroundMusic("Sound/Sl_BackMusic.mp3");
    
    // 下面的自适应方式是按640 960 标准来 用引擎自带的全局缩放规则进行缩放 全部显示 低层加上超大的背景层,游戏有效区在一个裁剪框上面
    /*
     ＊＊＊＊＊＊＊＊ 被＊号注释掉的新的适配策略 这样其实主游戏区的适配UI布局是不变的，这样可以适配所有的机器类型 包括ipone ipad android 等
     新的适配策略是底图用一张大的背景， 然后再底图上加一层 裁剪层作为游戏区， 所有的触摸显示都在这个裁剪区内，裁剪区外的们触摸要屏蔽掉！切换场景
     等都在这个裁剪层上进行操作，等于是 游戏一旦启动，裁剪层作为低层是不会变了*/
    
    CCSize size = CCDirector::sharedDirector()->getgSceneSize();
    CCSize size1 = CCDirector::sharedDirector()->getWinSize();
    CCSprite* pSceneBG = CCSprite::create("Image/TopLayer/DeviceLayerBG.png");
    pSceneBG->setPosition(ccp(size1.width/2, size1.height/2));
    pSceneBG->setScale(0.938/0.833);
    this->addChild(pSceneBG, 0);
    CLSceneManager::sharedSceneManager()->setpDeviceBG(pSceneBG);
    
    
    CLRealGameLayer* pSceneLayer = CLRealGameLayer::create(size.width, size.height);
    pSceneLayer->setAnchorPoint(CCPointZero);
    pSceneLayer->setPosition(ccp((size1.width - pSceneLayer->getContentSize().width)/2, (size1.height - pSceneLayer->getContentSize().height)/2));
    this->addChild(pSceneLayer, 1, 10086);
    
    CLLoadingLayer* pLoadingLayer = CLLoadingLayer::create(size.width, size.height);
    pLoadingLayer->setAnchorPoint(CCPointZero);
    pLoadingLayer->setPosition(CCPointZero);
    pLoadingLayer->setVisible(false);
    pSceneLayer->addChild(pLoadingLayer, 9);
    
    CLTopLayerScene* pTopLayer = CLTopLayerScene::create();
    pTopLayer->setAnchorPoint(CCPointZero);
    pTopLayer->setPosition(CCPointZero);
    
    CLSceneManager::sharedSceneManager()->setpLoadingLayer(pLoadingLayer);
    CLSceneManager::sharedSceneManager()->setpParentNode(pSceneLayer);
    CLSceneManager::sharedSceneManager()->ChangeScene(pTopLayer);
    
    //数据库打开
    //Resource::sharedResource()->open();
    
    setKeypadEnabled(true);
    
    return true;
    
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::keyBackClicked() //重写键盘代码事件
{
    //音效
    CLSoundManager::sharedSoundManager()->PlayEffectSound("Sound/Sl_BtnClick.wav");
    if (this->getChildByTag(10086)->getChildByTag(100000) == NULL)
    {
        DlgLayerBtnB * appExitLayer = DlgLayerBtnB::createBtn(CLChangeString::sharedChangeString()->GetChangeString("quxiao"),
                                                              CLChangeString::sharedChangeString()->GetChangeString("queren"),
                                                              CLChangeString::sharedChangeString()->GetChangeString("ConfirmExit"));
        this->getChildByTag(10086)->addChild(appExitLayer, 10000, 100000);
        appExitLayer->pBtnRight->setOnClickCallback(callfuncO_selector(HelloWorld::menuCloseCallback),this);
    }
    
}
void HelloWorld::keyMenuClicked()
{
    
}
void HelloWorld::exitFunction(CCObject* r)
{
    exit(0);
}
