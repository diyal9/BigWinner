//
//  CLTopLayerScene.h
//  Yy_mobileGame3
//
//  Created by Zhang Qiyun on 13-6-20.
//
//

#ifndef __Yy_mobileGame3__CLTopLayerScene__
#define __Yy_mobileGame3__CLTopLayerScene__

#include "cocos2d.h"
#include "LGameCommonDefine.h"
USING_NS_CC;

class CLTopLayerScene : public CLSceneLayer
{
public:
    CLTopLayerScene();
    virtual ~CLTopLayerScene();
    
    virtual bool init();
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void AdvImageLoadEnd();                   //异步加载图片完成后的回调 一般都是开始切换场景
    
    
    CC_SYNTHESIZE(bool, m_bHaveClicked, bHaveClicked);
    CC_SYNTHESIZE(bool, m_bHasAccountRecord, bHasAccountRecord);    //是否有帐号纪录
    
    CC_SYNTHESIZE(std::string, m_sAccount, sAccount);
    CC_SYNTHESIZE(std::string, m_sPassword, sPassword);
    
    CREATE_FUNC(CLTopLayerScene)
    
};

#endif /* defined(__Yy_mobileGame3__CLTopLayerScene__) */
