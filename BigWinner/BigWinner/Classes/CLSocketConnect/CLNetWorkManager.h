//
//  CLNetWorkManager.h
//  BigWinner
//
//  Created by ZhangQiyun on 13-10-8.
//
//

#ifndef __BigWinner__CLNetWorkManager__
#define __BigWinner__CLNetWorkManager__

#include "cocos2d.h"
#include "CClientSocket.h"
#include "JsonParser.h"
USING_NS_CC;



class CLNetWorkManager:public cocos2d::CCObject
{
public:
    CLNetWorkManager();
    ~CLNetWorkManager();
    
    static CLNetWorkManager* shardNetWorkManager();
    
    //调用请求一个真正的网络请求 包括创建线程
	bool   Connect(const char* szServerIP, int wPort);
    
    //登陆请求
    bool   LogonReq();
    
    //断开连接
    bool  onsocketDicconnect();
    
    //断网重连
    void reconnectOk(CCObject* r);
    bool ReConnectNoti();                                  //断网重练DLG
    
    //发送网络数据
    void   SendNetMessage(int nMsgID, Json::Value& value);
    
    //发送心跳包
    void   JumpSendMessage();
    
    //接收网络数据
    void   RecvNetMessage();
    
    bool CheckHasAccountRecord(std::string& sAccount, std::string& sPassword);// 判定机器中是否有帐号纪录
    
    //获取网络状态是否断开了连接
    bool  getNetWorkStatus()
    {
        return m_pClientSocket->IsDisconnecting();
    }
    
    void removeDlgA(CCNode *r);
    
    long getCurrentTime(); //android设备ID 临时使用
    
protected:
    Json::Value*   m_backValue;  //数据的备份
    bool            m_isReconnect; //是否是重连的LOGON
    bool            m_isFirstJoin;//是否是第一次进游戏
private:
    CClientSocket* m_pClientSocket;
    
    
    CC_SYNTHESIZE(bool, m_bHasConnectLS, bHasConnectLS);           //是否已经连接上LoginServer
};

#endif /* defined(__BigWinner__CLNetWorkManager__) */
