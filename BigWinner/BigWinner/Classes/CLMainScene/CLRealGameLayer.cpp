//
//  CLRealGameLayer.cpp
//  Yy_mobileGame3
//
//  Created by Zhang Qiyun on 13-6-20.
//
//

#include "CLRealGameLayer.h"
#include "CLLoadImageManager.h"
#include "CLNetWorkManager.h"
#include "DlgLayerBtnB.h"
#include "CLChangeString.h"

CLRealGameLayer::CLRealGameLayer()
{
    m_pSelector = NULL;
}

CLRealGameLayer::~CLRealGameLayer()
{
    
}

CLRealGameLayer* CLRealGameLayer::create(float fwidth, float fhight)
{
    CLRealGameLayer* player = new CLRealGameLayer();
    if(!player->init(fwidth, fhight))
    {
        delete player;
        return NULL;
    }
    player->autorelease();
    return player;
    
}

bool CLRealGameLayer::init(float fwidth, float fhight)
{
    if(!CClipRectLayer::init(fwidth, fhight))
        return false;
    
    this->StartSchedule(0.1);
    
    return true;
}

void CLRealGameLayer::StartSchedule(/*SEL_SCHEDULE pSelector,*/ float ftime)          // 为没有添加到父节点的类提供一个计时函数
{
//    if(m_pSelector)   //如果已经有一个在开了 那么新添加的忽略
//        return;
//    
//    m_pSelector = pSelector;
    this->schedule(schedule_selector(CLRealGameLayer::LoadingAdvImages) ,ftime);
    
}

//开启网络接收线程
void CLRealGameLayer::StartLoadingNetWork()
{
    this->schedule(schedule_selector(CLRealGameLayer::LoadingNetWork));
}

void CLRealGameLayer::LoadingNetWork()
{
    CLNetWorkManager::shardNetWorkManager()->RecvNetMessage();
}

void CLRealGameLayer::LoadingAdvImages()
{
    CLLoadImageManager::shardLIManager()->LoadingAdvImages();
}

void CLRealGameLayer::EndSchedule()                                               // 结束计时函数
{
    this->unschedule(schedule_selector(CLRealGameLayer::LoadingAdvImages));
    m_pSelector = NULL;
}

//开启心跳检测  当socket断开连接时应该关闭心跳检测
void CLRealGameLayer::StartJumpNetWork()
{
    this->unschedule(schedule_selector(CLRealGameLayer::LoadingJump));
    this->schedule(schedule_selector(CLRealGameLayer::LoadingJump), 20);
}

  //关闭心跳检测
void CLRealGameLayer::StopJumpNetWork()
{
    this->unschedule(schedule_selector(CLRealGameLayer::LoadingJump));
}

//发送心跳包
void CLRealGameLayer::LoadingJump()
{
    //是否断开连接
    bool isDisConnect = CLNetWorkManager::shardNetWorkManager()->getNetWorkStatus();
    //已经断开 提示是否重新连接
    if (isDisConnect)
    {
        StopJumpNetWork();
        CLNetWorkManager::shardNetWorkManager()->ReConnectNoti();
    }
    //没有断开
    else
    {
        CLNetWorkManager::shardNetWorkManager()->JumpSendMessage();
    }
    
}








