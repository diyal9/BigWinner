//
//  CLRealGameLayer.h
//  Yy_mobileGame3
//
//  Created by Zhang Qiyun on 13-6-20.
//
//

#ifndef __Yy_mobileGame3__CLRealGameLayer__
#define __Yy_mobileGame3__CLRealGameLayer__

#include <iostream>
#include "cocos2d.h"
#include "CClipRectLayer.h"

USING_NS_CC;

//真实游戏区类
class CLRealGameLayer : public CClipRectLayer
{
public:
    CLRealGameLayer();
    ~CLRealGameLayer();
   
    static CLRealGameLayer* create(float fwidth, float fhight);
    bool init(float fwidth, float fhight);
    void StartSchedule(/*SEL_SCHEDULE pSelector, */float ftime = 0.0f);   // 为没有添加到父节点的类提供一个计时函数
    void EndSchedule();                                               // 结束计时函数
    void LoadingAdvImages();                             // 预加载图片加载进行中的回调函数
    void LoadingNetWork();                               // 开启网络功能
    void LoadingJump();                                  //心跳检测方法 每隔20秒发送一次
    
    
    
    void StartLoadingNetWork();                          // 开启网络接受线程
    
    void StartJumpNetWork();                            //开启心跳检测  当socket断开连接时应该关闭心跳检测
    void StopJumpNetWork();                             //关闭心跳检测

private:
    SEL_SCHEDULE m_pSelector;
};

#endif /* defined(__Yy_mobileGame3__CLRealGameLayer__) */
